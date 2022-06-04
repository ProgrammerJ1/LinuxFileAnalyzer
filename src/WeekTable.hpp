#include <map>
#include <cstdint>
using namespace std;
map<uint8_t,char*> getWeekTable() {
    map<uint8_t,char*> weekTable{{0,"Sunday"},{1,"Monday"},{2,"Tuesday"},{3,"Wednesday"},{4,"Thursday"},{5,"Friday"},{6,"Saturday"}};
    return weekTable;
}