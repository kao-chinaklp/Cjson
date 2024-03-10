// This is a mini library used to parse JSON, 
// where "C" represents "chinaklp" rather than "cpp".

#ifndef CJSON_H_
#define CJSON_H_

#include "Val.h"

#include <sstream>

using std::stringstream;

class Cjson{
    public:
        Cjson(){};
        ~Cjson(){};

        Val Parse(const String& json);
        Val Parse(const char* json);

    protected:
        Val ParseVal();
        Val ParseNum();
        Val ParseBool();
        Val ParseStr();
        Val ParseList();
        Val ParseDict();

    private:
        stringstream ss;
};

#endif