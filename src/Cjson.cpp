#include "Cjson.h"

using std::get;

void Val::GetValue(const List& val){
    Del();
    Lst=val;
    _type=Type::List;
}

void Val::GetValue(const Dict& val){
    Del();
    Dct=val;
    _type=Type::Dict;
}

void Val::GetValue(const Val& val){
    Del();
    *this=val;
}

void Val::GetValue(Value&& val){
    Del();
    this->val=val;
    switch(val.index()){
        case 1:_type=Type::Int;break;
        case 2:_type=Type::Double;break;
        case 3:_type=Type::Bool;break;
        case 4:_type=Type::String;break;
        default:_type=Type::Null;
    }
    val=0;
}

Val& Val::operator[](const Val& key){
    if(_type==Type::List){
        if(key.val.index()!=1)throw "Unexpect index!";
        else return Lst[get<int>(key.val)];
    }
    if(_type==Type::Dict)return Dct.at(key);
    return *this;
}

const Val& Val::operator[](const Val& key)const{
    if(_type==Type::List){
        if(key.val.index()!=1)throw "Unexpect index!";
        else return Lst[get<int>(key.val)];
    }
    if(_type==Type::Dict)return Dct.at(key);
    return *this;
}

void Val::Del(){
    if(_type==Type::List)Lst.Clear();
    else if(_type==Type::Dict)Dct.clear();
    else val=0;
}

bool operator<(const Val& x, const Val& y){
    if(x._type!=y._type)return x._type<y._type;
    if(x._type==Type::List)return x.Lst<y.Lst;
    else if(x._type==Type::Dict)return x.Dct<y.Dct;
    else return x.val<y.val;
}

ostream& operator<<(ostream& out, const Val& val){
    if(val._type==Type::List){
        out<<'[';
        for(ui i=0, lim=val.Lst.Size();i<lim;i++){
            if(i)out<<',';
            out<<val[i];
        }
        out<<']';
    }
    if(val._type==Type::Dict){
        out<<'{';
        for(auto p=val.Dct.begin();p!=val.Dct.end();p++){
            if(p!=val.Dct.begin())out<<',';
            out<<p->second;
        }
        out<<'}';
    }
    if(val._type==Type::Int)out<<get<int>(val.val);
    if(val._type==Type::Double)out<<get<double>(val.val);
    if(val._type==Type::Bool)out<<get<bool>(val.val);
    if(val._type==Type::String)out<<get<String>(val.val);
    return out;
}