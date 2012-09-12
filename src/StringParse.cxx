/**
 * @file
 * @brief StrinParse class definition.
 * This code is used to parse a string.
 * It breaks it into components which are seperated by spaces 
 * in the original. The components can be accessed and converted 
 * to doubles or ints.
 * @author Ian Hinchliffe 
 * @author Krzysztof Ciba
 */



// uncomment this line to use it in ATLAS code 
//#include "GeneratorUtils/StringParse.h"

//
//  ...and don't forget to comment this line out
#include "StringParse.h"


#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <sstream>


//! default constructor
CLUE::StringParse::StringParse(): m_status( 0 ), 
																	m_init( false ),
																	m_command( std::string() ),
																	m_label( std::string() ) { 
	std::string empty("");
	m_init = initStringParser( empty );
};


//! a constructor with a string 
CLUE::StringParse::StringParse( std::string command ) : m_status(0), 
																												m_init(false),
																												m_command(std::string() ),
																												m_label(std::string() ) {
	m_init = initStringParser( command );
}

//! a destructor
CLUE::StringParse::~StringParse() { };


bool CLUE::StringParse::initStringParser( std::string command ) {
	
	// clear the status
	m_status = 0;
	// clear the stream and the iostate bits 
	m_inout.str("");
	m_inout.clear();
	// clear the label and command
	m_label.clear();
	m_command.clear();

	// set the command
	m_command = command;
	
	// make sure command != empty string 
	if ( m_command.size() == 0 ) {
		m_status |= ( 0x1 << StringParse::EmptyCommand );
		return false;
	}		
	
	// feed the strem
	m_inout << m_command;
	// get label from stream
	m_inout >> m_label;
		
	// check if label not begins with a digit
	if ( ! isalpha(m_label[0]) ) {
		m_status |= ( 0x1 << StringParse::LabelNotAlpha );
		return false;
	} 
		
	// check if there are something else in the stringstream
	if ( m_inout.eof() ) {
		m_status |= ( 0x1 << StringParse::ValueNotSet );
		return false;
		m_nword = 0;
	} else 
		// for old interface
		{
			std::string str = m_inout.str();
			std::stringstream tmp;
			tmp << str;
			while( tmp >> str ) m_words.push_back(str);
			m_nword = m_words.size();
		}
	

	return true;
};


//! set the command
void CLUE::StringParse::setCommand( const std::string command ) {
	m_init = initStringParser( command );
};


//! special method to convert string to c++ index of array
//! so you don't have to decrease its value by one
//! otherwise use templated getVal, but be sure to 
//! make a decresion 
bool CLUE::StringParse::getIndex( int & index ) {

	if ( m_init ) {
		bool state = rdstate();
		if ( state )	{ 
			m_inout >> index;

			// check if next character is a space, \t, \n etc.
			int space = isspace( m_inout.peek() );
			
			if (  space == 0  ) {
				// no blanks after the number, do not trust to the result
				m_status |= ( 0x1 << StringParse::NotAnIndex );
				return false;
			} else {
				return true;
			}
		}
		// something goes wrong
		return state;
	}
	// m_init != 0
	return false;
}

//! check the state of stream
bool CLUE::StringParse::rdstate() {

	bool bad = m_inout.bad();
	bool fail = m_inout.fail();
	bool eof = m_inout.eof();
	bool good = m_inout.good();
	if ( bad ) {
		//  uncoverable error, lost integity in streambuf 
		m_status |= ( 0x1 << StringParse::BadBitSet );
	} else if ( fail ) {
		//  I/O operation failure
		m_status |= ( 0x1 << StringParse::FailBitSet );
	} else if ( eof ) {
		//  End-Of-File reached
		m_status |= ( 0x1 << StringParse::EndOfInput );
	} else if ( good ) {
		// stringsteram is ok
		return good;
	}
	// if we are here, then one of bad, fail or eof bits is set
	return false;
};

//! messages for possible errors 
std::string CLUE::StringParse::what() {

	std::stringstream out;

	out << "CLUE::StringParse : ";
	switch (m_status) {
	case 0:
	case 1:
		out << "ok";
		break;
	case 2:
		out << "command not set";
		break;
	case 4:
		out << "label " << m_label << " not begins with an alpha character, check label?";
		break;
	case 8:
		out << "value(s) after label " << m_label << " not set, check values?";
		break;
	case 16:
		out << "not an int, when trying to get index";
		break;
	case 32:
		out << "eofbit set - end of input reached";
		break;
	case 64:
		out << "failbit set - last I/O operation failed before completion";
		break;
	case 128:
		out << "badbit set - lost integrity of stream buffer";
		break;
	default:
		out << "unknown error";
		break;
	}
	return out.str();
}


//////////////////////////////////////////////////////////
/// OLD INTERFACE

std::string CLUE::StringParse::piece(const int &num){
  if(num <= m_nword){
    return m_words[num-1];
  }
  else { 
    return std::string("?!?");
  }
}

int CLUE::StringParse::intpiece(const int & num){  
if(num <= m_nword){
     int i= atoi(m_words[num-1].c_str());
     return i;
  }
  else { 
    return -1;
  }
}

long CLUE::StringParse::longpiece(const int & num){  
if(num <= m_nword){
     int i= atol(m_words[num-1].c_str());
     return i;
  }
  else { 
    return -1;
  }
}
double CLUE::StringParse::numpiece(const int & num){  
if(num <= m_nword){
     double x= atof(m_words[num-1].c_str());
     return x;
  }
  else { 
    return -1.1;
  }
}


inline unsigned int CLUE::StringParse::string_size() const { 
	return m_nword; 
}

