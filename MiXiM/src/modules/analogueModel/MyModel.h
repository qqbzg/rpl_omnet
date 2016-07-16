/*
 * MyModel.h
 *
 *  Created on: Mar 23, 2013
 *      Author: Hamidreza Kermajani
 */

#ifndef MYMODEL_H_
#define MYMODEL_H_


#include <cstdlib>

#include "MiXiMDefs.h"
#include "AnalogueModel.h"
#include "BaseWorldUtility.h"

/**
 * @brief Basic implementation of a MyModel.
 * This class can be used to implement the Free space path loss model.
 *
 * @ingroup analogueModels
 */
class MIXIM_API MyModel : public AnalogueModel
{
protected:

//  /** @brief Model to use for distances below reference distance */
//  FreeSpaceModel closeRangeModel;


    /** @brief reference distance */
    double ReferenceDistance;

    /** @brief pathloss exponents */
    double PathLossAlpha;


    /** @brief Carrier frequency in Hz */
    double carrierFrequency;

    /** @brief Information needed about the playground */
    bool useTorus;

    /** @brief The size of the playground.*/
    Coord playgroundSize;

    /** @brief Whether debug messages should be displayed. */
    bool debug;


public:
    /**
     * @brief Initializes the analogue model. playgroundSize
     * need to be valid as long as this instance exists.
     *
     * The constructor needs some specific knowledge in order to create
     * its mapping properly:
     *
     * @param alpha the coefficient alpha (specified e.g. in config.xml and passed
     *              in constructor call)
     * @param carrierFrequency the carrier frequency
     * @param useTorus information about the playground the host is moving in
     * @param playgroundSize information about the playground the host is moving in
     * @param debug display debug messages?
     */
    MyModel()
        : AnalogueModel()
        , ReferenceDistance(ReferenceDistance)
        , PathLossAlpha(0)
        , carrierFrequency(carrierFrequency)
        , useTorus(useTorus)
        , playgroundSize(playgroundSize)
        , debug(debug)
    { }

    /** @brief Initialize the analog model from XML map data.
     *
     * This method should be defined for generic analog model initialization.
     *
     * @param params The parameter map which was filled by XML reader.
     *
     * @return true if the initialization was successfully.
     */
    virtual bool initFromMap(const ParameterMap&);

    /**
     * @brief Filters a specified AirFrame's Signal by adding an attenuation
     * over time to the Signal.
     */
    virtual void filterSignal(airframe_ptr_t, const Coord&, const Coord&);

};

#endif /* MYMODEL_H_ */
