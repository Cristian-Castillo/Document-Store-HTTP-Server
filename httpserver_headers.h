/*
    Cristian C. Castillo 
*/

#ifndef HTTPSERVER_Headers_H
#define HTTPSERVER_Headers_H

/* Socket Setup Error Checking */
bool f_bool_check_file(char *parseFile);
bool f_bool_check_file_len(char *fileLength);
void f_void_server_arg_error();
void f_void_setsocket_error(int setsocket);
void f_void_bind_error(int bind);
void f_void_listen_error(int socket_listen);
void f_void_accept_error(int accept);

/* 200 and 201 Creation/Found*/
void f_client_req_found(int file,int clientsocket);
void f_client_req_created(int int_client_sockd);

/* 400-500 & Error Checks */
void f_void_400(int clientsock);
void f_void_client_error_forbid(int);
void f_void_client_error_not_found(int);
void f_void_intr_error(int);
void f_void_kill_request(int clientsock,struct addrinfo *res);
void f_void_path_error(int);
void f_void_getaddrinfo_error(int status,struct addrinfo *res);
void f_void_error_on_accept();

/* New File openings */
int f_fetch_file(char *char_file);

#endif