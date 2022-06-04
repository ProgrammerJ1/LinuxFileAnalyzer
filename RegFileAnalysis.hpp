#include "FileAnalysis.hpp"
using namespace std;
class RegAnalysis {
    Analysis Base;
    uint8_t* Data;
    RegAnalysis(Analysis x,uint8_t* y) {
        Base=x;
        Data=y;
    }
};