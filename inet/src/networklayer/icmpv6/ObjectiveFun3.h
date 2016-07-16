/**
 * This function is added for a new metric which similar to transmission times. It is a counter
 * about the node from which received most msg. However, it is an accumulative metric rather than
 * a recorded one. Hope it will work.
 */
#ifndef OBJECTIVEFUN3_H
#define OBJECTIVEFUN3_H

#include "INETDefs.h"
#include "IPv6Address.h"
#include "MACAddress.h"

class IInterfaceTable;

class INET_API ObjectiveFun3{
protected:
    int NofParents;
    int MaxNofParents;
    struct ParentStructure {
        MACAddress ParentMACAddr;
        IPv6Address ParentId;
        int srnode;
        double Rcvdcounter;
        double Addcounter = 1;
        double ParentRank;
    }*Parents;

    bool IsJoined;
    IInterfaceTable *ift;
public:
    //Added after July to cache the accumulative count of rcvd msg from specific src Addr

    typedef std::vector<ParentStructure*> CounterCache;
    CounterCache countercache;

    enum PARENT_TYPES {
            NOT_EXIST, EXIST, SHOULD_BE_UPDATED,
        };
    ObjectiveFun3()
    {
        prfparent = IPv6Address();
        prfMACparent = MACAddress();
    };
    virtual int IsParent(const IPv6Address& srcAddr, double rank);
    /***********for sake of of425, added this counter function*************/
    virtual double UpdateParent(double counter, int srID, const IPv6Address& srcAddr,int rank, const MACAddress& srcMACAddr);
    virtual double counterCal(int srID, double precount, int NumDio, double param);
    virtual void test();
    virtual int counterSetting(int srID);
    static bool counterLessThan(const ParentStructure *a, const ParentStructure *b);
    double Rank = 1;
    double metrics = 1;
    int PRNodeID;
    IPv6Address prfparent;
    //even when it is a set, the size of elements is 3. Waiting for extension when multi-parents are allowed.
    MACAddress prfMACparent;
};

#endif
