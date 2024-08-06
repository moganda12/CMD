#include <fstream>
#include <gmpxx.h>
#include <string>

using str = std::string;

using integer = mpz_class;
using flat = mpf_class;
using number = mpq_class;

struct TclickerSave {
    number transistorBalance;
    number totalTransistors;
    number ascentionLevels;
    number heavenlyMicrochips;
    integer clicks, cusors, moss, smallFABs, mediumFABs, largeFABs; 
    //integer i860s, startups;
};

extern "C" TclickerSave load(str filename, str version) {
    TclickerSave save;
    str saveversion;
    std::ifstream file(filename);
}

extern "C" void save(str filename, TclickerSave save, str version);
