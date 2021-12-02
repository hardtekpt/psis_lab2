#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(){


	int fd;
	char str[100];
	int n;
    int size=0;
	while((fd = open("/tmp/fifo_server", O_WRONLY))== -1){
	  if(mkfifo("/tmp/fifo_server", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened for writing\n");


	while(1){
		printf("write the name of the function to execute:");
		fgets(str, 100, stdin);
        while (1)
        {
            if (!isspace(str[size])){
                //printf("%c 1 %d\n", str[size],size);
                size++;
                
            }
            else {
                str[size] = '\0';
                size++;
                break;
            }
        }
        
		write(fd, str, size);
		printf("%s\n", str);

        size = 0;
	}

}
