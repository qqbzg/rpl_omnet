/*
 * MyModel.cc
 *
 *  Created on: Mar 23, 2013
 *      Author: Hamidreza Kermajani
 */

#include "MyModel.h"

#include "MiXiMAirFrame.h"

#define debugEV (ev.isDisabled()||!debug) ? ev : ev << "PhyLayer(MyModel): "

bool MyModel::initFromMap(const ParameterMap& params) {
    ParameterMap::const_iterator it;
    bool                         bInitSuccess = true;

    if ((it = params.find("seed")) != params.end()) {
        srand( ParameterMap::mapped_type(it->second).longValue() );
    }
    if ((it = params.find("alpha")) != params.end()) {
        PathLossAlpha = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No alpha(PathLoss) defined in config.xml for MyModel!");
    }
    if ((it = params.find("ReferenceDistance")) != params.end()) {
        ReferenceDistance = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No ReferenceDistance defined in config.xml for MyModel!");
    }
    if ((it = params.find("carrierFrequency")) != params.end()) {
        carrierFrequency = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No carrierFrequency defined in config.xml for MyModel!");
    }
    if ((it = params.find("useTorus")) != params.end()) {
        useTorus = ParameterMap::mapped_type(it->second).boolValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No useTorus defined in config.xml for MyModel!");
    }
    if ((it = params.find("PgsX")) != params.end()) {
        playgroundSize.x = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No PgsX defined in config.xml for MyModel!");
    }
    if ((it = params.find("PgsY")) != params.end()) {
        playgroundSize.y = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No PgsY defined in config.xml for MyModel!");
    }
    if ((it = params.find("PgsZ")) != params.end()) {
        playgroundSize.z = ParameterMap::mapped_type(it->second).doubleValue();
    }
    else {
        bInitSuccess = false;
        opp_warning("No PgsZ defined in config.xml for MyModel!");
    }
    if ((it = params.find("debug")) != params.end()) {
        debug = ParameterMap::mapped_type(it->second).boolValue();
    }
    else {
        debug = false;
    }


    return AnalogueModel::initFromMap(params) && bInitSuccess;
}

void MyModel::filterSignal(airframe_ptr_t frame, const Coord& sendersPos, const Coord& receiverPos) {
    Signal& signal = frame->getSignal();

    /** Calculate the distance factor */
    double distance = useTorus ? receiverPos.sqrTorusDist(sendersPos, playgroundSize)
                                  : receiverPos.sqrdist(sendersPos);
    distance = sqrt(distance);
    debugEV << "distance is: " << distance << endl;

    if(distance <= 1.0) {
        //attenuation is negligible
        return;
    }

    debugEV << "\nPathLoss: " <<PathLossAlpha << "\t\t ReferenceDistance: " <<ReferenceDistance<< "\t\t CarrierFrequency: " <<carrierFrequency<<"\n\n";

    double wavelength = (BaseWorldUtility::speedOfLight/carrierFrequency);
    debugEV << "wavelength is: " << wavelength << endl;

    // PL_db = PL_d0_db + 10 * pathLossAlpha * log10(distance/d0) + Lognormal(mean, stdDev);
    // PL(d) = PL0 + 10 PathLossAlpha log10 (d/d0) + X

    // PL0 = Free Space path Loss at the reference distance d0 = 1,(d0 = 8m from IEEE standard 802.15.4 2006)
    double PL0 = ((16.0 * M_PI * M_PI) * pow(ReferenceDistance, PathLossAlpha)) / (wavelength * wavelength );

    //    double attenuation = PL0 + pow(distance/d0, PathLossAlpha ) + X;
    double attenuation = 1.0 ;
    attenuation = attenuation * PL0 ;
    attenuation = attenuation * pow(distance/ReferenceDistance, PathLossAlpha ) ;


    attenuation = 1.0 / attenuation;

//    EV << "attenuation is: " << attenuation<< endl;
    debugEV << "attenuation is: " << attenuation << endl;

    //const DimensionSet& domain = DimensionSet::timeDomain;
    Argument arg;   // default constructor initializes with a single dimension, time, and value 0 (offset from signal start)
    TimeMapping<Linear>* attMapping = new TimeMapping<Linear> ();   // mapping performs a linear interpolation from our single point -> constant
    attMapping->setValue(arg, attenuation);

    /* at last add the created attenuation mapping to the signal */
    signal.addAttenuation(attMapping);
}

