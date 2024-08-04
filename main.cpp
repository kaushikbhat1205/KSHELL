#include "bits.h"
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<fstream>

using namespace std;

void displayPrompt(){
    cout << "kshell>> " ;
    cout.flush();
}
void getCommand(string &cmmnd){
    displayPrompt();
    getline(cin,cmmnd);
}
vector<string> parseInput(string cmmnd){
    stringstream ss(cmmnd);
    string token;
    vector<string >tokens;
    while(ss>>token){
        tokens.push_back(token);
    }
    return tokens;
}

void executeCommand(vector<string>&args){
    if(args.empty())return ;
    pid_t pid =fork();

    if(pid==-1){
        perror("fork");
    }else if(pid == 0) {
        vector<char *> chars;
        for (const auto &arg: args) {
            chars.push_back(const_cast<char *>(arg.c_str()));
        }
        chars.push_back(nullptr);
        execvp(chars[0], chars.data());
        perror("execvp");
        exit(1);
    }else{
        //parent process
        int status;
        waitpid(pid,&status,0);
    }
}
void executeCatFileCreation(vector<string>&args){
    if(args.size() !=3)cerr<<"cat:operand missing/n";
    else{
        string fileName = args[2];
        ofstream outfile(fileName);
        if(!outfile.is_open()){
            cerr<<"Could'nt open file " << fileName << "/n";
            return ;
        }
        string line;
        while(getline(cin,line)){
            if(line=="^D")break;
            outfile<<line<<"/n";
        }
        if(!cin.eof())cerr<<"error while getting input/n";

        outfile.close();
    }
}

void executecd(vector<string>&args){
    if(args.size()<2){
        cerr<<"cd:operand missing/n";
        return ;
    }else{
        if(chdir(args[1].c_str())!=0){
            perror("cd");
        }
    }
}

int main() {
    while(1){
        string cmmnd = "";
        getCommand(cmmnd);

        vector<string>tokens;
        tokens = parseInput(cmmnd);

        if(tokens[0] == "exit"){
            displayPrompt();
            cout<<"exiting....."<<endl;
            break;
        }else if(tokens[0]=="cat" && tokens[1]==">"){
            executeCatFileCreation(tokens);
        }else if(tokens[0]=="cd"){
            executecd(tokens);
        }
        else
            executeCommand(tokens);
        cout<<endl;
    }
    return 0;
}
