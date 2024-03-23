#ifndef COBEJCT_H_
#define COBEJCT_H_

#include <map>
#include <variant>

#include "String.h"

using std::variant, std::nullptr_t, std::map, std::get_if, std::get,
      std::visit, std::forward;

enum class CobjectType{Null, Int, Double, Bool, String, List, Dict, Error};

template<typename...  T>struct overload:T...{
    using T::operator()...;
};
template<typename... T>overload(T...)->overload<T...>;
class Cobject{
    public:
        struct Error{Error(long long arg=0){};};
        typedef Vector<Cobject> List;
        typedef map<String, Cobject> Dict;
        typedef variant<nullptr_t, int, double, bool, String, List, Dict, Error> ValueType;

    public:
        Cobject()=default;
        Cobject(const Cobject&)=default;
        Cobject(Cobject&&)=default;
        Cobject& operator=(const Cobject&)=default;
        Cobject& operator=(Cobject&&)=default;

        Cobject(nullptr_t):Val(nullptr){}
        Cobject(const ValueType& Val):Val(Val){}
        Cobject(ValueType&& Val):Val(std::move(Val)){}

        template<typename T>
        static constexpr int Index(CobjectType Type){
            return static_cast<int>(Type);
        }
        template<typename T>T& Get(){return get<T>(Val);}
        template<typename T>const T& Get()const{return get<T>(Val);}
        template<typename T>T* GetPtr(){return get_if<T>(&Val);}
        template<typename T>const T* GetPtr()const{return get_if<T>(&Val);}
        template<typename... T>auto Visit(T&&... Visitor){
            return std::visit(overload{forward<T>(Visitor)...}, Val);
        }
        template<typename... T>auto Visit(T&&... Visitor)const{
            return std::visit(overload{forward<T>(Visitor)...}, Val);
        }
        const Cobject& operator[](const ui idx)const;
        const Cobject& operator[](const String& Key)const;

        CobjectType GetType()const;
    
    private:
        static const Cobject Null;
        ValueType Val;
};

inline const Cobject Cobject::Null=Cobject(nullptr);

inline const Cobject& Cobject::operator[](const ui idx)const{
    if(GetType()!=CobjectType::List)return Null;;
    auto& List=Get<Vector<Cobject>>();
    if(idx>=List.Size())return Null;
    return List[idx];
}

inline const Cobject& Cobject::operator[](const String& Key)const{
    if(GetType()!=CobjectType::Dict)return Null;
    auto& Dict=Get<map<String, Cobject>>();
    auto It=Dict.find(Key);
    if(It==Dict.end())return Null;
    return It->second;
}

inline CobjectType Cobject::GetType()const{
    return static_cast<CobjectType>(Val.index());
}

template<typename... T>
Cobject NewObject(T&&... Args){
    return Cobject(Cobject::ValueType(std::forward<T>(Args)...));
}

#endif