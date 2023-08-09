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

/*
1.  Once the Chief Bunny has received the signals, he forwards to both Counting Commissioners(Tapsi and Hapsi) the selected area name to be surveyed by message queue. Both Tapsi and Hapsi read the name of the area from message queue and write it  to the screen. Then they finish their activity for the day (child processes exit), which is waited by Chief Bunny, who writes on the screen that both Tapsi and Hapsi are going to rest, and Chief Bunny also exit. 

2. The results of bonny counting (random numbers between 50 and 100) are also sent back to Chief Bunny via a message queue, who writes them on the screen.  Chief Bunny has the results of the previous bunny count (random number between 50 and 100 for each area), so he immediately compares whether there has been a bunny loss or gain in the area, and writes the results on the screen(e.g. There is a bonny loss in "North".)

3. If there has been a bunny loss in the area, Chief Bunny sends a "Search_more" signal, otherwise he sends a "Home" signal to Tapsi and Hapsi. If Tapsi or Hapsi receives a "Home" signal, they stop their activity, if they receive a "Search_more" signal, they go around the area again and send back the result of the new count (random number between 70 and 100) on a message queue as well, which Chief Bunny writes to the screen

4. Chief Bunny writes the results of the two area samples to both the shared memory and the screen, and uses them to determine whether there has been a population decrease or increase in the bunny count.  Protect writes to shared memory with semaphore.
*/

#define TEXTBUFFER 1024
#define WORDBUFFER 128
char areas[4][WORDBUFFER] = {"North", "East", "South", "West"};

// structure for message queue
struct messg { 
    long mtype;//this is a free value e.g for the address of the message
    char area[WORDBUFFER];
}; 

struct messg2 {
    long ptype;
    char area[WORDBUFFER];
};

int send_to_Tapsi(int mqueue, char area[WORDBUFFER])
{ 
    struct messg m;
    m.mtype = 6;
    strcpy(m.area, area);
    int st;
    // printf("%s\n", m.area);
    st = msgsnd(mqueue, &m, strlen(m.area)+1, 0);
    if (st<0){
        perror("msgsnd error!");
    } 
    return 0; 
} 

int send_to_Hapsi(int mqueue, char area[WORDBUFFER])
{
     struct messg2 p;
     p.ptype = 7;
     strcpy(area, p.area);
    int st2;
    st2 = msgsnd(mqueue, &p, strlen(p.area)+1, 0);
    if (st2<0){
        perror("msgsnd2 error!");
    }
    return 0; 
}

int receive_Tapsi(int mqueue)
{
    struct messg m;
    int st;
    st = msgrcv(mqueue, &m, WORDBUFFER, 6, 0);
    if (st<0) perror("msgsnd error!");
    else printf("Tapsi: Recieved area %s\n", m.area);
    return 0;
}

int receive_Hapsi(int mqueue)
{
    struct messg2 p;
    int st2;
    st2 = msgrcv(mqueue, &p, WORDBUFFER, 7, 0);
    if (st2<0) perror("msgsnd2 error!");
    else printf("Hapsi: Recieved area %s\n", p.area);
    return 0;
}
// define pipes here if needed

// signal handlers
void handle_Tapsi(int signum)
{
    printf("\n-Tapsi is ready!\n");
    return;
}
void handle_Hapsi(int signum)
{
    printf("\n-Hapsi is ready!\n");
    return;
}

int main(int argc, char const *argv[])
{
    signal(SIGUSR1, handle_Tapsi);
    signal(SIGUSR2, handle_Hapsi);

    key_t key = ftok(argv[0], 1);

    int messg;
    messg = msgget(key, 0600 | IPC_CREAT);
    if(messg < 0){
        perror("msgget error");
        return 1;
    }

    pid_t Tapsi, Hapsi;
    ((Tapsi = fork()) && (Hapsi = fork()));
    if (Tapsi < 0 || Hapsi < 0)
    {
        printf("Error forking the processes!\n");
    }

    if (Tapsi == 0)
    {
        // Tapsi
        sleep(2);
        kill(getppid(), SIGUSR1);
        sleep(1);
        receive_Tapsi(messg);
        exit(1);
    }
    else if (Hapsi == 0)
    {
        //Hapsi
        sleep(2);
        kill(getppid(), SIGUSR2);
        sleep(1);
        receive_Hapsi(messg);
        exit(1);
    }
    else{
        // parent
        pause(); 
        pause();

        // char first_area[WORDBUFFER];
        // strcpy(first_area, areas[rand()%2]);

        // char second_area[WORDBUFFER];
        // strcpy(second_area, areas[2 + rand()%2]);
        // printf("%s , %s \n", areas[rand()%2], areas[2+rand()%2]);
        
        send_to_Tapsi(messg, areas[rand()%2]);
        send_to_Hapsi(messg, areas[2+rand()%2]);
        
        sleep(3);
        printf("Tapsi and Hapsi are going to rest\n");
    }
    return 0;
}
