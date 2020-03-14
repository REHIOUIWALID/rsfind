#include <stdio.h>
#include <pcre.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>

/* Compilation : gcc -Wall experimentation.c -o experiment -lpcre */

void* tiretGrandT(char** argv){
	//S'assurer que pas un dossier
	int prob=1;
	struct stat sb;
	if(lstat(argv[1],&sb)==-1){
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	switch (sb.st_mode & S_IFMT) {
		case S_IFREG : 
		prob=0;
		break;
		default : 
		prob=1;
		break;
	}
	

	if(prob==0){
	//lecture du nom de fichier où chercher + test si s ouvre bien
	char* file;
	int fd;
	file=argv[1];
	fd=open(file,O_RDONLY,0);
	if(fd==-1){
		//char* messErr="Ne peut ouvrir le fichier \n";
		//write(2,messErr,strlen(messErr));
		exit(0);
	}
	//Expression a chercher
	char* expression;
	expression=argv[0];
	
	//Compilation
	pcre* reCompiled;
	int errofset;
	const char* errptr;
	reCompiled=pcre_compile(expression,0,&errptr,&errofset,NULL);
	if(reCompiled==NULL){
		exit(1);
	}
	
	//Amélioration
	pcre_extra* pcreExtra;
	pcreExtra=NULL;
	const char* studerrptr;
	pcreExtra=pcre_study(reCompiled,0,&studerrptr);
	if(errptr!=NULL){
		exit(1);
	}

	int substrVec[30];
	//tiretTPerll(nomFichier,reCompiled,pcreExtra,substrVec);

	
	//tiretTPerll
	char* c;
	c=(char*) malloc(sizeof(char));//parcours du fichier
	int neof;
	neof=read(fd,c,1);	


	int longMot=0;
	int nbMots=0;
	char* String;
	int found=0;
	//Tant que pas tout parcouru
	while(neof && !found){
		
		//Saute les espaces
		while(neof && (*c==' ' || *c=='\n' || *c=='\t')){
			neof=read(fd,c,1);
		}
		
		//Un mot parcouru jusqu'à sa fin
		while(neof && *c!=' ' && *c!='\n' && *c!='\t'){
			longMot++;
			neof=read(fd,c,1);
		}
		//Retour en arrière pour le mettre dans un string
		lseek(fd,-longMot-1,SEEK_CUR);
		String=(char*)malloc(sizeof(char)*longMot);
		neof=read(fd,String,longMot);
		if(pcre_exec(reCompiled,pcreExtra,String,longMot,0,0,substrVec,30)>=0){
			found=1;//un mot du fichier correspond au regex pcre
		}
		free(String);
		lseek(fd,1,SEEK_CUR);

		neof=read(fd,c,1);
		nbMots++;
		longMot=0;
		
	}
	close(fd);
	//Free
	pcre_free(reCompiled);
	if(pcreExtra!=NULL){
		#ifdef PCRE_CONFIG_JIT
			pcre_free_study(pcreExtra);
		#else
			pcre_free(pcreExtra);
		#endif
	}


	if(found==1){
	        pthread_exit((void*)1);
    	}
	}
	pthread_exit((void*)0);

}

// https://www.mitchr.me/SS/exampleCode/AUPG/pcre_example.c.html
// vu le 30 Oct 2018
