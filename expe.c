#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
On suppose que commande ainsi : cmde1 | cmde 2 {}
	-> ou cmde1 | cmde 2 
*/



// Renvoie le nombre d'options + 1
	// grep -r -l renverrait 3 par exemple
int getNbOptCmd(char* cmde){

	int nb=0;//nombre options + commande
	char* ptr=cmde;
	while(*ptr!='\0'){
		while(*ptr!='\0' && *ptr==' '){
			ptr++;
		}
		while(*ptr!='\0' && *ptr!=' '){
			ptr++;
		}
		nb++;
		while(*ptr!='\0' && *ptr==' '){
			ptr++;
		}

	}
	return nb;
}
// Renvoie un tableau ou chaque case correspond à la commande puis les options. Se termine par 0.
	// grep -r -l donnerait {grep,-r,-l,0}
char** tabCmdeOpt(char* cmde, int nb){

	char** ret;
	ret=(char**)malloc(sizeof(char*)*(nb+1));
	char* ptrArr=cmde;
	char* ptrCurr=cmde;
	int ind=0;
	int deplct=0;
	while(*ptrCurr!='\0'){
		while(*ptrCurr!='\0' && *ptrCurr==' '){
			ptrCurr++;
		}
		ptrArr=ptrCurr;
		while(*ptrCurr!='\0' && *ptrCurr!=' '){
			ptrCurr++;
		}
		ret[ind]=(char*)malloc(sizeof(char)*(ptrCurr-ptrArr));
		while(ptrArr!=ptrCurr){
			*(ret[ind]+deplct)=*(ptrArr);
			ptrArr++;
			deplct++;
		}
		ind++;
		deplct=0;
		while(*ptrCurr!='\0' && *ptrCurr==' '){
			ptrCurr++;
		}
		ptrArr=ptrCurr;
		
	}
	for(int i=0 ; i<nb ; i++){
		printf("\t %s \n",ret[i]);
	}
	ret[nb]=NULL;
	return ret;

}

char** tabCmdeOptFirstSon(char* file,char* cmde,int nb){
	
	char** ret;
	ret=(char**)malloc(sizeof(char*)*(nb));
	char* ptrArr=cmde;
	char* ptrCurr=cmde;
	int ind=0;
	int deplct=0;
	int calcul=0;

	while(*ptrCurr!='\0'){
		while(*ptrCurr!='\0' && *ptrCurr==' '){
			ptrCurr++;
		}
		ptrArr=ptrCurr;
		while(*ptrCurr!='\0' && *ptrCurr!=' '){
			ptrCurr++;
		}
		calcul = ptrCurr-ptrArr;
		//printf("deplacement %d", ptrCurr-ptrArr );
		ret[ind]=(char*)malloc(sizeof(char)*(ptrCurr-ptrArr)+1);
		for(deplct = 0; deplct < calcul; deplct ++){
			
			*(ret[ind]+deplct)=*(ptrArr);
			//printf("charactère %c", *(ret[ind]+deplct));
			ptrArr ++;
		}
		ret[ind][calcul] = '\0';
		ind++;
		deplct=0;
		while(*ptrCurr!='\0' && *ptrCurr==' '){
			ptrCurr++;
		}
		ptrArr=ptrCurr;
		
	}
	ret[nb]=(char*)malloc(sizeof(char)*strlen(file));
	strcpy(ret[nb],file);
	ret[nb+1]=NULL;
	return ret;

}




///////////////////////////////////////////////////////////////////////////////



void exec(/*char** path,*/char* argument,char* file/*,int nbfiles*/){

	// I Nombre de pipes
	int nbpip=0;
	char* ptr=argument;
	while(*ptr!='\0' && *ptr!='{'){
		if(*ptr=='|'){
			nbpip++;
		}
		ptr++;
	}
	//printf(" nbpip %d \n",nbpip);
////////////////////////////////////////////////////////////////////////////////

	// II Suite de commandes dans un tableau
	char** tabCommandes;
	tabCommandes=(char**)malloc(sizeof(char*)*nbpip+1);
	ptr=argument;
	char* deb;//debut mot
	deb=ptr;
	int nb;//taille mot 
	nb=0;
	for(int i=0;i<nbpip;i++){
		while(*ptr!='|'){
			ptr++;
			nb++;
		}
		tabCommandes[i]=(char*)malloc(sizeof(char)*nb+1);
		for(int j=0 ; j<nb ; j++){
			*(tabCommandes[i]+j)=*(deb+j);
		}//recopie la commande dans le tableau
		tabCommandes[i][nb]='\0';
		nb=0;
		ptr++;
		deb=ptr;
	}
	//Dernière commande de la ligne
	while(*ptr!='\0'){
		ptr++;
		nb++;
	}

	tabCommandes[nbpip]=(char*)malloc(sizeof(char)*nb+1);
	for(int j=0 ; j<nb+1 ; j++){
		*(tabCommandes[nbpip]+j)=*(deb+j);
	}
	//Fonctionnne
	
	int a = 0;
  ///////////////////////////////////////////////////////////////////////
		char ** tab;

  //////////////////////////////////////////////////////////////////////
	if (nbpip==0){
		if(fork()==0){
			nb=getNbOptCmd(tabCommandes[0]);
			//printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d \n", nb);
			tab=tabCmdeOptFirstSon(file,tabCommandes[0],nb);
			//printf("tab : %s \n", tab[0]);
			execvp(tab[0],tab);

			exit(1);
		}
		else{
			wait(NULL);
		}
	}
	//printf("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	if(nbpip>0){

		int fd[nbpip][2];
		for (int i = 0; i<nbpip; i++){
			pipe(fd[i]);
		}


		for (int i = nbpip; i>=0; i--){
			wait(NULL);
			
			if(fork()==0){
				if(i == 0){
				close(STDOUT_FILENO);
				dup2(fd[0][1],1);
				if (a == 0){wait(NULL); a=1;}
				
				}
				else if(i == nbpip){
				close(STDIN_FILENO);
				dup2(fd[i-1][0],0);
				
				}
				else{
				
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				dup2(fd[i-1][0],0);
				dup2(fd[i][1],1);

				}

				for (int j = 0; j<nbpip; j++){
					close(fd[j][0]);
					close(fd[j][1]);
				}




				nb=getNbOptCmd(tabCommandes[0]);
				tab=tabCmdeOptFirstSon(file,tabCommandes[0],nb);
				execvp(tab[0],tab);

				exit(1);

			}
			




			
		}

		for (int j = 0; j<nbpip; j++){
					close(fd[j][0]);
					close(fd[j][1]);
		}
		wait(NULL);

			// IV Free
		for(int i=0 ; i<nbpip ; i++){
			free(tabCommandes[i]);
		}
		free(tabCommandes);

	}

}





int execmain(char** path,char* argument, int nb_fichier){
	//argument = "ls -l";
	char* arg;
	for(int i=0; i<nb_fichier; i++ ){
		arg=(char*)malloc(sizeof(char)*strlen(argument)+1);
		//argument[2]='\0';
		strcpy(arg,argument);
		arg[strlen(argument)]='\0';
		exec(arg,path[i]);
		free(arg);
	}

	return 0;
	
}
