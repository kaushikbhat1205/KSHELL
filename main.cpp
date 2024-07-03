#include "bits.h"
//#include<sys/wait.h>
//#include<sys/types.h>
#include<unistd.h>

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


int main() {
    while(1){
        string cmmnd = "";
        getCommand(cmmnd);

        vector<string>tokens;
        tokens=parseInput(cmmnd);

        if(tokens[0] == "exit"){
            displayPrompt();
            cout<<"exiting....."<<endl;
            break;
        }
        else
            executeCommand(tokens);
        cout<<endl;
    }
    return 0;
}
