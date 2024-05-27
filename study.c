#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int err(char *str)
{
	while(*str)
		write(2, str++, 1);
	return 1;
}

int cd(char **argv, int i)
{
	//cd tam olarak iki arguman alir.
	// 1. argumanla chdir yapilir eger chdir -1 donerse error dondurulur.
	//hata yoksa buraya geldik 0 dondururuz.
	if (i != 2)
		return err("error: cd: bad arguments\n");
	if (chdir(argv[1]) == -1)
		return err("error: cd: cannot change directory to "), err(argv[1]), err("\n");
	return 0;
}

int exe_cmd(char **argv, char **envp, int i)
{
	int fd[2];	// iki elemanli file discriptor tanimla
	int status;	// int status tanimla
	int has_pipe = argv[i] && !strcmp(argv[i], "|");	//eger arguman[i] varsa VE bu arguman | pipe ise int has_pipe'i tanimla

	if (!has_pipe && !strcmp(*argv, "cd")) //has_pipe yoksa VE ilk arguman cd ise direk cd ye return et.
		return cd(argv, i);

	if(has_pipe && pipe(fd) == -1) //has_pipe varsa fd ile pipe olustur ve bu -1 donuyorsa pipe olusmamustur error: fatal\n dondur.
		return(err("error: fatal\n"));
	//yukarida pipe oldugunu ogrendik simdi fork ile child prosess olusturalim.
	int pid = fork();
	//eger child proses olusmussa (pid == 0)
	if(pid == 0)
	{
		argv[i] = NULL; // | pipelari null la degistiririz.
		//pipe varsa VE fd[1] dosya tanımlayıcısını standart çıktı (stdout) olan dosya tanımlayıcı 1 ile değiştirir .ve fd lari kapatiriz hata donerlerse error doneriz.
		if(has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return(err("error fatal\n"));

		if (!strcmp(*argv, "cd"))
			return (cd(argv, i));
		execve(*argv, argv, envp);
		return err("error: cannot execute "), err(*argv), err("\n");
	}
	waitpid(pid, &status, 0);
	//pipe varsa VE fd[0] dosya tanımlayıcısını standart girdi (stdin) olan dosya tanımlayıcı 0 ile değiştirir .ve fd lari kapatiriz hata donerlerse error doneriz.
	if(has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return(err("error fatal\n"));
	return WIFEXITED(status) && WEXITSTATUS(status);


}

int main (int argc, char **argv, char **envp)
{
	int	i = 0;
	int	status = 0;

	if(argc >1)
	{
		//her iki argumanda varsa bu donguye girilir.
		//i =0 ilk basta ve argv[0] ve argc[1]  varken. yani ilk iki arguman null degilse.
		//burada i yi bir arttirmis olduk ve iki argumani ayni anda varligini kontrol ettik.
		while(argv[i] && argv[++i])
		{
			argv += i; //argv i kadar ittare etmis olduk. aslinda bir sonraki argumana gecmis olduk.
			i = 0; // boylece basa donduk argunai bir atlatip bastan bakacagiz argumanlara.
			while(argv[i] && !strcmp(argv[i], "|") &&  !strcmp(argv[i], ";"))
				i++; //"|" ve ":" burada bunlarin i sini tutariz sonra exe de kullanip NULL yapacagiz.
					while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
				i++;
			if(i) //ifadesi, i sıfırdan büyükse anlamına gelir. Yani, i bir komutun uzunluğunu belirtir.
				status = exe_cmd(argv, envp, i);
		}
	}
	return status;
}

/*


 argv[i] = 0; satırıyla, argv dizisinin i'nci elemanını NULL olarak ayarlar. Bu, argv dizisinin sonunu belirtir.

Örneğin, bir komut satırı programınız var ve kullanıcı aşağıdaki gibi bir komut giriyor:

program arg1 arg2 | arg3 arg4

Bu durumda, argv dizisi şu şekilde görünür:

argv[0] = "program"
argv[1] = "arg1"
argv[2] = "arg2"
argv[3] = "|"
argv[4] = "arg3"
argv[5] = "arg4"
argv[6] = NULL

Eğer | karakterini gördüğünüzde bir pipe oluşturuyorsanız ve arg1 arg2 komutunu yürütmek istiyorsanız, argv dizisini execve() fonksiyonuna geçiremezsiniz çünkü bu fonksiyon arg3 ve arg4 argümanlarını da alır. Bunun yerine, argv dizisini şu şekilde ayarlamalısınız.

argv[0] = "program"
argv[1] = "arg1"
argv[2] = "arg2"
argv[3] = NULL
argv[4] = "arg3"
argv[5] = "arg4"
argv[6] = NULL

Bu durumda, execve() fonksiyonu sadece program, arg1 ve arg2 argümanlarını alır. argv[3] = 0; satırı, argv dizisini bu şekilde ayarlar. Bu, execve() fonksiyonunun yürüteceği programın argüman listesini doğru bir şekilde ayarlar.


*/
