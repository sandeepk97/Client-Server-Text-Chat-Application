#ifndef COMMON_H_
#define COMMON_H_

#include<iostream>
#include<sstream>
#include<string>
#include<arpa/inet.h>
#include<cctype>
#include<algorithm>
#include<cstdlib>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<vector>
#include<ifaddrs.h>
#include<netdb.h>
#include<stdio.h>
#include<signal.h>
#include<sys/socket.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

#define PORT "9000"

#define STDIN 0   // file descriptor for standard input
#define UBIT_NAME "skunusot"
#define LOCAL_HOST "127.0.0.1"
#define SUCCESS "SUCCESS"
#define ERROR "ERROR"
#define END "END"


#include "./message.h"
#include "./block.h"
#include "./global.h"
#include "./logger.h"

/* Client structure */
struct ClientNode
{
	bool reg;
        bool log_status;
        int ack;
        int port;
        int sockfd;
        int total;
        int num_recv;
	int num_sent;
        char IP[50];
        bool blocked;
        vector<struct Block>BlockList;
	vector<struct message>buffer;
};


#endif