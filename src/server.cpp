/**
 * @skunusot_assignment1
 * @author  Sandeep Kunusoth <skunusot@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#pragma warning(push, 0)        
#include "../include/server.h"

struct Comparator
{
	bool operator()(ClientNode a, ClientNode b)
	{
		return !(a.port >= b.port);
	}
};

Server::Server(char *P)
{					   // Refered from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf CHAPTER 7. SLIGHTLY ADVANCED TECHNIQUES Page: 50
		short int yes = 1; // for setsockopt() SO_REUSEADDR, below

		// get us a socket and bind it
		memset(&res, 0, sizeof(res));
		__socket_type socketNo = SOCK_STREAM;
		res.sin_family = AF_INET;
		stringstream portdiff(P);

		if ((portdiff >> port))
			socketNo = SOCK_STREAM;
		else
			port = 0;

		res.sin_addr.s_addr = htonl(INADDR_ANY);
		res.sin_port = htons(port);

		// lose the pesky "address already in use" error message
		if (((listenerSockFd = socket(AF_INET, socketNo, 0)) == -1) || (setsockopt(listenerSockFd, 1, 2, &yes, sizeof(int)) == -1) || (bind(listenerSockFd, (struct sockaddr *)&res, sizeof(res)) == -1) || (listen(listenerSockFd, 40) == -1))
			exit(-1);
	}

void error_print_s(char *command_str)
{
	cse4589_print_and_log("[%s:%s]\n", command_str, ERROR);
	fflush(NULL);
	cse4589_print_and_log("[%s:%s]\n", command_str, END);
	fflush(NULL);
}

void Server::ipCommand()
{
	char IP[2000];
	struct ifaddrs *ifa, *p;
	char command_str[] = "IP";
	if (getifaddrs(&ifa) == -1) // Referred from https://man7.org/linux/man-pages/man3/getifaddrs.3.html
	{
		error_print_s(command_str);
		return;
	}
	p = ifa;

	while ( p != NULL)
	{
		struct sockaddr *s = p->ifa_addr;
		struct ifaddrs *erc, *r;
		if (s->sa_family == AF_INET)
		{
			inet_ntop(AF_INET, &(((struct sockaddr_in *)s)->sin_addr), IP, sizeof(IP));

			if (!(strcmp(IP, LOCAL_HOST) == 0))
			{
				cse4589_print_and_log("[%s:%s]\n", command_str, SUCCESS);
				fflush(NULL);
				cse4589_print_and_log("IP:%s\n", IP);
				fflush(NULL);
				cse4589_print_and_log("[%s:%s]\n", command_str, END);
				fflush(NULL);
				break;
			}
		}
		p = p->ifa_next;
	}
}

void Server::portCommand()
{
	char command_str[] = "PORT";
	cse4589_print_and_log("[%s:%s]\n", command_str, SUCCESS);
	fflush(NULL);
	cse4589_print_and_log("PORT:%d\n", ntohs(res.sin_port));
	fflush(NULL);
	cse4589_print_and_log("[%s:%s]\n", command_str, END);
	fflush(NULL);
}
		
char *Server::get_list_of_connected_clients(char *m)
{
	char *p = new char[500];
	p[0] = '\0';
	short int i;
	char host[200], service[200];
			struct sockaddr_in s;
			vector<char *> str;
	socklen_t slen = sizeof(s);
			
	std::sort(connectedClientList.begin(), connectedClientList.end(), Comparator());
			
	while (i < connectedClientList.size())
	{
		if (connectedClientList[i].reg && connectedClientList[i].log_status)
		{
			strcat(p, "-");

					stringstream stream;
						vector<char *> str1;
			stream << (i + 1);
			string temp = stream.str();
					string temp1 = temp;
					char *num = new char[temp.length() + 1];
					str1.insert(str1.end(), num);
					strcpy(num, temp.c_str());

					strcat(p, num);
					strcat(p, " ");
					str1.insert(str1.end(), num);
					inet_aton(connectedClientList[i].IP, &s.sin_addr);
					temp1 = stream.str();  
					str1.insert(str1.end(), num);
					s.sin_family = AF_INET;
				
			getnameinfo((struct sockaddr *)&s, slen, host, sizeof(host), service, sizeof(service), NI_NAMEREQD);
					str1.insert(str1.end(), num);
					strcat(p, host);
					
					strcat(p, " ");
			inet_ntop(AF_INET, &(s.sin_addr), host, INET_ADDRSTRLEN);

					strcat(p, host);
					str1.insert(str1.end(), num);
					strcat(p, " ");
					stream.str("");
					str1.insert(str1.end(), num);
					stream << connectedClientList[i].port;
					temp = stream.str();
					str1.insert(str1.end(), num);
					delete[] num;
					num = new char[temp.length() + 1];
					strcpy(num, temp.c_str());
					str1.insert(str1.end(), num);
					strcat(p, num);
					delete[] num;
				}
				i++;
			}	
				
			strcpy(m, p);
		
			return m;
		}

void Server::displayList()
{	
	char hostname[200], service[100];
	sockaddr_in s;
	struct sockaddr_in s_in;
	socklen_t size = sizeof(s);
	stringstream stream;
	int i = 0;
	char command_str[] = "LIST";
	cse4589_print_and_log("[%s:%s]\n", command_str, SUCCESS);
	fflush(NULL);
	while (i < connectedClientList.size())
	{	
		ClientNode client = connectedClientList[i];
		if (client.reg && client.log_status)
		{
			inet_aton(client.IP, &s.sin_addr);
			s_in.sin_family = AF_INET;
			stream << i;
			s.sin_family = AF_INET;
			getnameinfo((struct sockaddr *)&s, size, hostname, sizeof(hostname), service, sizeof service, NI_NAMEREQD);
			cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i + 1, hostname, client.IP, client.port);
			fflush(NULL);
		}
		i++;
	}	
	cse4589_print_and_log("[%s:%s]\n", command_str, END);
	fflush(NULL);
}
	
void Server::sendall(int sock, char *type, char *buf)
		{ // Referred from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf 7.4 Handling Partial send()s
			char *message = new char[1048];
			strcat(type, buf);
			int total = 0, size = strlen(type), sock1 = 0;
			strcpy(message, type);
			long int n, sock2 = 0, y = 0;
			send(sock, &size, sizeof(size), 0);
			for (;total < size;)
			{
				n = send(sock, message + total, size, 0);
				y =1;
				if (n == -1)
					break;
				int sock3 = 1;
				total += n;
				size -= n;
				sock2 = 1;
			}
			delete[] message;
		}

char *Server::receiveMessage(int listenerSockFd, char *buf) 
{
	// Referred from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf stream sockets and recv()
	int size;
	char *str = new char[1500];
	short int i = 0;
	if (recv(listenerSockFd, &size, sizeof(size), 0) == 0)
	{	
		FD_CLR(listenerSockFd, &master); // Remove fd from the mteras
		while (i < connectedClientList.size())
		{
			int j = i;
			if (connectedClientList[i].sockfd == listenerSockFd)
				connectedClientList.erase(connectedClientList.begin() + j);
			i++;
		}

		close(listenerSockFd);
	}
	
			int n;
			int total = 0;
	while (total < size)
	{
		n = recv(listenerSockFd, str + total, size, 0);
					int j = 0;
		if (n == -1)
					break;

		total += n;
		j = n;
		size-=n;
	}

	str[total] = '\0';

return str;
			}


int Server::findClient(int sock)
{
	int i = 0;
	for (; i < connectedClientList.size(); i++)
		if (connectedClientList[i].sockfd == sock)
			break;
	if (i!= connectedClientList.size()) return i;
	else return -1;
}

char *Server::getFirstCommandArgument(char *input)
{
	char *first = new char[30];
    short int j = 0, i = 0;
    while ( input[i] != ' ' && input[i] != '\0')
    {
        first[j] = input[i];
        i++;
        j++;
    }
    char ch = '\0';
    first[j] = ch;
    return first;
}

char *Server::getSecondCommandArgument(char *input)
{
	char *second = new char[300];
    int j = 0, i = 0;
    char ch = '\0';
    while (input[i] != ' ' && input[i] != '\0')
        i++;

    i++;
    while (input[i] != ' ' && input[i] != '\0')
    {
        second[j] = input[i];
        i++;
        j++;
    }

    second[j] = ch;
    return second;
}

char *Server::getThirdCommandArgument(char *input)
{
	char *third = new char[1000];
        char ch = '\0';
    int j = 0, i = 0;
    while (input[i] != ' ' && input[i] != '\0')
        i++;

    i++;
    while (input[i] != ' ' && input[i] != '\0')
        i++;
    i++;

    while (input[i] != '\0')
    {
        third[j] = input[i];
        i++;
        j++;
    }

    third[j] = ch;

    return third;
}

int Server::findClient(char *IP)
{
	int i = 0;
	for (; i < connectedClientList.size(); i++)
		if (strcmp(connectedClientList[i].IP, IP) == 0)
			break;
	if (i!= connectedClientList.size()) return i;
	else return -1;
}
			

bool Server::isBlocked(int sendsock, int rind)
{
	int i = 0;
	int index = findClient(sendsock);

	while (i < connectedClientList[rind].BlockList.size()) {
		if (strcmp(connectedClientList[index].IP, connectedClientList[rind].BlockList[i].IP) == 0)
			break;
		i++;
	}

	if (i==connectedClientList[rind].BlockList.size()) return false;
	else return true;
}

void Server::removeBlock(int listenerSockFd, char *IP)
{
	int index;
	int eind;
	int i = 0;
	index = findClient(listenerSockFd);

	while ( i < connectedClientList[index].BlockList.size())
	{
		if (strcmp(connectedClientList[index].BlockList[i].IP, IP) == 0)
		{
			eind = i;
			break;
		}
		 i++;
	}

	connectedClientList[index].BlockList.erase(connectedClientList[index].BlockList.begin() + eind);
}

void Server::printStatistics()
{
	char hostname[200], service[100];
	sockaddr_in s_in;
	sockaddr_in s;
	socklen_t size = sizeof(s);
	cse4589_print_and_log("[%s:%s]\n", "STATISTICS", SUCCESS);
	char log[20];
	for (int i = 0; i < connectedClientList.size(); i++)
	{
		if (connectedClientList[i].reg)
		{
			inet_aton(connectedClientList[i].IP, &s.sin_addr);
			s.sin_family = AF_INET;
			socklen_t size_in = sizeof(s_in);
			getnameinfo((struct sockaddr *)&s, size, hostname, sizeof(hostname), service, sizeof service, NI_NAMEREQD);

			char* format = "%-5d%-35s%-8d%-8d%-8s\n";
			if (connectedClientList[i].log_status)
			{
				char* ch =  "logged-in";
				strcpy(log, ch);
			}
			else if (!connectedClientList[i].log_status)
			{
				char* ch = "logged-out";
				strcpy(log, ch);
			}

			cse4589_print_and_log(format, i + 1, hostname, connectedClientList[i].num_sent, connectedClientList[i].num_recv, log);
			fflush(NULL);
		}
	}
	char ch[] = "STATISTICS";
	cse4589_print_and_log("[%s:END]\n",ch);
}	

void Server::printBlockList(char *IP)
{
	sockaddr_in s, s_in;
	char hostname[200], service[100];
	socklen_t size = sizeof(s);
	int i;
	int j=0;
	vector<ClientNode> tempBlock;
	socklen_t size_in = sizeof(s_in);
	int index = findClient(IP);
	if (index != -1 && connectedClientList[index].reg == true)
	{
		cse4589_print_and_log("[%s:%s]\n", "BLOCKED", SUCCESS);
		fflush(NULL);
		i =0;
		while ( i < connectedClientList[index].BlockList.size())
		{
			int j = findClient(connectedClientList[index].BlockList[i].IP);
			tempBlock.insert(tempBlock.end(), connectedClientList[j]);
			i++;
		}

		i = 0;
		std::sort(tempBlock.begin(), tempBlock.end(), Comparator());
		while ( i < tempBlock.size())
		{
			inet_aton(tempBlock[i].IP, &s.sin_addr);
			s.sin_family = AF_INET;
			socklen_t size_in = sizeof(s);
			if (getnameinfo((struct sockaddr *)&s, size, hostname, sizeof(hostname), service, sizeof service, NI_NAMEREQD)==0);

			cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", i + 1, hostname, tempBlock[i].IP, tempBlock[i].port);
			fflush(NULL);

			i++;
		}
		cse4589_print_and_log("[%s:%s]\n", "BLOCKED", END);
		fflush(NULL);
	}
	else
	{
		// Incorrect IP address
		error_print_s("BLOCKED");
	}
}

char *Server::prepareBuffer(int rind, char *message)
		{
	char *mod = new char[1024];
			sockaddr_in s;
	char *third = new char[1500];
			socklen_t size = sizeof(s);
	third = getThirdCommandArgument(message);
	mod[0] = message[0];
	mod[1] = message[1];
			int i=0;
	strcat(mod, connectedClientList[rind].IP);
	strcat(mod, " ");
			i=1;
	strcat(mod, third);
	cout << mod << endl;
			size = sizeof(s);
			delete[] third;
			return mod;	 
		}

bool Server::isPAddressValid(char *IP)
		{
	// referred from beej's example on how to use inet_pton(),inet_ntop() functions

			struct sockaddr_in s;
			return inet_pton(AF_INET, IP, &(s.sin_addr))!=0;
		}
void Server::startServer()
{
	// keeping track of the biggest file descriptor
	fdmax = listenerSockFd;
	int buf_len = 0;
	// add the listener to the master set
	FD_SET(STDIN, &master);
	FD_SET(listenerSockFd, &master);
	struct sockaddr_in s;
	int buflen, i, index;
	char buf[500], remoteIP[INET6_ADDRSTRLEN]; // buffer for client data
	while (1 == 1)
	{ // Select part from beej's example and Wenjun's material
		read_fds = master;
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1);

		for (i = 0; i <= fdmax; i++)
			if (FD_ISSET(i, &read_fds))
			{	
				socklen_t size = sizeof(s);
				if (i == listenerSockFd)
				{
					// handle new connections
					addrlen = sizeof(remoteaddr);
					newfd = accept(listenerSockFd, &remoteaddr, &addrlen);

					if (newfd == -1)
					{
						perror("accept");
						exit(-1);
					}
					else
					{
						FD_SET(newfd, &master);					 // add to master set
						(newfd > fdmax) ? fdmax = newfd : fdmax; // keep track of the max
						struct ClientNode temp;
						inet_ntop(AF_INET, &(((struct sockaddr_in *)&remoteaddr)->sin_addr), remoteIP, INET_ADDRSTRLEN);
						size = sizeof(s);
						recv(newfd, &temp.port, sizeof(int), 0);

						if ((index = findClient(remoteIP)) != -1)
						{
							// remaining clients login
							connectedClientList[index].reg = true;
							connectedClientList[index].log_status = true;
							size = sizeof(s);
							strcpy(connectedClientList[index].IP, remoteIP);
							connectedClientList[index].sockfd = newfd;
							char type[] = "L ";
							connectedClientList[index].port = temp.port;
						}
						else
						{
							// first client log in
							strcpy(temp.IP, remoteIP);
							temp.sockfd = newfd;
							size = sizeof(s);
							temp.num_sent = 0;
							temp.num_recv = 0;
							char type[] = "L ";
							temp.reg = true;
							temp.log_status = true;
							connectedClientList.insert(connectedClientList.end(), temp);
						}

						// updating list of connected clients in server
						char list[500], *l;
						char list1[] = "O ";
						l = get_list_of_connected_clients(list);
						char type[] = "L ";
						sendall(newfd, type, list);
					}
				}
				else if (i == 0)
				{
					char input[500] = "";
					fgets(input, sizeof(input), stdin);
					char *p, l[500];
					input[strlen(input) - 1] = '\0';
					char *first = getFirstCommandArgument(input);

					if (strcmp(first, "LIST") == 0)	
					{
						p = get_list_of_connected_clients(l);
						displayList();
					}
					else if (strcmp(first, "AUTHOR") == 0)
					{
						cse4589_print_and_log("[%s:%s]\n",  "AUTHOR", SUCCESS);
						fflush(NULL);
						cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", UBIT_NAME);
						fflush(NULL);
						cse4589_print_and_log("[%s:%s]\n",  "AUTHOR", END);
						fflush(NULL);
					}
					else if (strcmp(input, "EXIT") == 0)
						close(listenerSockFd);
					else if (strcmp(first, "PORT") == 0)
						portCommand(); 
					else if (strcmp(first, "BLOCKED") == 0)
					{
						char *IP = getSecondCommandArgument(input);
						if (!isPAddressValid(IP))
							error_print_s("BLOCKED");
						else
							printBlockList(IP);
					}
					else if (strcmp(first, "STATISTICS") == 0)
					{
						printStatistics();
					}
					else if (strcmp(first, "IP") == 0)
						ipCommand(); 
				}

				else
				{
					char response[1500];
					char *message = receiveMessage(i, response);		
							
					if (message[0] == '0')
					{
						char list[500];
						char *p = get_list_of_connected_clients(list);
						char type[1024]="0 "; //#updated
						sendall(i,type,p);
					}
					else if (message[0] == '1')
					{
						int j;
						int send = findClient(i);
						connectedClientList[send].num_sent = connectedClientList[send].num_sent + 1;
						char *IP = getSecondCommandArgument(message);
						char *third = getThirdCommandArgument(message);
						char m[1400];
						j = findClient(IP);
						if (j == -1)
						{
							strcpy(m, message);
							struct ClientNode temp2;
							strcpy(temp2.IP, IP);
							temp2.num_sent = 0;
							temp2.reg = false;
							temp2.num_recv = 0;
							temp2.log_status = false;
							connectedClientList.push_back(temp2);
							int ind = findClient(temp2.IP);
							if(ind != -1)
							{	
								struct message mbuf;
								char *prep = prepareBuffer(send, message);
								strcpy(mbuf.info, prep);
								connectedClientList[ind].buffer.push_back(mbuf);
							}
						}
						else
						{	

							if (!isBlocked(i, j))
							{	
								char type[] = "";
								
								strcpy(m, message);

								if (connectedClientList[j].log_status)
								{	
									char *prep = prepareBuffer(send, m);
									sendall(connectedClientList[j].sockfd, prep, type);
									sockaddr seedaddr;
									connectedClientList[j].num_recv++;	
									//  Relaying Send								
									cse4589_print_and_log("[RELAYED:SUCCESS]\n");
									fflush(NULL);
									cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",connectedClientList[send].IP,connectedClientList[j].IP,third);
									fflush(NULL);
									cse4589_print_and_log("[RELAYED:END]\n");
									fflush(NULL);
								}
								else
								{
									struct message mbuf;
									vector<struct message> buffer;
									char *prep = prepareBuffer(send, message);
									strcpy(mbuf.info, prep);
									connectedClientList[j].buffer.insert(connectedClientList[j].buffer.end(), mbuf);
								}
							}	
						}
					}

					else if (message[0] == '2')
					{
						int send = findClient(i);
						connectedClientList[send].num_sent++;
						sockaddr seedaddr;
						int client;
						bool flag = false;
						for (int j = 1; j <= fdmax; j++)
						{
							char ch[] = "";
							client = findClient(j);
							if (FD_ISSET(j, &master))
							{
								if (j != listenerSockFd && j != i && connectedClientList[client].log_status && !isBlocked(i, client))
								{
									// check
										connectedClientList[client].num_recv++;
										sendall(j, message, ch);
										flag = (j == j);
								}
							}

							else if (client != -1 && !isBlocked(i, client))
							{ 
								// handle bUffering
								struct message m;
								strcpy(m.info, message);
								connectedClientList[client].buffer.insert(connectedClientList[client].buffer.end(), m);
							}
						}
						if (flag)
						{
							// broadcast
							char *third = getThirdCommandArgument(message);
							cse4589_print_and_log("[RELAYED:%s]\n", SUCCESS);
							fflush(NULL);
							cse4589_print_and_log("msg from:%s, to:255.255.255.255\n[msg]:%s\n", connectedClientList[send].IP, third);
							fflush(NULL);
							cse4589_print_and_log("[RELAYED:%s]\n", END); //#updated
							fflush(NULL);
						}
					}
					else if (message[0] == '3')
					{
						index = findClient(i);
				
						if (index != -1)
						{
							char *second = getSecondCommandArgument(message);
							Block tempBlock;
							strcpy(tempBlock.IP, second);
							connectedClientList[index].BlockList.insert(connectedClientList[index].BlockList.end(), tempBlock);
						}
					}

					else if (message[0] == '4')
					{

						index = findClient(i);

						if (index != -1)
						{
							char *second = getSecondCommandArgument(message);
							Block tempBlock;
							removeBlock(i, second);
						}
					}
					else if (message[0] == '9')
					{
						int ind = findClient(i);
						connectedClientList[ind].log_status = false;
						connectedClientList[ind].reg = false;
						Block tempBlock;
						connectedClientList[ind].num_sent = 0;
						connectedClientList[ind].num_recv = 0;
						int j = i;
						FD_CLR(j, &master);
						close(j);
					}
					else if (message[0] == 'X')
					{
						int ind = findClient(i);
						// updating status of current client to logout
						connectedClientList[ind].log_status = false;
						int j =i;
						FD_CLR(j, &master);
						close(j);
					}
					else if (message[0] == 'B')
					{
						int j = findClient(i);
						int c = -1;
						int k = c;
						char *from, *message;
						int size = connectedClientList[j].buffer.size();
						k=j;
						if (size > 1)
						{
							char type[]="B";  //#updated
							sendall(connectedClientList[j].sockfd, type, connectedClientList[j].buffer[0].info);
							sockaddr peeraddr;
							from = getSecondCommandArgument(connectedClientList[j].buffer[0].info);
							message = getThirdCommandArgument(connectedClientList[j].buffer[0].info);
							connectedClientList[j].num_recv++;
							socklen_t plen = sizeof(peeraddr);
							connectedClientList[j].buffer.erase(connectedClientList[j].buffer.begin());

							if (connectedClientList[j].buffer[0].info[0] == '2') c = 1; else c = 0;
						}

						else if (size == 1)
						{
							char type[]="F";  //#updated
							sendall(connectedClientList[j].sockfd, type, connectedClientList[j].buffer[0].info);
							connectedClientList[j].num_recv++;
							sockaddr peeraddr;
							from = getSecondCommandArgument(connectedClientList[j].buffer[0].info);
							message = getThirdCommandArgument(connectedClientList[j].buffer[0].info);
							socklen_t plen = sizeof(peeraddr);
							connectedClientList[j].buffer.erase(connectedClientList[j].buffer.begin());

							if (connectedClientList[j].buffer[0].info[0] == '2') c = 1; else c = 0;
						}
						else
						{
							char type[]="N";
							char resp[]=" ";  //#updated
							sendall(connectedClientList[j].sockfd, type, resp);
						}

						if (c == 1)
						{
							// Sending the data to the “global” broadcast address.
							char* ch = "RELAYED";
							cse4589_print_and_log("[RELAYED:%s]\n", SUCCESS);  //#updated
							fflush(NULL);
							cse4589_print_and_log("msg from:%s, to:255.255.255.255\n[msg]:%s\n", from, message);
							fflush(NULL);
							cse4589_print_and_log("[RELAYED:%s]\n", END);  //#updated
							fflush(NULL);
						}
						else if (c == 0)
						{
							cse4589_print_and_log("[RELAYED:%s]\n", SUCCESS);
							fflush(NULL);
							cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", from, connectedClientList[j].IP, message);
							fflush(NULL);
							cse4589_print_and_log("[RELAYED:END]\n");
							fflush(NULL);
						}
					}
					delete[] message;
						}
					}
				}
		}
	

#pragma warning(pop)
