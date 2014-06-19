/*
 * Env.cpp
 *
 *  Created on: Jun 15, 2014
 *      Author: Miro B.
 */


#include <common/Env.h>



std::string Env::srcFile = "";
std::string Env::srcDeserialize = "";
std::string Env::dstClassFile = "";
std::string Env::dstSerialize = "";
std::string Env::dstGraphviz = "";
bool Env::isQuiet = false;


std::vector<std::string> Env::warnings;
std::vector<std::string> Env::errors;
bool Env::warningsOccurred = false;
bool Env::errorsOccurred = false;

