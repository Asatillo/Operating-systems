#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include<time.h>

typedef struct
{
    char name[128];
    char area[128];
    int entries;
} Participant;

typedef struct
{
    char name[128];
    int eggs;
} ParticipantShort;

char areas[7][128] = {"Baratfa", "Lovas", "Szula", "Kigyos-patak", "Malom-telek", "Paskom", "Kaposztas-kert"};
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
int startCompetition();
void easterEgg();

int exists(char *name);

int main(int argc, char *argv[])
{
    printf("=============================================\nWelcome to the Easter Bunny King competition!\n=============================================\n");
    participants = malloc(sizeof(Participant) * 30);
    numOfParticipants = 0;
    if (participants == NULL)
    {
        printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
        exit(-1);
    }

    readFromFile();
    
    int cont;
    do
    {
        if (numOfParticipants == sizeof(participants) / sizeof(Participant))
        {
            participants = realloc(participants, (5 + numOfParticipants) * sizeof(Participant));
            if (participants == NULL)
            {
                printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
                exit(-1);
            }
        }
        makeDecision();
        printf("=============================================\nDo you want to continue?\n1. YES\n2. NO\n");
        scanf("%d", &cont);
        printf("=============================================\n");
    } while (cont == 1);
    writeToFile();
    free(participants);
    printf("CHIEF BUNNY: It was nice to see you here! Have a good day!\n");
    printf("=============================================\n");
    easterEgg();
    return 0;
}

void makeDecision()
{
    int decision;
    do
    {
        printf(" 1. Register a participant\n 2. Modify data of the participant\n 3. Remove participants data\n 4. List participants by area\n 5. List participants\n 6. START COMPETITION\n");
        printf("=============================================\nCHIEF BUNNY: Choose what are you going to do: ");
        scanf("%d", &decision);
    } while (1 > decision || decision > 8);

    switch (decision)
    {
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
        listParticipants(areas[decision - 1]);
        break;
    case 5:
        listParticipants("");
        break;
    case 6:
        startCompetition();
        break;
    default:
        printf("CHIEF BUNNY: Ups...Something went wrong!\n");
        break;
    }
}

///////////////////REGISTER PARTICIPANT///////////////////
//////////////////////////////////////////////////////////
void getArea()
{
    printf("=============================================\n");
    printAreas();
    int decision;
    printf("CHIEF BUNNY: Choose the number of the area: ");
    scanf("%d", &decision);

    if (decision > 0 && decision < 8)
    {
        strcpy(participants[numOfParticipants].area, areas[decision - 1]);
        printf("CHIEF BUNNY: Nice, you chose %s.\n", participants[numOfParticipants].area);
    }
    else
    {
        printf("CHIEF BUNNY: There was some kind of mistake! \nMake sure to choose between 1 and 7!\n");
        getArea();
    }
}

void getName()
{
    printf("=============================================\n");
    printf("CHIEF BUNNY: Name of your bunny: ");
    scanf("%s", &participants[numOfParticipants].name);
    printf("CHIEF BUNNY: Your bunnies name is %s\n", &participants[numOfParticipants].name);
}

void getEntry()
{
    printf("=============================================\n");
    int entryNum;
    printf("CHIEF BUNNY: The number of entries: ");
    scanf("%d", &participants[numOfParticipants].entries);
    printf("CHIEF BUNNY: Your bunny will have %d entries\n", participants[numOfParticipants].entries);
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void modify()
{
    char bunny[128];
    printf("CHIEF BUNNY: Which bunny do you want to modify?  ");
    scanf("%s", &bunny);
    int exists = 0;
    // check if there is such bunny
    int index = 0;
    while (index < numOfParticipants && strcmp(bunny, participants[index].name) != 0){index++;}
    // if exists
    if (index < numOfParticipants)
    {
        int decision;
        printf("CHIEF BUNNY: We found your bunny!\n");
        do
        {
            printf("1. Name\n2. Area\n3. Entries\nWhat do you want to change? ");
            scanf("%d", &decision);
        } while (decision < 1 || decision > 3);

        int modifiedArea;
        int modifiedEntries;
        char modifiedName[128];
        switch (decision)
        {
        case 1:
            printf("CHIEF BUNNY: Write new name: ");
            scanf("%s", &modifiedName);
            strcpy(participants[index].name, modifiedName);
            printf("CHIEF BUNNY: You changed the name to %s\n", participants[index].name);
            break;
        case 2:
            do
            {
                printAreas();
                printf("CHIEF BUNNY: Choose new area: ");
                scanf("%d", &modifiedArea);
            } while (modifiedArea > 7 || modifiedArea < 1);
            strcpy(participants[index].area, areas[modifiedArea - 1]);
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
    else
    {
        printf("CHIEF BUNNY: Unfortunately, there is no such bunny!\n");
    }
}

void removeParticipant()
{
    Participant *tempParticipants = malloc(sizeof(Participant) * numOfParticipants);
    if (tempParticipants == NULL)
    {
        printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
        exit(-1);
    }

    char name[128];
    printf("CHIEF BUNNY: Which participant do you want to remove: ");
    scanf("%s", &name);
    int afterRem = 0;
    for (int i = 0; i < numOfParticipants; i++)
    {
        if (strcmp(participants[i].name, name) != 0)
        {
            strcpy(tempParticipants[afterRem].name, participants[i].name);
            strcpy(tempParticipants[afterRem].area, participants[i].area);
            tempParticipants[afterRem].entries = participants[i].entries;
            afterRem++;
        }
    }

    if (numOfParticipants != afterRem)
    {
        numOfParticipants = afterRem;
        free(participants);
        participants = malloc(sizeof(Participant) * numOfParticipants);

        memmove(&participants, &tempParticipants, sizeof(tempParticipants) * numOfParticipants);
        printf("CHIEF BUNNY: Participant %s has been removed!\n", name);
    }
    else
    {
        printf("CHIEF BUNNY: No such participant like %s!\n");
    }
}

int exists(char *name)
{
    for (int i = 0; i < numOfParticipants; i++)
    {
        if (strcmp(participants[i].name, name) == 0)
        {
            printf("%s is already in the list of participants!\n");
            return 1;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////
////////////////READ WRITE AND PRINT//////////////////////
void writeToFile()
{
    FILE *ptr = fopen("list.txt", "w");
    if (ptr == NULL)
    {
        printf("CHIEF BUNNY: Oops...Something went wrong with file!");
    }
    else
    {
        for (int i = 0; i < numOfParticipants; i++)
        {   
            if(participants[i].entries != 0){
                fprintf(ptr, "%2d. %10s %15s %10d\n", (i + 1), participants[i].name, participants[i].area, participants[i].entries);
            }
        }
    }
    fclose(ptr);
}

void printAreas()
{
    for (int i = 0; i < 7; i++)
    {
        printf("%d. %s\n", (i + 1), areas[i]);
    }
}

void listParticipants(char *name)
{
    if (name == "")
    {
        printf("=============================================\n         Name            Area       Entries\n=============================================\n");
        for (int i = 0; i < numOfParticipants; i++)
        {
            if(participants[i].entries != 0){
                printf("%2d. %10s %15s %10d\n", (i + 1), &participants[i].name, participants[i].area, participants[i].entries);
            }
        }
    }
    else
    {
        printf("CHIEF BUNNY: List of bunnies by area %s:\n", name);
        printf("=============================================\n         Name            Area       Entries\n=============================================\n");
        int cnt = 1;
        for (int i = 0; i < numOfParticipants; i++)
        {
            if (strcmp(participants[i].area, name) == 0)
            {
                printf("%2d. %10s %15s %10d\n", (cnt++), &participants[i].name, participants[i].area, participants[i].entries);
            }
        }
    }
}
void readFromFile()
{
    FILE *fp = fopen("list.txt", "r");
    if (fp == NULL)
    {
        exit(-1);
    }
    char name[128];
    char area[128];
    int entries;
    while (fscanf(fp, "%*d. %10s %15s %10d", &name, &area, &entries) != EOF)
    {
        if (exists(name) == 0)
        {
            strcpy(participants[numOfParticipants].name, name);
            strcpy(participants[numOfParticipants].area, area);
            participants[numOfParticipants].entries = entries;
            printf("CHIEF BUNNY: %s from %s with %d entries\n", &participants[numOfParticipants].name, participants[numOfParticipants].area, participants[numOfParticipants].entries);
            numOfParticipants++;
            if (numOfParticipants == sizeof(participants) / sizeof(Participant))
            {
                participants = realloc(participants, (5 + numOfParticipants) * sizeof(Participant));
                if (participants == NULL)
                {
                    printf("CHIEF BUNNY: Oops...Something went wrong while allocating memmory!\n");
                    exit(-1);
                }
            }
        }
    }
    printf("=============================================\n");
    fclose(fp);
}

////////////COMPETITION////////////////////
int startCompetition()
{
    srand(time(NULL));
    printf("CHIEF BUNNY: We are starting competition!\n\n");
    pid_t child_a, child_b;
    
    // for sending length of array of participants
    int lenChild1[2];
    int lenChild2[2];

    // for sending array of participants to supervisors
    int parentToChild1[2];
    int parentToChild2[2];

    // for recieving indexes of participants with max ammount eggs
    int child1ToParent[2];
    int child2ToParent[2];


    if (pipe(parentToChild1) == -1 || pipe(parentToChild2) == -1 || pipe(child1ToParent) == -1 || pipe(child2ToParent) == -1 || pipe(lenChild1) == -1 || pipe(lenChild2) == -1 )
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }

    child_a = fork();

    if (child_a < 0)
    {
        perror("Error creating a process!\n");
        return 1;
    }

    if (child_a > 0)
    {
        // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT
        // pipes for lengths of arrays
        close(lenChild1[0]);
        close(lenChild2[0]);
        
        // pipes for array of participants
        close(parentToChild1[0]);
        close(parentToChild2[0]);

        // pipes to send back max indexes
        close(child1ToParent[1]);
        close(child2ToParent[1]);

        ParticipantShort participantsChild1[numOfParticipants];
        ParticipantShort participantsChild2[numOfParticipants];

        int cntChild1 = 0;
        int cntChild2 = 0;
        
        printf("=============================================\n");
        
        // data for the first child
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < numOfParticipants; j++)
            {
                if (strcmp(participants[j].area, areas[i]) == 0)
                {
                    strcpy(participantsChild1[cntChild1].name, participants[j].name);

                    printf("Supervisor1: %10s\n", participantsChild1[cntChild1].name);
                    cntChild1++;
                }
            }
        }
        // data for the second child
        for (int i = 4; i < 7; i++)
        {
            for (int j = 0; j < numOfParticipants; j++)
            {
                if (strcmp(participants[j].area, areas[i]) == 0)
                {
                    // generate rand number and sent it with the name
                    strcpy(participantsChild2[cntChild2].name, participants[j].name);

                    printf("Supervisor2: %10s\n", participantsChild2[cntChild2].name);
                    cntChild2++;
                }
            }
        }
        
        printf("=============================================\n");
        // Sending array of the participants who are in the first "district" 
        if(write(lenChild1[1], &cntChild1, sizeof(int)) < 0){
            perror("Error sending data!");
            exit(EXIT_FAILURE);
        }else{
            // and then the array itself
            close(lenChild1[1]);
            if(write(parentToChild1[1], participantsChild1, sizeof(ParticipantShort)*cntChild1) < 0){
                perror("Error sending data!");
                exit(EXIT_FAILURE);
            }else{
                printf("List of Participants to be supervised by the FIRST Supervisor has been sent to FIRST Supervisor!\n");
            }
        }

        // Sending array of the participants who are in the second "district" 
        if(write(lenChild2[1], &cntChild2, sizeof(int)) < 0){
            perror("Error sending data!");
            exit(EXIT_FAILURE);
        }else{
            close(lenChild2[1]);
            if(write(parentToChild2[1], participantsChild2,sizeof(ParticipantShort)*cntChild2) < 0){
                perror("Error sending data!");
                exit(EXIT_FAILURE);
            }else{
                printf("List of Participants to be supervised by the SECOND Supervisor has been sent to SECOND Supervisor!\n");
            } 
        }

        close(parentToChild1[1]);
        close(parentToChild2[1]);

        // wait for answers
        ParticipantShort max1, max2;

        sleep(6);
        read(child1ToParent[0], &max1, sizeof(ParticipantShort));
        printf("CHIEF BUNNY recieved participant with maximum eggs from the FIRST Supervisor!\n");
        read(child2ToParent[0], &max2, sizeof(ParticipantShort));
        printf("CHIEF BUNNY recieved participant with maximum eggs from the SECOND Supervisor!\n");
        printf("=============================================\n");

        printf("CHIEF BUNNY: MAX from CHILD_1 is %s with %d eggs was recieved\n", max1.name, max1.eggs);
        printf("CHIEF BUNNY: MAX from CHILD_2 is %s with %d eggs was recieved\n", max2.name, max2.eggs);

        printf("=============================================\n");

        // compare and output
        char winner[128];
        if (max1.eggs != max2.eggs){
            if(max1.eggs >= max2.eggs){
                strcpy(winner, max1.name);
            }else{
                strcpy(winner, max2.name);
            }
            printf("CHIEF BUNNY: Winner of the competition is %s. My congratulations!\n", winner);
        }else{
            printf("CHIEF BUNNY: There are two winners. My congratulations to both!\n", winner);
        }



        // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT // PARENT
    }else{   
        child_b = fork();

        if (child_b < 0)
        {
            perror("Error creating a process!\n");
            return 1;
        }

        if (child_b > 0){
            // CHILD 1 //CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1
            sleep(2);

            // pipes for length
            close(lenChild1[1]);
            close(lenChild2[0]);
            close(lenChild2[1]);

            // pipes for array of participants
            close(child2ToParent[0]);
            close(child2ToParent[1]);
            close(child1ToParent[0]);
            close(parentToChild1[1]);
            close(parentToChild2[0]);
            close(parentToChild2[1]);

            int len;
            read(lenChild1[0], &len, sizeof(int));
            close(lenChild1[0]);

            ParticipantShort participantsChild1[len];
            read(parentToChild1[0], &participantsChild1, len*sizeof(ParticipantShort));
            
            int indMax = -1;

            if(len>0){
                indMax++;
                participantsChild1[indMax].eggs = rand() % 99 + 1;
                printf("%8s -> %2d eggs\n", participantsChild1[indMax].name, participantsChild1[indMax].eggs);
                for(int i=1;i<len;i++){
                    participantsChild1[i].eggs = rand() % 99 + 1;
                    printf("%8s -> %2d eggs\n", participantsChild1[i].name, participantsChild1[i].eggs);
                    if(participantsChild1[i].eggs > participantsChild1[indMax].eggs){
                        indMax = i;
                    }

                }

            }

            // send to parent
            if(write(child1ToParent[1], &participantsChild1[indMax], sizeof(ParticipantShort)) < 0){
                perror("Error sending data!");
                exit(EXIT_FAILURE);
            }else{
                printf("=============================================\n");
                printf("CHILD_1: Max is %s, he has %d eggs!. Has been sent to CHIEF BUNNY\n", participantsChild1[indMax].name, participantsChild1[indMax].eggs);
                printf("=============================================\n");
            }

            // close 
            close(child1ToParent[1]);
            close(parentToChild1[0]);
            exit(1);
            // CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1//CHILD 1
        }
        else if(child_b == 0){
            // CHILD 2 //CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2
            sleep(2);
            // pipes for length
            close(lenChild2[1]);
            close(lenChild1[0]);
            close(lenChild1[1]);

            // pipes for array of participants
            close(child2ToParent[0]);
            close(child1ToParent[0]);
            close(child1ToParent[1]);
            close(parentToChild1[0]);
            close(parentToChild1[1]);
            close(parentToChild2[1]);

            int len2;
            read(lenChild2[0], &len2, sizeof(int));
            close(lenChild2[0]);

            ParticipantShort participantsChild2[len2];
            read(parentToChild2[0], &participantsChild2, len2*sizeof(ParticipantShort));
            
            int indMax = -1;

            if(len2>0){
                indMax++;
                participantsChild2[indMax].eggs = rand() % 99 + 1;
                printf("%8s -> %2d eggs\n", participantsChild2[indMax].name, participantsChild2[indMax].eggs);
                for(int i=1;i<len2;i++){
                    participantsChild2[i].eggs = rand() % 99 + 1;
                    printf("%8s -> %2d eggs\n", participantsChild2[i].name, participantsChild2[i].eggs);
                    if(participantsChild2[i].eggs > participantsChild2[indMax].eggs){
                        indMax = i;
                    }
                }

            }

            // send to parent
            if(write(child2ToParent[1], &participantsChild2[indMax], sizeof(ParticipantShort)) < 0){
                perror("Error sending data!");
                exit(EXIT_FAILURE);
            }else{
                printf("=============================================\n");
                printf("CHILD_2: Max is %s, he has %d eggs! Has been sent to CHIEF BUNNY\n", participantsChild2[indMax].name, participantsChild2[indMax].eggs);
                printf("=============================================\n");
            }

            close(child2ToParent[1]);
            close(parentToChild2[0]);
            
            exit(1);
            return 0;
            // CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2//CHILD 2
        }
    }
    return 0;
}
////////////COMPETITION////////////////////
void easterEgg()
{
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
    printf("\n");
}