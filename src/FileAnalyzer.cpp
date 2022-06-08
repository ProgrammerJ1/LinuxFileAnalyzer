#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include "FileAnalysis.hpp"
#include "MonthTable.hpp"
#include "WeekTable.hpp"
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <regex>
#include <sstream>
#include <blkid/blkid.h>
#include <algorithm>
#include <linux/kdev_t.h>
using namespace std;
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
        if (GUI) {
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
            printf("Group Id Owner: %u",analysisoffile.GroupId);
            printf("Owner ID: %u",analysisoffile.OnwerId);
            printf("%s","Would you like to print file type specific information? [Y/n]");
            char Choice=getchar();
            bool SpecialInfo;
            switch (Choice) {
                case 'Y':
                    SpecialInfo=true;
                default:
                    SpecialInfo=false;
            }
            if (SpecialInfo) {
                switch (analysisoffile.filetype) {
                    case 0:
                        {
                            uint8_t* Contents;
                            FILE* fp=fopen(File,"rb");
                            fread(Contents,1,FileStat.st_size,fp);
                            fclose(fp);
                            printf("%s","File Contents: ");
                            for (size_t i=0;i<sizeof(Contents);i++) {
                                printf("%X",Contents[i]);
                                if (i+1!=sizeof(Contents)) {
                                    printf("%c",' ');
                                }
                            }
                            break;
                        }
                    case 1:
                        {
                            printf("%s","Directory Contents\n________________");
                            DIR *d;
                            struct dirent *dir;
                            d=opendir(File);
                            if (d) {
                                while ((dir = readdir(d)) != NULL) {
                                    printf("%s",dir->d_name);
                                }
                            }
                            closedir(d);
                        }
                    case 2:
                      {
                            struct stat BlockDeviceStats;
                            stat(File,&BlockDeviceStats);
                            dev_t BlockDeviceId=BlockDeviceStats.st_rdev;
                            char* BlockDevName=blkid_devno_to_devname(BlockDeviceId);
                            printf("Block Device Name: %s",BlockDevName);
                            printf("Block Device Major Number: %u",MAJOR(BlockDeviceId));
                            printf("Block Device Minor Number: %u",MINOR(BlockDeviceId));
                            char* BlockDevInfoPath="/sys/block/";
                            strcat(BlockDevInfoPath,BlockDevName);
                            char* BlockDevRemovableFlagFilePath;
                            memcpy(BlockDevRemovableFlagFilePath,BlockDevInfoPath,sizeof(BlockDevInfoPath));
                            strcat(BlockDevRemovableFlagFilePath,"/removeable");
                            char BlockDevRemovableFlag;
                            FILE* BlockDevRemovableFlagFile=fopen(BlockDevRemovableFlagFilePath,"r");
                            fread(&BlockDevRemovableFlag,sizeof(char),1,BlockDevRemovableFlagFile);
                            if (BlockDevRemovableFlag=='0') {
                                printf("%s","Block Device Removable: No");
                            } else {
                                printf("%s","Block Device Removable: Yes");
                            }
                            fclose(BlockDevRemovableFlagFile);
                            blkid_loff_t BlockDevRemovableSize=blkid_get_dev_size(fileno(BlockDevRemovableFlagFile));
                            printf("Block Device Size: %u",BlockDevRemovableSize);
                            char* BlockDevTypeFlagFilePath;
                            memcpy(BlockDevTypeFlagFilePath,BlockDevInfoPath,sizeof(BlockDevInfoPath));
                            FILE* BlockDevTypeFlagFile=fopen(BlockDevTypeFlagFilePath,"rb");
                            uint8_t BlockDevTypeFlag;
                            fread(&BlockDevTypeFlag,sizeof(char),1,BlockDevTypeFlagFile);
                            switch (BlockDevTypeFlag) {
                                case 0:
                                    printf("%s","Block Device Type: Disk");
                                    break;
                                case 1:
                                    printf("%s","Block Device Type: Tape Storage");
                                    break;
                                case 2:
                                    printf("%s","Block Device Type: Printer");
                                    break;
                                case 3:
                                    printf("%s","Block Device Type: HP Scanner Processor");
                                    break;
                                case 4:
                                    printf("%s","Block Device Type: Write Once Read Many Memory");
                                    break;
                                case 5:
                                    printf("%s","Block Device Type: Read Only Memory");
                                    break;
                                case 6:
                                    printf("%s","Block Device Type: Scanner");
                                    break;
                                case 7:
                                    printf("%s","Block Device Type: Magnetic Optical disk");
                                    break;
                                case 8:
                                    printf("%s","Block Device Type: Medium Changer");
                                    break;
                                case 9:
                                    printf("%s","Block Device Type: Communications Device");
                                    break;
                                case 0x0c:
                                    printf("%s","Block Device Type: Raid");
                                    break;
                                case 0x0e:
                                    printf("%s","Block Device Type: RBC");
                                    break;
                                case 0x7f:
                                    printf("%s","Block Device Type: No Lun");
                                    break;
                                case default:
                                    printf("%s","Block Device Type: Other");
                            }
                            FILE* MountListFile=fopen("/proc/mounts","r")
                            char* MountListCharArr;
                            struct stat MountListFileStats;
                            stat("/proc/mounts",MountListFileStats);
                            size_t MountListSize=MountListFileStats.st_size;
                            fread(MountListCharArr,sizeof(char),MountListSize,MountListFile);
                            string MountListString=MountList;
                            replace(MountListString.begin(),MountListString.end(),'\n','\0');
                            char** MountList;
                            memcpy(MountListString.c_str(),MountListSize,MountList);
                    }
                }
            }
            return 0;
        }
    } else {
        perror("Error: File not accessible");
        return 1;
    }
};
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
