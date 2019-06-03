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

#include "processor.h"

// Socket initialization.
int socket_connection;
struct sockaddr_in server_address;
data_packet data;
int nBytes = 0;
socklen_t addr_size = 0;
struct sockaddr_in incoming;

// for processing errors
int segcheck1 = 0;
int segcheck2 = 1;
uint8_t packetCounter = 0;
uint8_t segArray[5];

// method to reset global variables once 5 packets have been received
void resetParameters(){
    if(packetCounter == 5){
        printf("\nReceived 5 packets. Resetting fields\n");
        packetCounter= 0;
        segcheck1 = 0;
        segcheck2 = 1;
        memset(segArray, 0, sizeof(segArray));
    }
}

// method to process received packet and check the parameters of packet
int processRecievedPacket(data_packet data){
    resetParameters();
    if(data.segmentNo == 99) return 5;
    if(packetCounter == 0){
        if(data.segmentNo == (segcheck1 + 1)){
            if(data.length != 3) return 2;
            if(data.endOfPacket!= END_PKT) return 3;
            segArray[segcheck1] = segcheck2;
            segcheck1++;
            segcheck2++;
            packetCounter++;
        }
        // handling out of sequence error
        else return 1;
    }
    else{
        int i =0;
        for(i = 0; i <5; i++){
        if(segArray[i] == data.segmentNo) return 4;
        }
        if(data.segmentNo != (segcheck1 + 1)) return 1;
        if(data.length != 3) return 2;
        if(data.endOfPacket!= END_PKT) return 3;
        segArray[segcheck1] = segcheck2;
        segcheck1++;
        segcheck2++;
        packetCounter++;
    }
    return 0;
}
int main(){
    if((socket_connection = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
        perror("Unable to create socket");
        return 0;
    }
    // clearning segment array
    memset(segArray, 0, sizeof(segArray));
    
    
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

    // to continously receive and reply data from client
// running server indefinetly 
    while(1){
        int bytes = recvfrom(socket_connection, &data, sizeof( data_packet), 0,(struct sockaddr* )&incoming, &addr_size);
        if(bytes < 0){
            printf("\n Error recieving data\n");
            return 1;
        }
        // printDataPacket(data);
        int status = processRecievedPacket(data);
        switch(status){
            // if success
            case 0: {
                printf("\nSending Acknowledgement ");
                ack_packet sendAck = createAckPacket(data);
                int err = sendto(socket_connection, &sendAck,sizeof(ack_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                    }
                
            } break;
            case 1:{
                // out of sequence detected
                printf("\nOut of Sequence error. Sending reject packet\n");
                reject_packet rej = createRejectPacket(data,OUT_OF_SEQ);
                int err = sendto(socket_connection, &rej,sizeof(reject_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                    }
            } break;
            // length mismatch error
            case 2:{
                printf("\nLength Mismatch error. Sending reject packet\n");
                reject_packet rej = createRejectPacket(data,LEN_MISMATCH);
                int err = sendto(socket_connection, &rej,sizeof(reject_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                    }
            }break;
            // end of packet missing error case
            case 3:{
                printf("\nEnd of packet missing error. Sending reject packet\n");
                reject_packet rej = createRejectPacket(data,END_PKT_MISS);
                int err = sendto(socket_connection, &rej,sizeof(reject_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                    }
            }break;
            // duplicate packet case
            case 4:{
                printf("\nDuplicate packets received. Sending reject packet\n");
                reject_packet rej = createRejectPacket(data,DUP_PKT);
                int err = sendto(socket_connection, &rej,sizeof(reject_packet), 0,(struct sockaddr* )&incoming, sizeof(incoming));
                if (err < 0) {
                    perror("\ncannot send data to Client\n");
                    }
            }
            case 5:{

            }break;
        }
    }
    return 0;
}
