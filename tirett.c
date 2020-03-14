#include<stdio.h> //
#include<unistd.h>
#include<sys/types.h> //
#include<sys/stat.h> //
#include<dirent.h>
#include<string.h> //
#include<stdlib.h> //
#include<fcntl.h> //
#include <errno.h>
#include <pthread.h>

/* En entrée : chemins de fichiers où travaille, chaîne de caractères
    En sortie : chemins de fichiers où chaîne présente */
void* tirett(char ** args){

    char  *mot ;
    char * file;

    file = args[1];
    mot = args[0];

    int fd;

    fd=open(file,O_RDONLY,0);

   
    if(fd==-1){
        printf("erreur ouverture file %s! \n", file);        
    }

    
    char* c;
    c=(char *)malloc(sizeof(char));
    //lecture caractère par caractère

    int i=0;
    int neof=read(fd,c,1);//lecture
   
    if (errno == 0){
        while(*(mot+i)!='\0' && neof){
            if(*(mot+i)==*c){
                i++;
                neof=read(fd,c,1);
            }
            else{
                i=0;
                neof=read(fd,c,1);
            }
        }
    }
    close(fd);


    
    if(neof==1){
        pthread_exit((void*)1);
    }
    
    pthread_exit((void*)0);


}

