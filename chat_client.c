/*
 * Author: Matthew Krilanovich
 * Program: chat_client.c
 * Description: Serves as the CLIENT in a chat system program. Takes in a host address and port number (same one used by chat_serve.py).
 *     Makes/connects a socket and initiates the chat functionality between the chat server.
 *     Client sends the first message and waits for a repsonse from the SERVER. Once message is received the CLIENT can send another message.
 *     Enter "\quit" to end the chat program
 *     Server must already be listening for request before this program is compiled. 
 * Source:
 * 1) https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-2:-Using-getaddrinfo
 * 2) https://msdn.microsoft.com/en-us/library/windows/desktop/ms737530(v=vs.85).aspx
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/************************************************
 * Function Prototypes
 ************************************************/
void chat(int, char*, char*);
/************************************************
 * Main
 ************************************************/
int main(int argc, char *argv[])
{
   // verify valid number of arguments (host address) and (port number)
    if(argc != 3)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        printf("Usage: ./client <Host Name> <Port Number>\n");
        exit(1);
    }

    // variables 
    char *host_name;
    char *port_num;
    char client_name[10];
    char server_name[10];
    struct addrinfo hints, *result;
    int sockfd;

    // init host and port
    host_name = argv[1];
    port_num = argv[2];
  
    // get client user name
    int valid = 1;
    memset(client_name,'\0',sizeof(client_name));
    while(valid)
    {
        printf("Enter a chat username: ");
        scanf("%s", client_name);

        if(strlen(client_name) > 10)
        {
            printf("Error: too many characters, has to be less than 10\n");    
        }
        else
        {
            valid = 0;
        }
    }
    //printf("1) Got client username.\n");
    // init host and port
    host_name = argv[1];
    port_num = argv[2];

    // get socket address
    memset(&hints,0,sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
 
    int status = getaddrinfo(host_name,port_num,&hints,&result);    

    if(status != 0)
    {
        fprintf(stderr,"Getting Socket address failed\n");
        exit(1);
    }
    //printf("2) Got socket address.\n");

    // make socket
    sockfd = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(sockfd == -1)
    {
        fprintf(stderr, "Creating Socket failed\n");
        close(sockfd);
        exit(1);
    }
    //printf("3)Connection established.\n");
  
    // connect the socket
    status = connect(sockfd,result->ai_addr,result->ai_addrlen);
    if(status == -1)
    {
         fprintf(stderr, "Connecting Socket failed\n");
         exit(1);
    }
    //printf("4) tcp connection.\n");
    
    // handshake for tcp connection
    memset(server_name,'\0',sizeof(server_name));
    send(sockfd,client_name,strlen(client_name),0);
    recv(sockfd,server_name,10,0);
    printf("Starting chat, go ahead and send a message.\n");
    
    // begin chat over socket coneection
    chat(sockfd, client_name, server_name);
    
    printf("6) clearing address struct.\n");
    
    // clear address struct 
    freeaddrinfo(result);

    return 0;
}





/************************************************
 * Function Definitions
 ************************************************/
// Function: Chat
// source: https://www.geeksforgeeks.org/fgets-gets-c-language/
void chat(int sockfd, char *client_name, char *server_name)
{
    char read_buffer[512];
    char send_buffer[512];
    memset(read_buffer,'\0',sizeof(read_buffer));
    memset(send_buffer,'\0',sizeof(send_buffer));
    int status;
    int send_check = 0;

    fgets(read_buffer, 512, stdin);
    while(1)
    {
        //memset(send_buffer,\0,sizeof(send_buffer));
        // send response
        //fflush(stdout);
        printf("%s> ", client_name);
        fgets(read_buffer, 512, stdin);

        // check if quit
        if(strncmp(read_buffer,"\\quit", 5) == 0)
        {
            break;
        }
        send_check = send(sockfd, read_buffer, strlen(read_buffer),0);
        if(send_check == -1)
        {
            fprintf(stderr, "Message was not successfully sent from host\n");
            exit(1);
        }

        status = recv(sockfd, send_buffer, 512, 0); 
        if(status == -1)
        {
            fprintf(stderr,"Message was not successfully received from host\n");
            exit(1);
        }
        else if(status == 0)
        {
            printf("Connection closed by server\n");
            break;
        }
        else
        {
            printf("%s> %s\n", server_name, send_buffer);
        }

        // clear buffers
        memset(read_buffer,'\0',sizeof(read_buffer));
        memset(send_buffer,'\0',sizeof(send_buffer));
    }
    memset(read_buffer,'\0',sizeof(read_buffer));
    memset(send_buffer,'\0',sizeof(send_buffer));
    close(sockfd);
    printf("Socket connection closed\n");
}
