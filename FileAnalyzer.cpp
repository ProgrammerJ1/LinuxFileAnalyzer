#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "FileAnalysis.hpp"
int main(int argc,char** argv) {
    if (argc==1) {
        perror("Error: No file to analyze");
        return 1;
    } else if (argc==2) {
        Program();
        return 0;
    } else {
        perror("Error: Can only analyze one file");
    }
}
int Program(char* File) {
    struct stat FileStat;
    stat(File,&FileStat);
    Analysis analysisoffile;
    realpath(File,(char*)analysisoffile.filename);
    analysisoffile.bytes=FileStat.st_size;
}