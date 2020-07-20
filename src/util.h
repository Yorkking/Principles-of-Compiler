#ifndef UTIL_H
#define UTIL_H 1

#include<iostream>
#include<map>

using std::cout;
using std::endl;
const int NN = 26;
const char DOT = '.';
const char EPSILON = 'E';

bool isLetter(char c){
    return (c>='a' && c <= 'z') || c == EPSILON;
}
bool priority(char a, char b){
    if(a == b) return true;
    if(a == '(') return false;
    if(a == '*' || a == '?' || a == '+') return true;
    if(b == '|') return true;

    return false;
}
static constexpr long long _MOD_ = (1LL << 31) - 1;
static constexpr long long _BASE_ = 173;

#endif