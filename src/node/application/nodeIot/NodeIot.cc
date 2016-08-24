/*
 * Author Shaza Hanif
 * Date 2 June, 2016
 *
 */

#include "NodeIot.h"

Define_Module(NodeIot);


void NodeIot::startup()
{
    //From connectivityApp
    trace() << "Alhumdulillah; This is startup() of NodeIot ";

    packetsReceived = 0;
    packetsSent = 0; //means no packet is sent so far

    double startTxTime = 10; //I think it sets the start time of packets sent according to simulation time
    setTimer(SEND_PACKET, startTxTime);

}

void NodeIot::fromNetworkLayer(ApplicationPacket * rcvPacket, const char *source, double rssi, double lqi)
{
    //ValueReportingDataPacket *rcvPacket = check_and_cast<ValueReportingDataPacket*>(genericPacket);
    //updateNeighborTable(atoi(source), rcvPacket->getSequenceNumber());
    std::string strSource(source);
    switch ((int)(rcvPacket->getData())) {
    case 1: { //Sn search Iot packets
        SnToIotPacket *rcvpkt = check_and_cast<SnToIotPacket*>(rcvPacket);
            trace()<<"received packet from "<< strSource << " with LQI = " << lqi
                    << " spEnergy = " << rcvpkt->getExtraData().spentEnergy
                    << " rcvPacketName = " << rcvPacket->getName() << " message type is "
                    << rcvPacket->getData();
            IotToSnReplyPacket *pkt = new IotToSnReplyPacket("IoTReplyPacket", APPLICATION_PACKET);
            iotInfo temp;
            temp.locX = mobilityModule->getLocation().x;
            temp.locY = mobilityModule->getLocation().y;
            temp.nodeID = self;
            temp.speed = (check_and_cast<LineMobilityManager*>(mobilityModule))->getSpeed();
            temp.broadcastPower = radioModule->getTxPowerIndBm();
            temp.direction = (check_and_cast<LineMobilityManager*>(mobilityModule))->getDirection();
            pkt->setExtraData(temp);
            pkt->setData(2); //according to messageType defined in GenericPacket.msg comments.
            pkt->setSequenceNumber(packetsReceived);
            pkt->setByteLength(packetSize);
            toNetworkLayer(pkt, source);
            packetsSent++;
    } break;
    case 4: {//data packet
//        GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
//        trace()<<"received packet from "<< strSource << " with LQI = " << lqi
//                            << " spEnergy = " << rcvpkt->getExtraData().spentEnergy
//                            << " rcvPacketName = " << rcvPacket->getName() << " message type is "
//                            << rcvPacket->getData();
        //send this data to sink node? or dropPacket messages?
        //save this data packet?
    }
    }
    packetsReceived ++;
}
void NodeIot::timerFiredCallback(int timerIndex)
{
    switch (timerIndex) {
    case SEND_PACKET:{
        if (packetsSent < 30) {
            trace() << "This is timerFiredCallback of NodeIoT ID " << self << " with packetsSent ==  " << packetsSent;
            toNetworkLayer(createGenericDataPacket(0.0, packetsSent, packetSize), BROADCAST_NETWORK_ADDRESS);
            packetsSent++;
            setTimer(SEND_PACKET, 10);
        }
    }
    }
}

void NodeIot::finishSpecific()
{
    declareOutput("Packets received");
    trace()<< "packets received = " << packetsReceived;
    collectOutput("Packets received", self, "ID" , packetsReceived ); //message at the output file with date.

    declareOutput("Packets sent");
    trace()<< "packets sent " << packetsSent; //message outputs at Castalia-trace
    collectOutput("Packets sent", self, "ID" , packetsSent ); //message at the output file with date.
//    for (int i = 0; i < (int)neighborTable.size(); i++) {
//        collectOutput("Packets received", neighborTable[i].id,
//                  "Success", neighborTable[i].receivedPackets);
//    }
}

//void NodeIot::updateNeighborTable(int nodeID, int serialNum)
//{
//    int i = 0, pos = -1;
//    int tblSize = (int)neighborTable.size();
//
//    for (i = 0; i < tblSize; i++)
//        if (neighborTable[i].id == nodeID)
//            pos = i;
//
//    if (pos == -1) {
//        neighborRecord newRec;
//        newRec.id = nodeID;
//        newRec.timesRx = 1;
//
//        if ((serialNum >= 0) && (serialNum < packetsPerNode))
//            newRec.receivedPackets = 1;
//
//        neighborTable.push_back(newRec);
//    } else {
//        neighborTable[pos].timesRx++;
//
//        if ((serialNum >= 0) && (serialNum < packetsPerNode))
//            neighborTable[pos].receivedPackets++;
//    }
//}

