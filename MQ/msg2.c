#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include<sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st{
	long int my_msg_type;
	char some_text[MAX_TEXT];
};


int main()
{
	int running = 1;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];

	msgid = msgget((key_t) 1234, 0666 | IPC_CREAT);

	if(msgid == -1){
		fprintf(stderr,"msgget failed with error: %d\n",errno);
		exit(EXIT_FAILURE);
	}


	while(running){
		printf("Enter some text:");
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 1;
		strcpy(some_data.some_text, buffer);
	
		if(msgsnd(msgid,(void *)&some_data,MAX_TEXT,0) == -1){
			fprintf(stderr,"msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

		if(strncmp(buffer,"end",3)==0){
			running = 0;
		}

	}

	exit(EXIT_SUCCESS);
}

/* Unlike in the pipes example, there's no need for the processes to provide hteir own synchronization method. This is significant advantage of messages over pipes.
 *
 * Poviding there's room in the message queue, the sender can create the queue, put some data into the queue, and then exit before the receiver even starts. You'll run the sender ,*msg2* ,first, Here's some sample output:
 */
