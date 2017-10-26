//Bryce Golamco
//bgolamco@sfu.ca

#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include "Assignment1.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//pthread mutexes and conditional variable and their initialization
pthread_mutex_t mutex_input = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_output = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sendin = PTHREAD_COND_INITIALIZER;


//Socket Programming

struct addrinfo in_socket;
struct addrinfo *in_socket_pointer;

struct addrinfo out_socket;
struct addrinfo *out_socket_pointer;

char dns_host_name[256]; //Host's Dns name
char *dns_send_name; //Target Dns name
char *host_port_number; // Host's port number
char *send_port_number; //Target's Port number




int mysocket = 0;
int hissocket = 0;

int waiting_send = 0;
char *input_msg = NULL;
char * input_msg_outside = NULL;
int done = 0;

//The implementation is made up of two lists.
LIST * input; //This list is manipulated by the Recv thread and the print thread
LIST * output; //This list is manipulated by the Send thread, keyboard input thread and the print thread

int read_output = 0;


//Print thread. Only the Print thread is responsible for removing the items off of each list.
void *print_screen(void * ptr){ 
	char * output_to_screen = NULL;
	char * input_to_screen = NULL;
	while(done != 1){
		if(ListCount(output) > 0 && read_output == 1){
			pthread_mutex_lock(&mutex_output); //P(Mutex)
			ListFirst(output);
			output_to_screen = ListRemove(output);
			if(ListCount(output) == 0) read_output = 0;
			pthread_mutex_unlock(&mutex_output); // V(Mutex)
			printf("You: %s\n", output_to_screen);
			free(output_to_screen);
			output_to_screen = NULL;
		}
		if(ListCount(input) > 0){
			pthread_mutex_lock(&mutex_input); //P(mutex)
			ListFirst(input);
			input_to_screen = ListRemove(input);		
			pthread_mutex_unlock(&mutex_input);
			printf("Stranger: %s\n", input_to_screen); //V(mutex)
			free(input_to_screen); //Frees the pointer				
			input_to_screen = NULL;
		}
	}

	return 0;
}

//Recv Thread. This thread will wait for the recieving input.
void *wait_reply(void * ptr){ 
	
	while(done != 1){
		
		input_msg_outside = malloc(sizeof(char[256]));
		recvfrom(mysocket, input_msg_outside, 256, 0, NULL, sizeof(struct sockaddr_in));
		pthread_mutex_lock(&mutex_input);
		ListAppend(input,input_msg_outside);
		pthread_mutex_unlock(&mutex_input);
	}
	return 0;
}

//Send thread. This thread Sends the input of the user from the keyboard. (It pops items from the list, sends them then adds them back onto the list
void *send_reply(void * ptr){
	char * string;
	int iter = 0;

	while(done != 1){
		pthread_mutex_lock(&mutex_output);
		waiting_send = 1;
		pthread_cond_wait(&sendin, &mutex_output);
		if(done == 1){
			pthread_mutex_unlock(&mutex_output);
			return 0;
		}
		
		string = ListRemove(output);
		sendto(mysocket, string, 256,0,out_socket_pointer->ai_addr, sizeof(struct sockaddr_in));
		
		ListPrev(output);
		ListAdd(output,string);		
		
		read_output = 1;
		pthread_mutex_unlock(&mutex_output);
	}	
	return 0;
}

//Keyboard Input Thread. This thread awaits input from the keyboard and pushes them onto a list (list input)
void *wait_input(void * ptr){
	while(done != 1){		
		input_msg = malloc(sizeof(char[256]));
		scanf("\n%256[^\n]", input_msg);
		while(waiting_send == 0);
		if(input_msg[0] == '!' && input_msg[1] == '\0'){ //Quit Condition
			done = 1;
		}
		pthread_mutex_lock(&mutex_output);
		ListLast(output);
		ListAdd(output,input_msg);
		waiting_send = 0;
		pthread_cond_signal(&sendin);
		pthread_mutex_unlock(&mutex_output);
	}
	return 0;
}



int main(int argc, char* argv[]){

	//Socket Initializations
	memset(&in_socket, 0, sizeof(in_socket));
	in_socket.ai_family = AF_INET;
	in_socket.ai_socktype = SOCK_DGRAM;
	in_socket.ai_flags = AI_PASSIVE;

	memset(&out_socket, 0, sizeof(out_socket));
	out_socket.ai_family = AF_INET;
	out_socket.ai_socktype = SOCK_DGRAM;
	out_socket.ai_flags = AI_PASSIVE;

	// Sends an error if the number of  parameters passed does not match.
	if(argc != 4){ 
		printf("Error, wrong number of parameters.\n");
		printf("Recieved %i number of inputs.\n", argc);
		printf("To start, type ./s-talk [my port number] [remote machine name] [remote port number]\n");

		return 1;
	}
	
	host_port_number = argv[1];
	dns_send_name = argv[2];
	send_port_number = argv[3];
	


	if(gethostname(dns_host_name, 256) != 0){
		printf("Error can't find PC's DNS host name\n");
		return 1;
	}

	getaddrinfo(dns_host_name, host_port_number, &in_socket, &in_socket_pointer);

	getaddrinfo(dns_send_name,send_port_number, &out_socket, &out_socket_pointer);
	
	mysocket = socket(AF_INET,SOCK_DGRAM, 0);
	bind(mysocket, in_socket_pointer->ai_addr, in_socket_pointer->ai_addrlen);


	pthread_mutex_init(&mutex_input,NULL);	
	pthread_mutex_init(&mutex_output,NULL);
	pthread_cond_init(&sendin,NULL);
	pthread_t thread[4];
	int output_array[4];
	int i = 0;
	int *x = 0;
	
	input = ListCreate();
	output = ListCreate();
	
	printf("Maximum characters of 256 per line\n");

	//All 4 threads are created
	output_array[0] = pthread_create(&thread[0],NULL,print_screen, (void*)x);
	output_array[1] = pthread_create(&thread[1],NULL,wait_reply, (void*)x);
	output_array[2] = pthread_create(&thread[2],NULL,send_reply, (void*)x);
	output_array[3] = pthread_create(&thread[3],NULL,wait_input, (void*)x);

	while(done != 1);
	pthread_cancel(thread[1]); //Force closes the Recv Thread, since it continuously asks for input
	for(i = 0; i < 4; i++){
		if(i == 1) continue; // skips the thread[1] (the waiting reply thread since we already cancelled its thread earlier)
		pthread_join(thread[i],NULL);
	}
	

	printf("Thread 1 = Screen printing thread\n");
	printf("Thread 2 = Reply waiting thread\n");
	printf("Thread 3 = Reply sending thread\n");
	printf("Thread 4 = Keyboard input thread\n\n");	

	for(i = 0; i < 4; i++){
		printf("Thread %i returns %d\n", i+1, output_array[i]);
	}

	//Destroys all the mutex and conditional variables
	pthread_cond_destroy(&sendin);
	pthread_mutex_destroy(&mutex_input);
	pthread_mutex_destroy(&mutex_output);

	return 0;
}










