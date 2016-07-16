//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package inet.nodes.ipv6;

import inet.networklayer.ipv6.IPv6ErrorHandling;
import inet.networklayer.ipv6.IPv6;
import inet.networklayer.icmpv6.IPv6NeighbourDiscovery;
import inet.networklayer.icmpv6.ICMPv6;
import inet.networklayer.IxMIPv6Support;
import inet.networklayer.IIPv6Tunneling;
import inet.base.IHook;


//
// Represents an IPv6 network layer (L3).
//
// The module has ports to connect to a higher layer (TCP, UDP) and
// several network interfaces.
//
module NetworkLayer6b
{
    parameters:
        bool xMIPv6Support = default(false);
        @display("i=block/fork");
    gates:
        input ifIn[] @labels(IPv6Datagram);
        output ifOut[] @labels(IPv6Datagram);
        input udpIn @labels(UDPPacket,IPv6ControlInfo/down);
        output udpOut @labels(UDPPacket,IPv6ControlInfo/up);
   

    submodules:
        ipv6: IPv6 {
            parameters:
                protocolMapping = "6:0,17:1,46:2,89:3,132:4"; // TODO register 132 as sctp in ipv6 code
                @display("p=84,129");
            gates:
                transportIn[1];
                transportOut[1];
                queueIn[sizeof(ifIn)];
                queueOut[sizeof(ifIn)];
        }
        icmpv6: ICMPv6 {
            parameters:
                @display("p=189,79");
        }
        ipv6ErrorHandling: IPv6ErrorHandling {
            parameters:
                @display("p=280,79");
        }
        neighbourDiscovery: IPv6NeighbourDiscovery {
            parameters:
                @display("p=257,149");
        }
        
    connections allowunconnected:  // FIXME remove 'nocheck'!
        // IPv6 to transport Layer
       
        ipv6.transportOut[0] --> { @display("m=n"); } --> udpOut;
        ipv6.transportIn[0] <-- { @display("m=n"); } <-- udpIn;


    

        // IPv6 to ICMPv6
        ipv6.icmpOut --> icmpv6.ipv6In;
        ipv6.icmpIn <-- icmpv6.ipv6Out;

        // ICMPv6 to IPv6ErrorHandling
        icmpv6.errorOut --> ipv6ErrorHandling.in;

       

        // IPv6 to Neighbour Discovery
        ipv6.ndOut --> neighbourDiscovery.ipv6In;
        ipv6.ndIn <-- neighbourDiscovery.ipv6Out;

        // IPv6 to L2
        for i=0..sizeof(ifIn)-1 {
            ifIn[i] --> { @display("m=s"); } --> ipv6.queueIn[i];
        }
        for i=0..sizeof(ifOut)-1 {
            ipv6.queueOut[i] --> { @display("m=s"); } --> ifOut[i];
        }
}
