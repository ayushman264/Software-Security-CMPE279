// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#define PORT 80 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    char fd_arr[3];

    if(argv[0][2]==26){
	valread = read( argv[0][1] , buffer, 1024); 
    	printf("%s\n",buffer ); 
    	send(argv[0][1] , hello , strlen(hello) , 0 ); 
    	printf("Hello message sent\n");
	return 0; 
    }
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

   int newproc=fork();
    //child process
    if(newproc==0){
	//setuid to nobody user
	int uid=setuid(65534);
	printf("Child process id: %d", uid);
	fd_arr[0]=new_socket;
	fd_arr[1]=new_socket;
	fd_arr[2]=26; //to compare and verify child process
	execl(argv[0], fd_arr, NULL);
    }
    //parent process
    else if(newproc>0){
	printf("Parent waiting, Process id: %d\n",newproc);
	wait(NULL);
    }
    else{
	printf("Fork failed");
    }
    return 0; 
} 
