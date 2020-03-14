#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "magic.h"
#include <dlfcn.h>
#include <pthread.h>


void* image (char ** args){

    char*path = args[0];

    
    void *lib;
    char *error;
    magic_t (*magic_open)(int);
    int (*magic_load)(magic_t, char*);
    char* (*magic_file)(magic_t, char*);
    void (*magic_close)(magic_t);
    char* (*magic_error)(magic_t);




    lib = dlopen("./libmagic/libmagic.so", RTLD_LAZY);
    if (!lib) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();
    
    magic_open = (magic_t (*)(int))dlsym(lib, "magic_open");
    
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    dlerror();
    
    (magic_load = dlsym(lib, "magic_load"));
    
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    dlerror();
    
    (magic_file = dlsym(lib, "magic_file"));
    
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    dlerror();

    (magic_close = dlsym(lib, "magic_close"));
    
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    dlerror();

    (magic_error = dlsym(lib, "magic_error"));
    
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    dlerror();
    
    magic_t cookies ;
    char* result = malloc(1000*sizeof(char));
    

    cookies = magic_open(MAGIC_MIME);

    if (cookies == NULL){
        printf("magic open ne s'initialise pas \n");
    }

    if (magic_load(cookies, "./misc/magic.mgc")!= 0){
        printf("libmagic ne se charge pas \n");
        fprintf(stderr, "%s\n", magic_error(cookies));
        exit(EXIT_FAILURE);
        magic_close(cookies);
        exit(2);
    }

    result = magic_file(cookies,path);
       
    strtok(result, "/");
   
     
    if (strcmp(result, "image")==0){
        
        //magic_close(cookies);
        
        free(result);
        dlclose(lib);
        pthread_exit((void*)1);
       
    }
    else{
        
        free(result);
        dlclose(lib);
        pthread_exit((void*)0);
       
    } 

}
