/**
 * This function is added for a new metric which similar to transmission times. It is a counter
 * about the node from which received most msg. However, it is an accumulative metric rather than
 * a recorded one. Hope it will work.
 */
#ifndef OBJECTIVEFUNFUZZ_H
#define OBJECTIVEFUNFUZZ_H

#include "INETDefs.h"
#include "IPv6Address.h"
#include "MACAddress.h"

class IInterfaceTable;

class INET_API ObjectiveFunFuzz{
protected:
    int NofParents;
    int MaxNofParents;
    struct ParentStructure {
        MACAddress ParentMACAddr;
        IPv6Address ParentId;
        int srnode;
        double Crisp_value = 1;
        double ParentRank;
        // The below parameters are used to calculate the addictive ETX which its result turns out to be metrics_t
        double Rcvdcounter;
        double Addcounter = 1;
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
    ObjectiveFunFuzz()
    {
        prfparent = IPv6Address();
        prfMACparent = MACAddress();
    };
    virtual int IsParent(const IPv6Address& srcAddr, double rank);
    /***********for sake of of425, added this counter function*************/
    virtual double UpdateParent(double crisp_value, int srID, const IPv6Address& srcAddr,int rank, const MACAddress& srcMACAddr);
    virtual double counterCal(int srID, double metrics_t, double metrics_plus);
    virtual double addictiveMetrics_t(int srID, double precount, int NumDio);
    virtual void test();
    static bool counterLessThan(const ParentStructure *a, const ParentStructure *b);
    virtual std::vector<double> fuzzy(double metrics_t);
    virtual std::vector<double> fuzzy1(double metrics_t);   //composite metrics, we need a second fuzzy input
    virtual std::vector<double> mamdani(std::vector<double> fuzz, std::vector<double> fuzz1);
    virtual double deffuzification(std::vector<double> mamdani);
    virtual double fuzzsystem(double metrics_t, double metrics_plus);
    double Rank = 1;
    double metrics = 1;
    //below metrics_t and metrics_plus are two metrics to be composited
    double metrics_t = 0;
    double metrics_plus = 1;

    int PRNodeID;
    IPv6Address prfparent;
    //even when it is a set, the size of elements is 3. Waiting for extension when multi-parents are allowed.
    MACAddress prfMACparent;
};

#endif
