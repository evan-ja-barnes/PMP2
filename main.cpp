/*
A simple pass word manager
Author: Evan Barnes
Date: 8/22/2020
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <windows.h>
#include <iostream> 




using namespace std;

vector<vector<string>> readCSV(string filename){
    string PATH = "C:/Users/Stran/Downloads/Docs/";
    int numRows;
    int numCols;
    ifstream dBImport(PATH + filename); 
    if(!dBImport.is_open()) throw runtime_error("could not open file");
    string line, colName;
    
    string val1;
    vector<vector<string>> outVect;

    if(dBImport.good()){
        int ct=0;
        int cct=0;
        while(getline(dBImport, line)){
            if(ct == 0){
                stringstream ss1(line);
                char c;
                while(ss1.get(c)){
                    if(c == ','){
                        cct++;
                    }
                }
            } 
            ct++;
        }
        
        numRows = ct;
        numCols = cct+1;
        dBImport.clear();
        dBImport.seekg(0);
        vector<vector<string> > outVect1( numRows , vector<string> (numCols, " ")); 
        int lnCT = 0;
        for(int i = 0; i<numRows; i ++){
            
            int colCT = 0;
            getline(dBImport, line);
            stringstream ss(line);
            for(int j = 0; j<numCols; j ++){
                
                string line2;
                getline(ss, line2, ',');
                int sz = line2.size()-1;
                if(sz>0){
                if(line2.at(sz) =='"'){
                    
                    if(outVect1[i][j-1].at(0) == '"'){

                        string st1 = (outVect1[i][j-1] + ',' + line2);
                        string st2 = st1.substr(1, st1.size()-2);
                        outVect1[i][j-1] =st2;
                        continue;
                    }
                }
                }
                outVect1[i][j] = line2;
                
                colCT++;
                
                if(ss.peek() == ',') {
                    ss.ignore();
                }
                
            }
            lnCT++;
        }
        outVect = outVect1;
        
    }

    int x;
    x <<0;
    dBImport.close();
    return outVect;
}
class PMP{
    public:

    vector<vector<string>> currentDB;
    
    PMP(string dbName){
        currentDB = readCSV(dbName);
        
    }
    string getAssPass(string inpass){
        for(auto line : currentDB){
            if(line[0] == inpass){
                return line[1];
            }
        }
        return "";
    }
    void typeOut(string typer){
        vector<INPUT> vec;
        for(auto ch : typer){
            //SendInput(ch);
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_UNICODE;
            input.ki.wScan = ch;
            vec.push_back(input);
            input.ki.dwFlags |= KEYEVENTF_KEYUP;
            vec.push_back(input);

            
        }
        
        SendInput(vec.size(), vec.data(), sizeof(INPUT));
    }
    string getActWind(){
        HWND top = GetForegroundWindow();   
        char charray[255];
        string finTop;
        GetWindowText(top,charray,sizeof(charray));
        string st = charray;
        stringstream ss(st);
        
        while(getline(ss, finTop, '-')){}
        finTop = finTop.substr(1, sizeof(finTop)-1);
        cout << finTop;
        return finTop;
    }
    
};

int main(){
    PMP daPMP("db.csv");
    string activeWindow = daPMP.getActWind();
    return 0;
}   