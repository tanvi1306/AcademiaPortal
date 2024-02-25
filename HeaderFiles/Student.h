#ifndef STUDENT_H
#define STUDENT_H
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include "Structures.h"

void display_menu_student(int socket)
{
    char buff[1024];
    memset(buff,0,sizeof(buff));
    strncpy(buff,"Welcome to Student Menu\n1.View All Courses\n2.Enroll new Course\n3.Drop Course\n4.View Enrolled Course Details\n5.Change Password\n6.Logout and Exit\nails\n",sizeof(buff));
    write(socket,buff,strlen(buff)); 
}
void view_courses(int socket)
{
    char temp[1024];
    memset(temp,0,sizeof(temp));
    int fd = open("CourseDetail.txt",O_RDONLY);
    struct Course c;
    int count=0;
    int rcount = read(fd,&c,sizeof(c));
    while(rcount!=0)
    {
        if(strncmp("",c.c_id,sizeof(c.c_id))==0)
        {
            count++;
            lseek(fd,count*sizeof(c),SEEK_SET);
            memset(&c,0,sizeof(c));
            rcount = read(fd,&c,sizeof(c));
            continue;
        }
        strcat(temp,"ID: ");
        strcat(temp,c.c_id);
        strcat(temp,"\nName: ");
        strcat(temp,c.name);
        strcat(temp,"\nTotal: ");
        strcat(temp,c.total);
        strcat(temp,"\nEnrolled Students: ");
        strcat(temp,c.stu_count);
        strcat(temp,"\nFaculty ID: ");
        strcat(temp,c.faculty);
        strcat(temp,"\n");
        strcat(temp,"Press X to Continue\n");
        count++;
        lseek(fd,count*sizeof(c),SEEK_SET);
        rcount = read(fd,&c,sizeof(c));
    }
    write(socket,temp,sizeof(temp));
}

void enroll_course(int socket,char sid[20])
{
    char msg[100];
    char data[20];
    struct Course c;
    int count=0;
    strncpy(msg,"Enter course Id:\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,data,sizeof(data));
    int fd = open("CourseDetail.txt",O_RDWR);
    char id_offset[19];
    int offset = atoi(strncpy(id_offset,data+1,19));
    lseek(fd,(offset-1)*sizeof(c),SEEK_SET);
    read(fd,&c,sizeof(c));
    if(strncmp(c.c_id,"",sizeof(c.c_id))==0)
    {
        strncpy(msg,"Course not Available\n",sizeof(msg));
    }
    else if(atoi(c.total)-atoi(c.stu_count)==0)
    {
        strncpy(msg,"Seats not available\n",sizeof(msg));
    }
    else
    {
        lseek(fd,(offset-1)*sizeof(c),SEEK_SET);
        char stu_count[20];
        int new_count =atoi(c.stu_count)+1;
        sprintf(stu_count, "%d", new_count);
        strncpy(c.stu_count,stu_count,sizeof(c.stu_count));
        write(fd,&c,sizeof(c));
        int relation = open("StudentCourse.txt",O_RDWR);
        lseek(relation,0,SEEK_END);
        struct RelateCourseStudent cs;
        strncpy(cs.c_id,data,sizeof(cs.c_id));
        strncpy(cs.s_id,sid,sizeof(cs.s_id));
        write(relation,&cs,sizeof(cs));
        strncpy(msg,"Enrolled Succcesfully\nPress X to Continue\n",sizeof(msg));
    }
    write(socket,msg,sizeof(msg));
}

void drop_course(int socket,char sid[20])
{
    char msg[100];
    char data[20];
    struct Course c;
    int count=0;
    strncpy(msg,"Enter course Id:\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,data,sizeof(data));
    int fd = open("CourseDetail.txt",O_RDWR);
    char id_offset[19];
    int offset = atoi(strncpy(id_offset,data+1,19));
    lseek(fd,(offset-1)*sizeof(c),SEEK_SET);
    read(fd,&c,sizeof(c));
    if(strncmp(c.c_id,"",sizeof(c.c_id))==0)
    {
        strncpy(msg,"Course not Available\n",sizeof(msg));
    }
    else
    {
        struct RelateCourseStudent cs;
        lseek(fd,(offset-1)*sizeof(c),SEEK_SET);
        char stu_count[20];
        int new_count =atoi(c.stu_count)-1;
        sprintf(stu_count, "%d", new_count);
        strncpy(c.stu_count,stu_count,sizeof(c.stu_count));
        write(fd,&c,sizeof(c));
        int relation = open("StudentCourse.txt",O_RDWR);
        int rcount = read(relation,&cs,sizeof(cs));
        while(rcount!=0)
        {
            if(strncmp(cs.c_id,data,sizeof(cs.c_id))==0 && strncmp(cs.s_id,sid,sizeof(cs.s_id))==0)
            {
                lseek(relation,count*sizeof(cs),SEEK_SET);
                strncpy(cs.c_id,"",sizeof(cs.c_id));
                strncpy(cs.s_id,"",sizeof(cs.s_id));
                write(relation,&cs,sizeof(cs));
                strncpy(msg,"Dropped Course\nPress X to Continue\n",sizeof(msg));
                break;
            }
            count++;
            lseek(relation,count*sizeof(cs),SEEK_SET);
            rcount = read(relation,&cs,sizeof(cs));
        }
    }
    write(socket,msg,sizeof(msg));
}

void view_enrolled_courses(int socket,char data[20])
{
    char msg[100];
    char curr_sid[20];
    char sid[20];
    char temp[1024];
    char cid[20];
    memset(temp,0,sizeof(temp));
    int flag=0;
    struct Student s;
    int relation = open("StudentCourse.txt",O_RDONLY);
    int courses = open("CourseDetail.txt",O_RDONLY);
    struct Course c;
    int rcount = read(relation,curr_sid,sizeof(s.s_id));
    while(rcount!=0)
    {
        flag=0;
        if(strcmp(data,curr_sid)==0)
        {
            read(relation,cid,sizeof(cid));
            if(strcmp("",cid)==0)
            {
                continue;
            }
            else
            {
                char id_offset[19];
                strncpy(id_offset,cid+1,19);
                int offset = atoi(id_offset);
                lseek(courses,(offset-1)*sizeof(c),SEEK_SET);
                int rcount =read(courses,&c,sizeof(c));
                strcat(temp,"Course ID: ");
                strcat(temp,c.c_id);
                strcat(temp,"\nName: ");
                strcat(temp,c.name);
                strcat(temp,"\nTotal Seats: ");
                strcat(temp,c.total);
                strcat(temp,"\nStudents Enrolled: ");
                strcat(temp,c.stu_count);
                strcat(temp,"\nFaculty ID: ");
                strcat(temp,c.faculty);
                strcat(temp,"\n");
                strcat(temp,"Press X to Continue\n");
                flag=1;
            }
        }
        if(flag==0)
        {
            lseek(relation,20,SEEK_CUR);
        }
        rcount = read(relation,curr_sid,sizeof(curr_sid));
    }
    write(socket,temp,sizeof(temp));
}

void change_pwd_s(int socket,char data[20])
{
    int fd = open("student.txt",O_RDWR);
    struct Student s;
    char pwd[20];
    char msg[100];
    char id_offset[14];
    strncpy(id_offset,data+6,14);
    int offset = atoi(id_offset);
    // printf("%d\n",offset);
    memset(msg,0,sizeof(msg));
    strncpy(msg,"Enter New Password\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,pwd,sizeof(pwd));
    lseek(fd,(offset-1)*(40),SEEK_SET);
    lseek(fd,sizeof(s.s_id),SEEK_CUR);
    write(fd,pwd,20);
    strncpy(msg,"Password changed Successfully!\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
}
#endif