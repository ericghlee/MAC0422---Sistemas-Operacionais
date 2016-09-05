#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>

char* get_file_name(FILE * file);
char* get_file_permissions(char* file_name);
char* get_file_owner(char* file_name);

int main(int argc, char **argv)
{
    DIR* FD;
    struct dirent* in_file;
    FILE* entry_file;
    char  buffer[BUFSIZ];
    char* file_name;
    char* file_permissions;
    char* file_owner;

    /* Scanning the in directory */
    if (NULL == (FD = opendir ("."))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));

        return 1;
    }

    while ((in_file = readdir(FD))) 
    {
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;

        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        entry_file = fopen(in_file->d_name, "r");
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));

            return 1;
        }

        file_name = get_file_name(entry_file);
        file_permissions = get_file_permissions(file_name);
        file_owner = get_file_owner(file_name);
        printf("%s %s %s\n",file_permissions, file_owner, file_name);

        // /* Doing some struf with entry_file : */
        // /* For example use fgets */
        // while (fgets(buffer, BUFSIZ, entry_file) != NULL)
        // {
        //     /* Use fprintf or fwrite to write some stuff into common_file*/
        // }

        // /* When you finish with the file, close it */
        free(file_permissions);
        free(file_owner);
        free(file_name);
        fclose(entry_file);
    }

    return 0;
}

char* get_file_name(FILE * file) 
{
    char path[1024];
    char aux[1024];
    int fd = fileno(file); 

    sprintf(path, "/proc/self/fd/%d", fd);
    memset(aux, 0, sizeof(aux));
    readlink(path, aux, sizeof(aux)-1);

    char* result = malloc(sizeof(basename(aux)));
    strcpy(result, basename(aux));

    return result;
}

char* get_file_permissions(char* file_name) 
{
    struct stat file_info;
    stat(file_name,&file_info);
    char* permissions = malloc(sizeof(char) * 10);

    strcat(permissions, (S_ISDIR(file_info.st_mode)) ? "d" : "-");
    strcat(permissions, (file_info.st_mode & S_IRUSR) ? "r" : "-");
    strcat(permissions, (file_info.st_mode & S_IWUSR) ? "w" : "-");
    strcat(permissions, (file_info.st_mode & S_IXUSR) ? "x" : "-");
    strcat(permissions, (file_info.st_mode & S_IRGRP) ? "r" : "-");
    strcat(permissions, (file_info.st_mode & S_IWGRP) ? "w" : "-");
    strcat(permissions, (file_info.st_mode & S_IXGRP) ? "x" : "-");
    strcat(permissions, (file_info.st_mode & S_IROTH) ? "r" : "-");
    strcat(permissions, (file_info.st_mode & S_IWOTH) ? "w" : "-");
    strcat(permissions, (file_info.st_mode & S_IXOTH) ? "x" : "-");

    return permissions;
}

char* get_file_owner(char* file_name) 
{
    struct stat file_info;
    stat(file_name,&file_info);
    struct passwd *pw;
    struct group  *gr;
    pw = getpwuid(file_info.st_uid);
    gr = getgrgid(file_info.st_gid);

    char* result = malloc(sizeof(pw->pw_name) + sizeof(gr->gr_name) + 1);
    strcat(result, pw->pw_name);
    strcat(result, " ");
    strcat(result, gr->gr_name);

    return result;
}