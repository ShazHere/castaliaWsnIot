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
#include "HelperMethods.h"

using namespace std;

struct iotDataPacketRecord {
    int oringinatorId;
    int senderID;
    GenericPacket *gp;
    bool isDropCheked; //first step is to check if there is SN avialable to drop messages.
    // after sending such message, this should be true, then after getting reply, such packets
    //should be dropped
};

struct iotDropReplySnRecord {
    int SnId;
    double locX;
    double locY;
    double spentEnergy;
};

enum NodeIotTimers {
    SEND_PACKET = 1,
    CHECK_DROP_PACKAGES = 2,
};

class NodeIot: public VirtualApplication {
private:
    // parameters and variables
    int controlPacketsReceived;
    int controlPacketsSent;
    int dataPacketsSent;
    int dataPacketsReceived;
    int noOfTimesDirectionCHecked;

    char *packetReceivedSource;
    double rssi;
    double lqi;
    int packetSize;

    vector<iotDataPacketRecord> dataPacketRecord;
    vector<iotDropReplySnRecord> dropReplySnRecord;
    // user defined methods:
    bool addDataPacketRecord(GenericPacket *, string );
    void updateDataPacketRecord(iotDataPacketRecord) ;
//    bool addDropReplySnPacketRecord(SnToIotPacket *, string );
//    void updateDropReplySnPacketRecord(iotDropReplySnRecord);
//    int getBestSn(iotDropReplySnRecord *);

 protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void finishSpecific();
    void timerFiredCallback(int);
};


#endif /* _NODEIOT_H_ */
