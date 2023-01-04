#ifndef CLIENT_H_
#define CLIENT_H_
#include "./common.h"

struct logList
{
	char IP[200], port[10], sno[2], name[100];
};

class Client
{
	int sockfd, p2pFileDescriptor, fdmax, listsize, a;
	int port, b;
	char myAddr[20], addresssize;
	struct addrinfo hints, *myres, *res, newhints;
	vector<struct Block> MyBlock;
	fd_set master, read_fds;
	vector<logList> L;
	bool isClientLoggedIn;
	bool isBroadcast;
	struct sockaddr_in peer;

public:
	Client(char *arg);

	void portCommand();

	void ipCommand();
	bool ipAddCommand();

	void divideArray(char *p);
	void sendall(int sockfd, char *type, char *buf);
	void handleInput(char *input);
	char *receiveMessage(int sockfd, char *message);
	void printList();
	void updateList(vector<char *> s);

	void handleNewConnections(int sockfd);

	char *getFirstCommandArgument(char *input);
	char *getSecondCommandArgument(char *input);
	char *getThirdCommandArgument(char *input);

	bool isPAddressValid(char *IP);
	bool isValidPort(char *Portent);
	bool isClientPresentInConnectedList(char *IP);
	char *findPort(char *IP);
	bool isBlocked(char *IP);
	bool removeBlock(char *IP);
	void sendFile(int *sock, char *fileName);
	void receiveBytes(int sock);
	void startClient();
};

#endif
