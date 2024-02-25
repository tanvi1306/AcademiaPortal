#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/select.h>
#include<stdlib.h>

int main()
{
    int c_socket = socket(AF_INET,SOCK_STREAM,0);
    if(c_socket==-1)
    {
        printf("Error in Server Socket Creation\n");
        return 0;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    int socket = connect(c_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(socket==-1)
    {
        printf("Error in connection\n");
        return 0;
    }
    else
    {
        char input[20];
        char msg[1024];
        int flag=0;
        char temp[1];
        memset(msg,0,sizeof(msg));
        read(c_socket,msg,sizeof(msg));
        // printf("Hello\n");
        while(strncmp(msg,"Logged Out Successfully\n",strlen(msg))!=0&&strncmp(msg,"Incorrect Password!\n",strlen(msg))!=0 )
        {
            flag=0;
            write(1,msg,sizeof(msg));
            memset(input,0,sizeof(input));
            scanf("%s",input);
            memset(temp,0,sizeof(temp));
            strncpy(temp,input,1);
            if(strncmp("X",input,sizeof(input))!=0)
            {
                // printf("1\n");
                write(c_socket,input,sizeof(input));
                flag=1;
            }
            memset(msg,0,sizeof(msg));
            read(c_socket,msg,sizeof(msg));
            // printf("Here: %s",msg);
        }
        if(strncmp(msg,"Logged Out Successfully\n",strlen(msg))==0)
        {
            write(1,"Logged Out Successfully\n",strlen("Logged Out Successfully\n"));
        }
        else
        {
            write(1,"Incorrect Password!\n",strlen("Incorrect Password!\n"));
        }
        exit(0);
    }
    return 0;   
}