/*
 * ArgsParser.h
 *
 *  Created on: 07.06.2017
 *      Author: stefan
 */

#ifndef ARGSPARSER_H_
#define ARGSPARSER_H_

#include <string>
#include <vector>

typedef struct
{
	std::string path;
	std::string filename;
	std::vector<std::string> options;

} Parsed;

class ArgsParser
{
public:
	ArgsParser();
	~ArgsParser();

	Parsed parseArguments(int argc, char* args);
};

#endif /* ARGSPARSER_H_ */
