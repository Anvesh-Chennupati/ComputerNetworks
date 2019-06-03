/*

 *  server.c
 *
 *  Created on: 27-May-2019
 *  Author: Anvesh Chennupati
 *  Stud ID: 00001538032
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "header_file.h"
#define dbCount 40 //number of entries in database

// Socket initialization.
int socket_connection;
struct sockaddr_in server_address;
int nBytes = 0;
socklen_t addr_size = 0;
request_packet req;
response_packet response;
struct sockaddr_in incoming;

//method to read data from verification_database and put it in memory
void readDatabase(subscribers map[]){
    char line[30];
	int i = 0;
	FILE *fp;
	fp = fopen("Verification_Database.txt", "rt");
	if(fp == NULL)
	{
		printf("cannot open file\n");
		return;
	}
	while(fgets(line, sizeof(line), fp) != NULL)
	{
		char * words;
		words = strtok(line," ");
		map[i].subscriberNumber =(unsigned) atol(words);
		words = strtok(NULL," ");
		map[i].technology = atoi(words);
		words = strtok(NULL," ");
		map[i].status = atoi(words);
		i++;
	}
	fclose(fp);
    printf("\nReading Verification_Database.txt finished.\n%d Entries read from database.\nWaiting for clients connection.\n",i);
}

// to search database for subsriber information.
// returns -1 if subscriber doesn't exist
// returns status of subsricber 0/1 if exists
int searchDatabase(subscribers map[],unsigned int subscriberNumber,uint8_t technology) {
    int value = -1;
    int j = 0;
	for(j = 0; j < dbCount;j++) {
		if(map[j].subscriberNumber == subscriberNumber && map[j].technology == technology) {
			return map[j].status;
		}
	}
	return value;
}

int main(){
	// stores the database enteries in memory
    subscribers map[dbCount];
	readDatabase(map);

	// creating a socket
    if((socket_connection = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
        perror("Unable to create socket");
        return 0;
    }
    
    // clearing the server variables
    memset(&server_address,0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7822);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_address.sin_zero, '\0',sizeof(server_address.sin_zero));

  if (bind(socket_connection, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        return 0;
}   
    addr_size = sizeof(incoming);
    printf("\n ~~~~~~~~~~~~~~~~~~~~~~~Server Started~~~~~~~~~~~~~~~~~~~~~~~");

while(1){
	// receiving data from client
    int bytes = recvfrom(socket_connection, &req, sizeof(request_packet), 0,(struct sockaddr* )&incoming, &addr_size);
    if(bytes < 0){
        printf("\n Error recieving data\n");
        return 1;
    }
    printf("\nReceived authentication request from client.\n");
    printRequestPacket(req);
    if(req.Acc_Per == 0XFFF8){
        response = initializeResponsePacket(req);
        int status = searchDatabase(map,req.SourceSubscriberNo,req.technology);
        if(status == 0) {
				// not paid
				response.type = NOT_PAID;
				printf("\nSubscriber has not paid\n");
			}
			else if(status == 1) {
				// paid
				printf("\nSubscriber has paid.\n");
				response.type = PAID;
			}

			else if(status == -1) {
				// does not exist
				printf("\nSubscriber does not exist\n");
				response.type = NOT_EXIST;
			}
			// sending the response packet
            int err = sendto(socket_connection, &response,sizeof(response_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                }
    }
    // printDataPacket(data);
    // int status = processRecievedPacket(data);
}
    return 0;
}
