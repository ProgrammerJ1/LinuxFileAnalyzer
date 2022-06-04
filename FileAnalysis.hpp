#include <cstddef>
#include <cstdbool>
#include <ctime>
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
        short int DeviceNumber;
        int inodenumber;
        short int NumberofHardLinks;
        bool Readable;
        bool Writeable;
        bool Executable;
        time_t AccessedLast;
        time_t ModifiedLast;
        time_t ChangedLast;
        time_t BirthDate;
        map<string,string> InodeTableData;
};