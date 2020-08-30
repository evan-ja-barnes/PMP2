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
#include <boost/algorithm/string.hpp>



using namespace std;
class PMP{
    public:
    vector<vector<string>> currentDB;
    string PATH = "C:/Users/Stran/Downloads/Docs/";
    string FILENAME;
    PMP(string dbName){
        currentDB = readCSV(dbName);
        
    }

  
    vector<vector<string>> readCSV(string filename){
        
        int numRows;
        int numCols;
        FILENAME = filename;
        ifstream dBImport(PATH + FILENAME); 
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
    void manage(){

    }
    string checkForProcess(string process){
        vector<vector<string>> matches;
        bool failed;
        for(auto arr : currentDB){
            if(arr[4]==process){
                matches.push_back(arr);
            }
        }
        if(sizeof(matches)==0){
            cout << "This is not associated with any credentials, would you like to add this to a previous log-on(1), create a new log-on(2), or exit(3)?\nplease enter 1, 2, or 3 for their corrosponding options\n";
            int inp;
            bool validInput = false;
            while(!validInput){
                try{
                    
                    cin >> inp;
                    if(inp > 0 && inp < 4){
                        validInput = true;
                        
                    }else{
                        cout << "Invalid entry, please try again.";
                    }
                }catch(exception& error){
                    cout << "Invalid entry, please try again";
                }
            }
            
            if(inp ==1){
                cout << "Please enter the name of the service you would like to assign this process too, or enter exit to exit\n";
                string strinp;
                cin >>strinp;
                strinp = boost::to_lower(strinp);
                while(!validInput){
                    if(strinp == "exit"){
                        validInput = true;
                        failed = true;
                    }else{
                        for(auto arr : currentDB){
                            if(arr[0] == strinp){
                                arr[4]=strinp;
                                //write_CSV(PATH+FILENAME, currentDB);
                                matches.push_back(arr);
                            }
                        }
                    }

                }

            }else if(inp = 2){
                failed = addNewLine();
                if(!failed){
                    matches.push_back(currentDB[sizeof(currentDB)-1]);
                    //write_CSV(PATH+FILENAME, currentDB);

                }
            }else{
                failed = true;
                
            }
            if(failed == true){
                return("failed");
            }else{
                return(matches[0][1]);
            }
        }else if(sizeof(matches)==1){
            return(matches[0][1])
        }else{
            cout << "Your choices are: ";
            int i;
            for(auto arro : matches){
                cout << arro[0] << "(" << i << "), "
            }
            cout << ("please enter the number corrosponding to your choice or 0 to exit.");
            int inp;
            bool valid;
            while(!valid){
                try{
                    
                    cin >> inp;
                    if(inp >= 0 && inp < sizeof(matches)){
                        valid = true;
                        
                    }else{
                        cout << "Invalid entry, please try again.";
                    }
                }catch(exception& error){
                    cout << "Invalid entry, please try again";
                }
            }
            if(inp == 0){
                return("failed");

            }else{
                return(matches[inp-1][2]);
            }
        }
    }
    bool addNewLine(){
        vector<string> newLine;
        bool failed = false;
        for(int i = 0; i< sizeof(currentDB[0]); i++){
            cout << "Please enter data for: " << currentDB[0][i] << ", or enter exit to exit.\n";
            string strinput;
            cin >> strinput;
            strinput = boost::to_lower(strinput);
            if(strinput != "exit"){
                //write_CSV(PATH+FILENAME, currentDB);

                newLine.push_back(strinput);

            }else{
                failed = true;
                i= sizeof(currentDB[0])+1;
            }
        }
        if(!failed){
            currentDB.push_back(newLine);
        }else{
            //write_CSV(PATH+FILENAME, currentDB);

        }
        return(failed);

    }
};

int main(){
    PMP daPMP("db.csv");
    string activeWindow = daPMP.getActWind();
    return 0;
}   