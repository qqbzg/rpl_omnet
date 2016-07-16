
cplusplus {{
#include "NetwPkt_m.h"
}}

class NetwPkt;
class LAddress::L3Type extends void;

message DIOMsg extends NetwPkt
{
    int VersionNumber;        // DODAG version number
    int Rank;                 // Node's rank
    int Grounded;             // Type of the DODAG, Grounded or Flooding
    int DTSN;                 // Destination Advertisement Trigger Sequence Number       
    double IMin;              // The size of Imin in Trcikle algorithm
    int NofDoub;              // Number of doubling in Trcikle algorithm
    int k;                    // Redundancy constant in Trcikle algorithm 
    double Lifetime;          // Path lifetime
    LAddress::L3Type DODAGID;              // IPv6 address set by DODAG root 
	LAddress::L3Type finalDestAddr;   
 	LAddress::L3Type initialSrcAddr;
}
