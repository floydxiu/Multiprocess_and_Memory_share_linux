#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <unistd.h>

struct mem_shared{
	int dec_num;
	char bin_num[200];
	int flag; //checked who is using the memory
};

typedef struct mem_shared m_sh;

int main(){
	const int SIZE = sizeof(m_sh);
	m_sh *shared_memory;
	int segment_id;
	segment_id = shmget(IPC_PRIVATE, SIZE, S_IRUSR | S_IWUSR);
	shared_memory = (m_sh *) shmat(segment_id, NULL, 0);
	shared_memory->flag = 0;
	shared_memory->dec_num = 0;
	char s[10];
	int switcher = 1;
	sprintf(s, "%d", segment_id);
	pid_t pid;

	pid = fork();

	if(pid < 0){
		printf("boom");
	}
	else if (pid == 0) { 
		while(1){
			if(shared_memory->flag == 0){
				printf("Please input a number:");
	    		scanf("%d", &shared_memory->dec_num);
	    		shared_memory->flag = 1;
			}
		}
    }
    else{
    	execlp("./b.out", s, (char *)0);
    }

    /* now detach the shared memory segment */
    shmdt(shared_memory);
 
    /* now remove the shared memory segment */
   	shmctl(segment_id, IPC_RMID, NULL);

    return 0;
}