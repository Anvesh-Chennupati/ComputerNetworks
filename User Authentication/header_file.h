/*

 *  header_file.h
 *
 *  Created on: 27-May-2019
 *  Author: Anvesh Chennupati
 *  Stud ID: 00001538032
 */

// header file contains all the methods and variables used in client and server
#include <inttypes.h>
// packet
#define START_PACKET    0xFFFF
#define END_PACKET      0xFFFF
#define CLIENT_ID       0xFF
#define ACCESS_PER      0xFFF8
#define LENGTH          0xFF
#define NOT_PAID        0xFFF9
#define NOT_EXIST       0xFFFA
#define PAID			0xFFFB

// celluar technologies
#define T2G 02
#define T3G 03
#define T4G 04
#define T5G 05

//request packet
typedef struct {
    uint16_t packetID;
	uint8_t clientID;
	uint16_t Acc_Per;
	uint8_t segment_No;
	uint8_t length;
	uint8_t technology;
	int64_t SourceSubscriberNo;
	uint16_t endpacketID;
} request_packet;

//response packet
typedef struct {
    uint16_t packetID;
	uint8_t clientID;
	uint16_t type;
	uint8_t segment_No;
	uint8_t length;
	uint8_t technology;
	uint64_t SourceSubscriberNo;
	uint16_t endpacketID;
} response_packet;

// structure to store user data locally
typedef struct{
    unsigned long subscriberNumber;
	uint8_t technology;
	int status;
} subscribers;

//initializing packets
request_packet initializeRequestPacket(){
    request_packet request;
	request.packetID = START_PACKET;
	request.clientID = CLIENT_ID;
	request.Acc_Per = ACCESS_PER;
	request.endpacketID = END_PACKET;
	return request;
}

//initializing response packet
response_packet initializeResponsePacket(request_packet request){
    response_packet response;
    response.packetID = request.packetID;
	response.clientID = request.clientID;
	response.length = request.length;
	response.technology = request.technology;
	response.SourceSubscriberNo = request.SourceSubscriberNo;
	response.endpacketID = request.endpacketID;
    return response;
}

// method to print request packet
void printRequestPacket(request_packet request ) {
	printf("packetID:\t\t%x\n",request.packetID);
	printf("Client id :\t\t%hhx\n",request.clientID);
	printf("Access permission:\t%x\n",request.Acc_Per);
	printf("Segment no :\t\t%d \n",request.segment_No);
	printf("length\t\t\t%d\n",request.length);
	printf("Technology\t\t%d \n", request.technology);
	printf("Subscriber no:\t\t%u \n",request.SourceSubscriberNo);
	printf("end of datapacket id:\t%x \n",request.endpacketID);
}

// method to print the response packet
void printResponsePacket(response_packet response){
	printf("packetID:\t\t%x\n",response.packetID);
	printf("Client id :\t\t%hhx\n",response.clientID);
	printf("Type:\t\t\t%x\n",response.type);
	printf("Segment no :\t\t%d \n",response.segment_No);
	printf("length\t\t\t%d\n",response.length);
	printf("Technology\t\t%d \n", response.technology);
	printf("Subscriber no:\t\t%u \n",response.SourceSubscriberNo);
	printf("end of datapacket id:\t%x \n",response.endpacketID);
}