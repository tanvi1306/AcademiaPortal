#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<fcntl.h>
#include "Login.h"


void *handle_client(void *arg) {
    int socket = *(int *)arg;
    int rcount;

    // Handle the client connection here
    // You can read and write data to the client
    char msg[1024];
    strncpy(msg,"Welcome to Course Portal\nEnter login type:\n1.Admin\n2.Faculty\n3.Student\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    // printf("hellothere\n");
    //to read login type choice
    char choice[20];
    rcount = read(socket,choice,sizeof(choice));
    // printf("hellohere\n");
    //to determine login type by choice
    char id[20];
    char passwd[20];
    switch (atoi(choice))
    {
        case 1:{
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Admin ID\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(id,0,sizeof(id));
            read(socket,id,sizeof(id));
            // printf("%s\n",id);
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Admin Password\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(passwd,0,sizeof(passwd));
            read(socket,passwd,sizeof(passwd));
            login(socket,choice,id,passwd);
            break;
        }
        case 2:{
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Faculty ID\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(id,0,sizeof(id));
            read(socket,id,sizeof(id));
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Faculty Password\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(passwd,0,sizeof(passwd));
            read(socket,passwd,sizeof(passwd));
            login(socket,choice,id,passwd);
            break;
        }
        case 3:{
             memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Student ID\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(id,0,sizeof(id));
            read(socket,id,sizeof(id));
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Enter Student Password\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            memset(passwd,0,sizeof(passwd));
            read(socket,passwd,sizeof(passwd));
            login(socket,choice,id,passwd);
            break;
        }
        default:{
            memset(msg,0,sizeof(msg));
            strncpy(msg,"Wrong Login Type\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            break;
        }
    }
    close(socket);
    return NULL;
}
int main()
{
    int s_socket = socket(AF_INET,SOCK_STREAM,0);
    if(s_socket==-1)
    {
        printf("Error in Server Socket Creation\n");
        return 0;
    }
    struct sockaddr_in server_addr,client_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    int bind_server = bind(s_socket,(const struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bind_server==-1)
    {
        printf("Errro in binding\n");
        return 0;
    }
    int listen_server = listen(s_socket,10);
    if(listen_server==-1)
    {
        printf("Error in listening\n");
        return 0;
    }
    while(1)
    {
        socklen_t client_addr_len = sizeof(client_addr);
        int socket= accept(s_socket,(struct sockaddr*)&client_addr,&client_addr_len);
        pthread_t thread_id;
        if(pthread_create(&thread_id, NULL, handle_client, &socket) != 0) 
        {
            printf("Error in thread creation\n");
            close(socket);
        } 
        else 
        {
            pthread_detach(thread_id); // Detach the thread to release resources when it exits
        }
    }
    return 0;
}