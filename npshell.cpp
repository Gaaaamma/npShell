#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LENGTH 15000
using namespace std;

void callPrintenv(string envVar);
void callSetenv(string envVar,string value);
void singleProcess(vector<string>commandVec);
void multiProcess(vector<string>commandVec,int process_count);

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
		if(commandVec.size()!=0 && commandVec[0]=="exit"){
			break ;
		}else if(commandVec.size()!=0 && commandVec[0]=="printenv"){
			if(commandVec.size()==2){
				callPrintenv(commandVec[1]);     
			} 
		}else if(commandVec.size()!=0 && commandVec[0]=="setenv"){
			if(commandVec.size()==3){
				callSetenv(commandVec[1],commandVec[2]);    
			}
		}else{
			// Not the three built-in command
			// Ready to handle the command. 

			// We want to know how much process need to call fork()
      		int process_count =1 ;
      		for(int i=0;i<commandVec.size();i++){
        		if(commandVec[i].find("|")!= string::npos){ //Really find '|' in this element
          			if(commandVec[i].length()==1){ // '|' Pipe only
            			process_count ++ ;
          			}else{ // it is number pipe
						// ... not yet develop.             
          			}
        		}
      		} 
			
			// Now we have the number of processes 
			// we can start to construct the pipe.
			if(process_count ==1){
				singleProcess(commandVec);	
			}else if(process_count>=2){
				multiProcess(commandVec,process_count);				
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
// Single process handle 
void singleProcess(vector<string> commandVec){
	pid_t child_done_pid;
	int child_done_status;
	// If number pipe is expired, we need to handle it.
	// ...
	
	pid_t fork_pid = fork();
	if(fork_pid ==-1){ //fork Error
  		cout <<"fork error\n" ;
    }else if(fork_pid ==0){ // Child
   		// handle execvp argument
       	char* arg[MAX_LENGTH] ;
        for(int i=0;i<commandVec.size();i++){
        	arg[i] = strdup(commandVec[i].c_str());
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
// Multi process handle
void multiProcess(vector<string> commandVec,int process_count){
	// First handle all of the command and it's argument
	int process_index =0;
	int process_cmd_index=0;
	pid_t child_done_pid;
	pid_t fork_pid;
	int child_done_status;
	char* arg[process_count][256]={NULL} ;

	for(int cmd_index=0; cmd_index<commandVec.size() ; cmd_index++){
		if( commandVec[cmd_index].find("|")!= string::npos){ //Find | in this string
			if(commandVec[cmd_index].length() ==1){
				// single "|"
				process_index ++;
				process_cmd_index =0 ;
			}else{
				// Number pipe.
			}
		}else{
			arg[process_index][process_cmd_index] = strdup(commandVec[cmd_index].c_str());
			process_cmd_index ++ ;
		}		
	}

	// All argument of process is handled.
	for(int process_index=0; process_index<process_count ; process_index++){
		fork_pid = fork();
		if(fork_pid ==-1){
			cout <<"multiProcess(...): fork error times:"<<process_index<<"\n" ;
		}else if(fork_pid ==0){ //Child			
   			// Ready to execvp
    	    cout <<"[Child] ready to execvp\n";
       		if(execvp(arg[process_index][0],arg[process_index])==-1){ // execvp fail
   				cout <<"Unknown command: ["<<arg[process_index][0]<<"].\n";
       			exit(10);
       		}	
		}else if(fork_pid >0){ //Parent	
   			cout <<"[Parent] child's pid:"<< fork_pid <<"\n" ;
			cout <<"[Parent] ready to wait child\n";
			child_done_pid = wait(&child_done_status);	
			cout <<"[Parent] had waited child_pid,status:"<<child_done_pid<<" "<<child_done_status<<"\n";
		}	
	}	
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
