#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "funcs.h"
#include <dlfcn.h>
#include <string.h>

int main(){
    void *lib_handle;
    int (*fn)();
    char *error;

	int fd;

	while((fd = open("/tmp/fifo_server", O_RDONLY))== -1){
	  if(mkfifo("/tmp/fifo_server", 0666)!=0){
			printf("problem creating the fifo\n");
			exit(-1);
	  }else{
		  printf("fifo created\n");
	  }
	}
	printf("fifo just opened\n");
	int n;
    int size;
    int res;
	char str[100];
    lib_handle = dlopen("./funcs.so", RTLD_LAZY);

    if (!lib_handle){
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

	while(1){
		n = read(fd, str, size);
		if(n<=0){
			perror("read ");
			exit(-1);
		}
		printf("Str %s (%d bytes)\n", str, n);

        if(strcmp("exit", str) == 0)
            break;
        
        fn = dlsym(lib_handle, str);
        
        if ((error = dlerror()) != NULL){
            fprintf(stderr, "%s\n", error);
            exit(1);
        }

        res = (*fn)();
        //write(fd, &res, sizeof(res));
		printf("%d\n", res);
	}
    dlclose(lib_handle);
}
