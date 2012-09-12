/**
 * @file
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 *
 * @brief Dummy Generator_i class declaration.
 */

#ifndef DUMMYGEN_I_H 
#define DUMMYGEN_I_H

#include <iostream>
#include <map>
#include <string>
#include <utility>

#ifndef STRINGPARSE_H
#include "StringParse.h"
#endif

#ifndef COMMANDBASE_H
#include "CommandBase.h"
#endif

#ifndef COMMAND_H
#include "Command.h"
#endif

// common definition
#include "DUMMYGEN_extern_C.h"
#include "DUMMYGEN_map.h"


/**
 * @class DummyGen
 * @date 5th Oct, 2006
 * @author Krzysztof Daniel Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl) 
 *
 * @brief A dummy generator wrapper.
 */
class DummyGen  {

public:
	//! default constructor
	DummyGen( std::string generator = std::string("DUMMYGEN") ) { };

	//! default destructor
	~DummyGen() {};

	//! generator name getter
	std::string generator() const { return m_generator; };

	//! a generator initalisation
	virtual void initGenerator( );

	//! print out GenCommands
	void printout();
	
	//! dummy m_command_vector initialisation
	//! @param command_vector dummy options
	//! only for testing purpose
	void dummyopts(std::vector< std::string > command_vector);

private:
	
	// generator name
	std::string m_generator;

	//! a vector with generator commands read from JobOptionsSvc
	std::vector< std::string > m_command_vector;



};

#endif // DUMMYGEN_I_H
