#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include "duepoll.h"

using namespace DURIANVER;
using namespace std::placeholders;

void task(const int fd){
    char buf[1024];
    while (1) {
        int count = read(fd, buf, sizeof(buf));
        if(count==0)
            break;
        else if(count<0){
            if(errno!=EAGAIN)
                break;
        }
        else
            write(fd,buf,count);
    }
}

int main() {
    Epoll netserver(18868);
    netserver.setTaskCallback(std::bind(&task,_1));
    netserver.start();
    return 0;
}