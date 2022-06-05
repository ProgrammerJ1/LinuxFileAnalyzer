#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include "FileAnalysis.hpp"
#include "MonthTable.hpp"
#include <WeekTable.hpp>
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
        printf("Filename: %s\n",analysisoffile.filename);
        printf("Amount of Bytes: %u\n",analysisoffile.bytes);
        printf("Amount of Blocks: %u\n",analysisoffile.blocks);
        printf("Block Size: %u\n",analysisoffile.blocksize);
        switch (analysisoffile.filetype) {
            case 0:
                printf("%s","File Type: Regular\n");
            case 1:
                printf("%s","File Type: Directory");
            case 2:
                printf("%s","File Type: Block Device");
            case 3:
                printf("%s","File Type: Character Device");
            case 4:
                printf("%s","File Type: Pipe");
            case 5:
                printf("%s","File Type: Link");
            case 6:
                printf("%s","File Type: Socket");
        }
        printf("Device Id: 0x%X\n",analysisoffile.DeviceNumber);
        printf("Inode Number: %u",analysisoffile.inodenumber);
        printf("Amount of Hard Links: %u",analysisoffile.NumberofHardLinks);
        printf("%s","Permissions: ");
        uint8_t AmountofPerms=0;
        if (analysisoffile.Readable) {
            printf("%s","Readable");
            AmountofPerms+=1;
        } else if (analysisoffile.Writeable) {
            if (AmountofPerms>1) {
                printf("%s",", Writable");
            } else {
                printf("%s","Writeable");
                AmountofPerms+=1;
            }
        } else if (analysisoffile.Executable) {
            if (AmountofPerms>1) {
                printf("%s",", Executable");
            } else {
                printf("Executable");
            }
        }
        AmountofPerms=NULL;
        printf("%c",'\n');
        struct tm TrueAccessTime;
        localtime_r(&analysisoffile.AccessedLast.tv_sec,&TrueAccessTime);
        printf("Last time accessed: %s, %s %s, %u %u:%u:%u %s\n",getWeekTable().find(TrueAccessTime.tm_wday)->second,getMonthTable().find(TrueAccessTime.tm_mon),FormatNumbertoDateNumber(TrueAccessTime.tm_mday),TrueAccessTime.tm_year+1900,TrueAccessTime.tm_hour,TrueAccessTime.tm_min,TrueAccessTime.tm_sec,TrueAccessTime.tm_zone);
        struct tm TrueModifiyTime;
        localtime_r(&analysisoffile.ModifiedLast.tv_sec,&TrueModifiyTime);
        printf("Last time modified: %s, %s %s, %u %u:%u:%u %s\n",getWeekTable().find(TrueModifiyTime.tm_wday)->second,getMonthTable().find(TrueModifiyTime.tm_mon),FormatNumbertoDateNumber(TrueModifiyTime.tm_mday),TrueModifiyTime.tm_year+1900,TrueModifiyTime.tm_hour,TrueModifiyTime.tm_min,TrueModifiyTime.tm_sec,TrueModifiyTime.tm_zone);
        struct tm TrueChangeTime;
        localtime_r(&analysisoffile.ChangedLast.tv_sec,&TrueChangeTime);
        printf("Last time changed: %s, %s %s, %u %u:%u:%u %s\n",getWeekTable().find(TrueChangeTime.tm_wday)->second,getMonthTable().find(TrueChangeTime.tm_mon),FormatNumbertoDateNumber(TrueChangeTime.tm_mday),TrueChangeTime.tm_year+1900,TrueChangeTime.tm_hour,TrueChangeTime.tm_min,TrueChangeTime.tm_sec,TrueChangeTime.tm_zone);
        struct tm TrueModifiyTime;
        printf("%s","Would you like to print file type specific information? [Y/n]");
        char Choice=getchar();
        bool SpecialInfo;
        switch (Choice) {
            case 'Y':
                SpecialInfo=true;
            case defualt:
                SpecialInfo=false;
        }
        if (SpecialInfo) {
            
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
char* FormatNumbertoDateNumber(uint8_t Number) {
    char* StrNum;
    sprintf(StrNum,"%u",(short)Number);
    if ((Number-1)%10==0) {
        strcat(StrNum,"st");
    } else if ((Number-2)%10==0) {
        strcat(StrNum,"nd");
    } else if ((Number-3)%10==0) {
        strcat(StrNum,"rd");
    } else {
        strcat(StrNum,"th");
    }
    return StrNum;
}