/***************************************************************************
 * file:        RPL.h
 *
 * author:      Hamidreza Kermajani
 *
 * copyright:   (C) 2013 UPC, Castelldefels, Spain.
 *
 * description: Implementation of the IPv6 Routing Protocol for Low power
 *              and Lossy Networks (RPL).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 *
 ***************************************************************************
 * last modification: 09/29/2013
 **************************************************************************/

#ifndef RPL_h
#define RPL_h

#include <map>
#include <omnetpp.h>

#include "MiXiMDefs.h"
#include "BaseNetwLayer.h"
#include "SimpleAddress.h"
#include "SimpleBattery.h"
#include "DIOMsg_m.h"
#include "DISMessage_m.h"

class SimTracer;
class DIOMsg;
class DISMessage;

struct DIOState{
    int version;
    int Sent;
    int Received;
    int Suppressed;
};

struct DISState{
    int Sent;
    int Received;
    int Suppressed;
};

struct NodeState{
    int Version;
    int Collision;
    int PacketLost;
    struct DIOState DIO;
    struct DISState DIS;

    double *PowerConsumption;
    int *Rank;
    simtime_t *JoiningDODAGTime;
    simtime_t DODAGsFormationTimeRecords;
    NodeState* Link;
}*NodeStateHeader=NULL,*NodeStateLast=NULL,*NodeStateNew=NULL;



/**
 * @brief IPv6 Routing Protocol for LLNs (RPL)provides a mechanism whereby
 * multipoint-to-point traffic from devices inside the LLN towards a central
 * control point is supported.
 *
 *
 * @ingroup netwLayer
 * @author Hamidreza Kermajani
 **/
class MIXIM_API RPL : public BaseNetwLayer
{
public:
    /** @brief Copy constructor is not allowed.
     */
    RPL(const RPL&);
    /** @brief Assignment operator is not allowed.
     */
    RPL& operator=(const RPL&);


    RPL()
        : BaseNetwLayer()
        , DIOheaderLength(0)
        , DISheaderLength(0)
        , macaddress()
        , sinkAddress()
        , debug(false)
    {};

    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);
    virtual void finish();
    friend NodeState* CreateNewNodeState(int Index, int VersionNo, simtime_t Time, int NodeRank);
    virtual ~RPL();

    enum messagesTypes {
        UNKNOWN=0,
        DATA,
        Global_REPAIR_TIMER,
        DIO,
        SEND_DIO_TIMER,
        DIS_UNICAST,
        SEND_DIS_UNICAST_TIMER,
        DIS_FLOOD,
        SEND_DIS_FLOOD_TIMER,
        RESET_Global_REPAIR_TIMER,
    };

    struct Route_Table
    {
        LAddress::L3Type Address;
        LAddress::L3Type NextHop;
        struct Route_Table* Link;
    }*RoutingTable;
    int NofEntry;


    /**
     * @brief Length of the NetwPkt header
     * Read from omnetpp.ini
     **/
    int DIOheaderLength;
    int DISheaderLength;
    int headerLength;


    LAddress::L2Type macaddress;

    LAddress::L3Type sinkAddress;

    bool useSimTracer;
    bool trace, stats, debug;
    double rssiThreshold;


    cMessage* GRepairTimer;
    cMessage* DIOTimer;
    cMessage* DISTimer;

    struct DIOStatus
    {
        unsigned int nbDIOSent;
        unsigned int nbDIOReceived;
        unsigned int nbDIOSuppressed;
        int version;
        struct DIOStatus* link;
    }*DIOStatusHeader,*DIOStatusNew,*DIOStatusLast;

    struct DISStatus
    {
        unsigned int nbDISSent;
        unsigned int nbDISReceived;
        unsigned int nbDISSuppressed;
        struct DISStatus* link;
    }*DISStatusHeader,*DISStatusNew,*DISStatusLast;


    char *FilePath;
    bool IsJoined;
    bool DISEnable;

    double GlobalRepairTimer;

    double DIOIntMin;
    int DIORedun;
    int DIOIntDoubl;
    simtime_t DIOIMaxLength;

    double DISIntMin;
    double DISStartDelay;
    int DISRedun;
    int DISIntDoubl;
    simtime_t DISIMaxLength;
    int DISVersion;



    LAddress::L3Type DODAGID;
    int Rank;
    simtime_t NodeStartTime;
    int VersionNember;
    int Grounded;
    simtime_t TimetoSendDIO;
    simtime_t TimetoSendDIS;
    struct DODAGJoiningtime{
        simtime_t TimetoJoinDODAG;
        int version;
        struct DODAGJoiningtime* link;
    }*DODAGJoinTimeHeader,*DODAGJoinTimeLast,*DODAGJoinTimeNew;

    int DIO_c;
    simtime_t DIO_CurIntsizeNow,DIO_CurIntsizeNext;
    simtime_t DIO_StofCurIntNow,DIO_StofCurIntNext;
    simtime_t DIO_EndofCurIntNow,DIO_EndofCurIntNext;

    int DIS_c;
    simtime_t DIS_CurIntsizeNow,DIS_CurIntsizeNext;
    simtime_t DIS_StofCurIntNow,DIS_StofCurIntNext;
    simtime_t DIS_EndofCurIntNow,DIS_EndofCurIntNext;


    LAddress::L3Type PrParent;
    int *NofParents;
    int MaxNofParents;
    struct ParentStructure{
        LAddress::L3Type ParentId;
        int ParentRank;
    }**Parents;
    enum PARENT_TYPES
    {
      NOT_EXIST,
      EXIST,
      SHOULD_BE_UPDATED,
    };

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage* msg);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage* msg);

    /** @brief Handle self messages */
    virtual void handleSelfMsg(cMessage* msg);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage* msg);


    /** @brief Decapsulate a message */
    cMessage* decapsMsg(DIOMsg *msg);

    /** @brief scheduling next DIO message transmission. */
    virtual void scheduleNextDIOTransmission();
    void scheduleNextDISTransmission();
    void ScheduleNextGlobalRepair();
    void DeleteScheduledNextGlobalRepair();

    void TrickleReset();
    void DeleteDIOTimer();
    void SetDIOParameters();
    void SetDISParameters();

    virtual int  IsParent(const LAddress::L3Type& id,int idrank);
    virtual void AddParent(const LAddress::L3Type& id,int idrank);
    virtual void DeleteParent(const LAddress::L3Type& id);

    DIOStatus* CreateNewVersionDIO(void);
    DISStatus* CreateNewVersionDIS(void);
    void DISHandler();
    DODAGJoiningtime* CreateNewVersionJoiningTime(void);

};

#endif

