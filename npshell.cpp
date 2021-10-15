#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LENGTH 15000
using namespace std;

void callPrintenv(string envVar);
void callSetenv(string envVar,string value);

int main(int argc, char *argv[]) {
  string input ="" ; 
  string aWord ="" ;
  stringstream ss;
  vector<string> commandVec;
 
  int child_done_status ;
  pid_t child_done_pid ;
  pid_t fork_pid ;
 
  // set $PATH to bin/ ./ initially
  callSetenv("PATH","bin:.");
  cout <<"% ";  
 
  while(getline(cin,input)){
    ss << input ;
    while (ss >> aWord) {
      commandVec.push_back(aWord);
    }
    // We want to check if the command is the three built-in command
    if(commandVec[0]=="exit"){
      break ;
    }else if(commandVec[0]=="printenv"){
      if(commandVec.size()==2){
        callPrintenv(commandVec[1]);     
      } 
    }else if(commandVec[0]=="setenv"){
      if(commandVec.size()==3){
        callSetenv(commandVec[1],commandVec[2]);    
      }
    }else{
      // Not the three built-in command
      // Ready to handle the command. 
      fork_pid = fork();

      if(fork_pid ==-1){ //fork Error
        cout <<"fork error\n" ;
        exit(-1);      
      }else if(fork_pid ==0){ // Child
        // handle execvp argument
        int arg_index =0;
        char* arg[MAX_LENGTH] ;
        for(int i=0;i<commandVec.size();i++){
          arg[i] = strdup(commandVec[i].c_str());
          arg_index++;
        }
      
        // Ready to execvp
        cout <<"[Child] ready to execvp\n";
        if(execvp(arg[0],arg)==-1){ // execvp fail
          cout <<"Unknown command: ["<<arg[0]<<"].\n";
          exit(10);
        }
  
      }else if(fork_pid >0){ //Parent
        cout <<"[Parent] child's pid:"<< fork_pid <<"\n" ;
        cout <<"[Parent] ready to wait child\n";
        child_done_pid = wait(&child_done_status);
        cout <<"[Parent] had waited child and child_pid,status:"<<child_done_pid<<" "<<child_done_status<<"\n";
      }
    }

    //one term command is done -> Initialize it.
    commandVec.clear();
    ss.str("");
    ss.clear();
    cout <<"% ";
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
