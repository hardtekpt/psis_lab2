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
    char *error;
	int fd;
	int fd2;
	int n;
    int size;
    int res;
	char str[100];
	int sizeStr = 0;
	int arg = 0;
    lib_handle = dlopen("./funcs.so", RTLD_LAZY);

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
            if (!isspace(str[sizeStr]))
                sizeStr++;
            else {
                sizeStr++;
                break;
            }
        }

		char aux[sizeStr];
		if (sscanf(str, "%s %d [^ ]", aux, &arg) != 2){
			int (*fn)();
			fn = dlsym(lib_handle, aux);
			if ((error = dlerror()) != NULL){
				fprintf(stderr, "%s\n", error);
				exit(1);
			}
			res = (*fn)();
			write(fd2, &res, sizeof(res));
		} else {
			int (*fn)(int);
			fn = dlsym(lib_handle, aux);
			if ((error = dlerror()) != NULL){
				fprintf(stderr, "%s\n", error);
				exit(1);
			}
			res = (*fn)(arg);
			write(fd2, &res, sizeof(res));
		}
		sizeStr = 0;
	}
    dlclose(lib_handle);
}
