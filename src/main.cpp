#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>

#define SERVER_PORT 18868
#define LISTEN_QUEUE_LENGTH 1024
#define BUFFER_SIZE 1024



int main() {


    while(1){
        struct sockaddr_in clientAddr;
        socklen_t client_len= sizeof(clientAddr);
        int connfd=accept(listenFd,(struct sockaddr*)&clientAddr,&client_len);
        if(connfd<0){
            std::cout<<"Accept Failed";
            break;
        }
        char buffer[BUFFER_SIZE];
        bzero(buffer,BUFFER_SIZE);
        size_t recv_len=recv(connfd,buffer,BUFFER_SIZE,0);
        if(recv_len<0){
            std::cout<<"Recieve Data Failed";
            break;
        }
        if(send(connfd,buffer,BUFFER_SIZE,0)<0){
            std::cout<<"Send Data Failed";
            break;
        }
        close(connfd);
    }
    close(listenFd);
    return 0;
}