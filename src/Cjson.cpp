#include "Cjson.h"

using std::isdigit;

Cobject Cjson::Parse(const String& Str){
    ss.clear();
    ss<<Str;
    return ParseValue();
}

String Cjson::Serialize(const Cobject& obj){
    String res;
    obj.Visit(
        [&](const String& sv){
            res.Append('"');
            res.Append(sv);
            res.Append('"');
        },
        [&](long long iv){res+=ToString(iv);},
        [&](double dv){res+=ToString(dv);},
        [&](bool bv){res+=bv?"true":"false";},
        [&](const List& lst){
            res.Append("[");
            for(ui i=0;i<lst.Size();++i){
                if(i)res.Append(',');
                res+=Serialize(lst[i]);
            }
            res.Append("]");
        },
        [&](const Dict& dct){
            if(dct.empty()){
                res.Append("{}");
                return;
            }
            bool flag=false;
            res.Append("{");
            for(auto& [key, val]:dct){
                if(flag)res.Append(",");
                else flag=true;
                res+=key;
                res.Append(':');
                res+=Serialize(val);
            }
            res.Append("}");
        },
        [&](nullptr_t){res.Append("null");},
        [&](auto arg){}
    );
    return res;
}

Cobject Cjson::ParseValue(){
    while(ss.peek()!=-1){
        if(ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r')ss.get();
        else if(ss.peek()=='"')return ParseString();
        else if(ss.peek()=='f'||ss.peek()=='t')return ParseBool();
        else if(ss.peek()=='[')return ParseList();
        else if(ss.peek()=='{')return ParseDict();
        else if(ss.peek()=='n'||ss.peek()=='N')return ParseNull();
        else ParseNum();
    }
    return Cobject();
}

Cobject Cjson::ParseNull(){
    char c;
    ss>>c;
    if(c=='n'||c=='N'){
        ss>>c;
        if(c=='u'||c=='U'){
            ss>>c;
            if(c=='l'||c=='L'){
                ss>>c;
                if(c=='l'||c=='L'){
                    return Cobject(nullptr);
                }
            }
        }
    }
    return Cobject();
}

Cobject Cjson::ParseBool(){
    char c;
    ss>>c;
    if(c=='t'||c=='T'){
        ss>>c;
        if(c=='r'||c=='R'){
            ss>>c;
            if(c=='u'||c=='U'){
                ss>>c;
                if(c=='e'||c=='E')
                    return Cobject(true);
            }
        }
    }
    else if(c=='f'||c=='F'){
        ss>>c;
        if(c=='a'||c=='A'){
            ss>>c;
            if(c=='l'||c=='L'){
                ss>>c;
                if(c=='s'||c=='S'){
                    ss>>c;
                    if(c=='e'||c=='E')
                        return Cobject(false);
                }
            }
        }
    }
    return Cobject();
}

Cobject Cjson::ParseNum(){
    String s;
    while(isdigit(ss.peek())||ss.peek()=='e'||ss.peek()=='E'||ss.peek()=='-'||ss.peek()=='+'||ss.peek()=='.')
        s.Append(ss.get());
    if(s.Find('.')!=(ui)-1||s.Find('e')!=(ui)-1||s.Find('E')!=(ui)-1)
        return Cobject(ToDouble(s));
    else
        return Cobject(ToDigit(s));
}

Cobject Cjson::ParseString(){
    ss.get();
    String s;
    char c;
    while(ss.peek()!='"'){
        c=ss.get();
        if(c=='\\')s.Append(ss.get());
        else s.Append(c);
    }
    ss.get();
    return Cobject(s);
}

Cobject Cjson::ParseList(){
    ss.get();
    Vector<Cobject> List;
    while(ss.peek()!=']'){
        List.PushBack(ParseValue());
        while(ss.peek()!=']'&&(ss.peek()==','||ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r'))ss.get();
    }
    ss.get();
    return Cobject(List);
}

Cobject Cjson::ParseDict(){
    ss.get();
    map<String, Cobject> Dict;
    while(ss.peek()!='}'){
        String Key=ParseValue().Get<String>();
        while(ss.peek()==' '||ss.peek()==':')ss.get();
        Cobject Value=ParseValue();
        Dict[Key]=Value;
        while(ss.peek()!='}'&&(ss.peek()==','||ss.peek()==' '||ss.peek()=='\n'||ss.peek()=='\t'||ss.peek()=='\r'))ss.get();
    }
    ss.get();
    return Cobject(Dict);
}