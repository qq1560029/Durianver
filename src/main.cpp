#include <iostream>
#include <unistd.h>
#include "duepoll.h"
#include "logging.h"

using namespace DURIANVER;
using namespace std::placeholders;

void task(const int fd){
    char buf[1024];
    while (1) {
        int count = read(fd, buf, sizeof(buf));
        std::cout<<buf;
        if(count<=0){
            if(errno!=EAGAIN) {
                close(fd);
                break;
            }
        }
        else
            write(fd,buf,count);
    }
}

int main() {
    LOGINFO<<"Init server, port:18868";
    Epoll netserver(18868);
    netserver.setTaskCallback(std::bind(task,_1));
    netserver.start();
    return 0;
}