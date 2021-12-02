#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "funcs.h"
#include <dlfcn.h>
#include <string.h>
#include <ctype.h>

int main(){
    void *lib_handle;
    int (*fn)();
    char *error;

	int fd;
	int fd2;

	while((fd = open("/tmp/fifo_server", O_RDONLY))== -1){
	  if(mkfifo("/tmp/fifo_server", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened for reading\n");

	while((fd2 = open("/tmp/fifo_server2", O_WRONLY))== -1){
	  if(mkfifo("/tmp/fifo_server2", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened for writing\n"); 

	int n;
    int size;
    int res;
	char str[100];
	int sizeStr = 0;
    lib_handle = dlopen("./funcs.so", RTLD_LAZY);

    if (!lib_handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

	while(1){
		n = read(fd, str, 100);
		if(n<=0){
			perror("read ");
			exit(-1);
		}


		while (1)
        {
            if (!isspace(str[sizeStr])){
                //printf("%c 1 %d\n", str[size],size);
                sizeStr++;
                
            }
            else {
                str[sizeStr] = '\0';
                sizeStr++;
                break;
            }
        }

		char aux[sizeStr];

		for (size_t j = 0; j < sizeStr; j++)
		{
			aux[j] = str[j];
		}
		
		sizeStr = 0;

		//printf("Str %s (%d bytes)\n", str, n);

        if(strcmp("exit", aux) == 0)
            break;
        
        fn = dlsym(lib_handle, aux);
        
        if ((error = dlerror()) != NULL){
            fprintf(stderr, "%s\n", error);
            exit(1);
        }

        res = (*fn)();
		write(fd2, &res, sizeof(res));
	}
    dlclose(lib_handle);
}
