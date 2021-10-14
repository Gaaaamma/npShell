#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#define MAX_LENGTH 15000
using namespace std;

void callPrintenv(string envVar);
void callSetenv(string envVar,string value);

int main(int argc, char *argv[]) {
  string input ="" ; 
  string aWord ="" ;
  stringstream ss;
  vector<string> commandVec;
  
  // set $PATH to bin/ ./ initially
  callSetenv("PATH","bin:.");
  callPrintenv("PATH");
 
  while(getline(cin,input)){
    ss << input ;
    while (ss >> aWord) {
      commandVec.push_back(aWord);
    }
    // Ready to handle the command.
    
    // ...

    //one term command is done -> Initialize it.
    commandVec.clear();
    ss.str("");
    ss.clear();
  }

  return 0;
}

// three built-in commands(setenv,printenv,exit)
void callPrintenv(string envVar){
  const char* input = envVar.c_str() ;
  char* path_string = getenv (input);

  if (path_string!=NULL)
    cout <<path_string<<endl;
}

void callSetenv(string envVar,string value){
  const char *envname = envVar.c_str();
  const char *envval = value.c_str();
  setenv(envname,envval,1);
}
