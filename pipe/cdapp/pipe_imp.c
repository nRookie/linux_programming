#include "cd_data.h"
#include "cliserv.h"
#include <errno.h>
#include <sys/msg.h>

static int server_fd = -1;
static pid_t mypid = 0;
static char client_pipe_name[PATH_MAX +1] = {'\0'};
static int client_fd = -1;
static int client_write_fd = -1;

static int serv_qid = -1;
static int cli_qid = -1;

#define SERVER_MQUEUE  1234
#define CLIENT_MQUEUE  4321
int server_starting(void)
{
	#if DEBUG_TRACE
		printf("%d :-server_starting()\n", getpid());
	#endif

		unlink(SERVER_PIPE);
	if (mkfifo(SERVER_PIPE,0777) == -1){
		fprintf(stderr,"Server startup error, no FIFO created\n");
		return(0);
	}

	if((server_fd = open(SERVER_PIPE,O_RDONLY)) == -1){
		if(errno ==EINTR) return(0);
		fprintf(stderr,"Server startup error, no FIFO opened \n");
		return(0);
	}
	return(1);
}


void server_ending(void)
{
#if DEBUG_TRACE
	printf("%d :- server_ending()\n", getpid());
#endif 

	(void)close(server_fd);
	(void)unlink(SERVER_PIPE);
}


int client_starting()
{
#if DEBUG_TRACE
	printf("%d:-client_Starting\n", getpid());
#endif 

	serv_qid = msgget((key_t)SERVER_MQUEUE,0666);
	if(serv_qid == -1) return(0);

	cli_qid = msgget((key_t)CLIENT_MQUEUE,0666);

	if(cli_qid == -1) return(0);
	return(1);
}

int read_request_from_client(message_db_t *rec_ptr)
{
	int return_code = 0;
	int read_bytes;

	#if DEBUG_TRACE
		printf("%d :- read_request_from_client() \n", getpid());
	#endif 
	
		if(server_fd !=-1){
			read_bytes = read(server_fd,rec_ptr, sizeof(*rec_ptr));
		}

		if(read_bytes ==0){
			(void)close(server_fd);
			if((server_fd = open(SERVER_PIPE,O_RDONLY)) == -1){
				if(errno !=EINTR){
					fprintf(stderr,"Server errno, FIFO open failed \n");
				}
				return (0);
			}
			read_bytes = read(server_fd,rec_ptr,sizeof(*rec_ptr));
		}
		if(read_bytes == sizeof(*rec_ptr)) return_code = 1;
		return(return_code);
}


void client_ending()
{
#if DEBUG_TRACE
	printf("%d :- client_ending()\n", getpid());
#endif

	serv_qid = -1;
	cli_qid = -1;
}


int send_mess_to_server(message_db_t mess_to_send)
{
	int write_bytes;
#if DEBUG_TRACE
	printf("%d :-send_mess_to_server()\n", getpid());
#endif

if(server_fd == -1) return(0);

mess_to_send.client_pid = mypid;
write_bytes = write(server_fd,&mess_to_send,sizeof(mess_to_send));
if(write_bytes != sizeof(mess_to_send)) return(0);
return(1);
}



int start_resp_to_client(const message_db_t mess_to_send)
{
	return (1);
}

void end_resp_to_client(void)
{
}

int start_resp_from_server(void)
{
	return(1);
}

void end_resp_from_server(void)
{
}



int database_initialize(const int new_database)
{
if(!client_starting()) return(0);

mypid = getpid();
return(1);


} /*database_initialize*/
