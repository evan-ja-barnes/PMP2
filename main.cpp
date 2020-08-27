#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>

#include <iostream> 


#include <sqlite3.h>
  


using namespace std;

vector<vector<string>> readCSV(string filename){
    
    
    
    int numRows;
    int numCols;
    ifstream dBImport(filename); 
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
int main(){
    vector<vector<string>> pp = readCSV("DB.csv");
    //cout << pp[0][0];
    for(vector<string> vec : pp){
        for(string st : vec){
            cout << st << "; ";
            
        }
        cout << "/n";
    }
    return 0;
}   