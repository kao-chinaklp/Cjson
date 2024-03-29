#include "String.h"

using std::min;

ui Strlen(const char* p){
    ui cnt=0;
    while(*p!='\0')++cnt, ++p;
    return cnt;
}

String::String(const char* init){
    if(init==0)init=new char[0];
    ui size=Strlen(init);
    Data.Resize(size);
    for(ui i=0;i<size;i++)
        Data[i]=init[i];
}

String::String(const String& init){
    ui size=init.Size();
    Data.Resize(size);
    for(ui i=0;i<size;i++)
        Data[i]=init[i];
}

String::String(String&& init)noexcept{
    Data=init.Data;
    init.Data.Clear();
}

String::String(String::iterator _start, String::iterator _end){
    ui size=_end-_start;
    Data.Resize(size);
    for(ui i=0;i<size;i++)
        Data[i]=_start[i];
}

String::~String(){
    Data.Clear();
    Data.ShrinkToFit();
}

void String::Assign(const ui size, const char &c){
    Data.Assign(size, c);
}

ui String::Find(const char c, const ui pos)
{
    for(ui p=pos;p<this->Size();p++)
        if(c==Data[p])return p;
    return -1;
}

ui String::Find(const String str, const ui pos){
    if(str.Size()>this->Size())return -1;
    // KMP
    ui* nxt=new ui[this->Size()];
    ui j=0, len=str.Size();
    for(ui i=0;i<len;i++){
        while(j!=0&&str[i]!=str[j])j=nxt[j-1];
        if(str[i]==str[j])j++;
        nxt[i]=j;
    }
    j=0;
    ui lim=this->Size();
    for(ui i=pos;i<lim;i++){
        while(j!=0&&Data[i]==str[j])j=nxt[j-1];
        if(Data[i]==str[j])j++;
        if(j==len)return i;
    }
    return -1;
}

ui String::Size()const{
    return Data.Size();
}

inline char& String::Front(){
    return Data.Front();
}

inline char& String::Back(){
    return Data.Back();
}

void String::PushBack(const char c){
    this->Append(c);
}

void String::PopBack(){
    Data.PopBack();
}

void String::Append(const char c){
    Data.PushBack(c);
}

void String::Append(const String str){
    Data.Reserse(this->Size()+str.Size());
    for(char& c:str)Data.PushBack(c);
}

void String::Clear(){
    Data.Clear();
}

void String::Resize(ui size){
    Data.Resize(size);
}

String::iterator String::Insert(const ui pos, const char& c){
	return Data.Insert(pos, c);
}

String::iterator String::Insert(String::iterator p, const char& c){
    return Data.Insert(p, c);
}

String::iterator String::Erase(const ui pos, const ui size){
    return Data.Erase(pos, size);
}

String::iterator String::Erase(String::iterator p){
    return Data.Erase(p);
}

String String::Substr(const ui pos, const ui size){
    assert(pos+size<=this->Size());
    String res="";
    for(ui p=pos;p<pos+size;p++)
        res.PushBack((*this)[p]);
    return res;
}

const String::iterator& String::cbegin()const{
    return Data.cbegin();
}

const String::iterator& String::cend()const{
    return Data.cend();
}

const String::iterator& String::begin()const{
    return Data.cbegin();
}

const String::iterator& String::end()const{
    return Data.cend();
}

const Vector<char>::reverseIterator& String::crbegin()const{
    return Data.crbegin();
}

const Vector<char>::reverseIterator& String::crend()const{
    return Data.crend();
}

String::iterator& String::begin(){
    return Data.begin();
}

String::iterator& String::end(){
    return Data.end();
}

Vector<char>::reverseIterator& String::rbegin(){
    return Data.rbegin();
}

Vector<char>::reverseIterator& String::rend(){
    return Data.rend();
}

char& String::operator[](ui pos){
    return Data[pos];
}

const char& String::operator[](ui pos)const{
    return Data[pos];
}

String& String::operator=(const String& str){
    Data=str.Data;
    return *this;
}

String& String::operator=(String&& str)noexcept{
    Data=str.Data;
    str.Data.Clear();
    return *this;
}

String String::operator+(const String& str){
    String tmp(*this);
    ui old_size=this->Size();
    ui new_size=this->Size()+str.Size();
    tmp.Resize(new_size);
    for(ui p=old_size;p<new_size;p++)
        tmp[p]=str[p-old_size];
    return tmp;
}

String String::operator+(const char c){
    String tmp(*this);
    tmp.Append(c);
    return tmp;
}

String String::operator+(const char* p){
    return *this+String(p);
}

String& String::operator+=(const String& str){
    this->Append(str);
    return *this;
}

String& String::operator+=(const char& c){
    this->Append(c);
    return *this;
}

String& String::operator+=(char* p){
    this->Append(p);
    return *this;
}

bool String::operator==(const String& str)const{
    if(this->Size()!=str.Size())return false;
    ui len=this->Size();
    for(ui i=0;i<len;i++)
        if(Data[i]!=str[i])return false;
    return true;
}

bool String::operator!=(const String& str)const{
    return !(*this==str);
}

bool String::operator<=(const String& str)const{
    ui len=min(this->Size(), str.Size());
    for(ui i=0;i<len;i++){
        if(Data[i]>str[i])return false;
        if(Data[i]<str[i])return true;
    }
    return true;
}

bool String::operator>=(const String& str)const{
    ui len=min(this->Size(), str.Size());
    for(ui i=0;i<len;i++){
        if(Data[i]<str[i])return false;
        if(Data[i]>str[i])return true;
    }
    return true;
}

bool String::operator<(const String& str)const{
    return !(*this>=str);
}

bool String::operator>(const String& str)const{
    return !(*this<=str);
}

std::istream& operator>>(std::istream& o, String& str){
    char tmp;
    str.Clear();
    while(true){
        tmp=o.get();
        if(tmp==' '||tmp=='\n'||tmp==EOF)break;
        str.PushBack(tmp);
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const String& str){
    for(char c:str)o<<c;
    return o;
}

std::stringstream& operator<<(std::stringstream& o, const String& str){
    for(char c:str)o<<c;
    return o;
}

String operator+(const char c, const String& str){
    String tmp(str);
    tmp.Insert(tmp.begin(), c);
    return tmp;
}

String operator+(const char* p, const String& str){
    return String(p)+str;
}

String ToString(long long num){
    if(num==0)return String("0");
    String ans;
    bool sign=true;
    if(num<0)sign=false;
    while(num){
        ans.PushBack(char(num%10+'0'));
        num/=10;
    }
    Reverse(ans.begin(), ans.end());
    return (sign?"":"-")+ans;
}

String ToString(double num){
    if(num==0)return String("0");
    String ans;
    bool sign=true;
    if(num<0)sign=false;
    long long int_part=(long long)num;
    double dec_part=num-int_part;
    if(int_part==0)ans.PushBack('0');
    else ans=ToString(int_part);
    if(dec_part==0)return (sign?"":"-")+ans;
    ans.PushBack('.');
    dec_part*=10;
    while(dec_part!=0){
        ans.PushBack(char((int)dec_part)+'0');
        dec_part*=10;
    }
    return (sign?"":"-")+ans;
}

long long ToDigit(String str){
    long long num=0;
    bool sign=false;
    ui p=0;
    if(str[p]=='-'){
        sign=true;
        p++;
    }
    while(p<str.Size()){
        char tmp=str[p];
        assert(std::isdigit(str[p]));
        num=num*10+str[p++]-'0';
    }
    return (sign?-1:1)*num;
}

double ToDouble(String str){
    double num=0;
    bool sign=false;
    ui p=0;
    if(str[p]=='-'){
        sign=true;
        p++;
    }
    while(p<str.Size()){
        if(str[p]=='.')break;
        assert(std::isdigit(str[p]));
        num=num*10+str[p++]-'0';
    }
    if(p<str.Size()){
        p++;
        double dec=0.1;
        while(p<str.Size()){
            assert(std::isdigit(str[p]));
            num+=dec*(str[p++]-'0');
            dec*=0.1;
        }
    }
    return (sign?-1:1)*num;
}

bool GetLine(std::istream& in, String& str){
    char c;
    str.Clear();
    while(in.get(c)){
        if(c=='\n')break;
        str.PushBack(c);
    }
    if(str.Size()==0)return false;
    return true;
}
