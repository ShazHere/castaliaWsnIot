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
#include <sstream>

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
    double lqi;
};

enum NodeIotTimers {
    SEND_PACKET = 1,
    CHECK_DROP_PACKAGES = 2,
    RECORD_ENERGY = 3,
};



class NodeIot: public VirtualApplication {
private:
    // parameters and variables
    int controlPacketsReceived;
    int controlPacketsSent;
    int dataPacketsSent;
    int dataPacketsReceived;

    double sinkX;
    double sinkY;

    char *packetReceivedSource;
    double rssi;
    double lqi;
    int packetSize;

    bool justReturned; // for keeping track of mobility direction of Iot. True means just took turn and should send drop replies
    // False means that either direction is towards sink or it has returned from quite some time.

    const int SEND_PACKET_INTERVAL = 10; //interval to call settimer()
    const int CHECK_DROP_PACKAGES_INTERVAL = 3; //interval to call settimer()

    vector<iotDataPacketRecord> dataPacketRecord;
    vector<iotDropReplySnRecord> dropReplySnRecord;
    // user defined methods:
    void sendDataPacketRecordsToBestSn(iotDropReplySnRecord* );
    bool addDataPacketRecord(GenericPacket *, string );
    void updateDataPacketRecord(iotDataPacketRecord) ;
    bool addDropReplySnPacketRecord(SnToIotPacket *, string, double );
    void updateDropReplySnPacketRecord(iotDropReplySnRecord);
    int getBestSn(iotDropReplySnRecord* &bestSn);
    bool directionCheckOk ();
    string getLocationText();

 protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void finishSpecific();
    void timerFiredCallback(int);
};


#endif /* _NODEIOT_H_ */
