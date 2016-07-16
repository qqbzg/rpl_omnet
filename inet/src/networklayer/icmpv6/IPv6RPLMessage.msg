//
// Copyright (C) 2005 Wei Yang, Ng
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, see <http://www.gnu.org/licenses/>.
//

cplusplus {{
#include "ICMPv6Message_m.h"
#include "IPv6Address.h"
#include "MACAddress.h"
}}


packet ICMPv6Message;

class noncobject IPv6Address;

class noncobject MACAddress;

class IPv6RPLPrefixInformation
{
    unsigned short prefixLength;
    IPv6Address prefix;
}

packet IPv6RPLMessage extends ICMPv6Message
{
	    //customize=true;
	    int type = 155;
}

packet DIO extends IPv6RPLMessage
{
        int RPLInstanceID;        // The ID of the RPL instance
        int VersionNumber;        // DODAG version number
        double Rank;                 // Node's rank
        int MOP;                  // Mode of operation: 0 indicates no downward routing,nodes mustn' t transmit DAO message
        int Grounded;             // Type of the DODAG, Grounded or Flooding		   
        int DTSN;                 // Destination Advertisement Trigger Sequence Number       
       
       //DODAG Configuration Option (ralated to trikle timer)
        double IMin;              // The size of Imin in Trcikle algorithm
        int NofDoub;              // Number of doubling in Trcikle algorithm
        int k;                    // Redundancy constant in Trcikle algorithm 
        
        
        double Lifetime;                  // Path lifetime
        IPv6Address DODAGID;              // IPv6 address set by DODAG root
        IPv6Address finalDestAddr;   	
        IPv6Address initialSrcAddr;
        MACAddress initialSrcMACAddr;
        int SrID;
        int numSent;
        IPv6RPLPrefixInformation prefixInformation[];
        
        double metrics;
        
        double metrics_t;
        double metrics_plus;
        
        int hopcount;    //Here we need to record this parameter to further assist the metric defination
}

packet DIS extends IPv6RPLMessage
{
	    int RPLInstanceID;        // The ID of the RPL instance
		int VersionNumber;        // DODAG version number
	    IPv6Address DODAGID;              // IPv6 address set by DODAG root
		IPv6Address finalDestAddr;   	
		IPv6Address initialSrcAddr;
		
}

packet DAO extends IPv6RPLMessage
{
        int RPLInstanceID;        // The ID of the RPL instance
        int VersionNumber; 
        IPv6Address DODAGID;    
        IPv6Address finalDestAddr;   	
		IPv6Address initialSrcAddr;
		MACAddress finalDestMACAddr;
		MACAddress initialMACAddr;
		unsigned short RouterLifetime;
}