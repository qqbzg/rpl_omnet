<#if protocolName=="CSMA 802.15.4">
<#if nedPackageName!="">package ${nedPackageName};</#if>

import org.mixim.modules.node.WirelessNodeBatteryPlusTran;

module Host802154 extends WirelessNodeBatteryPlusTran
{
    parameters:
        arpType       = "ArpHost";
        nicType       = "Nic802154_TI_CC2420";
        transportType = "Aggregation";
        double NodeStartTime =0; //uniform(0.0,0.008*pow(2,20))/1.0;//uniform(0.0,10)/1.0 
}
</#if>
