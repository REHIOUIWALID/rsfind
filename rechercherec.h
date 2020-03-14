#ifndef _rechercherec_
#define _rechercherec_
#include <pcre.h>
#include <glob.h>


#include <dirent.h>

//NORMAL ET NAME
int recherche(char* argv, char* name, char **table);
int recursive(DIR* dir,char* path,char* cherche, char **table);


char* format(char* arg);


//PERL
int recherchePerll(char* argv, char* ename, char** table,int nbFich);
int recursivePerll(DIR* dir,char* path,char **table,pcre* reCompiled,pcre_extra* pcreExtra,int* substrVec);


//ENAME
int tiretEname(char* argv,char* ename,char**table,int nbfich);
int recursiveEname(char* path,char** table,char* ename,glob_t* globbuff);

#endif
