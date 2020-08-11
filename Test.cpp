#include <bits/stdc++.h>
#define INPUTE "File/input.txt"

using namespace std;

#define checkBit(n,p) (bool)(n&(1<<p))
string toHex(string binary_str)
{
    bitset<8> set(binary_str);
    string s;
    stringstream st;
    st<< hex << set.to_ulong();
    st>>s;
    return s;
}
int main()
{
    //cout<<toHex("0100");
    string bytecode = "0000 010 011 001 000", hold, target = "", hex = "";
    stringstream st;
    st << bytecode;
    for(target = ""; st ;target+=hold)st >> hold;
    for(int i = 0; i<13 ;i+=4)hex += toHex(target.substr(i,4));
    cout<< hex;
    return 0;
}
