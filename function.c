#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void rm_handler(char *buffer) {
    char *file_name = buffer + 7;

    if (strlen(file_name) == 0) {
        perror("Error: No file name provided.\n");
        return;
    }

    if (remove(file_name) == 0) {
        printf("Remove successfuly. \n");
    } else {
        perror("removed failed");
    }
}

void rmdir_handler(char *buffer) {
    char *dir_name = buffer + 6;

    if (strlen(dir_name) == 0) {
        perror("Error: No directory name provided. \n");
        return;
    } 

    if (rmdir(dir_name) == -1) {
        perror("mkdir failed");
    } else {
        printf("Direoctory '%s' removed successfully. \n", dir_name);
    }
}

void mkdir_handler(char *buffer) {
    char *dir_name = buffer + 6;

    if (strlen(dir_name) == 0) {
        perror("Error: No directory name provided. \n");
        return;
    } 

    if (mkdir(dir_name, 0777) == -1) {
        perror("mkdir failed");
    } else {
        printf("Direoctory '%s' created successfully. \n", dir_name);
    }
}

void pwd_handler() {
    // I know it's easier to get here thanks to getcwd, but it's more interesting this way :)
    char cwd[1024];

    // /proc/self/cwd is a symbolic link that points to the current working directory of the process requesting this information.
    ssize_t len = readlink("/proc/self/cwd", cwd, sizeof(cwd)-1);

    if (len == -1) {
        perror("readlink failed");
        return;
    }

    cwd[len] = '\0';
    printf("%s\n", cwd);
}

void cat_handler(char *buffer) {
    char *file_name = buffer + 4;
    FILE *file = fopen(file_name, "r");
    char data[1024];

    if (file == NULL) {
        perror("Error opening file.");
        return;
    }

    while (fgets(data, sizeof(data), file) != NULL) {
        printf("%s", data);
    }

    fclose(file);
}

void ls_handler() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main() {
    char buffer[100];

    // Shell functionality is just a formality here.
    while (1) {
        printf("myshell> ~ ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strstr(buffer, "ls") != NULL && strlen(buffer) == 2) {
            ls_handler();
        } else if (strstr(buffer, "cat") != NULL) {
            cat_handler(buffer);
        } else if (strstr(buffer, "pwd") != NULL) {
            pwd_handler();
        } else if (strstr(buffer, "mkdir") != NULL) {
            mkdir_handler(buffer);
        } else if (strstr(buffer, "rmdir") != NULL) {
            rmdir_handler(buffer);
        } else if (strstr(buffer, "rm") != NULL) {
            rm_handler(buffer);
        }
    }

    return 0;
}
