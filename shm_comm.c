#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int shmid;
    char *shared_memory;
    pid_t pid;
    
    // Create shared memory segment
    shmid = shmget(IPC_PRIVATE, 100, 0666 | IPC_CREAT);
    printf("Shared Memory ID: %d\n", shmid);
    
    // Attach shared memory
    shared_memory = shmat(shmid, NULL, 0);
    
    pid = fork();
    
    if(pid == 0) {
        // Child process
        printf("Child reading from shared memory...\n");
        printf("Child received: %s\n", shared_memory);
        shmdt(shared_memory);
        exit(0);
    }
    else {
        // Parent process
        printf("Parent writing to shared memory...\n");
        strcpy(shared_memory, "Hello from Parent!");
        printf("Parent wrote: Hello from Parent!\n");
        
        wait(NULL);
        shmdt(shared_memory);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Shared memory removed\n");
    }
    
    return 0;
}
