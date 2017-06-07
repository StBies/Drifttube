/*
 * ArgsParser.h
 *
 *  Created on: 07.06.2017
 *      Author: stefan
 */

#ifndef ARGSPARSER_H_
#define ARGSPARSER_H_

class ArgsParser
{
public:
	ArgsParser();
	~ArgsParser();

	parseArguments(int argc, char* args);
};

#endif /* ARGSPARSER_H_ */
