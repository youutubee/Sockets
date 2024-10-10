#include <stdbool.h>
#include<unistd.h>
#include<pthread.h>
#include "socketUtil.h"



struct AcceptedSocket{
    int acceptedSocektFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccess;
};

struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD);
void startAcceptingIncomingConnection(int serverSocketFD);
void RecieveandPrintIncomingData(int socketFD);
void acceptNewConnectionandRecieveandPrintIncomingData(int serverSocketFD);
void sendRecievedMessagestoOtherClients(char *buffer,int socketFD);


struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD) {
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    int clientSocketFD = accept(serverSocketFD, &clientAddress ,&clientAddressSize);

    struct AcceptedSocket *acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocektFD = clientSocketFD;
    acceptedSocket->acceptedSuccess = clientSocketFD > 0;

    if (!acceptedSocket->acceptedSuccess) {
        acceptedSocket->error = clientSocketFD; //if not connected successfully the error will have the error number of the clientsocketfd instead of its value
    }
    return acceptedSocket;
}

void RecieveandPrintIncomingData(int socketFD) {      // this function takes the message or incoming data and prints it
    char buffer[1024];
    while(true) {

        ssize_t amountRecieved = recv(socketFD,buffer,1024,0);
        if (amountRecieved > 0 ) {//if the amont of data recieved is greater than 0 it means there was a response
            buffer[amountRecieved] = 0;
            printf("%s\n",buffer);

            sendRecievedMessagestoOtherClients(buffer,socketFD);
        }

        if(amountRecieved == 0) { //if the amount recieved is less than 0 it means
            break; //either there is some problem recieving it or the client has disconnected here in this case has clicked exit
        }
    }
    close(socketFD);
}

void RecieveandPrintDataonSeperateThread(struct AcceptedSocket *pSocket) {
    pthread_t id;
    pthread_create(&id,NULL,RecieveandPrintIncomingData,pSocket->acceptedSocektFD);

}   // This func is called and the it creates a seperate thread for both recieving and printing data as both of these functions have a while true loop so both of these loops would be blocking each other


struct AcceptedSocket accepted_socket[10];
int accepted_socket_count= 0;


void startAcceptingIncomingConnection(int serverSocketFD) {

    while(true) {
        struct AcceptedSocket *clientSocket = acceptIncomingConnection(serverSocketFD);
        accepted_socket[accepted_socket_count++] = *clientSocket;

        RecieveandPrintDataonSeperateThread(clientSocket);

    }

}

void sendRecievedMessagestoOtherClients(char *buffer,int socketFD) {

    for(int i=0;i<accepted_socket_count;i++) {
        if(accepted_socket[i].acceptedSocektFD!= socketFD) {
            send(accepted_socket[i].acceptedSocektFD,buffer,strlen(buffer),0);
        }
    }
}

int main() {
    int serverSocketFD = CreateIPv4Socket();
    struct sockaddr_in *serverAddress = CreateIPv4Address("",2000);

    int result = bind(serverSocketFD, (struct sockaddr *)serverAddress, sizeof(*serverAddress));
    if (result == -1) {
        printf("Bind failed\n");
    }
    else {
        printf("Bind done\n");
    }
    int listenresult = listen(serverSocketFD,10); //10 is the backlog amount means the max connection in queue
    if (listenresult == -1) {
        printf("Listen failed\n");
    }
    else {
        printf("Listen done\n");
    }

    startAcceptingIncomingConnection(serverSocketFD);

    shutdown(serverSocketFD,SHUT_RDWR);

    return 0;
}
