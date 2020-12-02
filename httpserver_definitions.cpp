/*
    Cristian C. Castillo 
*/

/* C-C++ lib */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>

/* getaddrinfo - Socket set up e.g ports, etc*/
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

/* Errors & warnings */
#include <errno.h>
#include <err.h>

/* header file included */
#include "httpserver_headers.h"

/* Defined Macros */
#define LIVE 1
#define BACKLOG 500
#define HEADER_BUFFER 4096
#define MAX_BUFFER 32768

/* Checking valid resource names */
bool f_bool_check_file(char *parseFile){
    for(size_t i = 0; i < strlen(parseFile); i++){
        if((parseFile[i] >= 'A' && parseFile[i] <= 'Z') || (parseFile[i] >= 'a' && parseFile[i] <= 'z') || (parseFile[i] <= '9' && parseFile[i] >= '0')){continue;}
        else{
            perror("Invalid file format name. Try accepted file format range: A-Z, a-z, 0-9.\n");
            return false;
        }
    }
    return true;
}

/* Checking file length */
bool f_bool_check_file_len(char *file){
    if(strlen(file) != 10){
        perror("Invalid file format length. Length must be ten characters.\n");
        return false;
    }
    else{
        return true;
    }
}

void f_void_server_arg_error(){
    perror("Server format failure. \nTry the following:  ./httpserver <ip><port> or sudo ./httpserver <ip>\n");  
    exit(EXIT_FAILURE); 
}
/* socket error */
void f_void_socket_error(int int_socket){

    if(int_socket < 0){

        if(errno == ENOTCONN){
        /* Socket is not connected */
            fprintf(stderr, "Socket not connected: %s\n", strerror(errno));
            close(int_socket);
            exit(EXIT_FAILURE);

        }
        else if(errno == EINTR){
        /* interrupted sytem call */
            fprintf(stderr, "Interrupted system call: %s\n", strerror(errno));
            close(int_socket);
            exit(EXIT_FAILURE);
        }
        else if(errno == ENOTSOCK){
        /* Not a socket */
            fprintf(stderr, "Not a socket: %s\n", strerror(errno));
            close(int_socket);
            exit(EXIT_FAILURE);
        }
        else if(errno == EOPNOTSUPP){
        /*Operation not supported on socket */
            fprintf(stderr, "Operation not supported on socket: %s\n", strerror(errno));
            close(int_socket);
            exit(EXIT_FAILURE);
        }
        else{
        perror("Socket operation failed\n");
        close(int_socket);
        exit(EXIT_FAILURE);
        }
    }
    else{
        perror("Socket operation failed\n");
        close(int_socket);
        exit(EXIT_FAILURE);
    }
}

/* set socket error checks*/
void f_void_setsocket_error(int int_setsocket){

    if(int_setsocket < 0){

        if(errno == EBADF){
            fprintf(stderr, "Argument socket is not a valid descriptor.: %s\n", strerror(errno));
            close(int_setsocket);
            exit(EXIT_FAILURE);
        }
        else if(errno == ENOTSOCK){
            fprintf(stderr, "Argument sockfd is a file, not a socket.: %s\n", strerror(errno));
            close(int_setsocket);
            exit(EXIT_FAILURE);
        }
        else{
            perror("Set Socket operation failure\n");
            close(int_setsocket);
            exit(EXIT_FAILURE);
        }
    }
    else{
        perror("Set Socket operation failure\n");
        close(int_setsocket);
        exit(EXIT_FAILURE);
    }
}

/* Bind error checks */
void f_void_bind_error(int bind){

    if(bind < 0){

        if(errno == EADDRINUSE){
            fprintf(stderr, "The given address is already in use.: %s\n", strerror(errno));
            close(bind);
            exit(EXIT_FAILURE);
        }
        else if(errno == EBADF){
            fprintf(stderr, "Socket is not a valid file descriptor: %s\n", strerror(errno));
            close(bind);
            exit(EXIT_FAILURE);
        }
        else if(errno == ENOTSOCK){
            fprintf(stderr, "The file descriptor sockfd does not refer to a socket.: %s\n", strerror(errno));
            close(bind);
            exit(EXIT_FAILURE);
        }
        else if(errno == EINVAL){
            fprintf(stderr, "The socket is already bound to an address.: %s\n", strerror(errno));
            close(bind);
            exit(EXIT_FAILURE);
        }
        else{
            perror("Socket failed to bind\n");
            close(bind);
            exit(EXIT_FAILURE);
        }
    }
    else{
        perror("Socket failed to bind\n");
        close(bind);
        exit(EXIT_FAILURE);
    }
}
/* Listen error */
void f_void_listen_error(int socket_listen){

    if(socket_listen < 0){

        if(errno == EADDRINUSE){
            fprintf(stderr, "Another socket is listening on same port: %s\n", strerror(errno));
            close(socket_listen);
            exit(EXIT_FAILURE);
        }
        else if(errno == EBADF){
            fprintf(stderr, "Socket is not a valid file descriptor: %s\n", strerror(errno));
            close(socket_listen);
            exit(EXIT_FAILURE);
        }
        else if(errno == EOPNOTSUPP){
            fprintf(stderr, "The socket is not of a type that supports the listen() operation.: %s\n", strerror(errno));
            close(socket_listen);
            exit(EXIT_FAILURE);
        }
        else{
            perror("Server failed to start listenting\n");
            close(socket_listen);
            exit(EXIT_FAILURE);
        }
    }
    else{
        perror("Server failed to start listenting\n");
        close(socket_listen);
        exit(EXIT_FAILURE);
    }
}



/* file exist, terminate child process and respond to client 200 */
void f_client_req_found(int fetch_file_fd,int clientSocket){
    
    struct stat buf;
    fstat(fetch_file_fd,&buf);

    int int_ch_content = buf.st_size;
    char server_length[10];

    char msg200[] = "HTTP/1.1 200 OK\r\n";
    char msgContent[] = "Content-Length: ";
    char term[] = "\r\n\r\n";

    sprintf(server_length,"%d",int_ch_content);

    write(clientSocket,msg200,strlen(msg200));
    write(clientSocket,msgContent,strlen(msgContent));
    write(clientSocket,server_length,strlen(server_length));
    write(clientSocket,term,strlen(term));
    return;
}

/* 201 file was created successful ! */
void f_client_req_created(int int_client_sockd){
    dprintf(int_client_sockd,"HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n");
    return;
}

/* Client error bad request 400 */
void f_void_400(int client_sockd){
    dprintf(client_sockd, "HTTP/1.1 400 BAD REQUEST\r\nContent-Length: 0\r\n\r\n");
    return;
}

/* Client error forbidden 403 */
void f_void_client_error_forbid(int int_client_sockd){
    dprintf(int_client_sockd, "HTTP/1.1 403 FORBIDDEN\r\nContent-Length: 0\r\n\r\n");
    return;
}

/* Client to server request, file not found */
void f_void_client_error_not_found(int int_client_sockd){
    dprintf(int_client_sockd,"HTTP/1.1 404 FILE NOT FOUND\r\nContent-Length: 0\r\n\r\n");
    return;
}

/* Client error Internal Service Error 500 */
void f_void_intr_error(int int_client_sockd){
    dprintf(int_client_sockd,"HTTP/1.1 500 Internal Service Error\r\nContent-Length: 0\r\n\r\n");
    return;
}

/* Client error bad reqeust */
void f_void_kill_request(int int_client_sockd,struct addrinfo *res){
    close(int_client_sockd);
    free(res);
    exit(EXIT_FAILURE);
}

/* Fail to read information from client curl */
void f_void_path_error(int int_client_sockd){
    perror("Read path failure from client file. \n");
    f_void_intr_error(int_client_sockd);
}

/* Check for error on getting the addr info */
void f_void_getaddrinfo_error(int status,struct addrinfo *res){

    if(status < 0){

        if(errno == EAI_AGAIN){
            fprintf(stderr,"The name server returned a temporary failure indication. Try again later: %s\n",strerror(errno));
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
        else if(errno == EAI_SOCKTYPE){
            fprintf(stderr,"The requested socket type is not supported: %s\n",strerror(errno));
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
        else if(errno == EAI_BADFLAGS){
            fprintf(stderr,"hints.ai_flags contains invalid flags: %s\n",strerror(errno));
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
        else if(errno == EAI_SOCKTYPE){
            fprintf(stderr,"Other system error, check errno for details: %s\n",strerror(errno));
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
        else{
            perror("Failed on getaddrinfo.\n");
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        } 
    }
    else{
        perror("Failed to translate server socket.\n");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    } 
}

void f_void_error_on_accept(){
     char msgAccept[] = "Warning on accept(). Found bundle for host. Re-using existing connection!\n";
     write(1,msgAccept,strlen(msgAccept));
     return;
}

/* open req file as rd only */
int f_fetch_file(char *char_file){
    return open(char_file,O_RDONLY);
}
