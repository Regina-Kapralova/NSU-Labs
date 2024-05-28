#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

char* inPath;
char* outPath;

int isFile(struct stat buff){
    return S_ISREG(buff.st_mode);
}
int isDirectory(struct stat buff){
    return S_ISDIR(buff.st_mode);
}

char* connectPaths(char* firstPath, char* secondPath){
    char* currentPath = (char*)malloc(strlen(firstPath) + strlen(secondPath) + 2);
    memcpy(currentPath, firstPath, strlen(firstPath));
    memcpy(currentPath + strlen(firstPath), "/", 1);
    memcpy(currentPath + strlen(firstPath) + 1, secondPath, strlen(secondPath) + 1);
    return currentPath;
}

int openInFile(char* path){
    int status = -1;
    while (status < 0){
        status = open(path, O_RDONLY);
        if ((status < 0) && (errno == EMFILE)){
            printf("Waiting opening file %s ...\n", path);
            sleep(1);
        }
        else if ((status < 0) && (errno == ENFILE)){
            printf("Waiting opening %s ...\n", path);
            sleep(1);
        }
        else
            return status;
    }
    return status;
}

int createOutFile(char* inPath, char* outPath){
    struct stat buff;
    if (stat(inPath, &buff) == -1){
        printf("Error: Can't get information from path %s\n", inPath);
        return -1;
    }
    int status = -1;
    while (status < 0){
        status = open(outPath, O_WRONLY | O_CREAT, buff.st_mode);
        if ((status < 0) && (errno == EMFILE)){
            printf("Waiting opening %s ...\n", outPath);
            sleep(1);
        }
        else if ((status < 0) && (errno == ENFILE)){
            printf("Waiting opening %s ...\n", outPath);
            sleep(1);
        }
        else
            return status;
    }
    return status;
}

int createOutDirectory(char* inPath, char* outPath){
    struct stat buff;
    if (stat(inPath, &buff) == -1){
        printf("Error: Can't get information from path %s\n", inPath);
        return -2;
    }
    int makeDirStatus = mkdir(outPath, buff.st_mode);
    if ((makeDirStatus == -1) && (errno == EEXIST))
        printf("Error: The named file exists.\n");
    else if ((makeDirStatus == -1) && (errno == EACCES))
        printf("Error: Access denided\n");
    else if ((makeDirStatus == -1) && (errno == ENOSPC))
        printf("Error: No memory to create directory.\n");
    return  makeDirStatus;
}

DIR* openInDirectory(char* path){
    DIR* openDirectory = NULL;
    while (openDirectory == NULL){
        openDirectory = opendir(path);
        if ((openDirectory == NULL) && (errno == EACCES)){ //доступ запрещен
            printf("Error: Access is denied %s\n", path);
            break;
        }
        else if ((openDirectory == NULL) && (errno == EMFILE)) //слишком много потоков
            sleep(1);
        else if ((openDirectory == NULL) && (errno == ENFILE)) //слишком много потоков
            sleep(1);
        else if ((openDirectory == NULL) && (errno == ENOMEM)) //недостаточно памяти
            sleep(1);
        else if (openDirectory == NULL) //ENOENT каталога не существует или ENOTDIR name не каталог
            break;
    }
    return openDirectory;
}

pthread_t newThreadCreate(void* (*function)(void*), void* param) {
    pthread_t thread;
    int code = -1;
    while (code != 0){
        code = pthread_create(&thread, NULL, function, param);
        if (code == EAGAIN){
            printf("Waiting resources to create new thread...\n");
            sleep(1);
        }
    }
    pthread_detach(thread);
    return thread;
}

void* copyFile(void* param) {
    char* getPath = (char*)param;
    char* fullInPath = connectPaths(inPath, getPath);
    char* fullOutPath = connectPaths(outPath, getPath);
    free(param);
    int inFile = openInFile(fullInPath);
    if (inFile == -1){
        printf("Error: Can't open %s\n", fullInPath);
        free(fullInPath);
        free(fullOutPath);
        pthread_exit(NULL);
    }
    int outFile = createOutFile(fullInPath, fullOutPath);
    if (outFile == -1){
        printf("**Error: Can't open %s\n", fullOutPath);
        free(fullInPath);
        free(fullOutPath);
        pthread_exit(NULL);
    }
    int readStatus = 1;
    int writeStatus;
    char* buf = (char*)malloc(512);
    while (readStatus > 0){
        if ((readStatus = read(inFile, buf, 512)) == -1){
            printf("Error: Read error %s\n", fullInPath);
            free(buf);
            if (close(inFile) == -1)
                printf("Error: Can't close file %s\n", fullInPath);
            if (close(outFile) == -1)
                printf("Error: Can't close file %s\n", fullOutPath);
            free(fullInPath);
            free(fullOutPath);
            pthread_exit(NULL);
        }
        if (write(outFile, buf, 512) == -1) {
            printf("Error: Write error %s\n", fullOutPath);
            free(buf);
            if (close(inFile) == -1)
                printf("Error: Can't close file %s\n", fullInPath);
            if (close(outFile) == -1)
                printf("Error: Can't close file %s\n", fullOutPath);
            free(fullInPath);
            free(fullOutPath);
            pthread_exit(NULL);
        }
    }
    printf("Copied %s ...\n", fullInPath);
    free(fullInPath);
    free(fullOutPath);
    if (close(inFile) == -1)
        printf("Error: Can't close file %s\n", fullInPath);
    if (close(outFile) == -1)
        printf("Error: Can't close file %s\n", fullOutPath);
    free(buf);
    pthread_exit(NULL);
}

void* copyDirectory(void* param){
    char* getPath = (char*)param;
    char* fullInPath = connectPaths(inPath, getPath);
    char* fullOutPath = connectPaths(outPath, getPath);
    char* spaceForDirent[sizeof(struct dirent) + pathconf(fullInPath, _PC_NAME_MAX) + 2];
    struct dirent* unknownFile = (struct dirent*)spaceForDirent;
    struct dirent* resultOfFile;
    struct stat buff;
    DIR* openDirectory = openInDirectory(fullInPath);
    if (openDirectory == NULL){
        printf("Error: Can't open directory %s\n", fullInPath);
        free(fullInPath);
        free(fullOutPath);
        free(param);
        pthread_exit(NULL);
    }
    if (createOutDirectory(fullInPath, fullOutPath) == -2){
        printf("Exist!\n");
        free(fullInPath);
        free(fullOutPath);
        free(param);
        pthread_exit(NULL);
    }
    while ((readdir_r(openDirectory, unknownFile, &resultOfFile) == 0) && (resultOfFile != NULL)) {
        if ((strcmp(unknownFile->d_name, ".") == 0) || (strcmp(unknownFile->d_name, "..") == 0))
            continue;
        char* newFullPath = connectPaths(fullInPath, unknownFile->d_name);
        if (stat(newFullPath, &buff) == -1) {
            printf("Error: Can't get information from path %s\n", newFullPath);
            continue;
        }
        free(newFullPath);
        pthread_t thread;
        char* newCurrentPath = connectPaths(getPath, unknownFile->d_name);
        if (isDirectory(buff))
            thread = newThreadCreate(copyDirectory, newCurrentPath);
        else if (isFile(buff))
            thread = newThreadCreate(copyFile, newCurrentPath);
    }
    printf("Copied %s ...\n", fullInPath);
    if (closedir(openDirectory) == -1)
        printf("Error: Can't close directory %s\n", fullInPath);
    free(fullInPath);
    free(fullOutPath);
    free(param);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t thread;
    struct stat buff;
    if (argc < 3) {
        printf("Usage two arguments: path floder \"from\" and \"to\"\n", argv[0]);
        return 0;
    }
    inPath = argv[1];
    outPath = argv[2];
    if (stat(inPath, &buff) == -1) {
        printf("Error: Can't get information about path %s\n", inPath);
        return 0;
    }
    if (isDirectory(buff))
        thread = newThreadCreate(copyDirectory, (void*)(""));
    else
        printf("Error: Can't find directory to copy\n");
    pthread_exit(NULL);
}
// NAME_MAX = 255  PATH_MAX = 1024 СТАНДАРТ XSI
