#ifndef OBJECTIVEFUN0_H
#define OBJECTIVEFUN0_H

#include "INETDefs.h"
#include "IPv6Address.h"
#include "MACAddress.h"

class IInterfaceTable;

class INET_API ObjectiveFun0{
protected:
    struct ParentStructure {
        MACAddress ParentMACAddr;
        IPv6Address ParentId;
        int srnode;
        int PRNodeID;
        int ParentRank;
    };

    bool IsJoined;
    IInterfaceTable *ift;
public:
    enum PARENT_TYPES {
            NOT_EXIST, EXIST, SHOULD_BE_UPDATED,
        };
    ObjectiveFun0()
    {
        prfparent = IPv6Address();
        prfMACparent =MACAddress();
    };
    typedef std::vector<ParentStructure*> CounterCache;
    CounterCache countercache;

    virtual int IsParent(const IPv6Address& srcAddr, double rank);
    virtual double UpdateParent(double metrics_hop, int srID, const IPv6Address& srcAddr,double rank, const MACAddress& srcMACAddr);
    virtual void test();
    static bool counterLessThan(const ParentStructure *a, const ParentStructure *b);


    double Rank;
    double metrics;
    int PRNodeID;
    IPv6Address prfparent;  //even when it is a set, the size of elements is 3.
    MACAddress prfMACparent;
};

#endif
