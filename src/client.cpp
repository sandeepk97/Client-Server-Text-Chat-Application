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

#include "../include/client.h"

Client::Client(char *arg)
{
    // refered from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf 6.3. DATAGRAM SOCKETS
    isClientLoggedIn = false;
    memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
    hints.ai_family = AF_INET;        // set to AF_INET to use IPv4
    hints.ai_flags = AI_PASSIVE;      // fill in my IP for me
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
    port = atoi(arg);
    if (getaddrinfo(NULL, arg, &hints, &myres) == -1);
    if ((p2pFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0);
    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    peer.sin_addr.s_addr = htonl(INADDR_ANY);
    const int yes = 1;
    if (setsockopt(p2pFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1);
    if (bind(p2pFileDescriptor, (struct sockaddr *)&peer, sizeof(peer)) == -1);
    if (listen(p2pFileDescriptor, 40) == -1);
}

void Client::portCommand()
{
    cse4589_print_and_log("[%s:%s]\n", "PORT", SUCCESS);
    fflush(NULL);
    cse4589_print_and_log("PORT:%d\n", port);
    fflush(NULL);
    cse4589_print_and_log("[%s:%s]\n", "PORT", END);
    fflush(NULL);
}

void Client::ipCommand()
{
    char IP[2000];
    struct ifaddrs *ifa, *p;
    if (getifaddrs(&ifa) == -1)
    {
        cse4589_print_and_log("[%s:%s]\n", "IP", ERROR);
        fflush(NULL);
        cse4589_print_and_log("[%s:%s]\n", "IP", SUCCESS);
        fflush(NULL);
        return;
    }

    p = ifa;
    while (p != NULL)
    {
        struct sockaddr *s;
        s = p->ifa_addr;

        if (s->sa_family == AF_INET)
        {
            // refered from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf Page 101
            inet_ntop(AF_INET, &(((struct sockaddr_in *)s)->sin_addr), IP, sizeof(IP));
            sockaddr_in s;
            if (strcmp(IP, LOCAL_HOST) != 0)
            {
                cse4589_print_and_log("[%s:%s]\n", "IP", SUCCESS);
                fflush(NULL);
                strcpy(myAddr, IP);
                cse4589_print_and_log("IP:%s\n", IP);
                fflush(NULL);
                cse4589_print_and_log("[%s:%s]\n", "IP", END);
                fflush(NULL);
                return;
            }
        }
        p = p->ifa_next;
    }
}

bool Client::ipAddCommand()
{
    char IP[2000];
    struct ifaddrs *ifa, *p;
    sockaddr_in s;
    char command_str[] = "IP";
    if (getifaddrs(&ifa) == -1)
        return false;
    p = ifa;
	socklen_t size = sizeof(s);
    while (p != NULL)
    {
        // refered from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf Page: 32// get sockaddr, IPv4 or IPv6:
        struct sockaddr *s = p->ifa_addr;
        if (s->sa_family == AF_INET)
        {
            int j=0;
            inet_ntop(AF_INET, &(((struct sockaddr_in *)s)->sin_addr), IP, sizeof(IP));
            if (strcmp(IP, LOCAL_HOST) != j)
            {
                strcpy(myAddr, IP);
                return true;
            }
        }
        p = p->ifa_next;
    }

    return (1==0);
}

char *Client::getFirstCommandArgument(char *input)
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

char *Client::getSecondCommandArgument(char *input)
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

char *Client::getThirdCommandArgument(char *input)
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

void Client::divideArray(char *p)
{
    char *print;
    char str[500];
    vector<char *> s;
    strcpy(str, p);
    int y = 0;
    int c;
    print = strtok(str, "-");
    for (; print != NULL;)
    {
        s.insert(s.end(), print);
        c = 0;
        print = strtok(NULL, "-");
    }

    if (c == 0)
        updateList(s);
}

void Client::updateList(vector<char *> s)
{
    L.clear();
    listsize = s.size();
    char str[500], *p;
    int i = 0;
    logList ll;
    while (i < listsize)
    {
        strcpy(str, s[i]);
        p = strtok(str, " ");
        strcpy(ll.sno, p);
        p = strtok(NULL, " ");
        int j = 0;
        strcpy(ll.name, p);
        p = strtok(NULL, " ");
        strcpy(ll.IP, p);
        if (j == 1)
            break;
        p = strtok(NULL, " ");
        strcpy(ll.port, p);
        L.insert(L.end(), ll);
        i++;
    }
}

char *Client::receiveMessage(int sockfd, char *message)
{
    // refered from beej page 35 6.2. A SIMPLE STREAM CLIENT
    char str[1500];
    char *buf;
    int size, numbytes;
    recv(sockfd, &size, sizeof(size), 0);
    int lowest = -1;
    char ch ='\0';
    int total=0;
    while (total < size)
    {
        numbytes = recv(sockfd, str + total, size, 0);
        if (numbytes == lowest)
            break;

        total += numbytes;
        size -= numbytes;
    }

    str[total] = ch;

    strcpy(message, str);
    return message;
}

void Client::sendall(int sockfd, char *type, char *buf)
{ // refered from beej 5.7 send() and recv()
    char *message = new char[1048];
    strcat(type, buf);
    int i=-1;
    int total = 0;
    strcpy(message, type);
    int size = strlen(message);
    int n;
    send(sockfd, &size, sizeof(size), 0);
    int j=0;
    while (true && total < size)
    {
        n = send(sockfd, message + total, size, j);

        if (n == i)
            break;

        total += n;
        size -= n;
    }
    delete[] message;
}

void Client::printList()
{
    int i = 0;
    cse4589_print_and_log("[%s:%s]\n", "LIST", SUCCESS);
    fflush(NULL);

    // printing list of clinets in buffer
    for (; i < L.size();) {
        cse4589_print_and_log("%-5d%-35s%-20s%-8s\n", i + 1, L[i].name, L[i].IP, L[i].port);
     i++;
    }
    fflush(NULL);
    cse4589_print_and_log("[%s:%s]\n", "LIST", END);
    fflush(NULL);
}

bool Client::isPAddressValid(char *IP)
{
    // referred from https://beej.us/guide/bgnet/pdf/bgnet_a4_bw_2.pdf 9.14.0.4 Example IPv4 demo of inet_ntop() and inet_pton()
    struct sockaddr_in s;
    int i = inet_pton(AF_INET, IP, &(s.sin_addr));
    return i != 0;
}

bool Client::isValidPort(char *Portent)
{
    bool isValid = true;
    int i = 0;

    while (i < strlen(Portent))
    {
        if (isdigit(Portent[i]) == 0)
        {
            isValid = false;
            break;
        }
        i++;
    }

    if (!isValid)
        return false;
    long v = atol(Portent);
    if (v < 0 || v > 65335)
        isValid = false;

    if (!isValid)
        return false;
    return true;
}

bool Client::isClientPresentInConnectedList(char *IP)
{
    int i = 0;
    if (ipAddCommand())
        while (i < L.size())
        {
            int j=0;
            if (strcmp(L[i].IP, IP) == j && strcmp(L[i].IP, myAddr) != 0)
                return (1==1);
            i++;
        }

    return (1==0);
}

char *Client::findPort(char *IP)
{
    int port = -1;
    int i = 0;
    while (i < L.size())
    {
        if (strcmp(L[i].IP, IP) == 0)
        {
            return L[i].port;
        }
        i++;
    }
}

void error_print(char *command_str)
{
    cse4589_print_and_log("[%s:%s]\n", command_str, ERROR);
    fflush(NULL);
    cse4589_print_and_log("[%s:%s]\n", command_str, END);
    fflush(NULL);
}

bool Client::isBlocked(char *IP)
{
    int i = 0;
    for (; i < MyBlock.size();)
    {
        if (strcmp(MyBlock[i].IP, IP) == 0)
            return true;
        i++;
    }

    return false;
}

bool Client::removeBlock(char *IP)
{
    int i = 0;
    while (i < MyBlock.size())
    {
        if (strcmp(MyBlock[i].IP, IP) == 0)
        {
            MyBlock.erase(MyBlock.begin() + i);
            return true;
        }
        i++;
    }
    return false;
}

void Client::sendFile(int *sock, char *fileName)
{
    FILE *fp;
    int lsock = *sock;
    unsigned long num_of_bytes;
    fp = fopen(fileName, "rb");

    if (fp == NULL)
        return;

    fseek(fp, 0L, SEEK_END);
    num_of_bytes = ftell(fp);
    unsigned long nobytes = num_of_bytes;
    char str[INET_ADDRSTRLEN];

    unsigned char c;

    send(lsock, &num_of_bytes, sizeof(num_of_bytes), MSG_DONTWAIT);

    fseek(fp, 0L, SEEK_SET);

    unsigned long freads = 1000;
    char str1[INET_ADDRSTRLEN];
    if (freads > nobytes)
        freads = nobytes;

    unsigned long total = 0;
    char str2[INET_ADDRSTRLEN];
    int k=0;
    while (total < nobytes)
    {
        // reading large file
        c = fgetc(fp);
        int n = send(lsock, &c, sizeof(c), k);
        // bytes sent total
        total += n;
    }

    fclose(fp);
    int l=0;
    int n = strlen(fileName);
    char str4[INET_ADDRSTRLEN];
    send(lsock, &n, sizeof(n), MSG_DONTWAIT);
    send(lsock, fileName, n, MSG_DONTWAIT);
}

void success_print(char *command_str)
{
    cse4589_print_and_log("[%s:%s]\n", command_str, SUCCESS);
    fflush(NULL);
    cse4589_print_and_log("[%s:%s]\n", command_str, END);
    fflush(NULL);
}

void Client::receiveBytes(int sock)
{

    FILE *fp, *a;
    char filename[50];
    short int n;
    unsigned char c;
    short int nsize;
    char str[INET_ADDRSTRLEN];
    unsigned long num_of_bytes;
    int i=0;
    recv(sock, &num_of_bytes, sizeof(num_of_bytes), i);

    unsigned long total = 0;
    fp = fopen("temp", "w+b");

    while (true && total < num_of_bytes)
    {
        unsigned long n = recv(sock, &c, sizeof(c), 0);
        if (n <= i)
        {
            break;
        }

        fputc(c, fp);
        total += n;
    }

    fflush(fp);
    fclose(fp);
    long int nobytes;
    recv(sock, &nobytes, sizeof(nobytes), i);
    unsigned int t = 0;
    long int e = nobytes;
    for (; true && t < nobytes;)
    {
        int n = recv(sock, filename, nobytes, i);
        char str[INET_ADDRSTRLEN];
        t = t+ n;
        nobytes = nobytes-n;
    }
    char ch = '\0';
    fp = fopen("temp", "rb");
    filename[e] = ch;
    a = fopen(filename, "w+b");

    total = 0;
    for (; total < num_of_bytes;)
    {
        c = fgetc(fp);
        char str[INET_ADDRSTRLEN];
        fputc(c, a);
        total++;
    }
    fclose(a);
    unlink("temp");
}

void error_print()
{
    cse4589_print_and_log("[LOGIN:ERROR]\n");
    fflush(NULL);
    cse4589_print_and_log("[LOGIN:END]\n");
    fflush(NULL);
}

void Client::handleInput(char *input)
{
    char ch = '\0';
    fgets(input, 1023, stdin);
    char *first, *second, *third;
    input[strlen(input) - 1] = ch;
    int j=0;

    first = getFirstCommandArgument(input);

    if (strcmp(first, "IP") == j)
    {
        ipCommand();
    }
    else if (strcmp(first, "PORT") == j)
    {
        portCommand();
    }
    else if (strcmp(first, "AUTHOR") == j)
    {
        const char *command_str = "AUTHOR";
        cse4589_print_and_log("[%s:%s]\n", command_str, SUCCESS);
        fflush(NULL);
        cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", UBIT_NAME);
        fflush(NULL);
        cse4589_print_and_log("[%s:%s]\n", command_str, END);
        j=0;
        fflush(NULL);
    }
    else if (strcmp(first, "LOGIN") == j)
    {
        memset(&newhints, j, sizeof(newhints));
        newhints.ai_family = AF_INET;
        newhints.ai_socktype = SOCK_STREAM;
        j=0;

        second = getSecondCommandArgument(input);
        third = getThirdCommandArgument(input);
        isClientLoggedIn = isClientLoggedIn; j=0;
        if (isPAddressValid(second) && isValidPort(third))
        {
            if (!isClientLoggedIn)
            {
                if (getaddrinfo(second, third, &newhints, &res) == -1);
                const int yes = 1;

                if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1);

                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1);

                if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1);

                isClientLoggedIn = true;
                FD_SET(sockfd, &master);
                j=0;
                !(sockfd < fdmax) ? fdmax = sockfd: fdmax; 
                send(sockfd, &port, sizeof(port), j);
            }
            else
                error_print();
        }
        else
        {
            error_print();
        }
    }
    else if (strcmp(first, "LIST") == j)
    {
        if (isClientLoggedIn)
        {
            printList();
        }
    }

    else if (strcmp(first, "REFRESH") == j)
    {
        // refresh the buffer
        if (isClientLoggedIn)
        {
            char type[]="0";
            char resp[]=" ";  //#updated
            sendall(sockfd,type,resp);
            success_print("REFRESH");
        }
        else
        {
            error_print("REFRESH");
        }
    }
    else if (strcmp(first, "SEND") == 0)
    {
        char command_str[] = "SEND";
        second = getSecondCommandArgument(input);
        if (isPAddressValid(second) && isClientPresentInConnectedList(second) && isClientLoggedIn)
        {
            char request[1024] = "1 ";
            strcat(request, second);
            strcat(request, " ");
            third = getThirdCommandArgument(input);
            sendall(sockfd, request, third);
            success_print("SEND");
        }
        else
        {
            error_print("SEND");
        }
    }
    else if (strcmp(first, "BROADCAST") == 0)
    {
        char *broadCastInput = new char[1024];
        int j = 0;
        int i = 0;
        while (input[i] != '\0' && input[i] != ' ')
            i++;

        i++;
        while (input[i] != '\0')
        {
            broadCastInput[j] = input[i];
			i++;
            j++;
        }

        broadCastInput[j] = '\0';
        ipCommand();
        char request[1024] = "2 ";
        strcat(request, myAddr);
        sockaddr sock;
        char* ch = " ";
        strcat(request, " ");
        sendall(sockfd, request, broadCastInput);

        success_print("BROADCAST");
    }
    else if (strcmp(first, "BLOCK") == 0)
    {
            struct Block temp;
        second = getSecondCommandArgument(input);
        if (isPAddressValid(second) && isClientPresentInConnectedList(second) && !isBlocked(second))
        {
            char type[]="3 ";   //#updated
            sendall(sockfd, type, second);
            strcpy(temp.IP, second);
            MyBlock.insert(MyBlock.end(), temp);
                    char command_str[] = "BLOCK";
            success_print(command_str);
        }
        else
        {
            char command_str[] = "BLOCK";
            error_print(command_str);
        }
    }
    else if (strcmp(first, "UNBLOCK") == 0)
    {
        char* s_inpt;
        second = getSecondCommandArgument(input);
        if (isPAddressValid(second) && isClientPresentInConnectedList(second) && isBlocked(second))
        {
            char type[]="4 ";   //#updated
            sendall(sockfd, type, second);
            struct Block temp;
            char command_str[] = "UNBLOCK";

            removeBlock(second);
            success_print(command_str);
        }
        else
        {
             char command_str[] = "UNBLOCK";
            error_print(command_str);
        }
    }
    else if (strcmp(first, "SENDFILE") == 0)
    {
        int tempfd;
        struct addrinfo myhints, *pres;
        memset(&myhints, 0, sizeof(myhints));
        myhints.ai_family = AF_INET;
        myhints.ai_socktype = SOCK_STREAM;
        int x = 1;
        char *clientIP = getSecondCommandArgument(input);
        char *fileName = getThirdCommandArgument(input);
        char *pno = findPort(clientIP);
        int yes = x;
        if ((getaddrinfo(clientIP, pno, &myhints, &pres) == -1) || ((tempfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) || (setsockopt(tempfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) || (connect(tempfd, pres->ai_addr, pres->ai_addrlen) == -1));

        sendFile(&tempfd, fileName);
        success_print("SENDFILE");

        close(tempfd);
    }
    else if (strcmp(first, "LOGOUT") == 0)
    {
        isClientLoggedIn = false;
        char type[]="X";
        char resp[]=" ";   //#updated
        sendall(sockfd,type,resp);
        FD_CLR(sockfd, &master);
        close(sockfd);
        success_print("LOGOUT");
    }
    else if (strcmp(first, "EXIT") == 0)
    {
        char type[]="9";
        char resp[]=" ";  //#updated
        sendall(sockfd,type,resp);
        FD_CLR(sockfd, &master);
        close(sockfd);
        success_print("EXIT");
        exit(0);
    }
}

void Client::handleNewConnections(int sockfd)
{
    // handle new connections
    char r[500];
    int i=0;
    char *m = receiveMessage(sockfd, r);
    if (m[0] == '0')  divideArray(m + 2);
    else if (m[i] == '1')
    {
        char *sender = getSecondCommandArgument(m);
        char *message = getThirdCommandArgument(m);
        int u=0;
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", SUCCESS);
        fflush(NULL);
        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", sender, message);
        u=1;
        fflush(NULL);
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", END);
        fflush(NULL);
    }
    else if (m[i] == '2')
    {
        int u=0;
        char *sender = getSecondCommandArgument(m);
        char *message = getThirdCommandArgument(m);

        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", SUCCESS);
        u=1;
        fflush(NULL);
        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", sender, message);
        fflush(NULL);
        u=2;
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", END);
        fflush(NULL);
    }
    else if (m[i] == 'L')
    {
        int x=2;
        divideArray(m + x);
        char type[1024] = "B"; //#updated
        char res[] = " "; 
        sendall(sockfd, type,  res);
    }
    else if (m[i] == 'B')
    {
        int i=1;
        char *sender = getSecondCommandArgument(m + i);
        char *message = getThirdCommandArgument(m + i);
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", SUCCESS);
        fflush(NULL);
        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", sender, message);
        fflush(NULL);
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", END);
        fflush(NULL);

        char type[1024] = "B";  //#updated
        char res[] = " "; 
        sendall(sockfd, type,  res);
    }
    else if (m[i] == 'F')
    {
        int i=1;
        char *sender = getSecondCommandArgument(m + i);
        char *message = getThirdCommandArgument(m + i);
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", SUCCESS);
        fflush(NULL);
        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", sender, message);
        fflush(NULL);
        cse4589_print_and_log("[%s:%s]\n", "RECEIVED", END);
        fflush(NULL);

        success_print("LOGIN");
    }
    else if (m[i] == 'N')
    {
        success_print("LOGIN");
    }
}

void Client::startClient()
{
    // refered from beej guide 7.3. SELECT()—SYNCHRONOUS I/O MULTIPLEXING, OLD SCHOOL Page 50
    FD_ZERO(&master);
    fdmax = p2pFileDescriptor;
    FD_SET(0, &master);
    FD_SET(p2pFileDescriptor, &master);
    char input[1024];
    int i;

    while (1)
    {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            continue;
        }

        // run through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == 0)
                {
                    handleInput(input);
                }

                else if (i == sockfd)
                {
                    handleNewConnections(sockfd);
                }

                else if (i == p2pFileDescriptor)
                {
                    // Client connected
                    int newfd, oldFd;
                    sockaddr peeraddr;
                    sockaddr seedaddr;
                    socklen_t plen = sizeof(peeraddr);
                    newfd = accept(p2pFileDescriptor, &peeraddr, &plen);
                    if (newfd == -1)
                    {
                        oldFd = 0;
                        continue;
                    }
                    else
                    {
                        receiveBytes(newfd);
                        oldFd = 1;
                        close(newfd);
                    }
                }
            }
        }
    }
}

#pragma warning(pop)

