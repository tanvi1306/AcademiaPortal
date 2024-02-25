#ifndef STRUCTURES_H
#define STRUCTURES_H
struct  Student
{
    char s_id[20];
    char name[20];
    char age[20];
    char email[20];
    char dept[20];
};
struct Faculty{
    char f_id[20];
    char name[20];
    char dept[20];
    char email[20];
};

struct Course{
    char c_id[20];
    char name[20];
    char total[20];
    char stu_count[20];
    char faculty[20];
};

struct RelateCourseFaculty{
    char f_id[20];
    char c_id[20];
};

struct RelateCourseStudent{
    char s_id[20];
    char c_id[20];
};
struct Credentials{
    char id[20];
    char pwd[20];
};
#endif


