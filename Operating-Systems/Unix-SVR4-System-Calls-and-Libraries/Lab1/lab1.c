#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <ulimit.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[]){

    if (argc < 2) {
		perror("Invalid arguments.");
		return 0;
	}

    int currentArgument;
    char options[] = "ispuU:cC:dvV:";
    struct rlimit rlp;

    while ((currentArgument = getopt(argc, argv, options)) != EOF)

    switch (currentArgument) {
    case 'i':
        printf("uid = %ld\n", getuid());
        printf("euid = %ld\n", geteuid());
        printf("gid = %ld\n", getgid());
        printf("egid = %ld\n", getegid());
        break;

    case 's':
        (void) setpgid(0,0); //������� ���������� ������� ������
        break;

    case 'p':
        printf("pid = %ld\n", getpid());
        printf("parent pid = %ld\n", getppid());
        printf("group pid = %ld\n", getpgid(0));
        break;

    case 'u': //������������ ���������� �������� ������ �� �������
        printf("ulimit = %ld\n", ulimit(UL_GETFSIZE, 0));
        break;

    case 'U':
        if(ulimit(UL_SETFSIZE, atol(optarg)) == -1)
            perror("ulimit can't be changed\n");
        break;

    case 'c': //�core dump� ����� ����������� ������ ���������,
                //����������� � ����� �� �����
                //��� ������������ �������
        getrlimit(RLIMIT_CORE, &rlp);
        printf("core size = %ld\n", rlp.rlim_cur);
        break;

    case 'C':
        getrlimit(RLIMIT_CORE, &rlp);
        rlp.rlim_cur = atol(optarg);
        if (setrlimit(RLIMIT_CORE, &rlp) == -1)
            perror( "size of core can't be changed\n");
        break;

    case 'd': //�������� ������� ������� ����������
        printf("current working directory is: %s\n", getcwd(NULL, 100));
        break;

    case 'v': //������������� ���������� ����� � �� ��������
        printf("environment variables are:\n");
        char **currentEnvVar;
        for (currentEnvVar = environ; *currentEnvVar; currentEnvVar++)
            printf("%s\n", *currentEnvVar);
        break;

    case 'V': //������ ����� ���������� � ����� ���
                //�������� �������� ������������ ����������.
        putenv(optarg);
        break;
    }

    return 0;
}

