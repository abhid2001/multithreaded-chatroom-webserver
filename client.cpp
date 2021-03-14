#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

using namespace std;
#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define NAME_LEN 32

volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[NAME_LEN];

void str_overwrite_stdout()
{
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
    { // trim \n
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

void catch_ctrl_c_and_exit(int sig){
    flag = 1;
}

void *recieve_msg_handler(void *arg){
    char message[BUFFER_SZ]={};
    while(1){
        int recieve = recv(sockfd, message, BUFFER_SZ, 0);
        if(recieve >0){
            printf("%s ", message);
            str_overwrite_stdout();
        }else if(recieve == 0){
            break; 
        }
        bzero(message, BUFFER_SZ);
    }
    return NULL;
}
void *send_msg_handler(void *arg){
    char buffer[BUFFER_SZ] = {};
    char message[BUFFER_SZ + NAME_LEN]={};
    while(1){
        str_overwrite_stdout();
        fgets(buffer, BUFFER_SZ, stdin);
        str_trim_lf(buffer, BUFFER_SZ);
        if(strcmp(buffer, "exit")==0){
            break;
        }else{
            sprintf(message, "%s: %s\n", name, buffer);
            send(sockfd, message, strlen(message), 0);
        }
        bzero(buffer, BUFFER_SZ);
        bzero(message, BUFFER_SZ + NAME_LEN);
    }
    catch_ctrl_c_and_exit(2);
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    signal(SIGINT, catch_ctrl_c_and_exit);
    printf("Enter your name: ");
    fgets(name, NAME_LEN, stdin);
    str_trim_lf(name, strlen(name));
    if(strlen(name)>NAME_LEN-1 || strlen(name)<2){
        printf("The name should be of length 2 to 32\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_in server_addr;
        /* Socket settings */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    
    //connect with the server
    int err = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        printf("ERROR: connect\n");
		return EXIT_FAILURE;
    }

    //send name 
    send(sockfd, name, NAME_LEN, 0);
    printf("=== WELCOME TO THE CHATROOM ===\n");
    pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, &send_msg_handler, NULL)!=0){
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }
    pthread_t recieve_msg_thread;
    if(pthread_create(&recieve_msg_thread, NULL, &recieve_msg_handler, NULL) != 0){
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }
    while(1){
        if(flag){
            printf("\n Have a nice day :) \n");
            break;
        }
    }
    close(sockfd);
    return EXIT_SUCCESS;
}