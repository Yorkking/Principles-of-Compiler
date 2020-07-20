#ifndef REG_H
#define REG_H 1

#include<string>
#include"DFA.h"

class Regex{
public:
    friend int compare(Regex& reg1, Regex& reg2){
        Dfa dfa1(reg1.getString());
        Dfa dfa2(reg2.getString());

        int flag1 = dfa1.contain(dfa2);
        int flag2 = dfa2.contain(dfa1);

        if(flag1 == -1 && flag2 == -1) return -2; // uncomparable;
        else if(flag1 == -1 && flag2 == 0) return -1; // < 
        else if(flag1 == 0 && flag2 == 0) return 0; // ==
        else return 1; // >
    }
    Regex(std::string& str){
        str.push_back(EPSILON);
        for(int i=0;i<str.length()-1;++i){
            if(isLetter(str[i]) && isLetter(str[i+1])){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(isLetter(str[i]) && str[i+1] == '('){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(str[i] == ')' && str[i+1] == '('){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(str[i] == ')' && isLetter(str[i+1])){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if((str[i] == '*'|| str[i] == '?' || str[i] == '+') && (isLetter(str[i+1]) || str[i+1] == '(')){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else{
                regString.push_back(str[i]);
            }
        }
        regString = regString.substr(0,regString.length()-1);
    }
    std::string& getString(){
        return regString;
    }
private:
    std::string regString;
};

#endif