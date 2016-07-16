<?xml version="1.0" encoding="UTF-8"?>
<root>
	<AnalogueModels>
<#if protocolName=="CSMA 802.15.4">
		<AnalogueModel type="MyModel">
		    <!-- My channel model -->
	    	<parameter name="alpha" type="double" value="3.5"/>
	    	<parameter name="ReferenceDistance" type="double" value="1.0"/>
	    	<parameter name="carrierFrequency" type="double" value="2.4E+9"/>
	    </AnalogueModel>
		<!-- we add a log-normal shadowing effect on top of the MyModel(FreeSpace) path loss --> 
	    <AnalogueModel type="LogNormalShadowing">
	    	<!-- Mean attenuation in dB -->
	    	<parameter name="mean" type="double" value="0.0"/>
	    	<!-- Standart deviation of the attenuation in dB -->
	    	<parameter name="stdDev" type="double" value="6.0"/>
	    	<!-- Interval in which to define attenuation for in seconds -->
	    	<parameter name="interval" type="double" value="0.001"/>
	    </AnalogueModel>
<#else>
		<AnalogueModel type="SimplePathlossModel">
	    	<parameter name="alpha" type="double" value="3.0"/>
	    	<parameter name="carrierFrequency" type="double" value="2.412e+9"/>
	    </AnalogueModel>
</#if>
	</AnalogueModels>
<#if protocolName=="802.11">
	<Decider type="Decider80211">
		<!-- SNR threshold [NOT dB]-->
		<parameter name="threshold" type="double" value="0.12589254117942"/>
		
		<!-- The center frequency on whcih the phy listens-->
		<parameter name="centerFrequency" type="double" value="2.412e9"/>
	</Decider>
<#elseif protocolName=="CSMA 802.15.4">
	<Decider type="Decider802154Narrow">
		 <!--Length of Start Frame Delimiter (used to compute probability of successful 
			synchronization)-->
		 <parameter name="sfdLength" type="long" value="8"/>
		
	   	 <!--minimum possible bit error rate (BER floor)-->
		 <parameter name="berLowerBound" type="double" value="1e-8"/>
		
		 <!--modulation type-->
		 <parameter name="modulation" type="string" value="oqpsk16"/>
	</Decider>	
<#else>	
	<Decider type="SNRThresholdDecider">
		<!-- SNR threshold (as fraction) above which the decider consideres a
			 a signal as received correctly. -->
		<parameter name="snrThreshold" type="double" value="0.12589254117942"/>
		<!-- RSSI (noise and signal) threshold (in mW) above which the 
			 channel is considered idle. If this parameter is
			 ommited the sensitivity of the physical layer is
			 used as threshold.-->
		<parameter name="busyThreshold" type="double" value="3.98107170553E-9"/>
	</Decider>
</#if>
</root>