/*
    Cristian C. Castillo
*/

#include "httpserver_definitions.cpp"

/* Master http_obj only stores content length */
struct http_node{
    int content_length;
};

int main(int argc, char *argv[]){

    /* Port Number and Configurations */
    const char *portNumber;
    const char *host_address = argv[1];

    struct http_node *child,process;
    child = &process;

    if(argc < 2 || argc > 3){
        f_void_server_arg_error();
    }

    if(argc == 2){ portNumber = "80";}
    else{ portNumber = argv[2]; }

    /*Get structs rdy */
    struct sockaddr_storage client_addr;
    struct addrinfo hints, *res;  
    socklen_t addr_size;
    int clientSocket = 0;  
    int enable = 1; 
    int serverSocket,ret;
    
    /* Configuration host and port number */
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    if((ret = getaddrinfo(host_address,portNumber,&hints,&res)) != 0){
        f_void_getaddrinfo_error(ret,res);
    }
    
    serverSocket = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(serverSocket < 0){ f_void_socket_error(serverSocket);}   

    if((ret=setsockopt(serverSocket, SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int))) < 0){
        f_void_setsocket_error(ret);
    }
    
    if((ret = bind(serverSocket,res->ai_addr,res->ai_addrlen)) < 0 ){ f_void_bind_error(ret); }

    if((ret = listen(serverSocket,BACKLOG)) == -1){ f_void_listen_error(ret);}

    while(LIVE){
        
        char data;
        if(recv(clientSocket,&data,1,MSG_PEEK) <= 0){
            clientSocket = accept(serverSocket,(struct sockaddr *)&client_addr, &addr_size);
        }
        else{ f_void_error_on_accept();}
    
        char buffer[HEADER_BUFFER];

        /* Read client request into buffer */
        if(read(clientSocket,buffer,HEADER_BUFFER) < 0){ f_void_path_error(clientSocket); }

        /* Parse HTTP Header request  for GET/PUT - file of requested */
        char request[4],file[50],protocol[10];
        sscanf(buffer,"%s %s %s",request,file, protocol);

        /* ------------------------REGEX SECTION ---------------------------- */

        char buffer_for_regex[80]; 

        strcpy (buffer_for_regex,request);

        strcat (buffer_for_regex,file);

        strcat (buffer_for_regex,protocol);

        std::regex ea("(GET|PUT)[/]?[a-zA-Z0-9]{10}HTTP/1.1"); 
        bool matcha = regex_match(buffer_for_regex, ea);

        if(matcha != 1){
            f_void_400(clientSocket);
            continue;
        }
        /*------------------------------------------------------_--*/

        /* Get the file name - delimiter dash */
        char *file_name = strtok(file,"/");

        /* Check length of file must be = 10 ASCII */
        bool check_file_length = f_bool_check_file_len(file_name);
      
        if(check_file_length == false){
            f_void_400(clientSocket);
            memset(buffer,0,HEADER_BUFFER);
            continue;
        }
      
        /* Checking valid resource names A-Z/a-z/0-9 */
        bool check_file_format = f_bool_check_file(file_name);
        
        if(check_file_format == false){
            f_void_400(clientSocket);
            memset(buffer,0,HEADER_BUFFER);
            continue;
        } 

        /* GET Request*/
        if(strcmp(request,"GET") == 0){
            
            memset(buffer,'\0',HEADER_BUFFER);

            /* Open the file for reads only! */
            int fetch_file_fd = f_fetch_file(file_name);

            if(fetch_file_fd == -1){ 
                f_void_client_error_not_found(clientSocket);
                close(fetch_file_fd);
                continue;
            }
            
            /* 200 OK client file found fetch from server */
            if(fetch_file_fd != -1){ 
                
                f_client_req_found(fetch_file_fd,clientSocket);
                ssize_t send_bytes;

                /* Get file size */
                struct stat buf;
                fstat(fetch_file_fd,&buf);
                int get_length = buf.st_size;

                char *buff = (char*)malloc(get_length*sizeof(char*));
                
                /* Store file contents */
                while((child->content_length = read(fetch_file_fd,buff,sizeof(buff))) != 0){
                    send_bytes =  write(clientSocket,buff,child->content_length);
                    if(send_bytes == get_length){break;}
                }
                close(fetch_file_fd);
                    
                if(child->content_length == 0){
                    free(buff);
                    continue;}
                else{ 
                    /* Internal error of 500 Content failuire*/
                    f_void_intr_error(clientSocket);
                    free(buff);
                    continue;
                    
                }
            }
        }
        /* If the client req is put.... do */
        if(strcmp(request,"PUT") == 0){

            /* Parse content length for chunk transfer. Must include for HTTP/1.1 compliance */
            int content_length;
            char *char_parse_content = strtok (buffer,"\n");
          
            while(char_parse_content != NULL){
                 sscanf(char_parse_content,"Content-Length: %d",&content_length);
                 char_parse_content = strtok (NULL, "\n");
            }

            /* Store content-length into struct for long usage */
            child->content_length = content_length;
            
            /* Reset the buffer so that child procees may reuse */
            memset(buffer,'\0',HEADER_BUFFER); // 4096
            
            int put_fd_request = open(file_name,O_CREAT | O_WRONLY | O_TRUNC,0664);

            if(put_fd_request == -1 ){f_void_client_error_not_found(clientSocket);}

            /* ---------------------- Content Length = 0 -------------------------------------------*/

            if(child->content_length == 0){ f_client_req_created(clientSocket);close(put_fd_request);}
           
            if(child->content_length > 0){
         
             /* Store contents received from connected client into buffer */
     
             /* --------------------------------------------------------------------------------------*/

             /*----------------------- Content Length = n --------------------------------------------*/

                ssize_t write_bytes = 0;
                ssize_t check_bytes_recv;
            
                char *buff = (char*)malloc(content_length*sizeof(char*));

                while((check_bytes_recv = read(clientSocket,buff,child->content_length)) >= 0){
                    write_bytes += write(put_fd_request,buff,check_bytes_recv);
                    if(write_bytes == child->content_length)break;
                }
    
                if(write_bytes == child->content_length){
                    f_client_req_created(clientSocket);
                    close(put_fd_request);
                    continue;
                }
                else{
                    f_void_intr_error(clientSocket);
                    close(put_fd_request);
                    continue;
                }
                free(buff);
            }
            //  /*------------------------- Content Length = ? Read-> EOF ---------------------------------*/
            else{ 
                
                ssize_t sendByte = 0;
                ssize_t recv_byte;
                              
                /* Read until end of file  must be manually closed. */
                while((recv_byte = recv(clientSocket,buffer,sizeof(buffer),0)) != EOF){
                    sendByte = write(put_fd_request,buffer,recv_byte);
                    memset(buffer,0,sizeof(buffer));

                    if(recv_byte == EOF || sendByte == EOF){
                        close(put_fd_request);
                    }
                }
            }
        }
        if((strcmp(request,"PUT") != 0 || ((strcmp(request,"GET") != 0)))) f_void_kill_request(clientSocket,res);
    }
    close(serverSocket);
    freeaddrinfo(res);
    exit(0);
}
