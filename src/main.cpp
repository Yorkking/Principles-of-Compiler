#include<iostream>
#include<string>

#include "regex.h"

int main(){
    int T;
    std::cin>>T;
    while(T--){
        std::string s1,s2;
        std::cin>>s1>>s2;
        Regex reg1(s1);
        Regex reg2(s2);

        int flag = compare(reg1,reg2);

        if(flag == 0) cout<<"="<<endl;
        else if(flag == -1) cout<<"<"<<endl;
        else if(flag == 1) cout<<">"<<endl;
        else cout<<"!"<<endl;
    }
    return 0; 
    
}
