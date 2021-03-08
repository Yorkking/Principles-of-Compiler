#include "util.h"

const int NN = 26;
const char DOT = '.';
const char EPSILON = 'E';
constexpr long long _MOD_ = (1LL << 31) - 1;
constexpr long long _BASE_ = 173;

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