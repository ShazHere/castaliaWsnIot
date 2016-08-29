/*
 * SensorNode.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: shaza
 */

#include "NodeSensor.h"

Define_Module(NodeSensor);

void NodeSensor::startup()
{
    if (self == 0) {
        isSink = true;
        trace() << "Alhumdulillah; This is startup() of NodeSensor and sink" ;
    }
    else
        trace() << "Alhumdulillah; This is startup() of NodeSensor ";
    dataPacketsSent = 0; //means no packet is sent so far
    controlPacketsSent = 0;
    dataPacketsReceived = 0;
    controlPacketsReceived = 0;
    proposalRecord.clear();
//    int length = -1;
//    assert(length >= 0);

    double startTxTime = 1;
    if (self == 9) { //only node 9 is sending data
        setTimer(SEND_PACKET, startTxTime);
        setTimer(CHECK_IOT_PROPOSALS, startTxTime+15);
    }

}

bool NodeSensor::usefulProposal(IotToSnReplyPacket *rcvpkt) {
    trace()<<"Direction is " << rcvpkt->getExtraData().direction;
    if (rcvpkt->getExtraData().direction == 1)
        return true;
    else
        return false;
}

void NodeSensor::fromNetworkLayer(ApplicationPacket * rcvPacket, const char *source, double rssi, double lqi)
{
    //updateNeighborTable(atoi(source), rcvPacket->getSequenceNumber());
    std::string strSource(source);
    if (!isSink) { //normal SN
        switch ((int)(rcvPacket->getData())) {
        case MESSAGETYPE_IOTTOSN_SEARCHREPLY: {// IotToSnSearchReply
            controlPacketsReceived++;
            IotToSnReplyPacket *rcvpkt = check_and_cast<IotToSnReplyPacket*>(rcvPacket);
            trace()<<"received packet from source"<< strSource << " with LQI = " << lqi
                   << " rcvPacketName = " << rcvPacket->getName() << " message type is "
                                        << rcvPacket->getData();
            if (usefulProposal(rcvpkt)){
                trace()<<"adding proposal";
                addProposalRecord(rcvpkt, strSource);
            }
            break;
        }
       case MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN: {// IotToSnSearchSnToDrop
           controlPacketsReceived++;
           GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
           trace()<<"received drop packet from "<< strSource << " with LQI = " << lqi
                               << " messageType = " << rcvpkt->getExtraData().messageType
                               << " rcvPacketName = " << rcvPacket->getName() << " message type in data is "
                               << rcvPacket->getData();
           SnToIotPacket *pkt = new SnToIotPacket("SNtoIoTDropReply", APPLICATION_PACKET);
                       snInfo temp;
                       temp.locX = mobilityModule->getLocation().x;
                       temp.locY = mobilityModule->getLocation().y;
                       temp.nodeID = self;
                       temp.spentEnergy = resMgrModule->getSpentEnergy();
                       pkt->setExtraData(temp);
                       pkt->setData(MESSAGETYPE_SNTOIOT_DROP_REPLY); //according to messageType defined in GenericPacket.msg comments.
                       pkt->setSequenceNumber(controlPacketsSent);
                       pkt->setByteLength(packetSize);
                       toNetworkLayer(pkt, source);
                       controlPacketsSent++;
           break;
       }
        //case 5: // IotToSnDataPacket
        }
    }
    else //means this is the sink SN
    {
        trace() << "Packet reached at sink from " << strSource << " sink id is " << self;
    }
}

bool NodeSensor::addProposalRecord(IotToSnReplyPacket *rcvpkt, string source) {
    iotProposalRecord ipt;
    //trace()<<"source value = " << source << " and pakcetNOdeId = "<<rcvpkt->getExtraData().nodeID;
    ipt.id = rcvpkt->getExtraData().nodeID;
    ipt.range = rcvpkt->getExtraData().broadcastPower;
    ipt.speed = rcvpkt->getExtraData().speed;

    updateIotProposalRecordTable(ipt);
}
void NodeSensor::updateIotProposalRecordTable(iotProposalRecord ipt) {
    int i = 0, pos = -1;
        int tblSize = (int)proposalRecord.size();

        for (i = 0; i < tblSize; i++)
            if (proposalRecord[i].id == ipt.id)
                pos = i;

        if (pos == -1) {
            proposalRecord.push_back(ipt);
        } else {
           /// Decide what should be done for muliple proposals from same iot object
            trace()<< "WARN: multiple proposals from same IoT!";
        }
}
void NodeSensor::timerFiredCallback(int timerIndex)
{
    switch (timerIndex) {
    case SEND_PACKET:{
        if (controlPacketsSent == 0) {
            trace() << "This is timerFiredCallback of NodeSensor ID " << self << " with packetsSent == " << controlPacketsSent;
            SnToIotPacket *pkt = new SnToIotPacket("SearchIotPacket", APPLICATION_PACKET);
            snInfo temp;
            temp.locX = mobilityModule->getLocation().x;
            temp.locY = mobilityModule->getLocation().y;
            temp.nodeID = self;
            temp.spentEnergy = this->resMgrModule->getSpentEnergy();
            pkt->setExtraData(temp);
            pkt->setData(MESSAGETYPE_SNTOIOT_SEARCHIOT);
            pkt->setSequenceNumber(controlPacketsSent);
            pkt->setByteLength(packetSize);
            toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
            controlPacketsSent++;
        } break;
    case CHECK_IOT_PROPOSALS: {
            int bestProposalId = getBestProposal();
            trace() << "best proposal id is " << bestProposalId;

            GenericPacket *pkt = new GenericPacket("genericDataPacket", APPLICATION_PACKET);
            packetInfo temp;
            temp.OriginNodeID = self;
            temp.messageType = MESSAGETYPE_SNTOIOT_DATAPACKET;
            pkt->setExtraData(temp);
            pkt->setData(MESSAGETYPE_SNTOIOT_DATAPACKET);
            pkt->setSequenceNumber(dataPacketsSent); //TODO: Check what should be sequence no.
            pkt->setByteLength(packetSize); //TODO: check what should be data packet size
            toNetworkLayer(pkt, getIntToConstChar(proposalRecord[bestProposalId].id));
            dataPacketsSent++;
        }

    }
    setTimer(SEND_PACKET, 10); //so that timer is called after every 10 (seconds or ms)
    }
}
int NodeSensor::getBestProposal() {
    int size = (int) (proposalRecord.size());
    if (size != 0) {
        //select IoT and respond with data object
        double bestRangeIoTId = 0; //at the momend decide based on range
        for (int i = 0; i < size; i++) {
            if (proposalRecord[i].range > proposalRecord[bestRangeIoTId].range)
                bestRangeIoTId = i;
        }
        removeProposals();
        return bestRangeIoTId;
    }
    else
        return -1;
}
void NodeSensor::removeProposals() {
    proposalRecord.clear();
    /***
     * AT the moment there is strong assumption that at one time, SN needs to ask proposal
     * for only one data, and will select an IoT for sending that data. Therefore propsals
     * should belong to that one data only.
     */
}


void NodeSensor::finishSpecific()
{
    declareOutput("Control Packets received");
    trace()<< "Control packets received = " << controlPacketsReceived;
    collectOutput("Control Packets received", self, "ID" , controlPacketsReceived ); //message at the output file with date.

    declareOutput("Control Packets sent");
    trace()<< "Control packets sent " << controlPacketsSent; //message outputs at Castalia-trace
    collectOutput("Control Packets sent", self, "ID" , controlPacketsSent); //message at the output file with date.

    declareOutput("Data Packets received");
    trace()<< "Data packets received = " << dataPacketsReceived;
    collectOutput("Data Packets received", self, "ID" , dataPacketsReceived ); //message at the output file with date.

    declareOutput("Data Packets sent");
    trace()<< "Data packets sent " << dataPacketsSent; //message outputs at Castalia-trace
    collectOutput("Data Packets sent", self, "ID" , dataPacketsSent); //message at the output file with date.
    //collectOutput()
//    for (int i = 0; i < (int)neighborTable.size(); i++) {
//        collectOutput("Packets received", neighborTable[i].id,
//                  "Success", neighborTable[i].receivedPackets);
//    }
}

//void NodeSensor::updateNeighborTable(int nodeID, int serialNum)
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
NodeSensor::NodeSensor() {
    // TODO Auto-generated constructor stub

}

NodeSensor::~NodeSensor() {
    // TODO Auto-generated destructor stub
}
