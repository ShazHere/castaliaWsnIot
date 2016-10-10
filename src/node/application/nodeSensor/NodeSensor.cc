/*
 * SensorNode.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: shaza
 */

#include "NodeSensor.h"

Define_Module(NodeSensor);

string NodeSensor::getLocationText() {
    std::string str;
    str =  " Location = (" + std::to_string((int)(floor(mobilityModule->getLocation().x))) + ", "
           + std::to_string((int)(floor(mobilityModule->getLocation().y))) + ")";
    return str;
}

/***
 * TODO latter think about using packet->dup() function to copy the packet
 */
void NodeSensor::generateDataPacket(int originatorId) {
    //Generate Data packets means original data to be routed to towards sink
    generatedDataPacketRecord gdpr;
    gdpr.generatedTime = (int) ((simTime().dbl()));
    gdpr.originatorId = originatorId;
    gdpr.packetSent = false;
    gdpr.searchIoTSent = false;

    dataPacketRecord.push_back(gdpr);
}

void NodeSensor::startup()
{
    isSink = par("isSink");
    if (isSink) {
        trace() << "Alhumdulillah; This is startup() of NodeSensor and SINK "
                << getLocationText();
    }
    else
        trace() << "Alhumdulillah; This is startup() of NodeSensor "
        << getLocationText();
    dataPacketsSent = 0; //means no packet is sent so far
    controlPacketsSent = 0;
    dataPacketsReceived = 0;
    controlPacketsReceived = 0;
    proposalRecord.clear();
    dataPacketRecord.clear();

    if (self == 1 || self == 2 ||self == 4 )
        isSinkNieghbor= true;
    else
        isSinkNieghbor = false;

    double startTxTime = 1;
    if (self == 9) { //only node 9 is sending data
        //Generate Data packets
        for (int i = 0; i< 5; i++){
            generateDataPacket(self);
        }
    }
    setTimer(SEND_PACKET, startTxTime+self);
    setTimer(CHECK_IOT_PROPOSALS, startTxTime+2+self);
    //declareOutput("EnergyConsumed");

    /// int length = -1;
    /// assert(length >= 0 && "Length is not greater than equal to 0 "); sample assert to recall
}

bool NodeSensor::usefulProposal(IotToSnReplyPacket *rcvpkt) {
    //trace()<<"Direction is " << rcvpkt->getExtraData().direction;
    if (rcvpkt->getExtraData().direction == 1)
        return true;
    else
        return false;
}

void NodeSensor::sendDropReplyTo(int source) {
    SnToIotPacket* pkt = new SnToIotPacket("SNtoIoTDropReply",
            APPLICATION_PACKET);
    snInfo temp;
    temp.locX = mobilityModule->getLocation().x;
    temp.locY = mobilityModule->getLocation().y;
    temp.nodeID = self;
    temp.spentEnergy = resMgrModule->getSpentEnergy();
    pkt->setExtraData(temp);
    pkt->setData(MESSAGETYPE_SNTOIOT_DROP_REPLY); //according to messageType defined in GenericPacket.msg comments.
    pkt->setSequenceNumber(controlPacketsSent);
    pkt->setByteLength(packetSize);
    trace()<<"sending MESSAGETYPE_SNTOIOT_DROP_REPLY to source = " << source;
    toNetworkLayer(pkt, getIntToConstChar(source));
    controlPacketsSent++;
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
            trace()<<"received packet from IOT "<< strSource << " with LQI = " << lqi
                   << " rcvPacketName = " << rcvPacket->getName() << " message type is "
                                        << getMessageTypeText(rcvPacket->getData());
            assert ((atoi(source) == rcvpkt->getExtraData().nodeID) && "source is not equal to getExtraData.nodeID");
            if (usefulProposal(rcvpkt)){
                addProposalRecord(rcvpkt, strSource);
            }
            break;
        }
       case MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN: {// IotToSnSearchSnToDrop
           controlPacketsReceived++;
           GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
           trace()<<"received drop packet from "<< strSource << " with LQI = " << lqi
                   << " messageType = " << getMessageTypeText(rcvpkt->getExtraData().messageType)
                   << " rcvPacketName = " << rcvPacket->getName() ;
           assert((rcvpkt->getExtraData().messageType == rcvPacket->getData()) && "messageType mismatch");
           sourcesForDropReply.push_back(atoi(source)); //don't immeditaly send reply since it causes interference
           double randomTime = uniform(0.1,1);  //choose a random no between 0.1s and 1s
           setTimer(CHECK_TOSEND_IOTDROPREPLY, randomTime);
           break;
       }
       case MESSAGETYPE_IOTTOSN_DATAPACKET: {// IotToSnDataPacket
           dataPacketsReceived++;
           GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
           trace()<<"received dataPacket from IOT "<< strSource << " with LQI = " << lqi
                   << " messageType = " << getMessageTypeText(rcvpkt->getExtraData().messageType)
                   << " seqNo = " << rcvPacket->getSequenceNumber();
           assert((rcvpkt->getExtraData().messageType == rcvPacket->getData()) && "messageType mismatch");
           if (isSinkNieghbor) {//send data packet to sink directly
               GenericPacket *pkt = createGenericDataPacket(rcvpkt->getExtraData().OriginNodeID, MESSAGETYPE_SNTOSINK_DATAPACKET, dataPacketsSent);
               trace()<<"created pkt originID=  " << pkt->getExtraData().OriginNodeID;
               const char *sinkid = "0";
               toNetworkLayer(pkt, sinkid);
               dataPacketsSent++;
           }
           else if (rcvpkt->getExtraData().OriginNodeID != self){
               generateDataPacket(rcvpkt->getExtraData().OriginNodeID);
           }
           //else --its re-receiving its own data packet that it has sent earlier
           break;
       }//end case MESSAGETYPE_IOTTOSN_DATAPACKET
       trace ()<< "packet received at sensor " << self << " but unknown type ";
      } // end switch()
    } //end if (!sink)
    else //means this is the sink SN
    {
        switch ((int)(rcvPacket->getData())){
        case MESSAGETYPE_IOTTOSN_DATAPACKET: {
            GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
            trace() << "Packet reached at sink from Iot= " << strSource << " originator is " << rcvpkt->getExtraData().OriginNodeID
                    << " messageType = " << getMessageTypeText(rcvpkt->getData());
            controlPacketsReceived++;
            break;
        }
        case MESSAGETYPE_SNTOIOT_DATAPACKET: {
            GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
            trace() << "Packet reached at sink from SN=" << strSource << " originator is " << rcvpkt->getExtraData().OriginNodeID
                    << " messageType = " << getMessageTypeText(rcvpkt->getData()) << " discarded";
            //dataPacketsReceived++;
            break;
        }
        case MESSAGETYPE_SNTOSINK_DATAPACKET: {
            GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
                        trace() << "Packet reached at sink from SN=" << strSource << " originator is " << rcvpkt->getExtraData().OriginNodeID
                                << " messageType = " << getMessageTypeText(rcvpkt->getData());
            dataPacketsReceived++;
            break;
        }
        trace() << "Packet reached at sink unknown type";
        } //end switch
    } //end else Sink
}
bool NodeSensor::addProposalRecord(IotToSnReplyPacket *rcvpkt, string source) {
    iotProposalRecord ipt;
    //trace()<<"source value = " << source << " and pakcetNOdeId = "<<rcvpkt->getExtraData().nodeID;
    ipt.id = rcvpkt->getExtraData().nodeID;
    ipt.range = rcvpkt->getExtraData().broadcastPower;
    ipt.speed = rcvpkt->getExtraData().speed;

    updateIotProposalRecordTable(ipt);
    return true;
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

GenericPacket* NodeSensor::createGenericDataPacket(int originNodeId, int messageType, int sequenceNO) {
    GenericPacket* pkt = new GenericPacket("genericDataPacket",
            APPLICATION_PACKET);
    packetInfo temp;
    temp.OriginNodeID = originNodeId;
    temp.messageType = messageType;
    pkt->setExtraData(temp);
    pkt->setData(messageType);
    pkt->setSequenceNumber(sequenceNO); //TODO: Check what should be sequence no.
    pkt->setByteLength(packetSize); //TODO: check what should be data packet size
    return pkt;
}

void NodeSensor::timerFiredCallback(int timerIndex)
{
    switch (timerIndex) {
    case SEND_PACKET: {
        int size = (int) (dataPacketRecord.size());
        int previousValueControlPacketsSent = controlPacketsSent;
        if (size > 0) {
            //trace() << "This is timerFiredCallback of NodeSensor ID " << self << " with controlpacketsSent == " << controlPacketsSent;
            for  (int i = 0; i < size; i++) {
                if (previousValueControlPacketsSent < controlPacketsSent)
                    dataPacketRecord[i].searchIoTSent = true; //one search IOT should be performed for all data packets at the moment.
                if (false == dataPacketRecord[i].searchIoTSent ) {
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
                    dataPacketRecord[i].searchIoTSent = true;
                    controlPacketsSent++;
                }
            }
           }// end if (size > 0)
        setTimer(SEND_PACKET, 10); //so that timer is called after every 10 (seconds or ms)
        break;
    }
    case CHECK_IOT_PROPOSALS: {
        int size = (int) (dataPacketRecord.size());
        if (size > 0) {
            int bestProposalId = getBestProposal();
            trace() << "best proposal id is IOT " << proposalRecord[bestProposalId].id;
            for (int i = 0; i<size; i++) {
                if (dataPacketRecord[i].packetSent == false) {
                    toNetworkLayer(createGenericDataPacket(self, MESSAGETYPE_SNTOIOT_DATAPACKET, dataPacketsSent), getIntToConstChar(proposalRecord[bestProposalId].id));
                    dataPacketsSent++;
                    dataPacketRecord[i].packetSent = true;
                }
            }
        }  //end if (size > 0)
        break;
    }// end case CHECK_IOT_PROPOSALS
    case CHECK_TOSEND_IOTDROPREPLY: {
        //send the IOTDROP reply
        int size = (int) (sourcesForDropReply.size());
        if (size != 0) {
            for (int i = 0; i < size; i++)
                sendDropReplyTo(sourcesForDropReply[i]);
        }
        sourcesForDropReply.clear();
        break;
    } //end case CHECK_TOSEND_IOTDROPREPLY

    }// end switch(timerIndex)
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


NodeSensor::NodeSensor() {
    // TODO Auto-generated constructor stub

}

NodeSensor::~NodeSensor() {
    // TODO Auto-generated destructor stub
}
