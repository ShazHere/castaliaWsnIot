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

//#include "assert.h"


using namespace std;

struct neighborRecord {
    int id;
    int timesRx;
    int receivedPackets;
};

struct iotProposalRecord {
    int id;
    double speed;
    double range;
};

enum NodeSensorTimers {
    SEND_PACKET = 1,
    CHECK_IOT_PROPOSALS = 2,
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
   //int totalSNnodes;
   vector<iotProposalRecord> proposalRecord;

   //user defined functions
   bool usefulProposal(IotToSnReplyPacket *rcvpkt);
   bool addProposalRecord(IotToSnReplyPacket *rcvpkt, string source);
   void updateIotProposalRecordTable(iotProposalRecord );
   int getBestProposal();
   void removeProposals();

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
