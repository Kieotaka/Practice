#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define Limit_Guide 3
#define  NAME_SIZE 10
#define TEL_SIZE 10
typedef struct abonent {
char name[NAME_SIZE];
char second_name[NAME_SIZE];
char tel[TEL_SIZE];
}Subscriber;

struct abonent massive[Limit_Guide] ={0};

int valid_name(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) { 
            return 0; 
        }
    }
    return 1; 
}
int valid_phone(const char *phone) {
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            return 0; 
        }
    }
    return 1; 
}

int display_menu(int *Flag_pointer){
    int number;
    printf(" To interact, enter one of the following keys: 1,2,3,4,5\n");
    printf(" 1) Add a subscriber\n");
    printf(" 2) Delete a subscriber\n");
    printf(" 3) Search for subscribers by name\n");
    printf(" 4) Display all records\n");
    printf(" 5) Log out\n");
    printf("Enter: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input. Please enter a number.\n"); 
        return 1;  
    }
    if (number < 1 || number > 5) {
        printf("Invalid choice. Please enter a number between 1 and 5.\n");
        return 1;
    }
    *Flag_pointer  = number ;
    return 0;
}

int Add_subscriber(int *Flag_pointer, int *abonent_count) {
    char answer[2]; 
    int Flag_answer = 0;
    int c;
        
    while ((c = getchar()) != '\n' && c != EOF);
    if (*abonent_count >= Limit_Guide) {
        printf("Maximum number of abonents reached.\n");
        *Flag_pointer = 0;
        return 1;
    }
    while(1){
        printf("Enter the name: ");
        fgets(massive[*abonent_count].name, NAME_SIZE, stdin);
        massive[*abonent_count].name[strcspn(massive[*abonent_count].name, "\n")] = 0;
        if (valid_name(massive[*abonent_count].name)) {
            break; 
        } 
        else {
            printf("Invalid name. Please enter only letters and spaces.\n");
        }
    }
    while(1){
        printf("Enter the second name: ");
        fgets(massive[*abonent_count].second_name, NAME_SIZE, stdin);
        massive[*abonent_count].second_name[strcspn(massive[*abonent_count].second_name, "\n")] = 0;
        if (valid_name(massive[*abonent_count].second_name)) {
            break; 
        } 
        else {
            printf("Invalid name. Please enter only letters and spaces.\n");
        }
    }
    while (1) {
        printf("Enter the phone number: ");
        fgets(massive[*abonent_count].tel, TEL_SIZE, stdin);
        massive[*abonent_count].tel[strcspn(massive[*abonent_count].tel, "\n")] = 0;
        if (valid_phone(massive[*abonent_count].tel)) {
            break; 
        } 
        else {
            printf("Invalid phone number. Please enter only digits.\n");
        }
    }
    *abonent_count = *abonent_count + 1; 

    printf("Abonent added successfully.\n");
    
    printf("Do you want to add another subscriber (Y)es/(N)o?\n Press Y or N: ");
    while (!Flag_answer) {
        scanf("%1s", answer);
        if (strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0) {
            *Flag_pointer = 0;
            Flag_answer = 1;
        } else if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) {
            *Flag_pointer = 1;
            Flag_answer = 1;
        } else {
            printf("Invalid choice, please press Y or N :");
        }
    }

    return 0;
}

int Delete_subscriber(int *Flag_pointer, int *abonent_count){

    int Flag_answer = 0;
    char answer[2];

    if (*abonent_count <= 0) {
        printf("No subscribers to delete.\n");
        *Flag_pointer = 0; 
        return 0; 
    }
    
    *abonent_count = *abonent_count - 1;
    printf("Abonent deleted successfully:\n");
    printf("  Name: %s\n", &massive[*abonent_count].name);
    printf("  Second Name: %s\n", &massive[*abonent_count].second_name);
    printf("  Phone: %s\n", &massive[*abonent_count].tel);
    
    memset(&massive[*abonent_count], 0, sizeof(Subscriber));
    printf("Do you want to deleted another subscriber (Y)es/(N)o?\n Press Y or N: ");
    while (!Flag_answer) {
        scanf("%1s", answer);
        if (strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0) {
            *Flag_pointer = 0;
            Flag_answer = 1;
        } else if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) {
            *Flag_pointer = 2;
            Flag_answer = 1;
        } else {
            printf("Invalid choice, please press Y or N :");
        }
    }

    return 0;
}

int Search_subscriber(int *Flag_pointer,int *abonent_count){
    int Flag_answer = 0;
    char answer[2];
    char search_name[NAME_SIZE];
    int c;
    int found_count = 0;
    printf("Enter the name to search for: ");
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(search_name, NAME_SIZE, stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    printf("\n--- Search Results ---\n");
    for (int i = 0; i < Limit_Guide; i++) {
        if (strcmp(search_name, massive[i].name) == 0) {
            printf("Subscriber found:\n");
            printf("  Name: %s\n", massive[i].name);
            printf("  Second Name: %s\n", massive[i].second_name);
            printf("  Phone: %s\n", massive[i].tel);
            printf("--------------------\n");
            found_count++;
        }
    }
    if (found_count == 0) {
        printf("No subscribers found with the name \"%s\".\n", search_name);
    }

    printf("Do you want to search another subscriber (Y)es/(N)o?\n Press Y or N: ");
    while (!Flag_answer) {
        scanf("%1s", answer);
        if (strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0) {
            *Flag_pointer = 0;
            Flag_answer = 1;
        } else if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) {
            *Flag_pointer = 3;
            Flag_answer = 1;
        } else {
            printf("Invalid choice, please press Y or N :");
        }
    }
    return 0;
}

int Display_subscribers(int *Flag_pointer,int *abonent_count){
    int found_count = 0;
    // Для демонстрации того что при удалении экземпляр структуры заполняется то при выводе проходимся по всему массиву Limit_Guide.
    // Для более правильной работы изменил бы проход в цикле for до *abonent_count тоесть до последнего введеного пользователем абонента.
    for (int i = 0; i < Limit_Guide; i++) {
        printf("Subscriber found:\n");
        printf("  Name: %s\n", massive[i].name);
        printf("  Second Name: %s\n", massive[i].second_name);
        printf("  Phone: %s\n", massive[i].tel);
        printf("--------------------\n");
        found_count++;
    }
    if (found_count == 0) {
        printf("There are no subscribers in the directory.\n");
    }
    *Flag_pointer = 0;
    return 0;
}
void menu(){
    int Flag = 0;
    int* Flag_pointer=&Flag;
    int abonent_count = 0;
    int* count_pointer=&abonent_count;
    while (Flag !=5)
    {
        if(Flag == 0){
            display_menu(Flag_pointer);
        }
        else if(Flag == 1){
            Add_subscriber(Flag_pointer,count_pointer);
        }
        else if(Flag == 2){
            Delete_subscriber(Flag_pointer,count_pointer);
        }
        else if(Flag == 3){
            Search_subscriber(Flag_pointer,count_pointer);
        }
        else if(Flag == 4){
            Display_subscribers(Flag_pointer,count_pointer);
        }
    }
    printf("The program is closed");
    
}

int main(){
    menu();
}