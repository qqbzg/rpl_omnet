/**
 * This function is added for a new metric which
 * similar to transmission times. It is a counter
 * about the node from which received most msg, this
 * one will be taken as a parent mode.
 */
#ifndef OBJECTIVEFUN1_H
#define OBJECTIVEFUN1_H

#include "INETDefs.h"
#include "IPv6Address.h"
#include "MACAddress.h"

class IInterfaceTable;

class INET_API ObjectiveFun1{
protected:
    struct ParentStructure {
        MACAddress ParentMACAddr;
        IPv6Address ParentId;
        int srnode;
        double Rcvdcounter;
        double recordCounter;
        int ParentRank;
    };

    bool IsJoined;
    IInterfaceTable *ift;
public:
    //Added after May to cache the count of rcvd msg from specific src Addr

    typedef std::vector<ParentStructure*> CounterCache;
    CounterCache countercache;

    enum PARENT_TYPES {
            NOT_EXIST, EXIST, SHOULD_BE_UPDATED,
        };
    ObjectiveFun1()
    {
        prfparent = IPv6Address();
        prfMACparent =MACAddress();
    };
    virtual int IsParent(const IPv6Address& srcAddr, double rank);
    virtual double UpdateParent(double counter, int srID, const IPv6Address& srcAddr,int rank, const MACAddress& srcMACAddr);
    virtual void test();
    /***********for sake of of425, added this counter function*************/
    virtual double counterCal(int srID, int NumDio);
    virtual int counterSetting(int srID);
    static bool counterLessThan(const ParentStructure *a, const ParentStructure *b);
    int Rank;
    double metrics;
    int PRNodeID;
    IPv6Address prfparent;  //even when it is a set, the size of elements is 3.
    MACAddress prfMACparent;
};

#endif
