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
                if(c=='l'||c=='L')
                    return Cobject(nullptr);
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
        if(c!='\\')s.Append(c);
        else {
            c=ss.get();
            if(c=='"')s.Append('"');
            else if(c=='\\')s.Append('\\');
            else if(c=='/')s.Append('/');
            else if(c=='b')s.Append('\b');
            else if(c=='f')s.Append('\f');
            else if(c=='n')s.Append('\n');
            else if(c=='r')s.Append('\r');
            else if(c=='t')s.Append('\t');
            else if(c=='u'){
                String hex;
                for(ui i=0;i<4;++i){
                    c=ss.get();
                    if((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))hex.Append(c);
                    else return NULL;
                }
                unsigned tmp=ParseHex4(hex);
                if(tmp>=0xD800&&tmp<=0xDBFF){
                    if(ss.get()!='\\'||ss.get()!='u')return NULL;
                    String hex2;
                    for(ui i=0;i<4;++i){
                        c=ss.get();
                        if((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))hex2.Append(c);
                        else return NULL;
                    }
                    unsigned tmp2=ParseHex4(hex2);
                    if(tmp2<0xDC00||tmp2>0xDFFF)return NULL;
                    tmp=(((tmp-0xD800)<<10)|(tmp2-0xDC00))+0x10000;
                }
                EncodeUtf8(s, tmp);
            }
        }
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

char Cjson::ParseHex4(const String& str){
    char res=0;
    for(ui i=0;i<4;++i){
        res<<=4;
        if(str[i]>='0'&&str[i]<='9')res|=str[i]-'0';
        else if(str[i]>='a'&&str[i]<='f')res|=str[i]-'a'+10;
        else if(str[i]>='A'&&str[i]<='F')res|=str[i]-'A'+10;
        else return '\0';
    }
    return res;
}

void Cjson::EncodeUtf8(String& str, unsigned c){
    if(c<=0x7F)str.Append(c&0xFF);
    else if(c<=0x7FF){
        str.Append(0xC0|(c>>6));
        str.Append(0x80|(c&0x3F));
    }
    else if(c<=0xFFFF){
        str.Append(0xE0|(c>>12));
        str.Append(0x80|((c>>6)&0x3F));
        str.Append(0x80|(c&0x3F));
    }
    else if(c<=0x10FFFF){
        str.Append(0xF0|(c>>18));
        str.Append(0x80|((c>>12)&0x3F));
        str.Append(0x80|((c>>6)&0x3F));
        str.Append(0x80|(c&0x3F));
    }
}