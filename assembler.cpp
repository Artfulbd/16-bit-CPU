#include <bits/stdc++.h>
#define INPUTE "File/input.txt"
using namespace std;
ofstream outputFileHex("File/output_hex.txt");
#define checkBit(n,p) (bool)(n&(1<<p))

//============================================================================//
///                      The only easy day,.... was yesterday                ///
//============================================================================//

map<string,string> getMap(string path)
{
    string word,value;
    map<string,string> m;
    ifstream file(path);
    while(file>>word>>value)
    {
        m[word] = value;
    }
    file.close();
    return m;
}

string decToBin(int num,int length)
{
    string result = "";
    for(int i=0; i<length; i++)
    {
        if(checkBit(num,i)) result = "1" + result;
        else result = "0" + result;
    }
    return result;
}

int countInstruction()
{

    ifstream inputFile(INPUTE);
    int totalIns = 0;
    string ins;
    for( ; getline(inputFile,ins); totalIns++ );
    inputFile.close();
    return totalIns;

}
string toString(int a)
{
    stringstream st;
    st<<a;
    string s;
    st>>s;
    return s;
}

string toHex(string binary_str)
{
    bitset<8> set(binary_str);
    string s;
    stringstream st;
    st<< hex << set.to_ulong();
    st>>s;
    return s;
}

string writeHex(string bytecode)
{

    string  hold, target = "", hex = "";
    stringstream st;
    st << bytecode;
    for(target = ""; st ; target+=hold)st >> hold;
    for(int i = 0; i<13 ; i+=4)hex += toHex(target.substr(i,4));
    outputFileHex << hex<<endl;
    return hex;

}



int main()
{
    cout << "Alhamdulillah ..!" << endl;
    int val, insCount = 1,totalIns;
    bool flag = false;
    string ins,op,r1,r2,r3,cmnt, bytecode,hold, target = "", hex = "";
    stringstream st;
    map<string,string> R_TYPE,I_TYPE,J_TYPE,RegisterValue;
    //Obtaining static values from storage
    R_TYPE = getMap("File/R_TYPE.txt");
    I_TYPE = getMap("File/I_TYPE.txt");
    J_TYPE = getMap("File/J_TYPE.txt");
    RegisterValue = getMap("File/RegisterValue.txt");
    ifstream inputFile(INPUTE);
    ofstream outputFile("File/output.txt");

    totalIns = countInstruction();
    cout<<"Total instruction: "<<totalIns<<endl;
    while(getline(inputFile,ins))
    {
        stringstream ss;

        cout << ins << endl;
        //Check for empty instruction
        if(ins.size() == 0)
        {
            bytecode = "Invalid: Empty instruction..!" ;
            goto jump;
        }
        ss << ins;
        ss >> op;
        //If operation code is J Type then the program will go in here
        if(J_TYPE.find(op)!=J_TYPE.end())
        {
            if(op=="din" || op=="dout")
            {
                ss >> r1;
                //Checking instruction validities
                if(r1[0]!='$')
                {
                    bytecode = "Invalid: '$' sign is missing before target";
                    goto jump;
                }
                else
                {
                    if(op == "din")
                    {
                        cout << "Enter value via keypad: ";
                        cin >> val;
                        if(val>32767)
                        {
                            bytecode = "Invalid: value is too large";
                            goto jump;
                        }
                        else if(val<-32768)
                        {
                            bytecode = "Invalid: value is too small";
                            goto jump;
                        }
                        else
                        {
                            cout<<"Converted value "<<decToBin(val,16)<<" will be stored on "<<r1<<endl;
                        }
                    }
                    bytecode = J_TYPE[op] + " " + ("000000000"+RegisterValue[r1]);
                    //cout << J_TYPE[op] << " " << ("000000000"+RegisterValue[r1]) << endl;
                    //outputFile << J_TYPE[op] << " " << ("000000000"+RegisterValue[r1]) << endl;
                    goto jump;
                }
            }
            val = -1;
            ss >> val;

            //Checking instruction validities
            if(val<-2048)
            {
                bytecode = "invalid: value is too small";
            }
            else if(val>2047)
            {
                bytecode = "invalid: value is too large";
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                bytecode = "Invalid: comment structure not maintained";
            }
            else if(((insCount+1)+val) < 1 || ((insCount+1)+val) > totalIns)
            {
                val += insCount + 1;
                bytecode = "Invalid Jump location, line "+toString(val)+" doesn't exist.!";
            }
            //Printing machine code
            else
            {
                bytecode = J_TYPE[op] + " " + decToBin(val,12);
                //cout << J_TYPE[op] << " " << decToBin(val,12) << endl;
                //outputFile << J_TYPE[op] << " " << decToBin(val,12) << endl;
            }
            goto jump;
        }
        ss >> r1;
        r1.erase(r1.end()-1);
        //If operation code is R Type then the program will go in here

        if(R_TYPE.find(op)!=R_TYPE.end())
        {
            val = 0;
            ss >> r2;
            if(r2[0]!='$')
            {
                bytecode = "Invalid: '$' sign is missing before rs";
                goto jump;
            }
            else if(r2[r2.size()-1]!=',')
            {
                bytecode = "Invalid: ',' is missing after rs";
                goto jump;
            }
            r2.erase(r2.end()-1);

            ss >> r3;
            if(r3[0]!='$')
            {
                bytecode = "Invalid: '$' sign is missing before rt";
                goto jump;
            }

            //Checking instruction validities
            if(RegisterValue.find(r1)==RegisterValue.end())
            {
                bytecode = "Invalid: rd not found";
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                bytecode = "Invalid: rs not found";
            }
            else if(RegisterValue.find(r3)==RegisterValue.end())
            {
                bytecode = "Invalid: rt not found";
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                bytecode = "Invalid: comment structure not maintained";
            }
            //Printing machine code if instruction is valid
            else
            {
                bytecode = "0000 "  + RegisterValue[r2] + " " + RegisterValue[r3] + " " + RegisterValue[r1]  + " " + R_TYPE[op];
                //cout << "0000"       << " " << RegisterValue[r2] << " " << RegisterValue[r3] << " " << RegisterValue[r1]  << " " << R_TYPE[op] << endl;
                //outputFile << "0000" << " " << RegisterValue[r2] << " " << RegisterValue[r3] << " " << RegisterValue[r1] <<  " " << R_TYPE[op] << endl;
            }
        }
        //If operation code is I Type then the program will go in here
        else if(I_TYPE.find(op)!=I_TYPE.end())
        {
            val = -1;
            if(op=="lw" ^ op=="sw")
            {
                ss >> val >> r2;
                if(r2[0]!='(')
                {
                    bytecode = "Invalid: '(' is missing before rs";
                    goto jump;
                }
                else if(r2[r2.size()-1]!=')')
                {
                    bytecode = "Invalid: ')' is missing after rs";
                    goto jump;
                }
                else if(r2[1]!='$')
                {
                    bytecode = "Invalid: '$' sign is missing before rs";
                    goto jump;
                }
                r2.erase(r2.begin());
                r2.erase(r2.end()-1);
                swap(r1,r2);
            }
            else
            {
                ss >> r2 >> val;
                if(r2[0]!='$')
                {
                    bytecode = "Invalid: '$' sign is missing before rs" ;
                    goto jump;
                }
                else if(r2[r2.size()-1]!=',')
                {
                    bytecode = "Invalid: ',' is missing after rs";
                    goto jump;
                }
                r2.erase(r2.end()-1);
                if(op == "addi" ^ op == "beq" ^ op == "sll")swap(r1,r2);
                else if(val<0)flag = true;
            }
            //Checking instruction validities
            if(RegisterValue.find(r1)==RegisterValue.end())
            {
                bytecode = "Invalid: rd not found";
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                bytecode = "Invalid: rs not found" ;
            }
            else if(op!="addi" && op!="beq" && val<0)
            {
                bytecode = "Invalid: value can not be neg";
            }
            else if ((op == "addi" ^ op == "beq") && val<-32)
            {
                bytecode = "Invalid: value is too small";
            }
            else if(( (op == "lw" ^ op == "sw" ^ op == "sll") && val>63) ^
                    ((op == "addi" ^ op == "beq") && val>31))
            {
                bytecode =  "Invalid: value is too large" ;
            }
            else if(op == "beq"  && (((insCount+1)+val) < 1 || ((insCount+1)+val) > totalIns))
            {
                val += insCount + 1;
                bytecode = "Invalid Jump location, line "+toString(val)+" doesn't exist.!";

            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                bytecode =  "Invalid: comment structure not maintained" ;
            }
            //Printing machine code if instruction is valid
            else
            {
                if(flag)
                {
                    val -= 10;
                    val = ceil(val/4.0);
                }
                bytecode = I_TYPE[op]+" " + RegisterValue[r1] + " " + RegisterValue[r2] + " " + decToBin(val,6);
                // cout << I_TYPE[op] << " " << RegisterValue[r1] << " " << RegisterValue[r2] << " " << decToBin(val,6) << endl;
                //outputFile << I_TYPE[op] << " " << RegisterValue[r1] << " " << RegisterValue[r2] << " " << decToBin(val,6) << endl;
            }
        }
        else
        {
            bytecode = "Invalid: Op code not found";
        }
jump:
        cout << bytecode;
        if(bytecode.substr(0,7) == "Invalid")
        {
            ofstream ofs, ofs2;
            ofs.open("File/output.txt", ofstream::out | ofstream::trunc);
            ofs <<"Invalid instruction on line "<<insCount<<": "<<bytecode.substr(8) << endl;
            ofs2.open("File/output_hex.txt", ofstream::out | ofstream::trunc);
            ofs2 <<"Invalid instruction on line "<<insCount<<": "<<bytecode.substr(8) << endl;
            ofs.close();
            ofs2.close();
            break;
        }
        outputFile << bytecode << endl;
        cout <<",  Hex: "<<  writeHex(bytecode)<<"\n\n";
        bytecode = "";
        insCount++;
    }
    inputFile.close();
}
