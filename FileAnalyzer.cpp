#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include "FileAnalysis.hpp"
int main(int argc,char** argv) {
    if (argc==1) {
        perror("Error: No file to analyze");
        return 1;
    } else if (argc==2) {
        return Program(argv[1]);
    } else {
        perror("Error: Can only analyze one file");
    }
}
int Program(char* File) {
    access((const char*)File,F_OK);
    struct stat FileStat;
    stat(File,&FileStat);
    Analysis analysisoffile;
    realpath(File,(char*)analysisoffile.filename);
    analysisoffile.bytes=FileStat.st_size;
    analysisoffile.blocks=FileStat.st_blocks;
    analysisoffile.blocksize=FileStat.st_blksize;
    analysisoffile.filetype=FindFileType(FileStat.st_mode);
    analysisoffile.DeviceNumber=FileStat.st_dev;
    analysisoffile.inodenumber=FileStat.st_ino;
    analysisoffile.NumberofHardLinks=FileStat.st_nlink;
    char* Username;
    getlogin_r(Username,SIZE_MAX);
    passwd* UserData=getpwnam(Username);
};
char FindFileType(mode_t FileStatMode) {
    if (S_ISREG(FileStatMode)) {
        return 0;
    } else if (S_ISDIR(FileStatMode)) {
        return 1;
    } else if (S_ISBLK(FileStatMode)) {
        return 2;
    } else if (S_ISCHR(FileStatMode)) {
        return 3;
    } else if (S_ISFIFO(FileStatMode)) {
        return 4;
    } else if (S_ISLNK(FileStatMode)) {
        return 5;
    } else {
        return 6;
    }
}