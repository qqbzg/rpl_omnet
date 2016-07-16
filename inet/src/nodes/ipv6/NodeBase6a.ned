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
import inet.base.NotificationBoard;
//import inet.battery.IBattery;
import inet.linklayer.IWirelessNic;
import inet.mobility.IMobility;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv6.RoutingTable6;
import inet.status.NodeStatus;


//
// BaseHost contains the common lower layers (linklayer and networklayer)
// of Router, StandardHost, WirelessHost etc.
//
// NOTE: - IPForward is turned on by default.
//       - use this model only as a base type. In simulations use Router, StandardHost,
//         WirelessHost and AndhocHost
//
module NodeBase6a
{
    parameters:
        @display("bgb=628,446");
        @node;
        int numRadios = default(0);               // the number of radios in the router. by default no wireless
        string mobilityType = default("StationaryMobility");
        string batteryType = default("InetSimpleBattery");
        bool hasStatus = default(false);
        xml routingTable = default(xml("<routingTable/>"));
        bool IPForward = default(true);
    gates:
        input radioIn[numRadios] @directIn;
    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=53,155;is=s");
        }

        // optional mobility module. Required only if wireless cards are present
        mobility: <mobilityType> like IMobility if mobilityType != "" && numRadios > 0 {
            parameters:
                @display("p=53,111");
        }

        // network layer
        networkLayer: NetworkLayer6c {
            parameters:
                @display("p=329,287;q=queue");
        }

        routingTable6: RoutingTable6 {
            parameters:
                @display("p=53,225;is=s");
                isRouter = IPForward;
                routingTable = routingTable;
        }
//        battery: <batteryType> like IBattery if batteryType != "" {
//            parameters:
//                @display("p=53,77;i=block/plug;is=s");
//        }

        status: NodeStatus if hasStatus {
            @display("p=50,50");
        }

        // linklayer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=53,287;is=s");
        }

        wlan[numRadios]: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=329,383;q=queue");
        }
    connections allowunconnected:
        for i=0..sizeof(radioIn)-1 {
            radioIn[i] --> wlan[i].radioIn;
            wlan[i].upperLayerOut --> networkLayer.ifIn++;
            wlan[i].upperLayerIn <-- networkLayer.ifOut++;
        }

}

