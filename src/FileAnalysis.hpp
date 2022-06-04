#include <stdbool.h>
#include <time.h>
#include <string>
#include <map>
using namespace std;
class Analysis {
    public:
        const char* filename;
        size_t bytes;
        size_t blocks;
        size_t blocksize;
        char filetype;
        unsigned long DeviceNumber;
        unsigned long inodenumber;
        unsigned long NumberofHardLinks;
        bool Readable;
        bool Writeable;
        bool Executable;
        timespec AccessedLast;
        timespec ModifiedLast;
        timespec ChangedLast;
        uid_t OnwerId;
        gid_t GroupId;
        map<string,string> InodeTableData;
};