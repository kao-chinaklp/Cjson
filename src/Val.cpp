#include "Val.h"

using std::get;

Val::Val(const int& val){this->val=val;_type=Type::Int;}
Val::Val(const double& val){this->val=val;_type=Type::Double;}
Val::Val(const bool& val){this->val=val;_type=Type::Bool;}
Val::Val(const String& val){this->val=val;_type=Type::String;}
Val::Val(const char* val){this->val=String(val);_type=Type::String;}
Val::Val(const List& val){this->val=val;_type=Type::List;}
Val::Val(const Dict& val){this->val=val;_type=Type::Dict;}
Val::Val(const Value& val){
    if(val.index()==0)_type=Type::Null;
    if(val.index()==1)_type=Type::Int;
    if(val.index()==2)_type=Type::Double;
    if(val.index()==3)_type=Type::Bool;
    if(val.index()==4)_type=Type::String;
    if(val.index()==5)_type=Type::List;
    if(val.index()==6)_type=Type::Dict;
    this->val=val;
}
Val::Val(const Val& val){this->val=val.val;_type=val._type;}

Val::Val(int&& val)noexcept{this->val=val;val=0;_type=Type::Int;}
Val::Val(double&& val)noexcept{this->val=val;val=0;_type=Type::Double;}
Val::Val(bool&& val)noexcept{this->val=val;val=false;_type=Type::Bool;}
Val::Val(String&& val)noexcept{this->val=val;val.Clear();_type=Type::String;}
Val::Val(List&& val)noexcept{this->val=val;val.Clear();_type=Type::List;}
Val::Val(Dict&& val)noexcept{this->val=val;val.clear();_type=Type::Dict;}
Val::Val(Value&& val)noexcept{
    if(val.index()==0)_type=Type::Null;
    if(val.index()==1)_type=Type::Int;
    if(val.index()==2)_type=Type::Double;
    if(val.index()==3)_type=Type::Bool;
    if(val.index()==4)_type=Type::String;
    if(val.index()==5)_type=Type::List;
    if(val.index()==6)_type=Type::Dict;
    this->val=val;val=0;
}
Val::Val(Val&& val)noexcept{this->val=val.val;val.Del();_type=val._type;}

void Val::Add(const Val& val){
    if(_type!=Type::List)throw "Unexpect type!";
    get<List>(this->val).PushBack(val);
}

void Val::Put(const Val& key, const Val& val){
    if(_type!=Type::Dict)throw "Unexpect type!";
    get<Dict>(this->val)[key]=val;
}

Val& Val::operator=(const Val& val){
    if(this==&val)return *this;
    this->val=val.val;
    _type=val._type;
    return *this;
}

Val& Val::operator=(Val&& val)noexcept{
    if(this==&val)return *this;
    this->val=val.val;
    _type=val._type;
    val.Del();
    return *this;
}

Val& Val::operator[](const Val& key){
    if(_type==Type::List){
        List& Lst=get<List>(val);
        if(key.val.index()!=1)throw "Unexpect index!";
        else return Lst[get<int>(key.val)];
    }
    if(_type==Type::Dict)return get<Dict>(val).at(key);
    return *this;
}

const Val& Val::operator[](const Val& key)const{
    if(_type==Type::List){
        const List& Lst=get<List>(val);
        if(key.val.index()!=1)throw "Unexpect index!";
        else return Lst[get<int>(key.val)];
    }
    if(_type==Type::Dict)return get<Dict>(val).at(key);
    return *this;
}

void Val::Del(){
    if(_type==Type::List)get<List>(val).Clear();
    else if(_type==Type::Dict)get<Dict>(val).clear();
    else val=0;
}

bool operator<(const Val& x, const Val& y){
    typedef Vector<Val> List;
    typedef map<Val, Val> Dict;
    if(x._type!=y._type)return x._type<y._type;
    if(x._type==Type::Int)return get<int>(x.val)<get<int>(y.val);
    if(x._type==Type::Double)return get<double>(x.val)<get<double>(y.val);
    if(x._type==Type::Bool)return get<bool>(x.val)<get<bool>(y.val);
    if(x._type==Type::String)return (String)get<String>(x.val)<(String)get<String>(y.val);
    if(x._type==Type::List)return get<List>(x.val)<get<List>(y.val);
    if(x._type==Type::Dict)return get<Dict>(x.val)<get<Dict>(y.val);
    return false;
}

ostream& operator<<(ostream& out, const Val& val){
    if(val._type==Type::List){
        out<<'[';
        for(int i=0, lim=get<Vector<Val> >(val.val).Size();i<lim;i++){
            if(i)out<<',';
            out<<val[i];
        }
        out<<']';
    }
    if(val._type==Type::Dict){
        out<<'{';
        const map<Val, Val>& Dct=get<map<Val, Val> >(val.val);
        for(auto p=Dct.begin();p!=Dct.end();p++){
            if(p!=Dct.begin())out<<',';
            out<<p->first<<':'<<p->second;
        }
        out<<'}';
    }
    if(val._type==Type::Int)out<<get<int>(val.val);
    if(val._type==Type::Double)out<<get<double>(val.val);
    if(val._type==Type::Bool)out<<get<bool>(val.val);
    if(val._type==Type::String)out<<get<String>(val.val);
    return out;
}