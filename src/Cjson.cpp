#include "Cjson.h"

using std::isdigit;

Val Cjson::Parse(const String& json){
    ss.clear();
    ss<<json;
    return ParseVal();
}

Val Cjson::Parse(const char* json){
    ss.clear();
    ss<<String(json);
    return ParseVal();
}

Val Cjson::ParseVal(){
    while(ss.peek()!=-1){
        if(ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r')ss.get();
        else if(ss.peek()=='"')return ParseStr();
        else if(ss.peek()=='[')return ParseList();
        else if(ss.peek()=='{')return ParseDict();
        else if(ss.peek()=='t'||ss.peek()=='f')return ParseBool();
        else return ParseNum();
    }
    return 0;
}

Val Cjson::ParseNum(){
    String num;
    while(isdigit(ss.peek())||ss.peek()=='e'||ss.peek()=='E'||ss.peek()=='.'||ss.peek()=='-'||ss.peek()=='+')
        num.PushBack(ss.get());
    if(num.Find('.')==(ui)-1||num.Find('e')==(ui)-1||num.Find('E')==(ui)-1)
        return ToDouble(num);
    return (int)ToDigit(num);
}

Val Cjson::ParseBool(){
    if(ss.peek()=='t'){
        ss.get();ss.get();ss.get();
        return true;
    }
    else{
        ss.get();ss.get();ss.get();ss.get();
        return false;
    }
}

Val Cjson::ParseStr(){
    ss.get();
    String str;
    while(ss.peek()!='"')str.PushBack(ss.get());
    ss.get();
    return str;
}

Val Cjson::ParseList(){
    ss.get();
    Val list=Vector<Val>();
    while(ss.peek()!=']'){
        list.Add(ParseVal());
        while(ss.peek()!=']'&&(ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r'||ss.peek()==','))
            ss.get();
    }
    ss.get();
    return list;
}

Val Cjson::ParseDict(){
    ss.get();
    Val dict=map<Val, Val>();
    while(ss.peek()!='}'){
        Val key=ParseVal();
        while(ss.peek()==' '||ss.peek()==':')
            ss.get();
        Val val=ParseVal();
        dict.Put(key, val);
        while(ss.peek()!='}'&&(ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r'||ss.peek()==','))
            ss.get();
    }
    ss.get();
    return dict;
}