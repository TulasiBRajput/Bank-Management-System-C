#include<stdio.h>

struct employee
{
    int emp_id;
    char name[50];
    char password[20];
    char role[30];
};

int main()
{
    struct employee e;
    FILE *fp;

    fp = fopen("employees.dat","wb");

    printf("Enter Employee ID: ");
    scanf("%d",&e.emp_id);

    printf("Enter Name: ");
    scanf("%s",e.name);

    printf("Enter Password: ");
    scanf("%s",e.password);

    printf("Enter Role: ");
    scanf("%s",e.role);

    fwrite(&e,sizeof(e),1,fp);

    fclose(fp);

    printf("Employee created successfully\n");

    return 0;
}
