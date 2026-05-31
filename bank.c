#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct account
{
    int acc_no;
    char name[50];
    char password[20];
    float balance;
};

struct employee
{
    int emp_id;
    char name[50];
    char password[20];
    char role[30];
};
void createCustomerAccount();
void employeeLogin();
void customerLogin();

void deposit(int acc_no);
void withdraw(int acc_no);
void transfer(int acc_no);
void viewAccount(int acc_no);

void viewAllAccounts();
int main()
{
    int choice;

    while(1)
    {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Customer Login\n");
        printf("2. Employee Login\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                customerLogin();
                break;

            case 2:
                employeeLogin();
                break;

            case 3:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}
void employeeLogin()
{
    struct employee e;
    int id;
    char pass[20];
    FILE *fp;

    printf("\nEnter Employee ID: ");
    scanf("%d",&id);

    printf("Enter Password: ");
    scanf("%s",pass);

    fp = fopen("employees.dat","rb");

    if(fp == NULL)
    {
        printf("Employee file not found\n");
        return;
    }

    int found = 0;

    while(fread(&e,sizeof(e),1,fp))
    {
        if(e.emp_id == id && strcmp(e.password,pass) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if(found == 0)
    {
        printf("Invalid Employee Login\n");
        return;
    }

    int choice;

    while(1)
    {
        printf("\n===== EMPLOYEE MENU =====\n");
        printf("1. Create Customer Account\n");
        printf("2. View All Accounts\n");
        printf("3. Logout\n");

        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                createCustomerAccount();
                break;

            case 2:
                viewAllAccounts();
                break;

            case 3:
                return;

            default:
                printf("Invalid choice\n");
        }
    }
}
void createCustomerAccount()
{
    struct account a;
    FILE *fp;

    fp = fopen("accounts.dat","ab");

    if(fp == NULL)
    {
        printf("File error\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d",&a.acc_no);

    printf("Enter Name: ");
    scanf("%s",a.name);

    printf("Enter Password: ");
    scanf("%s",a.password);

    printf("Enter Initial Deposit: ");
    scanf("%f",&a.balance);

    fwrite(&a,sizeof(a),1,fp);

    fclose(fp);

    printf("Customer account created successfully\n");
}
void viewAllAccounts()
{
    struct account a;
    FILE *fp;

    fp = fopen("accounts.dat","rb");

    if(fp == NULL)
    {
        printf("No accounts found\n");
        return;
    }

    printf("\n===== CUSTOMER ACCOUNTS =====\n");

    while(fread(&a,sizeof(a),1,fp))
    {
        printf("\nAccount Number : %d\n",a.acc_no);
        printf("Name           : %s\n",a.name);
        printf("Balance        : %.2f\n",a.balance);
    }

    fclose(fp);
}
void customerLogin()
{
    struct account a;
    int acc_no;
    char pass[20];
    FILE *fp;

    printf("\nEnter Account Number: ");
    scanf("%d",&acc_no);

    printf("Enter Password: ");
    scanf("%s",pass);

    fp = fopen("accounts.dat","rb");

    if(fp == NULL)
    {
        printf("No accounts found\n");
        return;
    }

    int found = 0;

    while(fread(&a,sizeof(a),1,fp))
    {
        if(a.acc_no == acc_no && strcmp(a.password,pass) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if(found == 0)
    {
        printf("Invalid Login\n");
        return;
    }

    int choice;

    while(1)
    {
        printf("\n===== CUSTOMER MENU =====\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Transfer Funds\n");
        printf("4. View Account\n");
        printf("5. Logout\n");

        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                deposit(acc_no);
                break;

            case 2:
                withdraw(acc_no);
                break;

            case 3:
                transfer(acc_no);
                break;

            case 4:
                viewAccount(acc_no);
                break;

            case 5:
                return;

            default:
                printf("Invalid choice\n");
        }
    }
}
void deposit(int acc_no)
{
    struct account a;
    FILE *fp;
    float amount;

    fp = fopen("accounts.dat","rb+");

    if(fp == NULL)
    {
        printf("File error\n");
        return;
    }

    while(fread(&a,sizeof(a),1,fp))
    {
        if(a.acc_no == acc_no)
        {
            printf("Enter amount to deposit: ");
            scanf("%f",&amount);

            a.balance += amount;

            fseek(fp,-sizeof(a),SEEK_CUR);
            fwrite(&a,sizeof(a),1,fp);

            printf("Deposit successful\n");
            printf("New balance: %.2f\n",a.balance);
            break;
        }
    }

    fclose(fp);
}
void withdraw(int acc_no)
{
    struct account a;
    FILE *fp;
    float amount;

    fp = fopen("accounts.dat","rb+");

    if(fp == NULL)
    {
        printf("File error\n");
        return;
    }

    while(fread(&a,sizeof(a),1,fp))
    {
        if(a.acc_no == acc_no)
        {
            printf("Enter withdraw amount: ");
            scanf("%f",&amount);

            if(a.balance >= amount)
            {
                a.balance -= amount;

                fseek(fp,-sizeof(a),SEEK_CUR);
                fwrite(&a,sizeof(a),1,fp);

                printf("Withdrawal successful\n");
                printf("Remaining balance: %.2f\n",a.balance);
            }
            else
            {
                printf("Insufficient balance\n");
            }

            break;
        }
    }

    fclose(fp);
}
void transfer(int acc_no)
{
    struct account a;
    FILE *fp;
    int receiver_acc;
    float amount;

    printf("Enter receiver account number: ");
    scanf("%d",&receiver_acc);

    printf("Enter amount to transfer: ");
    scanf("%f",&amount);

    fp = fopen("accounts.dat","rb+");

    if(fp == NULL)
    {
        printf("File error\n");
        return;
    }

    int sender_found = 0;
    int receiver_found = 0;
    long pos_sender, pos_receiver;

    while(fread(&a,sizeof(a),1,fp))
    {
        if(a.acc_no == acc_no)
        {
            if(a.balance < amount)
            {
                printf("Insufficient balance\n");
                fclose(fp);
                return;
            }

            sender_found = 1;
            pos_sender = ftell(fp) - sizeof(a);
        }

        if(a.acc_no == receiver_acc)
        {
            receiver_found = 1;
            pos_receiver = ftell(fp) - sizeof(a);
        }
    }

    if(sender_found && receiver_found)
    {
        fseek(fp,pos_sender,SEEK_SET);
        fread(&a,sizeof(a),1,fp);
        a.balance -= amount;
        fseek(fp,-sizeof(a),SEEK_CUR);
        fwrite(&a,sizeof(a),1,fp);

        fseek(fp,pos_receiver,SEEK_SET);
        fread(&a,sizeof(a),1,fp);
        a.balance += amount;
        fseek(fp,-sizeof(a),SEEK_CUR);
        fwrite(&a,sizeof(a),1,fp);

        printf("Transfer successful\n");
    }
    else
    {
        printf("Account not found\n");
    }

    fclose(fp);
}
void viewAccount(int acc_no)
{
    struct account a;
    FILE *fp;

    fp = fopen("accounts.dat","rb");

    if(fp == NULL)
    {
        printf("File error\n");
        return;
    }

    while(fread(&a,sizeof(a),1,fp))
    {
        if(a.acc_no == acc_no)
        {
            printf("\n===== ACCOUNT DETAILS =====\n");
            printf("Account Number : %d\n",a.acc_no);
            printf("Name           : %s\n",a.name);
            printf("Balance        : %.2f\n",a.balance);
            break;
        }
    }

    fclose(fp);
}
