#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){


	int fd;
	int fd2;
	char str[100];
	int n;
	int i;
	while((fd = open("/tmp/fifo_server", O_WRONLY))== -1){
	  if(mkfifo("/tmp/fifo_server", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened for writing\n");

	while((fd2 = open("/tmp/fifo_server2", O_RDONLY))== -1){
	  if(mkfifo("/tmp/fifo_server2", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened for reading\n");


	while(1){
		printf("write the name of the function to execute:");
		fgets(str, 100, stdin);
        
		write(fd, str, 100);
		//printf("%s\n", str);

		n = read(fd2, &i, 100);
		if(n<=0){
			perror("read ");
			exit(-1);
		}
		printf("%d\n", i);
	}
}
