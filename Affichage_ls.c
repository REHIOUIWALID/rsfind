#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

void permission(mode_t perms){
        
        if (perms & S_IFDIR){printf("d");}else{printf("-");}
        if (perms & S_IRUSR){printf("r");}else{printf("-");}
        if (perms & S_IWUSR){printf("w");}else{printf("-");}
        if (perms & S_IXUSR){printf("x");}else{printf("-");}
        if (perms & S_IRGRP){printf("r");}else{printf("-");}
        if (perms & S_IWGRP){printf("w");}else{printf("-");}
        if (perms & S_IXGRP){printf("x");}else{printf("-");}
        if (perms & S_IROTH){printf("r");}else{printf("-");}
        if (perms & S_IWOTH){printf("w");}else{printf("-");}
        if (perms & S_IXOTH){printf("x");}else{printf("-");}

    }

void Affichage(char* file ){

    
    struct stat stats;
    stat(file, &stats);
                   
    int size = stats.st_size;
    mode_t st_mode=stats.st_mode;
    nlink_t st_nlink=stats.st_nlink;
    uid_t st_uid=stats.st_uid;
    gid_t st_gid=stats.st_gid;
    time_t timem= stats.st_mtime;
    char date[36];
    
    strftime(date, 36, "%b %e %H:%M", localtime(&timem));
    
   
    permission(st_mode);

    //
    printf(" %d", (int)st_nlink);
    printf(" %s", getpwuid(st_uid)->pw_name);
    printf(" %s", getgrgid(st_gid)->gr_name);
    if (st_mode & S_IFDIR){printf(" 4096");}else{printf(" %d", size );}
    
    printf(" %s", date);
    printf(" %s\n", file);

}
    
