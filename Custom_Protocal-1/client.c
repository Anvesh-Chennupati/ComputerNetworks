/*

 *  client.c
 *
 *  Created on: 27-May-2019
 *  Author: Anvesh Chennupati
 *  Stud ID: 00001538032
 */

// imports
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>

#include "processor.h"

// sockets
int socket_client;
struct sockaddr_in server_address;
struct sockaddr_storage incoming;
socklen_t addr_size = 0;

//timer
#define TIMEOUT 3
int main() {

// creating socket
// create socket
    socket_client = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_client < 0) {
        perror("cannot create socket\n");
        return 0;
    }
    else {
        printf("Socket created\n");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7822);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));
    addr_size = sizeof(incoming);
    
    // creating an array of payloads
    uint8_t *allPayloads[5] = {payload1,payload2,payload3,payload4,payload5};

    //timer
    struct timeval tv;
	tv.tv_sec = TIMEOUT;  /* 3 Secs Timeout */
	tv.tv_usec = 0;
	setsockopt(socket_client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));

    while(1){
        int choice;
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~Choose one of the options from below~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("\n1. Send Packets");
        printf("\n2. Simulate Out of Sequence error");
        printf("\n3. Simulate Length Mismatch error");
        printf("\n4. Simulate Missing End of Packet Identifier error");
        printf("\n5. Simulate Duplicate Packet error");
        printf("\n6. All errors");
        printf("\n7. Timer case");
        printf("\n8. Exit\n");
        scanf("%d",&choice);

        switch(choice){
            case 1:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Sending 5 packets to server with no error~~~~~~~~~~~~~~~~~~~~~~~");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                for(i = 0; i<5;i++){
                    dp = createDataPacket(i+1,3,allPayloads[i]);
                    printDataPacket(dp);
                    sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                    bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                    if(bytes <0){
                        printf("\nResponse from server not received\n");
                    }
                    else printAckPacket(ack);
                }

            } break;
            
            case 2:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Simulating Out of Sequence error~~~~~~~~~~~~~~~~~~~~~~~");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                reject_packet rej;

                //correct packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //out of sequence error
                dp = createDataPacket(3,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //out of sequence error
                dp = createDataPacket(4,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //out of sequence error
                dp = createDataPacket(5,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //correct packet
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

            } break;

            case 3:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Simulating Length mismatch error~~~~~~~~~~~~~~~~~~~~~~~");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                reject_packet rej;

                //correct packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //correct packet
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //length mismatch error
                dp = createDataPacket(3,3,allPayloads[i]);
                dp.length = dp.length+1;
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //length mismatch error
                dp = createDataPacket(3,3,allPayloads[i]);
                dp.length = dp.length+1;
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //correct packet
                dp = createDataPacket(3,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);
            } break;

            case 4:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Simulating Missing End of Packet Identifier~~~~~~~~~~~~~~~~~~~~~~~");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                reject_packet rej;

                //correct packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //Missing end of packet error
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                dp.endOfPacket = 0;
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //Missing end of packet error
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                dp.endOfPacket = 0;
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //Missing end of packet error
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                dp.endOfPacket = 0;
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //correct packet
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);
            } break;

            case 5:{
                printf("\n Simulating Duplicate Packet error");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                reject_packet rej;

                //correct packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //duplicate packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //correct packet
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //duplicate packet
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //correct packet
                dp = createDataPacket(3,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);
            } break;
            case 6:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Simulating All errors~~~~~~~~~~~~~~~~~~~~~~~");
                int i =0;
                int bytes =0;
                data_packet dp;
                ack_packet ack;
                reject_packet rej;

                //correct packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&ack,sizeof(ack_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printAckPacket(ack);

                //out of sequence packet
                dp = createDataPacket(5,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //length mismatch error
                dp = createDataPacket(2,3,allPayloads[i]);
                dp.length = dp.length+1;
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //Missing end of packet error
                dp = createDataPacket(2,3,allPayloads[i]);
                printDataPacket(dp);
                dp.endOfPacket = 0;
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);

                //Duplicate packet
                dp = createDataPacket(1,3,allPayloads[i]);
                printDataPacket(dp);
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                if(bytes <0){
                    printf("\nResponse from server not received\n");
                }
                else printRejectPacket(rej);
            } break;

            case 7:{
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~Timer case~~~~~~~~~~~~~~~~~~~~~~~");
                int counter = 0;
                int i =0;
                int bytes =0;
                data_packet dp;
                reject_packet rej;
                dp = createDataPacket(99,3,allPayloads[i]);
                printDataPacket(dp);
                while(bytes <=0 && counter<3){
                sendto(socket_client,&dp,sizeof(data_packet),0,(struct sockaddr *)&server_address,sizeof(server_address));
                bytes = recvfrom(socket_client,&rej,sizeof(reject_packet),0,NULL,NULL);
                    printf("\nResponse from server not received.Trying again....\n");
                    counter++;
                }
                if(counter == 3){
                    printf("\nServer is not responding. Please try again.\n");
                    counter = 0;
                }
            } break;

            case 8:{
                printf("\n ~~~~~~~~~~~~~~~~~~~~~~~Exiting~~~~~~~~~~~~~~~~~~~~~~~\n");
                exit(0);
            } break;

            default:{
                printf("\n Invalid Selection. Try again.....");
            } break;
        }
    }
    return 0;
}