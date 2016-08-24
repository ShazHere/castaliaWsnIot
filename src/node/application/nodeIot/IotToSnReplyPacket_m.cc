//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/nodeIot/IotToSnReplyPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "IotToSnReplyPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

iotInfo::iotInfo()
{
    nodeID = 0;
    locX = 0;
    locY = 0;
    speed = 0;
    broadcastPower = 0;
    direction = 0;
}

void doPacking(cCommBuffer *b, iotInfo& a)
{
    doPacking(b,a.nodeID);
    doPacking(b,a.locX);
    doPacking(b,a.locY);
    doPacking(b,a.speed);
    doPacking(b,a.broadcastPower);
    doPacking(b,a.direction);
}

void doUnpacking(cCommBuffer *b, iotInfo& a)
{
    doUnpacking(b,a.nodeID);
    doUnpacking(b,a.locX);
    doUnpacking(b,a.locY);
    doUnpacking(b,a.speed);
    doUnpacking(b,a.broadcastPower);
    doUnpacking(b,a.direction);
}

class iotInfoDescriptor : public cClassDescriptor
{
  public:
    iotInfoDescriptor();
    virtual ~iotInfoDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(iotInfoDescriptor);

iotInfoDescriptor::iotInfoDescriptor() : cClassDescriptor("iotInfo", "")
{
}

iotInfoDescriptor::~iotInfoDescriptor()
{
}

bool iotInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<iotInfo *>(obj)!=NULL;
}

const char *iotInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int iotInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int iotInfoDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *iotInfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "nodeID",
        "locX",
        "locY",
        "speed",
        "broadcastPower",
        "direction",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int iotInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeID")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "locX")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "locY")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+3;
    if (fieldName[0]=='b' && strcmp(fieldName, "broadcastPower")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "direction")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *iotInfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned short",
        "double",
        "double",
        "double",
        "double",
        "int",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *iotInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int iotInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    iotInfo *pp = (iotInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string iotInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    iotInfo *pp = (iotInfo *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->nodeID);
        case 1: return double2string(pp->locX);
        case 2: return double2string(pp->locY);
        case 3: return double2string(pp->speed);
        case 4: return double2string(pp->broadcastPower);
        case 5: return long2string(pp->direction);
        default: return "";
    }
}

bool iotInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    iotInfo *pp = (iotInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nodeID = string2ulong(value); return true;
        case 1: pp->locX = string2double(value); return true;
        case 2: pp->locY = string2double(value); return true;
        case 3: pp->speed = string2double(value); return true;
        case 4: pp->broadcastPower = string2double(value); return true;
        case 5: pp->direction = string2long(value); return true;
        default: return false;
    }
}

const char *iotInfoDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *iotInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    iotInfo *pp = (iotInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(IotToSnReplyPacket);

IotToSnReplyPacket::IotToSnReplyPacket(const char *name, int kind) : ::ApplicationPacket(name,kind)
{
}

IotToSnReplyPacket::IotToSnReplyPacket(const IotToSnReplyPacket& other) : ::ApplicationPacket(other)
{
    copy(other);
}

IotToSnReplyPacket::~IotToSnReplyPacket()
{
}

IotToSnReplyPacket& IotToSnReplyPacket::operator=(const IotToSnReplyPacket& other)
{
    if (this==&other) return *this;
    ::ApplicationPacket::operator=(other);
    copy(other);
    return *this;
}

void IotToSnReplyPacket::copy(const IotToSnReplyPacket& other)
{
    this->extraData_var = other.extraData_var;
}

void IotToSnReplyPacket::parsimPack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimPack(b);
    doPacking(b,this->extraData_var);
}

void IotToSnReplyPacket::parsimUnpack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimUnpack(b);
    doUnpacking(b,this->extraData_var);
}

iotInfo& IotToSnReplyPacket::getExtraData()
{
    return extraData_var;
}

void IotToSnReplyPacket::setExtraData(const iotInfo& extraData)
{
    this->extraData_var = extraData;
}

class IotToSnReplyPacketDescriptor : public cClassDescriptor
{
  public:
    IotToSnReplyPacketDescriptor();
    virtual ~IotToSnReplyPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(IotToSnReplyPacketDescriptor);

IotToSnReplyPacketDescriptor::IotToSnReplyPacketDescriptor() : cClassDescriptor("IotToSnReplyPacket", "ApplicationPacket")
{
}

IotToSnReplyPacketDescriptor::~IotToSnReplyPacketDescriptor()
{
}

bool IotToSnReplyPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<IotToSnReplyPacket *>(obj)!=NULL;
}

const char *IotToSnReplyPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int IotToSnReplyPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int IotToSnReplyPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *IotToSnReplyPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "extraData",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int IotToSnReplyPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "extraData")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *IotToSnReplyPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "iotInfo",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *IotToSnReplyPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int IotToSnReplyPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    IotToSnReplyPacket *pp = (IotToSnReplyPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string IotToSnReplyPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    IotToSnReplyPacket *pp = (IotToSnReplyPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getExtraData(); return out.str();}
        default: return "";
    }
}

bool IotToSnReplyPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    IotToSnReplyPacket *pp = (IotToSnReplyPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *IotToSnReplyPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(iotInfo));
        default: return NULL;
    };
}

void *IotToSnReplyPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    IotToSnReplyPacket *pp = (IotToSnReplyPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getExtraData()); break;
        default: return NULL;
    }
}


