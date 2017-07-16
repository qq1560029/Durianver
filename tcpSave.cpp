#include <iostream>
#include <fstream>
#include <iomanip>
#include "unistd.h"
#include "netinet/in.h"
#include "memory.h"

std::ofstream logFile;
using namespace std;

int initlog(){
    std::string logFileName="tcplog.txt";
    logFile.open(logFileName.c_str(), std::ios::out | std::ios::app | std::ios::ate);
    if (logFile.bad()){
        cout<<"init log failed";
        return -1;
    }
    return 0;
}

int main()
{
    if(initlog()!=0){
        return -1;
    }
    int port = 43432;
    struct sockaddr_in serverAddr;
    //bzero((char *)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((unsigned short)port);

    int listenFd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET PF_INET almost same
    if (listenFd < 0)
        cout<<"get listen fd failed";

    int flag = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&flag, sizeof(int)) < 0)
        cout<<"set listen fd failed";

    if (bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        cout<<"bind listen fd failed";

    if(listen(listenFd,1024)<0)
        cout<<"listen failed";

    struct sockaddr inAddr;
    socklen_t inAddrLen = 0;
    int inFd = accept(listenFd, &inAddr, &inAddrLen);
    char rBuf[65536];
    if (inFd > 0)
    {
        int n=read(inFd,&rBuf,sizeof(rBuf));
        string logs(rBuf,n);
        logFile<<logs;
        logFile.close();
    }
    else
        cout<<"accept fd failed";

    return 0;
}