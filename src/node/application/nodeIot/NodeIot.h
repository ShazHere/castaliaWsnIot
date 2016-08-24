/*
 * WsnIot.h
 *
 *  Created on: Jun 2, 2016
 *      Author: shaza
 */

#ifndef NODEIOT_H_
#define NODEIOT_H_

#include "VirtualApplication.h"
#include "IotToSnReplyPacket_m.h"
#include "SnToIotPacket_m.h"
#include "LineMobilityManager.h"
#include "GenericPacket_m.h"

using namespace std;

struct neighborRecord {
    int id;
    int timesRx;
    int receivedPackets;
};

enum NodeIotTimers {
    SEND_PACKET = 1,
};

class NodeIot: public VirtualApplication {
private:
    // parameters and variables
    int packetsReceived;
    char *packetReceivedSource;
    double rssi;
    double lqi;

    // parameters related to sending
    //int packetsPerNode;
    int packetSize;
    int packetsSent;

 protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
        //From Connectivity App
    void finishSpecific();
    void timerFiredCallback(int);
   // void updateNeighborTable(int nodeID, int theSN);
};


#endif /* _NODEIOT_H_ */
