//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/nodeIot/IotToSnReplyPacket.msg.
//

#ifndef _IOTTOSNREPLYPACKET_M_H_
#define _IOTTOSNREPLYPACKET_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
 
#include "ApplicationPacket_m.h" 
// }}

/**
 * Struct generated from src/node/application/nodeIot/IotToSnReplyPacket.msg:23 by nedtool.
 */
struct iotInfo
{
    iotInfo();
    unsigned short nodeID;
    double locX;
    double locY;
    double speed;
    double broadcastPower;
    int direction;
};

void doPacking(cCommBuffer *b, iotInfo& a);
void doUnpacking(cCommBuffer *b, iotInfo& a);

/**
 * Class generated from <tt>src/node/application/nodeIot/IotToSnReplyPacket.msg:32</tt> by nedtool.
 * <pre>
 * packet IotToSnReplyPacket extends ApplicationPacket
 * {
 *     iotInfo extraData;
 * }
 * </pre>
 */
class IotToSnReplyPacket : public ::ApplicationPacket
{
  protected:
    iotInfo extraData_var;

  private:
    void copy(const IotToSnReplyPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const IotToSnReplyPacket&);

  public:
    IotToSnReplyPacket(const char *name=NULL, int kind=0);
    IotToSnReplyPacket(const IotToSnReplyPacket& other);
    virtual ~IotToSnReplyPacket();
    IotToSnReplyPacket& operator=(const IotToSnReplyPacket& other);
    virtual IotToSnReplyPacket *dup() const {return new IotToSnReplyPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual iotInfo& getExtraData();
    virtual const iotInfo& getExtraData() const {return const_cast<IotToSnReplyPacket*>(this)->getExtraData();}
    virtual void setExtraData(const iotInfo& extraData);
};

inline void doPacking(cCommBuffer *b, IotToSnReplyPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, IotToSnReplyPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef _IOTTOSNREPLYPACKET_M_H_

