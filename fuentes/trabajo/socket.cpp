/* AF_INET -> Para IPv4
 * SOCK_STREAM -> Para TCP
 * 0 -> IP Protocol
 */

#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<thread>
#ifdef WIN32
    #include <windows.h>
    #include <map>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket, *new_sock;
    struct sockaddr_in server , client;
    int c;
    char *message;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    puts("Bind done");

    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);

    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        puts("Connection accepted");

        //Reply to the client
        message = "Hello Client , I have received your connection. But I have to go now, bye\n";
        write(new_socket, message, strlen(message));

        pthread_t sniffer_thread;
        new_sock = (int*) malloc(sizeof(int));
	   *new_sock = new_socket;
	   	if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
	           {
	               perror("could not create thread");
	               return 1;
	   }

	   //Now join the thread , so that we dont terminate before the thread
	   //pthread_join( sniffer_thread , NULL);
	   puts("Handler assigned");
    }

    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
        return 1;
    }

    closesocket(s);
    WSACleanup();

    return 0;
}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;

    char *message;

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));

    message = "Its my duty to communicate with you";
    write(sock , message , strlen(message));

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
