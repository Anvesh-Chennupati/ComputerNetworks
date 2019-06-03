/*

 *  client.c
 *
 *  Created on: 27-May-2019
 *  Author: Anvesh Chennupati
 *  Stud ID: 00001538032
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>

// contains user defined methods
#include "header_file.h"

// sockets
int socket_client;
struct sockaddr_in server_address;
struct sockaddr_storage incoming;
socklen_t addr_size = 0;

//timer
#define TIMEOUT 3

int main(int argc, char**argv) {    
// creating socket
    socket_client = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_client < 0) {
        perror("cannot create socket\n");
        return 0;
    }
    else {
        printf("\n\nSocket created. Reading subsribers information from input.txt\n\n");
    }

    // clearning server variables
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7822);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));
    addr_size = sizeof(incoming);
    

    //timer
    struct timeval tv;
	tv.tv_sec = TIMEOUT;  /* 3 Secs Timeout */
	tv.tv_usec = 0;
	setsockopt(socket_client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));

    // reading input file
    char line[30];
	int i = 0;
    int n =0;
	FILE *fp;

    fp = fopen("input.txt", "rt");
	if(fp == NULL)
	{
		printf("cannot open file\n");
	}

    // creating a request packet
    request_packet request = initializeRequestPacket();
    response_packet response;
    // reads input.txt which contains all the subsribers who requires authentication
	while(fgets(line, sizeof(line), fp) != NULL) {
        int counter =0;
		n = 0;
        int bytes = 0;
		printf("\n%d.Subscriber: ",i+1);
		char * words;
		// splitting the line in to words
		words = strtok(line," ");
		request.length = strlen(words);
        printf("%s",words);
		request.SourceSubscriberNo = (uint64_t) atoi(words);
		words = strtok(NULL," ");
        printf("\tTechnology %s\n",words);
		request.length += strlen(words);
		request.technology = atoi(words);
        request.segment_No = i+1;
		words = strtok(NULL," ");
        i++;
        printf("Request packet\n");
        printRequestPacket(request);
        printf("\nSending data to server for user authentication.\n");

        while(bytes <= 0 && counter < 3) {
            sendto(socket_client,&request,sizeof(request_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
            bytes = recvfrom(socket_client,&response,sizeof(response_packet),0,NULL,NULL);
            if(bytes <= 0 ) {
				// If no response recieved from server
				printf("Took more than three seconds to receive response from server.Trying again......\n");
				counter ++;
			}
            else if(bytes > 0){
                // if subsriber hasn't paid
                printf("\nRecieved response from server.\n");
                printf("\nStatus:");
				if(response.type == NOT_PAID) {
					printf("\tSUBSCRIBER HAS NOT PAID\n");
				}
                // if subsriber doesn't exist
				else if(response.type == NOT_EXIST ) {
					printf("\tSUBSCRIBER DOES NOT EXIST\n");
				}
                // successful case 
				else if(response.type == PAID) {
					printf("\tPERMITTED TO ACCESS THE NETWORK\n");
				}
            }
        }
        if(counter >= 3 ) {
			printf("Server is not responding. Please check......\n");
			exit(0);
		} 
        
        // printResponsePacket(response);
    }
    return 0;
}