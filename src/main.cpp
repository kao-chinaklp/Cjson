#include "Cjson.h"

#include <String.h>

#include <iostream>

using namespace std;

int main(){
    // Used for testing
    freopen("compile_commands.json", "r", stdin);
    String str, tmp;
    while(cin>>tmp)
        str+=tmp;
    str+=tmp;
    Cjson json;
    auto obj=json.Parse(str);
    cout<<json.Serialize(obj)<<endl;
    return 0;
}