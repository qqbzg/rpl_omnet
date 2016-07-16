
cplusplus {{
#include "NetwPkt_m.h"
}}

class NetwPkt;
class LAddress::L3Type extends void;

message DISMessage extends NetwPkt
{
    int RPLInstanceID;          // The ID of the RPL instance
    int VersionNumber;          // DODAG version number
    int V;                      // Node's rank
    int I;                      // Type of the DODAG, Grounded or Flooding
    int D;                      // Destination Advertisement Trigger Sequence Number       
    int Flag;                   // The size of Imin in Trcikle algorithm
    LAddress::L3Type DODAGID;   // IPv6 address set by DODAG root 
	LAddress::L3Type finalDestAddr;   
 	LAddress::L3Type initialSrcAddr;
}
