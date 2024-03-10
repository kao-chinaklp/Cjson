#ifndef VAL_H_
#define VAL_H_

#include "String.h"

#include <map>
#include <variant>
#include <iostream>

using std::ostream, std::variant, std::map;

typedef unsigned long long ull;

enum class Type{Null, Int, Double, Bool, String, List, Dict};

class Val{
    typedef Vector<Val> List;
    typedef map<Val, Val> Dict;
    typedef variant<std::monostate, int, double, bool, String, List, Dict> Value;

    public:
        Val():Val(Value()){}

        Val(const int& val);
        Val(const double& val);
        Val(const bool& val);
        Val(const String& val);
        Val(const char* val);
        Val(const List& val);
        Val(const Dict& val);
        Val(const Value& val);
        Val(const Val& val);

        Val(int&& val)noexcept;
        Val(double&& val)noexcept;
        Val(bool&& val)noexcept;
        Val(String&& val)noexcept;
        Val(List&& val)noexcept;
        Val(Dict&& val)noexcept;
        Val(Value&& val)noexcept;
        Val(Val&& val)noexcept;

        ~Val(){Del();}

    public:
        void Add(const Val& val);
        void Put(const Val& key, const Val& val);

        Val& operator=(const Val& val);
        Val& operator=(Val&& val)noexcept;

        Val& operator[](const Val& key);
        const Val& operator[](const Val& key)const;
        friend bool operator<(const Val& x, const Val& y);
        friend ostream& operator<<(ostream& out, const Val& val);

    protected:
        void Del();

    private:
        Value val;
        Type _type;
};

bool operator<(const Val& x, const Val& y);
ostream& operator<<(ostream& out, const Val& val);

#endif