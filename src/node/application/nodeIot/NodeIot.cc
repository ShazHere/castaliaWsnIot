/*
 * Author Shaza Hanif
 * Date 2 June, 2016
 *
 */

/**
 * Comments on SOF about the packet storage error:
 *
what is GenericPacket? – fritzone 18 hours ago
3
possibly GenericPacket doesn't follow the Rule of Three, and is destructing its internal resource after the line idpr.gp = *rcvpkt; completes. Or perhaps its assignment operator does something weird. – M.M 18 hours ago
1
Why it should not be possible? Check GenericPacket copy constructor and assignment operator. There is no guarantee that name member is copied. – Ari0nhh 18 hours ago
2
It would help to at least link to the source code for GenericPacket – M.M 18 hours ago
2
Where gp is a class from a tested library -- Looks like this library needs a bit more testing. – PaulMcKenzie 18 hours ago

Or is GenericPacket just never intended for copying? – Hayt 18 hours ago

Ok, so its not my fault in using pointers? I better ask the library developers then. –
 */

#include "NodeIot.h"

Define_Module(NodeIot);


void NodeIot::startup()
{
    //From connectivityApp
    trace() << "Alhumdulillah; This is startup() of NodeIot ";
    //fix sink location, make consistent with omnetpp.ini of project
    double sinkX = 1;
    double sinkY = 0;

    dataPacketsSent = 0; //means no packet is sent so far
    controlPacketsSent = 0;
    dataPacketsReceived = 0;
    controlPacketsReceived = 0;

    dataPacketRecord.clear();
    dropReplySnRecord.clear();

    noOfTimesDirectionCHecked = 0; // just for checking

    double startTxTime = 10; //I think it sets the start time of packets sent according to simulation time
    //setTimer(SEND_PACKET, startTxTime);
    setTimer(CHECK_DROP_PACKAGES, 2);

}

void NodeIot::fromNetworkLayer(ApplicationPacket * rcvPacket, const char *source, double rssi, double lqi)
{
    std::string strSource(source);
    int messageType = (int)(rcvPacket->getData());
    switch (messageType) {
    case MESSAGETYPE_SNTOIOT_SEARCHIOT: { //Sn search Iot packets
        controlPacketsReceived ++;
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
        pkt->setData(MESSAGETYPE_IOTTOSN_SEARCHREPLY); //according to messageType defined in GenericPacket.msg comments.
        pkt->setSequenceNumber(controlPacketsSent);
        pkt->setByteLength(packetSize);
        toNetworkLayer(pkt, source);
        controlPacketsSent++;
        break;
    }
    case MESSAGETYPE_SNTOIOT_DROP_REPLY: { //SN to IoT  drop reply
        controlPacketsReceived ++;
        SnToIotPacket *rcvpkt = check_and_cast<SnToIotPacket*>(rcvPacket);
               trace()<<"Received drop reply packet from "<< strSource << " with LQI = " << lqi
                       << " spEnergy = " << rcvpkt->getExtraData().spentEnergy
                       << " rcvPacketName = " << rcvPacket->getName() << " message type is "
                       << rcvPacket->getData();
               addDropReplySnPacketRecord(rcvpkt, source, lqi);
///////////////START HERE
        break;
    }
    case MESSAGETYPE_SNTOIOT_DATAPACKET: {//data packet
        dataPacketsReceived++;
        GenericPacket *rcvpkt = check_and_cast<GenericPacket*>(rcvPacket);
        trace()<<"received packet from "<< strSource << " with LQI = " << lqi
                            << " messageType = " << rcvpkt->getExtraData().messageType
                            << " rcvPacketName = " << rcvPacket->getName() << " message type in data is "
                            << rcvPacket->getData();
        addDataPacketRecord(rcvpkt, source);
        //send this data to sink node? or dropPacket messages?
        //save this data packet?
        break;
    }
    }
    ;
}
bool NodeIot::addDropReplySnPacketRecord(SnToIotPacket *rcvpkt, string source, double lqi) {
    iotDropReplySnRecord idrsr;
    //trace()<<"source value = " << source << " and pakcetNOdeId = "<<rcvpkt->getExtraData().nodeID;
    idrsr.SnId = rcvpkt->getExtraData().nodeID;
    idrsr.locX =  rcvpkt->getExtraData().locX;
    idrsr.locY =  rcvpkt->getExtraData().locY;
    idrsr.spentEnergy = rcvpkt->getExtraData().spentEnergy;
    idrsr.lqi = lqi;
    //trace() << "checking addDropReplySnPacketRecord method.. sendderID is " << idrsr.SnId << " and Source is " << source;
    updateDropReplySnPacketRecord(idrsr);
}
void NodeIot::updateDropReplySnPacketRecord(iotDropReplySnRecord idrsr) {
    int i = 0, pos = -1;
        int tblSize = (int)dropReplySnRecord.size();

        for (i = 0; i < tblSize; i++)
            if (dropReplySnRecord[i].SnId == idrsr.SnId)
                pos = i;
        if (pos == -1) {
            dropReplySnRecord.push_back(idrsr);
            //trace()<< "added new SN record";
        }
        else {
            //trace()<< "Updating SN record";
            dropReplySnRecord[pos].spentEnergy = idrsr.spentEnergy;
            dropReplySnRecord[pos].lqi = idrsr.lqi;
        }
        if (tblSize > (int)dropReplySnRecord.size())
            trace() << "drop reply packet added/updated at Iot";
}
bool NodeIot::addDataPacketRecord(GenericPacket *rcvpkt, string source) {
    iotDataPacketRecord idpr;
    //trace()<<"source value = " << source << " and pakcetNOdeId = "<<rcvpkt->getExtraData().nodeID;
    idpr.oringinatorId = rcvpkt->getExtraData().OriginNodeID;
    idpr.isDropCheked = false;
    idpr.senderID = std::stoi(source); //tested working
    idpr.gp = rcvpkt;
    trace() << "packet Name originally  = " << rcvpkt->getName();
    trace() << "packet name after storing = " << idpr.gp->getName();
    updateDataPacketRecord(idpr);
}
void NodeIot::updateDataPacketRecord(iotDataPacketRecord idpr) {
    int i = 0, pos = -1;
        int tblSize = (int)dataPacketRecord.size();
        for (i = 0; i < tblSize; i++)
            if (dataPacketRecord[i].oringinatorId == idpr.oringinatorId
                    && dataPacketRecord[i].gp->getSequenceNumber() == idpr.gp->getSequenceNumber() )
                pos = i;
        if (pos == -1) {
            dataPacketRecord.push_back(idpr);
        } else {
           /// Decide what should be done for muliple proposals from same iot object
            trace()<< "WARN: multiple data packet from same SN and same sequence no";
        }
        if ((int)dataPacketRecord.size() > tblSize)
            trace() << "Data packet added at Iot";
//        trace()<<"Checking datapacketRecord messageType = " << dataPacketRecord[0].gp.getData()
//                << " and Name = " << dataPacketRecord[0].gp.getName();
}
void NodeIot::timerFiredCallback(int timerIndex)
{
    switch (timerIndex) {
    case SEND_PACKET: {
//        if (packetsSent < 10) {
//                    trace() << "This is timerFiredCallback of NodeIoT ID " << self << " with packetsSent ==  " << packetsSent;
//                    toNetworkLayer(createGenericDataPacket(0.0, packetsSent, packetSize), BROADCAST_NETWORK_ADDRESS);
//                    packetsSent++;
//                    setTimer(SEND_PACKET, 10);
//                }
        break;
    }
    case CHECK_DROP_PACKAGES:{
        setTimer(CHECK_DROP_PACKAGES, 2);
        if (!directionCheckOk())
            break;
        if ((int)dataPacketRecord.size() == 0)
            break;
        iotDropReplySnRecord *bestSn;
        int returnCode = -1;
        returnCode = getBestSn(bestSn);
        if (returnCode == 0){ //means there is a best SN
            trace()<< "Found best SN with id = "<< bestSn->SnId;
            //send the data packet so to best SN and clear the dataPacketRecord table and drop SN table
            sendDataPacketRecordsToBestSn(bestSn);
            dataPacketRecord.clear();
            dropReplySnRecord.clear();
        }
        else {
            trace()<<"NO SN FOUND";
            ///TODO There should be no need to send drop messages when bestSN is already there.
            int tblSize = (int)dataPacketRecord.size();
            for (int i = 0; i < tblSize; i++) // if there is any record without check drop, just drop mesages
                if (dataPacketRecord[i].isDropCheked == false) {
                  //  trace() << "Checking datapacketRecord originID = " << dataPacketRecord[i].oringinatorId;
                  // trace() << "Checking datapacketRecord messageType = " << dataPacketRecord[i].gp.getData();
                    GenericPacket *pkt = new GenericPacket("IoTDropPacket", APPLICATION_PACKET);
                    packetInfo temp;
                    temp.OriginNodeID = self; //no need to put origin data id in it
                    temp.messageType = MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN;
                    pkt->setExtraData(temp);
                    pkt->setData(MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN); //according to messageType defined in GenericPacket.msg comments.
                    pkt->setSequenceNumber(controlPacketsSent); //TODO: Check what should be sequence no.
                    pkt->setByteLength(packetSize); //TODO: check what should be data packet size
                    toNetworkLayer(pkt,BROADCAST_NETWORK_ADDRESS);
                    controlPacketsSent++;
                    break; //one drop boradcast message is enough to get replies from SN's
                }
        }
        break;
    } //End of case CHECK_DROP_PACKAGES
    }//End of switch (timerIndex)
}//End of method

int NodeIot::getBestSn(iotDropReplySnRecord* &bestSn) {

    int tblSize = (int)dropReplySnRecord.size();
    if (tblSize == 0)
        return -1; //return with error code
    double selfX = mobilityModule->getLocation().x;
    double selfY = mobilityModule->getLocation().y;
    double closestSnDist = getDistance(selfX, selfY, dropReplySnRecord[0].locX, dropReplySnRecord[0].locY);
    int closestSnId = 0;
    for (int i = 0; i < tblSize; i++) { //decide based on distance
        double dist = getDistance(selfX, selfY, dropReplySnRecord[i].locX, dropReplySnRecord[i].locY);
        if (dist<closestSnDist) {
            closestSnDist = dist;
            closestSnId = i;
        }
    }
    trace()<< "value of bestSn from vector " << dropReplySnRecord[closestSnId].SnId;
    bestSn = &(dropReplySnRecord[closestSnId]);
    //trace()<< "value of bestSn before calling clear() " << bestSn->SnId;
    //dropReplySnRecord.clear();

    return 0; //normal return should be 0
}
/**
 * TODO: fix this method because I'm unable to store and then send data packet to network layer.
 * PS: may be create new data packets each time...since I any way don't need the same data packet to be sent
 */
void NodeIot::sendDataPacketRecordsToBestSn(iotDropReplySnRecord* bestSn) {
    int tblSize = (int)dataPacketRecord.size();
    trace()<< "sending " << tblSize << " data packets to " << bestSn->SnId;
    for (int i = 0; i < tblSize; i++) {
        //before sending gp, reset it and send a new packet. this is because of error in storing data
        GenericPacket *pkt = new GenericPacket("genericDataPacket", APPLICATION_PACKET);
        packetInfo temp;
        temp.OriginNodeID = dataPacketRecord[i].oringinatorId;
        temp.messageType = MESSAGETYPE_IOTTOSN_DATAPACKET;
        pkt->setExtraData(temp);
        pkt->setData(MESSAGETYPE_IOTTOSN_DATAPACKET);
        pkt->setSequenceNumber(dataPacketsSent); //TODO: Check what should be sequence no.
        pkt->setByteLength(packetSize); //TODO: check what should be data packet size
        //                trace() << " dataPacketRecord chk messageType = " << pkt->getData() << " and Name = "
        //                        << pkt->getName() << " sequenceNo =  " << pkt->getSequenceNumber();
        toNetworkLayer(pkt,getIntToConstChar(bestSn->SnId));
        trace()<< "DONE aLHUMD" ;
        dataPacketsSent++;
    }
}
/**
 * return True if Iot object should drop messages considering its direction.
 * At the moment return true whatever is the direction.
 *
 */
bool NodeIot::directionCheckOk ()
{
    return true;
    bool iotDirection = (check_and_cast<LineMobilityManager*>(mobilityModule))->getDirection();
    if (iotDirection == 1) {//0 means going away from sink
        noOfTimesDirectionCHecked++;
    }
    //TODO adjut how should i actually send data packets after reciving drop replies.
    trace()<< "Direction checked this no of times: " << noOfTimesDirectionCHecked;
}
void NodeIot::finishSpecific()
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

