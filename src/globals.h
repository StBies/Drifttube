/*
 * globals.h
 *
 *  Created on: 17.10.2016
 *      Author: stefan
 */


/**
 * Header containing globally available data constants, that are needed within the program.
 * These variables are used throughout any analysis where results are hardware-specific.
 *
 * @author Stefan
 * @date October 18, 2016
 * @version 0.8
 */
#ifndef GLOBALS_H_
#define GLOBALS_H_

//variables for FADC calibration:
static const double ADC_CHANNELS_TO_VOLTAGE = 2.0 / 4096; //V per channel
static const short ADC_BINS_TO_TIME = 4; //ns
static const unsigned short ADC_TRIGGERPOS_BIN = 0;
static const unsigned short OFFSET_ZERO_VOLTAGE = 2200; //channels

//variables describing the drift tube:
static const double DRIFT_TUBE_RADIUS = 18.15;//mm

//values for actual physics:
static const short EVENT_THRESHOLD_VOLTAGE = -300; //channels relative to OFFSET_ZERO_VOLTAGE


#endif /* GLOBALS_H_ */
