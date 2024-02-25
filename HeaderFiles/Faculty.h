#ifndef FACULTY_H
#define FACULTY_H
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include "Structures.h"

void display_menu_faculty(int socket)
{
    char buff[1024];
    memset(buff,0,sizeof(buff));
    strncpy(buff,"Welcome to Faculty Menu\n1.View offering Courses\n2.Add new Course\n3.Remove Course\n4.Update Course Details\n5.Change Password\n6.Logout and Exit\nails\n",sizeof(buff));
    write(socket,buff,strlen(buff)); 
}
void add_course(int socket,char data[20])
{
    struct Course c;
    char msg[1024];
    strncpy(msg,"Enter Course Name\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,&c.name,sizeof(c.name));
    // write(1,c.name,sizeof(c.name));
    memset(msg,0,sizeof(msg));
    strncpy(msg,"Enter Total Seats\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,&c.total,sizeof(c.total));
    // write(1,c.total,sizeof(c.total));
    memset(msg,0,sizeof(msg));
    strncpy(c.stu_count,"0",sizeof(c.stu_count));
    strncpy(c.faculty,data,sizeof(c.faculty));
    int fd = open("CourseDetail.txt",O_RDWR);
    int pos = lseek(fd,0,SEEK_END);
    int next_id = (pos/(sizeof(c)))+1;
    char id[20];
    char temp[20];
    strncpy(temp,"C",1);
    sprintf(id, "%d", next_id);
    strcat(temp,id);
    strncpy(c.c_id,temp,sizeof(c.c_id));
    write(fd,&c,sizeof(c));

    //Add entry in course_faculty file realting course and faculty
    struct RelateCourseFaculty cf;
    strncpy(cf.f_id,data,sizeof(cf.f_id));
    strncpy(cf.c_id,c.c_id,sizeof(cf.c_id));
    fd = open("FacultyCourse.txt",O_RDWR);
    pos = lseek(fd,0,SEEK_END);
    write(fd,&cf,sizeof(cf));
    memset(msg,0,sizeof(msg));
    strncpy(msg,"Course Added Successfully!\nPress X to Continue\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
}

void view_offering_courses(int socket,char data[20])
{
    char msg[1024];
    char curr_fid[20];
    char cid[20];
    char temp[1024];
    memset(temp,0,sizeof(temp));
    int flag=0;
    struct Faculty f;
    // strncpy(msg,"Enter Faculty ID\n",sizeof(msg));
    // write(socket,msg,sizeof(msg));
    // read(socket,data,sizeof(data));
    int relation = open("FacultyCourse.txt",O_RDONLY);
    int courses = open("CourseDetail.txt",O_RDONLY);
    struct Course c;
    int rcount = read(relation,curr_fid,sizeof(f.f_id));
    while(rcount!=0)
    {
        flag=0;
        if(strcmp(data,curr_fid)==0)
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
        rcount = read(relation,curr_fid,sizeof(curr_fid));
    }
    write(socket,temp,sizeof(temp));
}
void remove_course(int socket,char data[20])
{
    char msg[1024];
    char cid[20];
    struct Course c;
    int flag=0;
    strncpy(msg,"Enter Course id\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,cid,sizeof(cid));
    int course = open("CourseDetail.txt",O_RDWR);
    char id_offset[19];
    strncpy(id_offset,cid+1,19);
    int offset = atoi(id_offset);
    int pos = lseek(course,(offset-1)*sizeof(c),SEEK_SET);
    int rcount =read(course,&c,sizeof(c));

    //Removing data related to that course from facutly_course relation
    int f_course = open("FacultyCourse.txt",O_RDWR);
    struct RelateCourseFaculty cf;
    int count =0;
    lseek(f_course,(count)*sizeof(cf),SEEK_SET);
    rcount =  read(f_course,&cf,sizeof(cf));
    while(rcount!=0)
    {
        if(strncmp(cf.c_id,cid,sizeof(cf.c_id))==0&&strncmp(cf.f_id,data,sizeof(cf.f_id))==0)
        {
            strncpy(cf.f_id,"",sizeof(cf.f_id));
            strncpy(cf.c_id,"",sizeof(cf.c_id));
            write(f_course,&cf,sizeof(cf));
            flag=1;
            break;
        }
        count++;
        lseek(f_course,(count)*sizeof(cf),SEEK_SET);
        memset(&cf,0,sizeof(cf));
        rcount = read(f_course,&cf,sizeof(cf));
    }
    if(flag==1)
    {
        int s_course = open("StudentCourse.txt",O_RDWR);
        struct RelateCourseStudent cs;
        int count =0;
        lseek(s_course,(count)*sizeof(cs),SEEK_SET);
        rcount =  read(s_course,&cs,sizeof(cs));
        while(rcount!=0)
        {
            if(strncmp(cs.c_id,cid,sizeof(cs.c_id))==0&&strncmp(cs.s_id,data,sizeof(cs.s_id))==0)
            {
                strncpy(cs.s_id,"",sizeof(cs.s_id));
                strncpy(cs.c_id,"",sizeof(cs.c_id));
                write(s_course,&cf,sizeof(cs));
                memset(&cs,0,sizeof(cs));
                strncpy(c.c_id,"",sizeof(c.c_id));
                strncpy(c.faculty,"",sizeof(c.faculty));
                strncpy(c.name,"",sizeof(c.name));
                strncpy(c.stu_count,"",sizeof(c.stu_count));
                strncpy(c.total,"",sizeof(c.total));
                lseek(course,pos,SEEK_SET);
                write(course,&c,sizeof(c));
            }
            count++;
            lseek(s_course,(count)*sizeof(cs),SEEK_SET);
            memset(&cs,0,sizeof(cs));
            rcount = read(f_course,&cf,sizeof(cf));
        }
        memset(msg,0,sizeof(msg));
        strncpy(msg,"Course removed Successfully!\nPress X to Continue\n",sizeof(msg));
    }
    else
    {
        memset(msg,0,sizeof(msg));
        strncpy(msg,"Error! Course cannot be removed!\nPress X to Continue\n",sizeof(msg));
    }
    write(socket,msg,sizeof(msg));
}

void change_pwd_f(int socket,char fid[20])
{
    int fd = open("faculty.txt",O_RDWR);
    struct Faculty f;
    char msg[1024];
    char data[20];
    char id_offset[19];
    strncpy(id_offset,fid+1,19);
    int offset = atoi(id_offset);
    memset(msg,0,sizeof(msg));
    memset(data,0,sizeof(data));
    strncpy(msg,"Enter New Password\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    read(socket,data,sizeof(data));
    lseek(fd,(offset-1)*(40),SEEK_SET);
    lseek(fd,sizeof(f.f_id),SEEK_CUR);
    write(fd,data,20);
    strncpy(msg,"Password changed Successfully!\nPress X to Continue\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
}
void modify_course(int socket,char fid[20])
{
    char msg[1024];
    char cid[20];
    char data[20];
    struct Course c;
    strncpy(msg,"Enter the Course ID\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    memset(msg,0,sizeof(msg));
    read(socket,cid,sizeof(cid));
    int fd = open("CourseDetail.txt",O_RDWR);
    char id_offset[19];
    strncpy(id_offset,cid+1,19);
    int offset = atoi(id_offset);
    lseek(fd,(offset-1)*sizeof(struct Course)+20,SEEK_SET);
    read(fd,&c,sizeof(c));
    if(strncmp(c.faculty,fid,sizeof(c.faculty))!=0)
    {
        strncpy(msg,"Error! Course details cannot be modified\nPress X to Continue\n",sizeof(msg));
        write(socket,msg,sizeof(msg));
        return;
    }
    strncpy(msg,"What do you want to modify\nname\ntotal seats\nfaculty\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
    memset(msg,0,sizeof(msg));
    memset(data,0,sizeof(data));
    read(socket,data,sizeof(data));
    if(strcmp("name",data)==0)
    {
        strncpy(msg,"Enter the Course Name\n",sizeof(msg));
        write(socket,msg,sizeof(msg));
        memset(data,0,sizeof(data));
        read(socket,data,sizeof(data));
        lseek(fd,(offset-1)*sizeof(struct Course)+20,SEEK_SET);
    }
    if(strcmp("total seats",data)==0)
    {
        char oldseats[20];
        strncpy(msg,"Enter the new total seats\n",sizeof(msg));
        write(socket,msg,sizeof(msg));
        memset(data,0,sizeof(data));
        read(socket,data,sizeof(data));
        lseek(fd,(offset-1)*sizeof(struct Course)+40,SEEK_SET);
        read(fd,oldseats,sizeof(oldseats));
    }
    if(strcmp("faculty",data)==0)
    {
        strncpy(msg,"Enter the new faculty id\n",sizeof(msg));
        write(socket,msg,sizeof(msg));
        memset(data,0,sizeof(data));
        read(socket,data,sizeof(data));
        lseek(fd,(offset-1)*sizeof(struct Course)+80,SEEK_SET);
        int f_course = open("FacultyCourse.txt",O_RDWR);
        struct RelateCourseFaculty cf;
        int count =0;
        lseek(f_course,(count)*sizeof(cf),SEEK_SET);
        int rcount =  read(f_course,&cf,sizeof(cf));
        while(rcount!=0)
        {
            if(strncmp(cf.c_id,cid,sizeof(cf.c_id))==0)
            {
                strncpy(cf.f_id,fid,sizeof(cf.f_id));
                write(f_course,&cf,sizeof(cf));
            }
            count++;
            lseek(f_course,(count)*sizeof(cf),SEEK_SET);
            memset(&cf,0,sizeof(cf));
            rcount = read(f_course,&cf,sizeof(cf));
        }
    }
    write(fd,data,sizeof(data));
    memset(msg,0,sizeof(msg));
    strncpy(msg,"Updated Successfully\nPress X to Continue\n",sizeof(msg));
    write(socket,msg,sizeof(msg));
}
#endif