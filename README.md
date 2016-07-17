# rpl_omnet
RPL deployment under OMNeT++

The project describes the RPL deployment under OMNeT++

It is based on INET 2.2.0 and MiXiM 2.3, therefore there are three directories - inet, MiXiM and RPL. The node structure in RPL project is constructed based on the network layer and upper layers in INET while the IEEE 802.15.4 MAC and PHY layers in MiXiM.

The overhead control is mainly realized in the IPv6NeighborDiscovery module. Objective Functions are set as a single class, which will be called in IPv6NeighborDiscovery. When it is necessary to define a new Objective Function, we can just add an independent objective function class and call it when making the parent selection and rank calculation. Objective functions are defined in inet/src/networklayer/icmpv6/. ObjectiveFun0 is for hop count metrics. ObjectiveFun2 is for addictive ETX metrics. ObjectiveFun3 is for modified ETX with RSSI taken into consideration. ObjectiveFunFuzz is an objective function which brings in fuzzification control into metrics design, it remains to be further learnt. 

We haven't taken into account the complicated calculation of rank. We just simplify this with the hop count.

DIO has been defined and realized for MP2P case. DAO and DIS mechanisms need to be further explored based on the interfaces we have defined.

The changes are mainly in the inet/src/icmpv6 and inet/src/ipv6. 
Minor changes will not be listed here. There are comments in the corresponding files.
