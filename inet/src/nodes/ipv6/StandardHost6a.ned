package inet.nodes.ipv6;

import inet.applications.IPingApp;
import inet.applications.IUDPApp;
import inet.transport.IUDP;


//
// IPv6 host with TCP, SCTP and UDP layers and applications.
// see ~StandardHost for configuration.
//
module StandardHost6a extends NodeBase6a
{
    parameters:
        @display("i=device/pc2");
        int numUdpApps = default(0);  // no of UDP apps. Specify the app types in INI file with udpApp[0..1].typename="UDPVideoStreamCli" syntax
        int numPingApps = default(0);  // no of PING apps. Specify the app types in INI file with pingApp[0..1].typename="PingApp" syntax
        string udpType = default(firstAvailable("UDP","UDP_None"));
        IPForward = default(true);  // disable routing by default
    submodules:
        udpApp[numUdpApps]: <> like IUDPApp {
            parameters:
                @display("p=336,54,row,60");
        }
        udp: <udpType> like IUDP if numUdpApps>0 {
            parameters:
                @display("p=336,141");
        }
       
    connections allowunconnected:
        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

        udp.ipv6Out --> networkLayer.udpIn if numUdpApps>0;
        udp.ipv6In <-- networkLayer.udpOut if numUdpApps>0;

       
}
