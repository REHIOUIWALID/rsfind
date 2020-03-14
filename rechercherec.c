/* --name CHAINE restreint la sortie aux fichiers nommés CHAINE */
/* rs fait un parcours du rép et de ses sous dossiers */
//stat
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "rechercherec.h"
#include "Affichage_ls.h"



static int nb;


int recursive(DIR* dir,char* path,char* cherche, char **table){

    struct dirent *dp;
    

    while( (dp=readdir(dir))!= NULL ){//tq pas parcouru tout le dossier
        
        if(dp->d_type==4){// elt un dossier
			if(strcmp(dp->d_name,"..")!=0 && strcmp(dp->d_name,".")!=0){//pas pere ni curr
				if(cherche==NULL || strcmp(dp->d_name,cherche)==0){//correspond aux criteres
					
					table[nb]=(char*)malloc(strlen(path)+(strlen(dp->d_name)+1)*sizeof(char));
					strcpy(table[nb], path);
					strcat(table[nb],dp->d_name);
					
					nb++;
			}
				DIR* d;

					char* nameFold;
				nameFold=malloc(sizeof(char)*(strlen(dp->d_name)+1));
				strcpy(nameFold,dp->d_name);
					nameFold=strcat(dp->d_name,"/");// ajout / à NomDossier 

				char* pathr;
				pathr=malloc(sizeof(char)*(strlen(path)+strlen(nameFold)+1));
				strcpy(pathr,path);
					strcat(pathr,nameFold);//pathr le chemin relatif

						d=opendir(pathr);
				
				if(d!=NULL){
							recursive(d,pathr,cherche, table);
						}
				else{
					return 1;//erreur pour ouverture fichier
				}
				free(pathr);
			}
        }
        else{//un fichier
			if(cherche==NULL || strcmp(dp->d_name,cherche)==0){//correspond aux criteres
					
					table[nb]=(char*)malloc(strlen(path)+(strlen(dp->d_name)+1)*sizeof(char));
					strcpy(table[nb], path);
					strcat(table[nb],dp->d_name);
					
					nb++;
			}
					
			}
        }
    
    closedir(dir);
    return 0;//tout ok
}


char* format(char* arg){// s assure que finit bien par /

	int taille;
	taille=strlen(arg);
	char c;
	c=*(arg+taille-1);
	
	char* path;
	if(c=='/'){//ok
		path=malloc(sizeof(char)*(strlen(arg)+1));
		strcpy(path,arg);
	}
	else{//rajoute /
		path=malloc(sizeof(char)*(strlen(arg)+2));
		strcpy(path,arg);
		strcat(path,"/");
	}

	return path;

}

char* CurrentFold(char* argv){//retourne le nom du dossier 

    char* ptr1=argv;
    char* ptr2=argv;
    int ecart=0;

    while(*ptr2!='\0'){

        if(*ptr2=='/' && *(ptr2+1)!='\0'){
            ptr1=ptr2;
            ecart=0;
        }
        ptr2++;
        ecart++;
    }

    
    
    char* nfd=malloc(sizeof(char)*ecart);// nom du dossier au bout du chemin
    
    for(int i=0 ; i<ecart-1 ; i++){//recopiage
        *(nfd+i)=*(ptr1+i);
    }
    if(*(ptr1+ecart-1)!='/'){
        *(nfd+ecart-1)=*(ptr1+ecart-1);
    }

    return nfd;
}


int recursivePerll(DIR* dir,char* path,char **table,pcre* reCompiled,pcre_extra* pcreExtra,int* substrVec){

    struct dirent *dp;
    int pcreExecRet;
    
    while( (dp=readdir(dir))!= NULL ){//tq pas parcouru tout le dossier
        
        if(dp->d_type==4){// elt un dossier
			if(strcmp(dp->d_name,"..")!=0 && strcmp(dp->d_name,".")!=0){//pas pere ni curr
                pcreExecRet=pcre_exec(reCompiled,pcreExtra,dp->d_name,strlen(dp->d_name),0,0,substrVec,30);
				if(pcreExecRet>=0){//correspond aux criteres
					
					table[nb]=(char*)malloc(strlen(path)+(strlen(dp->d_name)+1)*sizeof(char));
					strcpy(table[nb], path);
					strcat(table[nb],dp->d_name);
					
					nb++;
			}
				DIR* d;

					char* nameFold;
				nameFold=malloc(sizeof(char)*(strlen(dp->d_name)+1));
				strcpy(nameFold,dp->d_name);
					nameFold=strcat(dp->d_name,"/");// ajout / à NomDossier 

				char* pathr;
				pathr=malloc(sizeof(char)*(strlen(path)+strlen(nameFold)+1));
				strcpy(pathr,path);
					strcat(pathr,nameFold);//pathr le chemin relatif

						d=opendir(pathr);
				
				if(d!=NULL){
							recursivePerll(d,pathr,table,reCompiled,pcreExtra,substrVec);
						}
				else{
					return 1;//erreur pour ouverture fichier
				}
				free(pathr);
			}
        }
        else{//un fichier
            pcreExecRet=pcre_exec(reCompiled,pcreExtra,dp->d_name,strlen(dp->d_name),0,0,substrVec,30);
			if(pcreExecRet>=0){//correspond aux criteres
					
					table[nb]=(char*)malloc(strlen(path)+(strlen(dp->d_name)+1)*sizeof(char));
					strcpy(table[nb], path);
					strcat(table[nb],dp->d_name);
					
					nb++;
			}
					
			}
        }
    
    closedir(dir);
    return 0;//tout ok
}


int recherche(char* argv, char* name, char** table){

    DIR* direct;
	nb = 0;

    if(argv==NULL){// pas d argument : rep courant
	direct=opendir("./");
	    if(direct!=NULL){	
		char* path;
		path=malloc(sizeof(char)*3);
		strcpy(path,"./");
        if(name!=NULL && strcmp(".",name)==0){// ne pas afficher . 
            table[nb]=(char *) malloc(sizeof(char)*2);
            strcpy(table[nb++],".");
        }
        else{
            if(name==NULL){
                table[nb]=(char *) malloc(sizeof(char)*2);
                strcpy(table[nb++],".");
            }
        }
        
		recursive(direct,path,name, table);//path vaut . au début
	    }
    }
    else{// un argument
        direct=opendir(argv);
	if(direct!=NULL){// bien ouvert
	    char* path=format(argv);
        char* nfold=CurrentFold(argv);
        if(name==NULL){
            table[nb]=malloc(sizeof(char)*strlen(argv));
            strcpy(table[nb],argv);
            nb++;
        }
        else{
            if(strcmp(name,nfold)==0){
                table[nb]=malloc(sizeof(char)*strlen(nfold));
                strcpy(table[nb],nfold);
            }
        }
        recursive(direct,path,name, table);//path vaut . au début
	    free(path);
       }
    }

    return nb;
}

int recursiveEname(char* path,char** table,char* ename,glob_t* globbuff){


	struct dirent*dp;
	DIR* rep;
	char* pthr;
	char* namedp;
	int Numfiles;


	char* nom; // ename que l'on adapte
	
	rep=opendir(path);//dossier parent
	while( (dp=readdir(rep))!=NULL){//lit dossier ouvert avant

		if(dp->d_type==4 && strcmp(dp->d_name,"..")!=0 && strcmp(dp->d_name,".")!=0){//un dossier
			namedp=(char*)malloc(sizeof(char)*(strlen(dp->d_name)+1));
			strcpy(namedp,dp->d_name);//nomDossierCurr
			pthr=(char*)malloc(sizeof(char)*(strlen(path)+strlen(namedp)+1) );
			strcpy(pthr,path); //nomDossier1/
			strcat(pthr,namedp);//nomDossier1/nomDossierCurr
			free(namedp);

			nom=(char*)malloc(sizeof(char) * (strlen(pthr)+1+strlen(ename)) );
			strcpy(nom,pthr); //nomDossier/nomDossierCurr
			strcat(nom,"/");
			strcat(nom,ename); // nom= nomdossier1/nomDossierCurr/pattern
			if(rep!=NULL){//dp on peut l'enlever ?!
				glob(nom,GLOB_DOOFFS,NULL,globbuff);//elts de dp 
				free(nom);
				Numfiles=(*globbuff).gl_pathc;// nb elts rep courant
				for(int i=0 ; i<Numfiles ; i++){
					table[nb]=(char*)malloc(sizeof(char)* strlen((*globbuff).gl_pathv[i])  ) ;
					strcpy(table[nb++],(*globbuff).gl_pathv[i]);
				}
				strcat(pthr,"/");
				recursiveEname(pthr,table,ename,globbuff);
				free(pthr);
			}
			
			
		}
	}
	closedir(rep);
	return 0;

}



int tiretEname(char* argv,char* ename,char**table,int nbfich){


	DIR* direct;
	nb=nbfich;
	int Numfiles;
	
	glob_t* globbuff;
	globbuff=malloc(sizeof(glob_t));
	//globbuff->gl_offs = 1000;


	//char* name;//pattern qui change

	if(argv==NULL || strcmp(argv,".")==0 || strcmp(argv,"./")==0 ){// pas d argument : rep courant
		direct=opendir("./");
	    	if(direct!=NULL){//bien ouvert dossier courant	
			char* path;
			path=malloc(sizeof(char)*3);
			strcpy(path,"./");
			glob(ename,GLOB_DOOFFS,NULL,globbuff);
			Numfiles=(*globbuff).gl_pathc;// nb elts rep courant
			for(int i=0 ; i<Numfiles ; i++){
				table[nb]=(char*)malloc(sizeof(char)*(strlen((*globbuff).gl_pathv[i])+strlen(path)));
				strcpy(table[nb],path);
				strcat(table[nb++],(*globbuff).gl_pathv[i]);
			}//rajout des elts du dossier courant
			recursiveEname(path,table,ename,globbuff);
			closedir(direct);
			free(path);
			//path= ./

		}
		
        }
	else{//un argument
		direct=opendir(argv);
		if(direct!=NULL){// bien ouvert
			char* name;// dossier/pattern
	    		char* path=format(argv);
			name=(char*)malloc(sizeof(char)*( strlen(path)+strlen(ename) ));
			strcpy(name,path);//dossier/
			strcat(name,ename);//dossier/pattern
			glob(name,GLOB_DOOFFS,NULL,globbuff);
			Numfiles=(*globbuff).gl_pathc;
			for(int i=0 ; i<Numfiles ; i++){//rajoute elts ok du dossier passe param
				table[nb]=(char*)malloc(sizeof(char)*(strlen((*globbuff).gl_pathv[i])));
				strcpy(table[nb++],(*globbuff).gl_pathv[i]);
			}
			recursiveEname(path,table,ename,globbuff);
			closedir(direct);
			// path = dossier/
			free(path);
			free(name);
		}
	}
	globfree(globbuff);
	return nb;//affiche on nombre
}



/* Prendre le dossier en finde chemin pour le mettre dans table[nb++] */
// https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
int recherchePerll(char* argv, char* ename, char** table,int nbFich){

	char* temp = malloc(sizeof(char)*(strlen(ename)+5));
	temp[0]='(';
	temp[1]=')';
	temp[2]='.';
	temp[3]='\0';
	strcat(temp, ename);
	strcat(temp,"$");	

	ename =temp;




    DIR* direct;
	nb = nbFich;
///////////////////////////////////PCRE 
    pcre* reCompiled;

	const char* errptr;//pour mettre mess. d'erreur

	int errofset;//offset du pattern où erreur trouvée


	reCompiled=pcre_compile(ename,0,&errptr,&errofset,NULL);	
	/*pcre *pcre_compile(const char *pattern, int options,
            const char **errptr, int *erroffset,
            const unsigned char *tableptr);*/

	if(reCompiled==NULL){
		exit(1);
	}
	// != NULL
	

	pcre_extra* pcreExtra;
	pcreExtra=NULL;

	const char* studerrptr;// ou mettre le message d'erreur

	pcreExtra=pcre_study(reCompiled,0,&studerrptr);//étudie le pattern compilé, cherche à accélerer reconnaissance
	if(errptr!=NULL){
		perror("[!] Erreur ! N'a pas pus étudier le pattern \n");
		exit(1);

	}
//////////////////////////////////////FPCRE

    int substrVec[30];


    if(argv==NULL){// pas d argument : rep courant
	direct=opendir("./");
	    if(direct!=NULL){	
		char* path;
		path=malloc(sizeof(char)*3);
		strcpy(path,"./");
        if(pcre_exec(reCompiled,pcreExtra,".",1,0,0,substrVec,30)>=0){//correspond aux critères
            table[nb]=(char *) malloc(sizeof(char)*2);
            strcpy(table[nb++],".");
        }
		recursivePerll(direct,path, table,reCompiled,pcreExtra,substrVec);//path vaut . au début
	    }
    }
    else{// un argument
        direct=opendir(argv);
	if(direct!=NULL){// bien ouvert
	    char* path=format(argv);
        char* nfold=CurrentFold(argv);
        if(pcre_exec(reCompiled,pcreExtra,nfold,strlen(nfold),0,0,substrVec,30)>=0){
            table[nb]=malloc(sizeof(char)*strlen(nfold));
            strcpy(table[nb],nfold);
        }
        recursivePerll(direct,path,table,reCompiled,pcreExtra,substrVec);//path vaut . au début
	    free(path);
        pcre_free(reCompiled);
	    if(pcreExtra!=NULL){
		    #ifdef PCRE_CONFIG_JIT
			    pcre_free_study(pcreExtra);// généralement appelé
		    #else
			    pcre_free(pcreExtra);
		    #endif
	    }
      }
    }

    return nb;

}
/* 
REMARQUE : ON PEUT FAIRE PROGRAMME DOSSIER OU PROGRAMME DOSSIER/ POUR TOUT DOSSIER
		. OU ./ FONCTIONNENT DONC !


CONCERNANT LA RECHERCHE : CE SERAIT LE DEUXIEME PARAMETRE DE ARGV. AFFICHERA JUSTE LES FICHIERS AYANT LE NOM STIPULE. 

*/

