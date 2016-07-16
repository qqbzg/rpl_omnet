<#if protocolName=="802.11">
<#assign nicIni="80211.ini.fti">
<#elseif protocolName=="CSMA Generic">
<#assign nicIni="CSMA.ini.fti">
<#elseif protocolName=="CSMA 802.15.4">
<#assign nicIni="802154.ini.fti">
<#elseif protocolName=="CSMA using old CSMAMacLayer">
<#assign nicIni="CSMAMacLayer.ini.fti">
<#elseif protocolName=="B-MAC">
<#assign nicIni="bmac.ini.fti">
<#elseif protocolName=="L-MAC">
<#assign nicIni="lmac.ini.fti">
</#if>

<#if applName="Traffic Generator">
<#assign applIni="TrafficGen.ini.fti">
<#elseif applName="Sensor Application Layer">
<#assign applIni="SensorApplLayer.ini.fti">
<#else>
<#assign applIni="BurstApplLayer.ini.fti">
</#if>

<#if netwName="Adaptive probabilistic broadcast">
<#assign netwIni="AdaptiveProbBroadcast.ini.fti">
<#elseif netwName="Probabilistic broadcast">
<#assign netwIni="ProbBroadcast.ini.fti">
<#elseif netwName="RPL">
<#assign netwIni="RPL.ini.fti">
<#elseif netwName="Wise route">
<#assign netwIni="WiseRoute.ini.fti">
<#elseif netwName="Flooding">
<#assign netwIni="Flooding.ini.fti">
<#else>
<#assign netwIni="BaseNetwLayer.ini.fti">
</#if>

<#if mobilityName="Constant speed">
<#assign mobIni="ConstSpeedMobility.ini.fti">
<#else>
<#assign mobIni="BaseMobility.ini.fti">
</#if>

[General]

cmdenv-express-mode = true
network = ${targetTypeName}


##########################################################
#			Simulation parameters                        #
##########################################################
**.phy.coreDebug = false
**.coreDebug = false
**.debug = false
##########################################################
**.playgroundSizeX = 20m
**.playgroundSizeY = 20m
**.playgroundSizeZ = 0m #ignored when use2D
**.numNodes = 14
**.SinkNodeAddr = 0

##########################################################
#			WorldUtility parameters                      #
##########################################################
**.world.useTorus = false
<#if dimensions="3-dimensional">
**.world.use2D = false
<#else>
**.world.use2D = true
</#if>

<#--########### Protocoll dependent include#############-->
<#include nicIni>

################ Application layer parameters ############
<#include applIni>

################ NETW layer parameters ###################
<#include netwIni>

################ Mobility parameters #####################
<#include mobIni>


#################### Random position #####################

**.node[*].mobility.UseRandomTopology=true

#################### Nodes positions #####################



**.node[0].mobility.initialX = 20m
**.node[0].mobility.initialY = 0m
**.node[0].mobility.initialZ = 0m

**.node[1].mobility.initialX = 17m
**.node[1].mobility.initialY = 5m
**.node[1].mobility.initialZ = 0m

**.node[2].mobility.initialX = 23m
**.node[2].mobility.initialY = 5m
**.node[2].mobility.initialZ = 0m

**.node[3].mobility.initialX = 10m
**.node[3].mobility.initialY = 8m
**.node[3].mobility.initialZ = 0m

**.node[4].mobility.initialX = 30m
**.node[4].mobility.initialY = 8m
**.node[4].mobility.initialZ = 0m

**.node[5].mobility.initialX = 20m
**.node[5].mobility.initialY = 5m
**.node[5].mobility.initialZ = 0m
