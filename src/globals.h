/*
 * globals.h
 *
 *  Created on: 17.10.2016
 *      Author: stefan
 */


/**
 * Header containing globally available data constants, that are needed within the program
 *
 * @author Stefan
 * @date October 18, 2016
 * @version 0.8
 */
#ifndef GLOBALS_H_
#define GLOBALS_H_

//variables for FADC calibration:
static const double ADC_CHANNELS_TO_VOLTAGE = 12.0 / 4096; //V per channel
static const short ADC_BINS_TO_TIME = 4; //ns
static const unsigned short ADC_TRIGGERPOS_BIN = 40;

#endif /* GLOBALS_H_ */
