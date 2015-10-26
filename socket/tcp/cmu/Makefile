CC = gcc
ARGS = -Wall

all: tcpserver tcpclient udpserver udpclient selectserver

tcpserver: tcpserver.c
	$(CC) $(ARGS) -o tcpserver tcpserver.c

tcpclient: tcpclient.c
	$(CC) $(ARGS) -o tcpclient tcpclient.c

udpserver: udpserver.c
	$(CC) $(ARGS) -o udpserver udpserver.c

udpclient: udpclient.c
	$(CC) $(ARGS) -o udpclient udpclient.c

selectserver: selectserver.c
	$(CC) $(ARGS) -o selectserver selectserver.c

clean:
	rm -f *.o tcpserver udpserver tcpclient udpclient selectserver *~

