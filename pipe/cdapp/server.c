#include "cd_data.h"
#include "cliserv.h"


#include<sys/msg.h>


#define SERVER_MQUEUE 1234
#define CLIENT_MQUEUE 4321


struct msg_passed{
	long int msg_key; /*used for client pid */
	message_db_t real_message;
};


static int cli_qid =-1;







int send_resp_to_client(const message_db_t mess_to_send)
{
	struct msg_passed my_msg;
#if DEBUG_TRACE
	printf("%d :- send_resp_to_client()\n", getpid());
#endif

	my_msg.real_message = mess_to_send;
	my_msg.msg_key = mess_to_send.client_pid;


	if(msgsnd(cli_qid,(void *)&my_msg, sizeof(mess_to_send),0)== -1){
		return(0);
	}
	return(1);
}


