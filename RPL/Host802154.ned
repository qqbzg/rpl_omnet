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

import inet.nodes.ipv6.WirelessHost6;
import org.mixim.modules.power.battery.BatteryStats;
import org.mixim.modules.power.battery.SimpleBattery;


// This host is an example on how a host with INET network stack
// can be equipped with mobility- and wireless-modules from MiXiM. 
//
// It contains an INET network stack and a MiXiM 802.15.4 NIC (TI_CC2420)
// as well as a battery module from MiXiM which is used by the NIC.
//
// This host's NIC is able to operate in a multi-ConnectionManager
// network, i.e. more than one physical network in parallel (non-interfering).
// TODO: factor out battery into INET
module Host802154 extends WirelessHost6
{
    submodules:
        batteryStats: BatteryStats {
            @display("p=135,42;i=block/circle");
        }
        simbattery: SimpleBattery {
            @display("p=135,119;i=block/control");
        }
}
