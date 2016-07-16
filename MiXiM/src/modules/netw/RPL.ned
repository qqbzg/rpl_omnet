//***************************************************************************
// * file:        RPL.ned
// *
// * author:      Hamidreza Kermajani
// *
// * copyright:   (C) 2013 UPC, Castelldefels, Spain.
// *
// * description: Implementation of the IPv6 Routing Protocol for Low power
// *              and Lossy Networks (RPL).
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// *
// *
// *
// ***************************************************************************
// * last modification: 09/29/2013
// **************************************************************************/

package org.mixim.modules.netw;
import org.mixim.base.modules.BaseNetwLayer;
simple RPL extends BaseNetwLayer
{
    parameters:
        int DIOheaderLength @unit(byte) = default(73 Byte);
        int DISheaderLength @unit(byte) = default(54 Byte);
        // debug switch
        bool debug = default(false);
        bool trace = default(false);
     	bool useSimTracer = default(false);
        // RSSI threshold for route selection
        double rssiThreshold @unit(dBm) = default(-50 dBm);
     	

        double DIOIntMin @unit(s) = default(0.008 s);
        int DIORedun= default(10);
        int DIOIntDoubl= default(20);

        bool DISEnable = default(false);
        double DISStartDelay = default(0);
        double DISIntMin @unit(s) = default(0.03s);
        int DISRedun= default(1);
        int DISIntDoubl= default(0);
        
        
        double GlobalRepairTimer @unit(s) = default(10000 s);
        int MaxNofParents= default(5);
        int NumberofIterations = default(20);
        string FilePath;

        
        @display("i=block/fork");
        @class(RPL);

}
