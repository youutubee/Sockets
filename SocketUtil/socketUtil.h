//
// Created by Shrijan Katiyar on 07/10/24.
//

#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int CreateIPv4Socket();

struct sockaddr_in* CreateIPv4Address(char* ip,int port);


#endif //SOCKETUTIL_H
