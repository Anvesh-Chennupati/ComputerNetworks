/*

 *  processor.h
 *
 *  Created on: 27-May-2019
 *  Author: Anvesh Chennupati
 *  Stud ID: 00001538032
 */

// imports 
#include<stdio.h>
#include<string.h>
#include<stdint.h>

// packet types
#define DATA_PKT    0xFFF1
#define ACK_PKT     0xFFF2
#define REJECT_PKT  0xFFF3
#define START_PKT   0xFFFF
#define END_PKT     0xFFFF

#define CLIENT_ID 0xFF
// reject codes
#define OUT_OF_SEQ      0xFFF4
#define LEN_MISMATCH    0xFFF5
#define END_PKT_MISS    0xFFF6
#define DUP_PKT         0xFFF7
#define SUCCESS         0xFFF8

// payloads
uint8_t payload1[] = {1,1,1};
uint8_t payload2[] = {2,2,2};
uint8_t payload3[] = {3,3,3};
uint8_t payload4[] = {4,4,4};
uint8_t payload5[] = {5,5,5};

// data packet structure 
typedef struct {
    uint16_t startOfPacket;
    uint8_t clientID;
    uint16_t type;
    uint8_t segmentNo;
    uint8_t length;
    uint8_t payload[255];
    uint16_t endOfPacket;       
} data_packet;

// acknowledgment packet structure
typedef struct {
    uint16_t startOfPacket;
    uint8_t clientID;
    uint16_t type;
    uint8_t segmentNo;
    uint16_t endOfPacket;
} ack_packet;

// reject packet structure
typedef struct {
    uint16_t startOfPacket;
    uint8_t clientID;
    uint16_t type;
    uint16_t subCode;
    uint8_t segmentNo;
    uint16_t endOfPacket;
} reject_packet;

// methods to handle packets
data_packet createDataPacket(uint8_t segNo,uint8_t len,uint8_t *payload){
    data_packet packet;
    packet.startOfPacket = START_PKT;
    packet.clientID = CLIENT_ID;
    packet.type = DATA_PKT;
    packet.segmentNo = segNo;
    packet.length = len;
    int i =0;
    for(i = 0; i< len; i++){
        packet.payload[i] = payload[i];
    }
    packet.endOfPacket = END_PKT; 
    return packet;
}


//creating acknowledgement packet
ack_packet createAckPacket(data_packet data){
    ack_packet packet;
    packet.startOfPacket = START_PKT;
    packet.clientID = CLIENT_ID;
    packet.type = ACK_PKT;
    packet.segmentNo = data.segmentNo;
    packet.endOfPacket = END_PKT;
    return packet;
}

// creating reject packet 
reject_packet createRejectPacket(data_packet data,uint16_t subCode){
    reject_packet packet;
    packet.startOfPacket = START_PKT;
    packet.clientID = CLIENT_ID;
    packet.type = REJECT_PKT;
    packet.subCode = subCode;
    packet.segmentNo = data.segmentNo;
    packet.endOfPacket = END_PKT;
    return packet;
}

// displaying content of data packet
void printDataPacket(data_packet data){
    printf("\n\nDATA PACKET---->");
    printf("\nStart of Packet: \t0x%x",data.startOfPacket);
    printf("\nClient ID: \t\t0x%x",data.clientID);
    printf("\nPacket Type: \t\t0x%x",data.type);
    printf("\nSegment Number: \t%d",data.segmentNo);
    printf("\nLength of Payload: \t%d ",data.length);
    printf("\nPayload:\t\t");
    int i =0;
    for(i = 0; i< 3; i++){
        printf("%d ",data.payload[i]);
    }
    printf("\nEnd of Packet:\t\t0x%x\n",data.endOfPacket);
}

// method to print acknowledgment packet
void printAckPacket(ack_packet data){
    printf("\n\nACKNOWLEDGMENT PACKET---->");
    printf("\nStart of Packet: \t0x%x",data.startOfPacket);
    printf("\nClient ID: \t\t0x%x",data.clientID);
    printf("\nPacket Type: \t\t0x%x",data.type);
    printf("\nSegment Number: \t%d",data.segmentNo);
    printf("\nEnd of Packet:\t\t0x%x\n",data.endOfPacket);
}

// method to print reject packet
void printRejectPacket(reject_packet data){
    switch(data.subCode){
        case OUT_OF_SEQ:{
            printf("\nOUT OF SEQUENCE ERROR ");
        }break;
        case LEN_MISMATCH:{
            printf("\nLENGTH MISMATCH ERROR ");
        }break;
        case END_PKT_MISS:{
            printf("\nEND OF PACKET MISSING ERROR ");
        } break;
        case DUP_PKT:{
            printf("\nDUPLICATE PACKET ERROR ");
        }break;
    }
    printf("\n\nREJECT PACKET---->");
    printf("\nStart of Packet: \t0x%x",data.startOfPacket);
    printf("\nClient ID: \t\t0x%x",data.clientID);
    printf("\nPacket Type: \t\t0x%x",data.type);
        printf("\nError Subcode: \t\t%x",data.subCode);
    printf("\nSegment Number: \t%d",data.segmentNo);
    printf("\nEnd of Packet: \t\t0x%x\n",data.endOfPacket);
}