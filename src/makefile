BOOST= /usr/local
INC= -I${BOOST}/include/boost -I. -I./spdlog

LIBS= -pthread -L/{BOOST}/lib -lboost_system

CC= g++
CC_FLAG= -std=c++11 -Wall -g

PRG= durianver
OBJ= epollevent.o socket.o socketwrap.o tcpconnection.o loop.o tcpserver.o acceptor.o main.o buffer.o

$(PRG):$(OBJ)
	$(CC) $(INC) $(LIBS) -o $@ $(OBJ)

%.o:%.cpp
	$(CC) -c $< -o $@ $(CC_FLAG) $(INC) $(LIBS)

.PRONY:clean
clean:
	@echo"Removing linked and compiled files......"
	rm -f *.o
