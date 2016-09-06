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
#include <time.h>

#include "ls.h"

char* get_file_name(FILE * file);
void print_file_info(char* file_name);

void list_files()
{
    DIR* FD;
    struct dirent* in_file;
    FILE* entry_file;
    char  buffer[BUFSIZ];
    char* file_name;
    char* file_permissions;
    char* file_owner;
    char* file_info;

    /* Scanning the in directory */
    if (NULL == (FD = opendir ("."))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));

        return;
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

            return;
        }

        file_name = get_file_name(entry_file);
        // file_permissions = get_file_permissions(file_name);
        // file_owner = get_file_owner(file_name);
        print_file_info(file_name);

        printf("%s\n", file_name);

        free(file_info);
        // free(file_name);
        // free(file_permissions);
        // free(file_owner);

        fclose(entry_file);
    }
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

void print_file_info(char* file_name) 
{
    struct stat file_info;
    struct passwd *pw;
    struct group  *gr;
    struct tm *tmp;
    char buffer[26];

    stat(file_name, &file_info);

    printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
    printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_info.st_mode & S_IROTH) ? "r" : "-");
    printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_info.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    printf("%d ", file_info.st_nlink);

    pw = getpwuid(file_info.st_uid);
    gr = getgrgid(file_info.st_gid);
    printf("%s %s ", pw->pw_name, gr->gr_name);

    printf("%d ", (int) file_info.st_size);

    strftime(buffer, sizeof (buffer), "%b  %d %H:%M", localtime(&file_info.st_mtime));
    printf("%s ", buffer);
}

void print_list_files_usage() 
{
	printf("Usage: ls -l \n");
}