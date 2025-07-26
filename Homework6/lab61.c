#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define Limit_Guide 3
#define  NAME_SIZE 10
#define TEL_SIZE 100
typedef struct abonent {
struct abonent *prev; 
struct abonent *next;
char name[NAME_SIZE];
char second_name[NAME_SIZE];
char tel[TEL_SIZE];
}Subscriber;


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

int Add_subscriber(int *Flag_pointer, Subscriber **head, Subscriber **tail) { 
    char answer[2];
    int Flag_answer = 0;
    int c;

    while ((c = getchar()) != '\n' && c != EOF);

    
    Subscriber *subscriber = (Subscriber *)malloc(sizeof(Subscriber));
    if (subscriber == NULL) {
        perror("Failed to allocate memory");
        *Flag_pointer = 0; 
        return 1;
    }

    while (1) {
        printf("Enter the name: ");
        fgets(subscriber->name, NAME_SIZE, stdin);
        subscriber->name[strcspn(subscriber->name, "\n")] = 0;
        if (valid_name(subscriber->name)) {
            break;
        } else {
            printf("Invalid name. Please enter only letters and spaces.\n");
        }
    }

    while (1) {
        printf("Enter the second name: ");
        fgets(subscriber->second_name, NAME_SIZE, stdin);
        subscriber->second_name[strcspn(subscriber->second_name, "\n")] = 0;
        if (valid_name(subscriber->second_name)) {
            break;
        } else {
            printf("Invalid name. Please enter only letters and spaces.\n");
        }
    }

    while (1) {
        printf("Enter the phone number: ");
        fgets(subscriber->tel, TEL_SIZE, stdin);
        subscriber->tel[strcspn(subscriber->tel, "\n")] = 0;
        if (valid_phone(subscriber->tel)) {
            break;
        } else {
            printf("Invalid phone number. Please enter only digits.\n");
        }
    }

    subscriber->next = NULL;
    if (*head == NULL) {
        subscriber->prev = NULL;
        *head = subscriber;
        *tail = subscriber;
    } else {
        subscriber->prev = *tail;
        (*tail)->next = subscriber;
        *tail = subscriber;
    }

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
            printf("Invalid choice, please press Y or N: ");
        }
    }

    return 0;
}

int Delete_subscriber(int *Flag_pointer, Subscriber **head, Subscriber **tail) {
    char answer[2];
    int Flag_answer = 0;
    char name_to_delete[NAME_SIZE];
    int c;

    while ((c = getchar()) != '\n' && c != EOF);

    if (*head == NULL) {
        printf("No subscribers to delete.\n");
        *Flag_pointer = 0;
        return 0;
    }

    printf("Enter the name of the subscriber to delete: ");
    fgets(name_to_delete, NAME_SIZE, stdin);
    name_to_delete[strcspn(name_to_delete, "\n")] = 0;

    Subscriber *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name_to_delete) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                *head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                *tail = current->prev;
            }

            printf("Subscriber deleted successfully:\n");
            printf("  Name: %s\n", current->name);
            printf("  Second Name: %s\n", current->second_name);
            printf("  Phone: %s\n", current->tel);

            free(current);
            printf("Do you want to delete another subscriber (Y)es/(N)o?\n Press Y or N: ");
            while (!Flag_answer) {
                scanf("%1s", answer);
                if (strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0) {
                    *Flag_pointer = 0;
                    Flag_answer = 1;
                } else if (strcmp(answer, "Y") == 0 || strcmp(answer, "y") == 0) {
                    *Flag_pointer = 2;
                    Flag_answer = 1;
                } else {
                    printf("Invalid choice, please press Y or N: ");
                }
            }

            return 0;
        }

        current = current->next;
    }

    printf("Subscriber with name '%s' not found.\n", name_to_delete);
    *Flag_pointer = 0;
    return 0;
}

int Search_subscriber(int *Flag_pointer, Subscriber **head) {
    int Flag_answer = 0;
    char answer[2];
    char search_name[NAME_SIZE];
    int c;
    Subscriber *current = *head;
    int found_count = 0;

    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter the name to search for: ");
    fgets(search_name, NAME_SIZE, stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    printf("\n--- Search Results ---\n");
    while (current != NULL) {
        if (strcmp(search_name, current->name) == 0) {
            printf("Subscriber found:\n");
            printf("  Name: %s\n", current->name);
            printf("  Second Name: %s\n", current->second_name);
            printf("  Phone: %s\n", current->tel);
            printf("--------------------\n");
            found_count++;
        }
        current = current->next;
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
            printf("Invalid choice, please press Y or N: ");
        }
    }
    return 0;
}

int Display_subscribers(int *Flag_pointer, Subscriber **head) {
    Subscriber *current = *head;
    int found_count = 0;

    printf("\n--- Subscriber List ---\n");
    if (current == NULL) {
        printf("There are no subscribers in the directory.\n");
    } else {
        while (current != NULL) {
            printf("Subscriber found:\n");
            printf("  Name: %s\n", current->name);
            printf("  Second Name: %s\n", current->second_name);
            printf("  Phone: %s\n", current->tel);
            printf("--------------------\n");
            found_count++;
            current = current->next;
        }
    }

    *Flag_pointer = 0;
    return 0;
}
void menu(){
    Subscriber *head = NULL; 
    Subscriber *tail = NULL;
    int Flag = 0;
    int* Flag_pointer=&Flag;
    int abonent_count = 0;
    while (Flag !=5)
    {
        if(Flag == 0){
            display_menu(Flag_pointer);
        }
        else if(Flag == 1){
            Add_subscriber(Flag_pointer, &head, &tail);
        }
        else if(Flag == 2){
             Delete_subscriber(Flag_pointer, &head, &tail);
        }
        else if(Flag == 3){
            Search_subscriber(Flag_pointer, &head);
        }
        else if(Flag == 4){
            Display_subscribers(Flag_pointer, &head);
        }
    }
    Subscriber *current = head;
    while (current != NULL) {
        Subscriber *temp = current;
        current = current->next;
        free(temp);
    }
    printf("The program is closed");
    
}

int main(){
    
    menu();
}