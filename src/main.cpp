#include "Cjson.h"

#include <String.h>
#include <fstream>
#include <iostream>

using namespace std;

int main(){
    std::ifstream fin("compile_commands.json");
    Cjson cjson;
    String str, tmp;
    while(GetLine(fin, tmp))
        str+=tmp;
    cout<<str<<endl;
    Val val=cjson.Parse(str);
    cout<<val["directory"]<<endl;
    return 0;
}