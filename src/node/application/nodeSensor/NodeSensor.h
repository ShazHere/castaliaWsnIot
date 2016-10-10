/*
 * NodeSensor.h
 *
 *  Created on: Jun 7, 2016
 *      Author: shaza
 */

#ifndef NODESENSOR_H_
#define NODESENSOR_H_

#include "VirtualApplication.h"
#include "SnToIotPacket_m.h"
#include "IotToSnReplyPacket_m.h"
#include "GenericPacket_m.h"
#include "HelperMethods.h"

#include "assert.h"
//#define NDEBUG //enable it to turn off debug mode and all assertions in current file

using namespace std;

struct iotProposalRecord {
    int id;
    double speed;
    double range;
};

struct generatedDataPacketRecord {
    int originatorId;
    int generatedTime;
    bool searchIoTSent; // true if for this packet, MESSAGETYPE_SNTOIOT_SEARCHIOT message has been sent
    bool packetSent; //true if actuall data packet has been sent to selected IOT proposal.
};

enum NodeSensorTimers {
    SEND_PACKET = 1,
    CHECK_IOT_PROPOSALS = 2,
    CHECK_TOSEND_IOTDROPREPLY = 3,
};

class NodeSensor:  public VirtualApplication {
private:
   // parameters and variables
   //int packetsPerNode;
   int packetSize;
   int controlPacketsReceived;
   int controlPacketsSent;
   int dataPacketsSent;
   int dataPacketsReceived;
   bool isSinkNieghbor; //to keep track if sink is neigbor (so that data packet can be sent directly)
   //int totalSNnodes;
   vector<iotProposalRecord> proposalRecord;
   vector<generatedDataPacketRecord> dataPacketRecord;
   vector<int> sourcesForDropReply;

   //user defined functions
   bool usefulProposal(IotToSnReplyPacket *rcvpkt);
   bool addProposalRecord(IotToSnReplyPacket *rcvpkt, string source);
   void updateIotProposalRecordTable(iotProposalRecord );
   int getBestProposal();
   void removeProposals();
   void generateDataPacket(int originatorId);
   GenericPacket* createGenericDataPacket(int originNodeId, int messageType, int sequenceNo);
   string getLocationText();
    void sendDropReplyTo(int source);

public:
    NodeSensor();
    virtual ~NodeSensor();
protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void timerFiredCallback(int);

    void finishSpecific();
   // void updateNeighborTable(int nodeID, int theSN);
};

#endif /* NODESENSOR_H_ */
