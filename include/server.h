#ifndef SERVER_H_
#define SERVER_H_
using namespace std;
#include "./common.h"

class Server
{
	// Refered from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf CHAPTER 7. SLIGHTLY ADVANCED TECHNIQUES Page: 50
	fd_set master;		// master file descriptor list
	fd_set read_fds;	// temp file descriptor list for select()
	int listenerSockFd; // listening socket descriptor
	int newfd;			// newly accept()ed socket descriptor
	vector<struct ClientNode> connectedClientList;
	struct sockaddr_in res;
	struct sockaddr remoteaddr; // client address
	int fdmax;					// maximum file descriptor number
	socklen_t addrlen;
	fd_set write_fds;
	int port;

public:
	Server(char *P);
	void ipCommand();

	void portCommand();

	char *get_list_of_connected_clients(char *m);

	void displayList();

	void sendall(int sock, char *type, char *buf);

	char *receiveMessage(int sockfd, char *buf);
	char *getFirstCommandArgument(char *input);

	char *getSecondCommandArgument(char *input);
	char *getThirdCommandArgument(char *input);
	int findClient(char *IP);

	int findClient(int sock);
	bool isBlocked(int sendsock, int rind);
	void removeBlock(int sockfd, char *IP);

	void printStatistics();

	void printBlockList(char *IP);
	char *prepareBuffer(int rind, char *message);
	bool isPAddressValid(char *IP);
	void startServer();
};

#endif