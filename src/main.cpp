#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include "duepoll.h"

using namespace DURIANVER;

void task(const char* buf,const int count,char* wbuf,int &len){
    for(int i=0;i<count;i++){
        wbuf[i]=buf[i]+1;
    }
    len=count;
}

int main() {
    Epoll netserver(18868);
    netserver.setTaskCallback(std::bind(&task,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
    netserver.start();
    return 0;
}