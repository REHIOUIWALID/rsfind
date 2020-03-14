#include <stdio.h>
#include <string.h>
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "dirent.h"
#include "rechercherec.h"
#include "Affichage_ls.h"
#include <pthread.h>
#include "tirett.h"
#include "tiretGrandT.h"
#include "image.h"
#include "expe.h"

int main(int argc, char **argv)
    {

            

           int comp = 1;
           int c;
           char *optionname = NULL;
           char *optiont = NULL;
	   char *optionT = NULL;
           char *optionexec = NULL;
           char *optionename = NULL;
           int nb_fichier = 0;
           char** table = (char**)malloc(sizeof(char*)*3000);
           pthread_t *threads;
           int nbthreads;


           int option_index = 1;
           static struct option long_options[] = {
               {"name", required_argument,0,'n'},
               {"exec", required_argument,0,'e'},
               {"ename", required_argument,0,'k'},
               {"print", no_argument,0,'p'}
           };

            
           
            int option[9]= {0, 0, 0, 0, 0, 0, 0, 0,0}; // 0: -l, 1:-t, 2:-i, 3:--name, 4:--exec, 5: -P; ; 6:ename; 7:print ; 8:-T

            
           



            c=getopt_long(argc, argv, "lit:P:pT:",long_options, &option_index);
            while(c!=-1){

                    //printf("chararctère %c \n", c);

                    comp++;

                    switch (c) {

                    case 'l':                     
                        option[0]=1;                  
                    break;

                    case 'n':
                        optionname = (char*)malloc(sizeof(char)*strlen(argv[optind-1]));
                        strcpy(optionname,argv[optind-1]);
                        option[3]=1;
                        break;

                    case 'e':
                        optionexec = (char*)malloc(sizeof(char)*strlen(argv[optind-1]));
                        strcpy(optionexec,argv[optind-1]);
                        option[4]=1;
                    break;

			case 'T':
			optionT=(char*)malloc(sizeof(char)*strlen(argv[optind-1]));
			strcpy(optionT,argv[optind-1]);
			option[8]=1;
			break;

                    
			case 't':
                        optiont = (char*)malloc(sizeof(char)*strlen(argv[optind-1]));
                        strcpy(optiont,argv[optind-1]);
                        option[1]=1;
                        break;
                    
                    case 'k':
                        optionename = (char*)malloc(sizeof(char)*strlen(argv[optind-1]));
                        strcpy(optionename,argv[optind-1]);
                        option[6]=1;
                        break;

                    case 'i':
                        option[2]=1;
                        break;
                    
                    case 'p':
                        option[7]=1;
                        break;

                    case 'P':
                        option[5]=1;
                        nbthreads = atoi(argv[optind-1]);
                        break;

                    default:
			exit(0);
                    }

                    c=getopt_long(argc, argv, "lt:iP:p",long_options, &option_index);
                }

            int sommeargv = option[3] + option[1] + option[4] + option[5] + option[6] + option[8];

            if(option[3] ){ //si arg en plus --name -t --exec || option[1] || option[4] || option[5]
                nb_fichier = recherche(argv[comp+sommeargv], optionname, table);
            }
            else if(option[6]){//ename
                nb_fichier = recherchePerll(argv[comp+sommeargv], optionename, table, nb_fichier);
            }            

            else{
                nb_fichier = recherche(argv[comp+sommeargv], NULL, table);
            }

            if(option[5]){ //-P
                threads = (pthread_t*)malloc(nbthreads * sizeof(pthread_t));
            }else{
                nbthreads = 1;
                threads = (pthread_t*)malloc(nbthreads * sizeof(pthread_t));
                
            }

            
            
            if(option[8]){ //-T
		char*** args = malloc(sizeof(char**)*nbthreads);
                for (int j=0; j < nbthreads; j++){
                	args[j]= malloc(sizeof(char*)*2);
                }
                void * retc;
                for (int i = 0; i<nb_fichier; i= i + nbthreads ){
                    for(int j=0; j < nbthreads; j++){
                        if (i+j<nb_fichier){
                           
                            /* args[j][0] = malloc(sizeof(char)*strlen(optiont));
                            strcpy(args[j][0],optiont);
                            args[j][1] = malloc(sizeof(char)*strlen(table[i+j]));
                            strcpy(args[j][1],table[i+j]); */
                            args[j][0] = optionT;
                            args[j][1] = table[i+j];
                            pthread_create(&threads[j],NULL,(void*)(*tiretGrandT),args[j]);
                            
                        };
                        
                    }

                    for(int l=0; l < nbthreads; l++){
                        if (i+l<nb_fichier){
                            pthread_join(threads[l],&retc);
                            if((int*)retc==0){
                                free(table[i+l]);
                                table[i+l]=NULL;}
                        }
                    }

                }

                for (int i = 0; i<nbthreads; i++){
                    free(args[i]);
                }

                free(args);
            
	}//
            
            

            if(option[1]){  //-t   

                char*** args = malloc(sizeof(char**)*nbthreads);
                for (int j=0; j < nbthreads; j++){
                args[j]= malloc(sizeof(char*)*2);
                }
                void * retc;
                for (int i = 0; i<nb_fichier; i= i + nbthreads ){
                    for(int j=0; j < nbthreads; j++){
                        if (i+j<nb_fichier){
                           
                            /* args[j][0] = malloc(sizeof(char)*strlen(optiont));
                            strcpy(args[j][0],optiont);
                            args[j][1] = malloc(sizeof(char)*strlen(table[i+j]));
                            strcpy(args[j][1],table[i+j]); */
                            args[j][0] = optiont;
                            args[j][1] = table[i+j];
                           
                            pthread_create(&threads[j],NULL,(void*)(*tirett),args[j]);
                        };
                        
                    }

                    for(int l=0; l < nbthreads; l++){
                        if (i+l<nb_fichier){
                            pthread_join(threads[l],&retc);
                            //printf("return %ld \n", (int*) retc);                          
                            if((int*)retc==0){
                                free(table[i+l]);
                                table[i+l]=NULL;}
                        }
                    }

                }

                for (int i = 0; i<nbthreads; i++){
                    free(args[i]);
                }

                free(args);
            
            }
            
            if(option[2]){  //-i

                char*** args = malloc(sizeof(char**)*nbthreads);
                for (int j=0; j < nbthreads; j++){
                    args[j]= malloc(sizeof(char*));
                }
                void * retc;

                for (int i = 0; i<nb_fichier; i= i + nbthreads ){
                    if (table[i]!=NULL){
                        for(int j=0; j < nbthreads; j++){
                            if (i+j<nb_fichier){
                            
                                args[j][0] = table[i+j];  
                                pthread_create(&threads[j],NULL,(void*)(*image),args[j]);
                                
                            };
                            
                        }
                
                        for(int l=0; l < nbthreads; l++){
                            if (i+l<nb_fichier){
                                pthread_join(threads[l],&retc);
                                //printf("return %ld \n", (int*) retc);                          
                                if((int*)retc==0){
                                    free(table[i+l]);
                                    table[i+l]=NULL;}
                            }
                        }
                    }
                }

                for (int i = 0; i<nbthreads; i++){
                    free(args[i]);
                }

                free(args);
                
            }

            
            if(option[0]){ //-l
                for (int i = 0; i<nb_fichier; i++){
                    if(table[i]!=NULL ){
                        Affichage(table[i]);
                    }

                }
            }else if(option[4] && !option[7]){ //-
            }else{
                for (int i = 0; i<nb_fichier; i++){
                    if(table[i]!=NULL ){
                        char* ptr;	
                        ptr=malloc(sizeof(char)*(strlen(table[i])+1));
                        strcpy(ptr,table[i]);
                        *(ptr+strlen(table[i]))='\n';
                        write(1,ptr,sizeof(char)*(strlen(table[i])+1));
                        free(ptr);
                    }

                }
            }

            if(option[4]){ //--exec
                int path = 0;
                int unsigned i = 0;
                while(i<strlen(optionexec)){
                    if ( optionexec[i] == '{'){
                        path = 1;
                        optionexec[i] = ' ';
                        optionexec[i+1] = ' ';
                        break;
                    }
                    i++;
                }         
                
                if (path){//jusque la ok ^
                    execmain(table,optionexec, nb_fichier);
                }
                else{//pas de path
                    int z;
                    for( z= 0; z<nb_fichier; z++){
                        strcpy(table[0], "./");
                        execmain(table,optionexec, 1);
                    }  
                }
                

                
            }

       
        free(threads);

       
        for (int i = 0; i<nb_fichier; i++){
            
            //if(table[i]!=NULL ){
                //printf("%s\n", table[i]);
                free(table[i]);
            //}
        }
        

        free(table);
 
        exit(0);

    }

