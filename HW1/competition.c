#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[128];
    char area[128];
    int entries;
}Participant;

char areas[7][128] = {"Baratfa", "Lovas", "Szula", "Kigyos-patak", "Malom-telek", "Paskom","Kaposztas-kert"};
int numOfParticipants;
Participant *participants;

// saving into file

void printAreas();
void getArea();
void getName();
void getEntry();
void makeDecision();
void modify();
void listParticipants(char *name);
void writeToFile();
void removeParticipant();
void readFromFile();
void easterEgg();
int exists(char* name);

int main(int argc, char *argv[]){   
    printf("=============================================\nWelcome to the Easter Bunny King competition!\n=============================================\n");
    participants = malloc(sizeof(Participant)*5);
    numOfParticipants = 0;
    if (participants == NULL){
        printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
        exit(-1);
    }
    
    readFromFile();

    int cont;
    do{
        if (numOfParticipants == sizeof(participants)/sizeof(Participant)){
            participants = realloc(participants, (5+numOfParticipants)*sizeof(Participant));
            if(participants == NULL){
                printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
                exit(-1);
            }
        }
        makeDecision();
        printf("=============================================\nDo you want to continue?\n1. YES\n2. NO\n");
        scanf("%d", &cont);
        printf("=============================================\n");
    }while(cont == 1);

    free(participants);
    printf("CHIEF BUNNY: It was nice to see you here! Have a good day!\n");
    printf("=============================================\n");
    easterEgg();
    return 0;
}

void makeDecision(){
    int decision;
    do{
        printf(" 1. Register a participant\n 2. Modify data of the participant\n 3. Remove participants data\n 4. List participants by area\n 5. Save to file\n 6. List participants\n 7. Read from file\n");
        printf("=============================================\nCHIEF BUNNY: Choose what are you going to do: ");
        scanf("%d", &decision);
    }while(1>decision || decision>7);

    switch (decision){
        case 1:
            getName();
            getArea();
            getEntry();
            printf("=============================================\n");
            printf("\nYou registered %s from %s with %d entries\n\n", &participants[numOfParticipants].name, participants[numOfParticipants].area, participants[numOfParticipants].entries);
            numOfParticipants++;
            break;
        case 2:
            modify();
            break;
        case 3:
            removeParticipant();
            break;
        case 4:
            printAreas();
            int decision;
            printf("CHIEF BUNNY: Choose the number of the area: ");
            scanf("%d", &decision);
            listParticipants(areas[decision-1]);
            break;
        case 5:
            writeToFile();
        case 6:
            listParticipants("");
            break;
        case 7:
            readFromFile();
            break;
        default:
            printf("CHIEF BUNNY: Ups...Something went wrong!\n");
            break;
        }
}

///////////////////REGISTER PARTICIPANT///////////////////
//////////////////////////////////////////////////////////
void getArea(){
    printf("=============================================\n");
    printAreas();
    int decision;
    printf("CHIEF BUNNY: Choose the number of the area: ");
    scanf("%d", &decision);

    if(decision>0 && decision<8){
        strcpy(participants[numOfParticipants].area, areas[decision-1]);
        printf("CHIEF BUNNY: Nice, you chose %s.\n", participants[numOfParticipants].area);
    } else {
        printf("CHIEF BUNNY: There was some kind of mistake! \nMake sure to choose between 1 and 7!\n");
        getArea();
    }
}

void getName(){
    printf("=============================================\n");
    printf("CHIEF BUNNY: Name of your bunny: ");
    scanf("%s", &participants[numOfParticipants].name);
    printf("CHIEF BUNNY: Your bunnies name is %s\n", &participants[numOfParticipants].name);
}

void getEntry(){
    printf("=============================================\n");
    int entryNum;
    printf("CHIEF BUNNY: The number of entries: ");
    scanf("%d", &participants[numOfParticipants].entries);
    printf("CHIEF BUNNY: Your bunny will have %d entries\n", participants[numOfParticipants].entries);
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void modify(){
    char bunny[128];
    printf("CHIEF BUNNY: Which bunny do you want to modify?  ");
    scanf("%s", &bunny);
    int exists = 0;
    //check if there is such bunny
    int index = 0;
    printf("%s will be compared with %s\n", bunny, &participants[index].name);
    printf("bunny: %d, participant: %d\n", sizeof(bunny)/sizeof(char), sizeof(participants[index].name)/sizeof(char));
    while(index < numOfParticipants && strcmp(bunny, participants[index].name) != 0){
        printf("%s\n", &participants[index].name);
        index++;
    }
    // if exists
    if (index < numOfParticipants){
        int decision;
        printf("CHIEF BUNNY: We found your bunny!\n");
        do{
            printf("1. Name\n2. Area\n3. Entries\nWhat do you want to change? ");
            scanf("%d", &decision);
        }while(decision<1 || decision > 3);

        int modifiedArea;
        int modifiedEntries;
        char modifiedName[128];
        switch(decision){
            case 1: 
                printf("CHIEF BUNNY: Write new name: ");
                scanf("%s", &modifiedName);
                strcpy(participants[index].name, modifiedName);
                printf("CHIEF BUNNY: You changed the name to %s\n", participants[index].name);
                break;
            case 2:
                do{
                    printAreas();
                    printf("CHIEF BUNNY: Choose new area: ");
                    scanf("%d", &modifiedArea);
                }while(modifiedArea > 7 || modifiedArea < 1);
                strcpy(participants[index].area, areas[modifiedArea-1]);
                printf("CHIEF BUNNY: You changed area to %s\n", participants[index].area);
                break;
            case 3:
                printf("CHIEF BUNNY: Print new number of entries: ");
                scanf("%d", &modifiedEntries);
                participants[index].entries = modifiedEntries;
                printf("CHIEF BUNNY: You changed the number of entries to %d\n", participants[index].entries);
                break;
            default:
                printf("CHIEF BUNNY: Ups...Something went wrong!\n");
                break;
        }
    } 
    // doesn't exist
    else {
        printf("CHIEF BUNNY: Unfortunately, there is no such bunny!\n");
    }
}

void removeParticipant(){
    Participant *tempParticipants = malloc(sizeof(Participant)*numOfParticipants);
    if(tempParticipants == NULL){
        printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
        exit(-1);
    }

    char name[128];
    printf("CHIEF BUNNY: Which participant do you want to remove: ");
    scanf("%s", &name);
    int afterRem = 0;
    for(int i=0;i<numOfParticipants;i++){
        if(strcmp(participants[i].name, name) != 0){
            strcpy(tempParticipants[afterRem].name, participants[i].name);
            strcpy(tempParticipants[afterRem].area, participants[i].area);
            tempParticipants[afterRem].entries  = participants[i].entries;
            afterRem++;
        }
    }

    if(numOfParticipants != afterRem){
        numOfParticipants = afterRem;
        free(participants);
        participants = malloc(sizeof(Participant)*numOfParticipants);

        memmove(&participants, &tempParticipants, sizeof(tempParticipants)*numOfParticipants);
        printf("CHIEF BUNNY: Participant %s has been removed!\n", name);
    } else{
        printf("CHIEF BUNNY: No such participant like %s!\n");
    }
}

int exists(char* name){
    for(int i=0;i<numOfParticipants;i++){
        if(strcmp(participants[i].name, name) == 0){
            printf("%s is already in the list of participants!\n");
            return 1;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////
////////////////READ WRITE AND PRINT//////////////////////
void writeToFile(){
    FILE * ptr = fopen("list.txt", "w");
    if(ptr == NULL){
        printf("CHIEF BUNNY: Oops...Something went wrong with file!");
    }
    else{
        for(int i=0; i<numOfParticipants;i++){
            fprintf(ptr, "%d. %10s %15s %10d\n", (i+1), participants[i].name, participants[i].area, participants[i].entries);
        }
    }
    fclose(ptr);
}

void printAreas(){
    for(int i=0;i<7;i++){
        printf("%d. %s\n", (i+1), areas[i]);
    }
}

void listParticipants(char *name){
    if(name==""){
        printf("=============================================\n         Name            Area       Entries\n=============================================\n");
        for(int i=0;i<numOfParticipants;i++){
            printf("%d. %10s %15s %10d\n", (i+1), &participants[i].name, participants[i].area, participants[i].entries);
        }
    } else {
        printf("CHIEF BUNNY: List of bunnies by area %s:\n", name);
        printf("=============================================\n         Name            Area       Entries\n=============================================\n");
        for(int i=0;i<numOfParticipants;i++){
            if(strcmp(participants[i].area, name) == 0){
                printf("%d. %10s %15s %10d\n", (i+1), &participants[i].name, participants[i].area, participants[i].entries);
            }
        }
    }
}
void readFromFile(){
    FILE * fp = fopen("list.txt", "r");
    if(fp == NULL){
        exit(-1);
    }
    char name[128];
    char area[128];
    int entries;
    while(fscanf(fp, "%*d. %10s %15s %10d", &name, &area, &entries)!=EOF){  
        if(exists(name) == 0){
            strcpy(participants[numOfParticipants].name, name);
            strcpy(participants[numOfParticipants].area, area);
            participants[numOfParticipants].entries = entries;
            printf("CHIEF BUNNY: %s from %s with %d entries was registered from database\n", &participants[numOfParticipants].name, participants[numOfParticipants].area, participants[numOfParticipants].entries);
            numOfParticipants++;

            if (numOfParticipants == sizeof(participants)/sizeof(Participant)){
                participants = realloc(participants, (5+numOfParticipants)*sizeof(Participant));
                if(participants == NULL){
                    printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
                    exit(-1);
                }
            }
        }
    }  
    printf("=============================================\n");
    fclose(fp); 
}

void easterEgg(){
    printf(R"EOF(
         / \
        / _ \
       | / \ |
       ||   || _______
       ||   || |\     \
       ||   || ||\     \
       ||   || || \    |
       ||   || ||  \__/
       ||   || ||   ||
        \\_/ \_/ \_//
       /   _     _   \
      /               \
      |    O     O    |
      |   \  ___  /   |
     /     \ \_/ /     \
    /  -----  |  --\    \
    |     \__/|\__/ \   | BYE BYE!
    \       |_|_|       /
     \_____       _____/
           \     /
           |     |
    )EOF");
}