#ifndef CJSON_H_
#define CJSON_H_

#include "Cobject.h"

#include <sstream>

using std::stringstream;

class Cjson{
    typedef Vector<Cobject> List;
    typedef map<String, Cobject> Dict;

    public:
        Cjson()=default;
        ~Cjson()=default;

        Cobject Parse(const String& Str);
        String Serialize(const Cobject& Obj);

    protected:
        Cobject ParseValue();
        Cobject ParseNull();
        Cobject ParseBool();
        Cobject ParseNum();
        Cobject ParseString();
        Cobject ParseNumber();
        Cobject ParseList();
        Cobject ParseDict();

    private:
        stringstream ss;
        Cobject Root;
};

#endif