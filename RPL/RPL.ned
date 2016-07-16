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

package rpl;
import org.mixim.base.connectionManager.IConnectionManager;
import org.mixim.base.modules.BaseWorldUtility;

//import rpl.Host802154;

network RPL
{
    parameters:
        int numNodes;     // total number of hosts in the network
        int SinkNodeAddr; // Sink node address
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        string cmType = default("org.mixim.base.connectionManager.ConnectionManager"); // connection manager to use
        @display("bgb=0,0");
    submodules:
        world: BaseWorldUtility {
            parameters:

                @display("p=37,25;is=s");
        }
        connectionManager: <cmType> like IConnectionManager {
            parameters:
                @display("p=280,100;i=abstract/multicast;is=s");
        }
        node[numNodes]: Host802154 {
            @display("p=117,95;is=s");
        }
    connections allowunconnected:
    // all connections and gates are to be generated dynamically
}
