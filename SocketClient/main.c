#include <stdbool.h>
#include<unistd.h>
#include<pthread.h>

#include "socketUtil.h"

void startListningandPrintMessagesonNewThread(int fd);

void listenAndPrint(int socketFD);

void listenAndPrint(int socketFD) {
    char buffer[1024];
    while(true) {

        ssize_t amountRecieved = recv(socketFD,buffer,1024,0);

        if (amountRecieved > 0 ) {//if the amont of data recieved is greater than 0 it means there was a response
            buffer[amountRecieved] = 0;
            printf("Response was: %s\n",buffer);

        }

        if(amountRecieved == 0) { //if the amount recieved is less than 0 it means
            break; //either there is some problem recieving it or the client has disconnected here in this case has clicked exit
        }
    }
    close(socketFD);
}

void startListningandPrintMessagesonNewThread(int socketFD) {

    pthread_t id;
    pthread_create(&id,NULL,listenAndPrint,socketFD);

}

int main() {
    // Create a socket
    int socketFD = CreateIPv4Socket();
    if (socketFD < 0) {
        printf("Socket creation failed");
        return 0;
    }

    struct sockaddr_in *address= CreateIPv4Address("127.0.0.1",2000);

    // Connect to the server
    int result = connect(socketFD,address, sizeof(*address));

    if(result == 0)
        printf("socket creation successful\n");
    else
        printf("Socket failed");

    startListningandPrintMessagesonNewThread(socketFD);

    char *name = NULL;
    size_t namesize  = 0;
    printf("Please enter your name?\n");
    ssize_t nameCount = getline(&name,&namesize,stdin);
    name[nameCount-1] = 0;

    char *line = NULL;
    size_t lineSize  = 0;
    printf("Please enter a line of text: (type exit to exit the chat) ");

    char buffer[1024];

    while(true) {
        ssize_t charCount = getline(&line, &lineSize, stdin);
        line[charCount-1] = 0;
        sprintf(buffer,"%s:%s ",name,line);

        if (charCount >0) {
            if(strcmp(line, "exit") == 0) {  //if exit is given break out of the loop
                break;
            }
            ssize_t amount_was_sent = send(socketFD,buffer,strlen(buffer),0); //returns the amount of data sent
        }

    }

    close(socketFD);

    return 0;
}