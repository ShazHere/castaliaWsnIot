/*
 * HelperMethods.h
 *
 *  Created on: Aug 25, 2016
 *      Author: shaza
 */

#ifndef HELPERMETHODS_H_
#define HELPERMETHODS_H_

#include<math.h>
//char const* getIntToConstChar(int number);

inline char const* getIntToConstChar(int number) {
    std::string s = std::to_string(number);
    char const *pchar = s.c_str();  //use char const* as target type
    return pchar;
}
inline double getDistance(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2;
    double y = y1 - y2;
    double dist;

    dist = pow(x,2)+pow(y,2);           //calculating distance by euclidean formula
    dist = sqrt(dist);                  //sqrt is function in math.h

    return dist;
}
/// enum MessageType tHISis the sequence used for message type.
// PROBLEM: i'm unable to use new user defined type in the packet infor, so i used the default 'data'
//  and explicitly maintain the folowing values.
// data field in ApplicationPacket
//    SnToIotSearchIot = 1,
//    IotToSnSearchReply = 2,
//    IotToSnSearchSnForDrop = 3,
//    SnToIotDataPacket = 4,
//    IotToSnDataPacket = 5
int const MESSAGETYPE_SNTOIOT_SEARCHIOT = 1;
int const MESSAGETYPE_IOTTOSN_SEARCHREPLY = 2;
int const MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN = 3;
int const MESSAGETYPE_SNTOIOT_DROP_REPLY = 4;
int const MESSAGETYPE_SNTOIOT_DATAPACKET = 5;
int const MESSAGETYPE_IOTTOSN_DATAPACKET = 6;
int const MESSAGETYPE_SNTOSINK_DATAPACKET = 7;

inline char const* getMessageTypeText(int messageType) {
    switch (messageType) {
    case MESSAGETYPE_SNTOIOT_SEARCHIOT:
        return "MESSAGETYPE_SNTOIOT_SEARCHIOT";
    case MESSAGETYPE_IOTTOSN_SEARCHREPLY:
        return "MESSAGETYPE_IOTTOSN_SEARCHREPLY";
    case MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN:
        return "MESSAGETYPE_IOTTOSN_DROPTOSEARCHSN";
    case MESSAGETYPE_SNTOIOT_DROP_REPLY:
        return "MESSAGETYPE_SNTOIOT_DROP_REPLY";
    case MESSAGETYPE_SNTOIOT_DATAPACKET:
        return "MESSAGETYPE_SNTOIOT_DATAPACKET";
    case MESSAGETYPE_IOTTOSN_DATAPACKET:
        return "MESSAGETYPE_IOTTOSN_DATAPACKET";
    case MESSAGETYPE_SNTOSINK_DATAPACKET:
        return "MESSAGETYPE_SNTOSINK_DATAPACKET";
    }
    return "None";
}

#endif /* HELPERMETHODS_H_ */
