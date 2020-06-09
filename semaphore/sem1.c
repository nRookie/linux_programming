#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include<sys/types.h>
#include<sys/ipc.h>

#include<sys/sem.h>
#include "semun.h"



//union semun {
//	int val;
//	struct semid_ds *buf;
//	unsigned short *array;
//};
static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main(int argc, char *argv[])
{
	int i;
	int pause_time;
	char op_char = 'O';

	srand((unsigned int) getpid());

	sem_id = semget((key_t)1234,1,0666|IPC_CREAT);
	if(argc>1){
		if(!set_semvalue()){
			fprintf(stderr,"Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
	op_char = 'X';
	sleep(2);
	}
	/* Then you have a loop that enters and leaves the critical section 10 times. There you first make a call to *semaphore_p*, which sets the semaphore to wait as this program is about to enter the critical section:*/


	for(i =0 ; i< 10; i++)
	{
		if(!semaphore_p()) exit(EXIT_FAILURE);
		printf("%c",op_char); fflush(stdout);
		pause_time = rand() %3;
		sleep(pause_time);
		printf("%c",op_char);fflush(stdout);
	


	/* After the critical section,you call semaphore_v,setting the semaphore as available, before going through the for loop again after a random wait. After the loop, the call to *del_semvalue is made to clean up the code*
	 */

		if(!semaphore_v()) exit(EXIT_FAILURE);
	
		pause_time = rand()%2;

		sleep(pause_time);
	}

	printf("\n%d - finished\n", getpid());

	if(argc >1){
		sleep(10);
		del_semvalue();
	}

	exit(EXIT_SUCCESS);


	
}


/* The function set_semvalue initializes the semaphore using the SETVAL command in a semctl call. You need to do this before you can use the semaphore:
 */


static int set_semvalue(void)
{
	union semun sem_union;
	sem_union.val = 1;

	if(semctl(sem_id,0,SETVAL,sem_union) == -1) return(0);
	return(1);
}

/* The del_semvalue function has almost the same form, except that the call to 
 * semctl uses the command IPC_RMID to remove the semaphore's ID:
 */

static void del_semvalue(void)
{
	union semun sem_union;

	if(semctl(sem_id, 0, IPC_RMID,sem_union) == -1)
		fprintf(stderr,"Failed to delete semaphore\n");
}


/* 6 semaphore_p changes the semaphore by -1. This is the "wait" operation:
 */


static int semaphore_p(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1){
		fprintf(stderr,"semaphore_p failed \n");
		return(0);
	}
	return(1);
}



/* semaphore_v is similar except for setting the sem_op part of the sembuf struct to 1. This is the "release" operation, so that the semaphore beomes available */


static int semaphore_v(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */

	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1)== -1)
	{
		fprintf(stderr,"semaphore_v failed \n");
		return(0);
	}
	return(1);
}

/* How it works 
 *
 * The program starts by obtaining a semaphore identity from the (arbitrary ) that you've chosen using the *semget* function. The *IPC_CREAT* flag causes the semaphore to be created if one is required. If the program has a parameter,it's responsible for initializing the semaphore, which it does with the function set_semvalue, a simplified interface to the more general *semctl* function. It also uses the presence of the parameter to determine which character it should print out. The *sleep* simply allows you some time to invoke other copies of the program before this copy gets to execute too many times around its loop. You use srand and rand to introduce some pseudo-random timing into the program.
 *
 * The program then loops 10 times , with pseudo-random waits in its critical and noncritical sections. The critical section is guarded by calls to your semaphore_p and semaphore_v functions, which are simplified  interfaces to the more general semop function.
 *
 * Before it deletes the semaphore, the program taht was invoked with a parameter then waits to allow other invocations to complete. If the semaphore isn't deleted , it will continue to exist in the system even though no programs are using it . In real programs, it's very important to ensure you don't unintentionally leave semaphores around after execution. It may cause problems next time you run the program, and semaphores are a limited resource that you must conserve.
 */


