// This is a mini library used to parse JSON, 
// where "C" represents "chinaklp" rather than "cpp".

#ifndef CJSON_H_
#define CJSON_H_

#include "String.h"

#include <map>
#include <variant>
#include <fstream>

using std::ostream, std::variant, std::is_same_v, std::remove_cv_t;
using std::forward, std::move;

typedef unsigned long long ull;

enum class Type{Null, Int, Double, Bool, String, List, Dict};

class Val{
    typedef Vector<Val> List;
    typedef std::map<Val, Val> Dict;
    typedef variant<std::monostate, int, double, bool, String, List, Dict> Value;

    public:
        Val():Val(0){}
        template<typename T>
        Val(const T& val);
        template<typename T>
        Val(T&& val);

        Val& operator[](const Val& key);
        const Val& operator[](const Val& key)const;
        friend bool operator<(const Val& x, const Val& y);
        friend ostream& operator<<(ostream& out, const Val& val);

    protected:
        void GetValue(const List& val);
        void GetValue(const Dict& val);
        void GetValue(const Val& val);

        void GetValue(Value&& val);
        void GetValue(List&& val);
        void GetValue(Dict&& val);

    protected:
        void Del();

    private:
        Value val;
        List Lst;
        Dict Dct;
        Type _type;
};

class Cjson{
    public:
        Cjson();
};

template<typename T>
Val::Val(const T& val){
    if(is_same_v<remove_cv_t<T>, List>){_type=Type::List;GetValue(val);}
    else if(is_same_v<remove_cv_t<T>, Dict>){_type=Type::Dict;GetValue(val);}
    else if(is_same_v<remove_cv_t<T>, Val>){Del();*this=val;}
    else GetValue(std::forward<Value&&>(Value(val)));
}

template<typename T>
Val::Val(T&& val){
    if constexpr (is_same_v<remove_cv_t<T>, List>){_type=Type::List;GetValue(forward<List&&>(val));}
    else if constexpr (is_same_v<remove_cv_t<T>, Dict>){_type=Type::Dict;GetValue(forward<Dict&&>(val));}
    else if(is_same_v<remove_cv_t<T>, Val>){*this=val;val=0;}
    else {GetValue(std::forward<Value&&>(Value(val)));val=0;}
}

bool operator<(const Val& x, const Val& y);
ostream& operator<<(ostream& out, const Val& val);

#endif