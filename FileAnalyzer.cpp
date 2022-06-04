#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include "FileAnalysis.hpp"
#include "RegFileAnalysis.hpp"
int main(int argc,char** argv) {
    if (argc==1) {
        perror("Error: No file to analyze");
        return 1;
    } else if (argc==2) {
        if (argv[1]=="-h"||argv[1]=="--help") {
            printf("%s","Usage: <flag?> <file>\n -h|--help Display this information and then exit\n -v|--version Display the version number\n-g|--gui Display this information as a gui");
            return 0;
        } else if (argv[1]=="-v"||argv[1]=="--version") {
            printf("%s","GNU FileAnalyzer 1.0.0");
            return 0;
        } else {
            return Program(argv[1],false);
        }
    } else {
        perror("Error: Can only analyze one file");
    }
}
int Program(char* File,bool GUI) {
    if (access((const char*)File,F_OK)) {
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
        analysisoffile.Readable=access(File,R_OK);
        analysisoffile.Writeable=access(File,W_OK);
        analysisoffile.Executable=access(File,X_OK);
        analysisoffile.AccessedLast=FileStat.st_atim;
        analysisoffile.ModifiedLast=FileStat.st_mtim;
        analysisoffile.ChangedLast=FileStat.st_ctim;
        analysisoffile.OnwerId=FileStat.st_uid;
        analysisoffile.GroupId=FileStat.st_gid;
        if (analysisoffile.filetype==0) {
            uint8_t* FileContents;
            FILE* fp=fopen(File,"rb");
            fread(FileContents,analysisoffile.bytes,sizeof(uint8_t),fp);
            fclose(fp);
            printf("Filename: %s\n",analysisoffile.filename);
            printf("Amount of Bytes: %s\n",analysisoffile.bytes);
            printf("Amount of Blocks: %s\n",analysisoffile.blocks);
        }
    } else {
        perror("Error: File not accessible");
        return 1;
    }
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