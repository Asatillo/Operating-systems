#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork
#include <sys/wait.h>  //waitpid
#include <sys/types.h> //open
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h> 
#include <time.h>
#include <sys/stat.h>
#include <sys/mman.h>   
#include <fcntl.h> //open,creat

#define TEXTBUFFER 1024
#define NUMBUFFER 24

pid_t Tutajos, Butyok;
int Butyok_to_Uncle[2];
int Tutajos_to_uncle[2];
int Uncle_to_boys[2];
int UncleComforts[2];
int Butyok_to_tutajos[2];
int fishRead[2];
int returnStatus;

int sh_mem_id;
int *shared_mem;
int sem_Mutex, sem_Mutex2;

struct messg { 
    long mtype;//this is a free value e.g for the address of the message
    char getWood[TEXTBUFFER];
}; 

struct messg2 {
    long ptype;
    char getFish[TEXTBUFFER];
};

int sendwood(int mqueue)
{ 
    const struct messg m = {5, "Collect 2 bundles of wood\n"};   
    int st;
    st = msgsnd(mqueue, &m, strlen(m.getWood) + 1, 0);
    if (st<0) perror("msgsnd error!");
    return 0; 
} 

int sendfish(int mqueue)
{
    const struct messg2 p = {4, "Catch 3 fish\n"};
    int st2;
    st2 = msgsnd(mqueue, &p, strlen(p.getFish) + 1, 0);
    if (st2<0) perror("msgsnd2 error!");
    return 0; 
}
int receive(int mqueue)
{
    struct messg m;
    int st;
    st = msgrcv(mqueue, &m, TEXTBUFFER, 5, 0);
    if (st<0) perror("msgsnd error!");
    else printf("%s\n", m.getWood);
    return 0;
}
int receivefish(int mqueue)
{
    struct messg2 p;
    int st2;
    st2 = msgrcv(mqueue, &p, TEXTBUFFER, 4, 0);
    if (st2<0) perror("msgsnd2 error!");
    else printf("%s\n", p.getFish);
    return 0;
}


int semaphore_create(const char* pathname,int semaphore_value){
    int semid;
    key_t key;
    
    key=ftok(pathname,1);    
    if((semid=semget(key,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
        perror("semget");

    if(semctl(semid,0,SETVAL,semaphore_value)<0)
        perror("semctl");
    return semid;
}


void semaphore_operation(int semid, int op){
    struct sembuf operation;
    
    operation.sem_num = 0;
    operation.sem_op  = op; // op=1 up, op=-1 down 
    operation.sem_flg = 0;

    if(semop(semid,&operation,1)<0)
        perror("semop");	    
}

void semaphore_delete(int semid){
      semctl(semid,0,IPC_RMID);
}

void handle_Tutajos(int signum)
{
    printf("\n-Tutajos is ready!\n");
    return;
}
void handle_Butyok(int signum)
{
    printf("\n-Butyok is ready, Task may begin!\n");
    return;
}

int main(int argc, char* argv[])
{
    signal(SIGUSR1, handle_Tutajos);
    signal(SIGUSR2, handle_Butyok);

    int messg;
    key_t key = ftok(argv[0], 1);
    key_t m_key = ftok(argv[0], 1);

    sem_Mutex = semaphore_create(argv[0],1);

    messg = msgget(key, 0600 | IPC_CREAT);
    if (messg < 0)
    {
        perror("msgget error");
        return 1;
    }
    srand(time(NULL));
    pipe(Butyok_to_Uncle);
    pipe(Tutajos_to_uncle);
    pipe(Uncle_to_boys);
    pipe(UncleComforts);
    pipe(Butyok_to_tutajos);
    pipe(fishRead);

    ((Tutajos = fork()) && (Butyok = fork()));

    if (Tutajos == 0) // child
    {
        sleep(1);
        kill(getppid(), SIGUSR1);
        receive(messg);
        close(Tutajos_to_uncle[0]);
        close(UncleComforts[1]);
        close(Butyok_to_tutajos[1]);
        close(fishRead[0]);

        int caughtFish = (rand() % 2) + 1;
        char shoutTur[TEXTBUFFER] = "The pike caught stop, help stop!\n"; 
        write(Tutajos_to_uncle[1], shoutTur, TEXTBUFFER);

        char Unclesays[TEXTBUFFER] = "";
        read(UncleComforts[0], Unclesays, TEXTBUFFER);
        printf("Uncle says to Tutajos: %s\n", Unclesays);

        int totalfish = 0;
        read(Butyok_to_tutajos[0], &totalfish, sizeof(int));

        int sendfish = ((caughtFish+totalfish) - 1);
        write(fishRead[1], &sendfish, sizeof(int));

    }
    else if (Butyok == 0) // child
    {
        sleep(2);
        kill(getppid(), SIGUSR2);
        receivefish(messg);
        sleep(1);
        close(Butyok_to_Uncle[0]);
        close(Uncle_to_boys[1]);
        close(Butyok_to_tutajos[0]);

        char shout[TEXTBUFFER] = "The wood is wet!\n";
        write(Butyok_to_Uncle[1], shout, TEXTBUFFER);

        char Unclesays[TEXTBUFFER] = "";
        read(Uncle_to_boys[0], Unclesays, TEXTBUFFER);
        printf("Uncle: %s\n", Unclesays);
        int fish = (rand() % 2) + 1;
        write(Butyok_to_tutajos[1], &fish, sizeof(int));

    }
    else //parent
    {
        pause(); // wait      
        pause();
        printf("\n\t___________Task Starts____________\n\n");
        char getWood[] = "Collect 2 bundles of wood!\n";
        char getFish[] = "Catch 3 fish\n";
        sendwood (messg);
        sendfish(messg);
        close(Butyok_to_Uncle[1]);
        close(Tutajos_to_uncle[1]);
        close(Uncle_to_boys[0]);
        close(UncleComforts[0]);
        close(fishRead[1]);

        char Butyoksays[TEXTBUFFER] = "";
        read(Butyok_to_Uncle[0], Butyoksays, TEXTBUFFER);
        printf("Butyok: %s\n", Butyoksays);

        char Tutajossays[TEXTBUFFER] = "";
        read(Tutajos_to_uncle[0], Tutajossays, TEXTBUFFER);
        printf("Tutajos: %s\n", Tutajossays);

        char Unclesays[TEXTBUFFER] = "Leave the wet wood to someone else, help the fisherman!\n";
        write(Uncle_to_boys[1], Unclesays, TEXTBUFFER);
        
        char Unclecomforted[TEXTBUFFER]= "since the pike didn't manage to eat you, I'm glad you're safe.\n";
        write(UncleComforts[1], Unclecomforted, TEXTBUFFER);

        // check:
        // int receivefishcnt = 0;
        // read(fishRead[0], &receivefishcnt, sizeof(int));
        // printf("Uncle: %d\n", receivefishcnt);

        int receivefishcnt = read(fishRead[0], &receivefishcnt, sizeof(int));
        semaphore_operation(sem_Mutex,-1); // Up
        FILE*fp;
        fp = fopen("fish.txt", "a");
        if (fp == NULL){
            perror("Error in opening the file, premission denied!");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "Total fish caught: %d\n", receivefishcnt-1);
        fclose(fp);

        semaphore_operation(sem_Mutex,1); // Up


        semaphore_delete(sem_Mutex);

        waitpid(Butyok, &returnStatus, 0);
        waitpid(Tutajos, &returnStatus, 0);
    }
    return 0;
}