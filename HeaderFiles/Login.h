#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include "Admin.h"
#include "Faculty.h"
#include "Student.h"
#include "Structures.h"
#define ADMIN_ID "Admin@iiitb"
#define ADMIN_PWD "12345"

void login(int socket,char choice_login[20],char userid[20],char userpwd[20])
{
    char msg[1024];
    char menu[1024];
    char choice[20];
    char data[20];
    memset(choice,0,sizeof(choice));
    memset(msg,0,sizeof(msg));
    memset(menu,0,sizeof(menu));
    memset(data,0,sizeof(data));
    int flag=0;
    
    switch(atoi(choice_login))
    {
        case 1:
        {
            if(strcmp(ADMIN_ID,userid)==0 && strcmp(ADMIN_PWD,userpwd)==0)
            {
                strncpy(msg,"Admin login Successful!\nPress X to Continue\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
                memset(msg,0,sizeof(msg));
                flag=0;
                while(flag==0)
                {
                    display_menu_admin(socket);
                    memset(choice,0,sizeof(choice));
                    read(socket,choice,sizeof(choice));
                    switch (atoi(choice))
                    {
                        case 1:
                        {
                            add_student(socket);
                            break;
                        }
                        case 2:
                        {
                            view_student(socket);
                            break;
                        }
                        case 3:
                        {
                            add_faculty(socket);
                            break;
                        }
                        case 4:
                        {
                            view_faculty(socket);
                            break;
                        }
                        case 5:
                        {
                            break;
                        }
                        case 6:
                        {
                            break;
                        }
                        case 7:
                        {
                            modify_student(socket);
                            break;
                        }
                        case 8:
                        {
                            modify_faculty(socket);
                            break;
                        }
                        case 9:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Logged Out Successfully\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            flag=1;
                            break;
                        }
                        default:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Wrong choice\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            break;
                        }
                    }
                }
            }
            else
            {
                strncpy(msg,"Incorrect Password!\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
            }
            break;
        }
        case 2:
        {
            int fd = open("faculty.txt",O_RDWR);
            char id_offset[19];
            strncpy(id_offset,userid+1,19);
            int offset = atoi(id_offset);
            char curr_pwd[20];
            lseek(fd,(offset-1)*(40)+20,SEEK_SET);
            int rcount = read(fd,curr_pwd,sizeof(curr_pwd));
            if(rcount!=0 && strcmp(curr_pwd,userpwd)==0)
            {
                strncpy(msg,"Faculty login Successful!\nPress X to Continue\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
                flag=0;
                while(flag==0)
                {
                    display_menu_faculty(socket);
                    memset(choice,0,sizeof(choice));
                    read(socket,choice,sizeof(choice));
                    switch (atoi(choice))
                    {
                        case 1:
                        {
                            view_offering_courses(socket,userid);
                            break;
                        }
                        case 2:
                        {
                            add_course(socket,userid);
                            break;
                        }
                        case 3:
                        {
                            remove_course(socket,userid);
                            break;
                        }
                        case 4:
                        {
                            modify_course(socket,userid);
                            break;
                        }
                        case 5:
                        {
                            change_pwd_f(socket,userid);
                            break;
                        }
                        case 6:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Logged Out Successfully\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            flag=1;
                            break;
                        }
                        default:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Wrong choice\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            break;
                        }
                    }
                }
            }
            else
            {
                strncpy(msg,"Incorrect Password!\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
            }
            break;
        }
        case 3:
        {
            int fd = open("student.txt",O_RDWR);
            char id_offset[14];
            strncpy(id_offset,userid+6,14);
            int offset = atoi(id_offset);
            char curr_pwd[20];
            lseek(fd,(offset-1)*(40)+20,SEEK_SET);
            read(fd,curr_pwd,sizeof(curr_pwd));
            if(strcmp(ADMIN_ID,userid)==0 && strcmp(curr_pwd,userpwd)==0)
            {
                strncpy(msg,"Student login Successful!\nPress X to Continue\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
                flag=0;
                while(flag==0)
                {
                    display_menu_student(socket);
                    memset(choice,0,sizeof(choice));
                    read(socket,choice,sizeof(choice));
                    switch (atoi(choice))
                    {
                        case 1:
                        {
                            view_courses(socket);
                            break;
                        }
                        case 2:
                        {
                            enroll_course(socket,userid);
                            break;
                        }
                        case 3:
                        {
                            drop_course(socket,userid);
                            break;
                        }
                        case 4:
                        {
                            view_enrolled_courses(socket,userid);
                            break;
                        }
                        case 5:
                        {
                            change_pwd_s(socket,userid);
                            break;
                        }
                        case 6:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Logged Out Successfully\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            flag=1;
                            break;
                        }
                        default:
                        {
                            memset(msg,0,sizeof(msg));
                            strncpy(msg,"Wrong choice\n",sizeof(msg));
                            write(socket,msg,sizeof(msg));
                            break;
                        }
                    }
                }
            }
            else
            {
                strncpy(msg,"Incorrect Password!\n",sizeof(msg));
                write(socket,msg,sizeof(msg));
            }
            break;
        }
        default:
        {
            strncpy(msg,"Wrong login type\n",sizeof(msg));
            write(socket,msg,sizeof(msg));
            exit(0);
        }
    }
}