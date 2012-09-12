/**
 * @file  StringParse.h
 * @brief Definition of StringParse class.
 * @author Ian Hinchliffe 
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date April 2000
 */

#ifndef STRINGPARSE_H
#define STRINGPARSE_H

#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdlib.h>

namespace CLUE {

	/**
	 * @class StringParse
	 * @author Ian Hinchliffe
	 * @author Krzysztof Ciba
	 * @brief A class to parse strings for physics generators wrappers. 
	 *
	 * This code is used to parse string using stringstream object. 
	 * The generator command is supposed to be in format:<br>
	 * <b>label</b>&nbsp;<i>val_0</i> [<i>val_1...</i>]<br>
	 * where:<br>
	 * <ul>
	 * <li> <b>label</b> is a uniqe name of command (and it is written using lowecase 
	 * AND alphanumeric chars only!)
	 * <li> <i>val_0</i> is mandatory value of command
	 * <li> [<i>val_1...</i>] are possible other values, which are required from logical
	 * point of view (e.g. in case of reading of the ith element of table, <i>val_0</i> is treated
	 * as the index of ith element, and <i>val_1</i> as a value of ith element).
	 * </ul>
	 *
	 * As the name of command is almost always mapped with name of variable, then the <b>label</b> follows 
	 * variable naming conventions e.g. cannot starts with digit. 
	 *
	 * For parsing the command you should use a getVal() method, generated from template.
	 * There is also a method getIndex(), which carries the reading unsigned int value from 
	 * stream and assumes that you want to read a C++ index (so it is internally decreased by one!). 
	 * The old interface is also kept for the compatibility, but it has some shortcommings.
	 * Please use it carefully, unless you know what you are doing (end even in that case).
	 *     
	 *
	 * OLD INTERFACE<br> 
	 * This code is used to parse a string. It breaks it into components which are seperated 
	 * by spaces in the original. 
	 * Three methods can then be called to return the nth component:
	 * <ul>
	 *  <li> as a string piece(const int & num) method (no conversion),
	 *  <li> as an integer intpiece(const int & num) method, if conversion is possible, 
	 * returns -1 if it is off the end and 0 if it cannot be converted, 
	 * <li> as a double numpiece(const int & num) method, if conversion is possible, 
	 * returns -1.1 if it is off the end  and 0 if it cannot be converted.
	 * </ul>
	 *
	 */
	class StringParse {
		
	public:
		
		//! a constructor with std::string
		//! @param command a command to parse
		StringParse(std::string command);
		
		//! default constructor
		StringParse(); 
		
		//! default destructor
		~StringParse(); 
		
		//! sets the command
		void setCommand( const std::string command );

		//! gets the command
		std::string getCommand()  { return m_command; }; 
		
		//! to check what's going on
		unsigned int status() { return m_status; };
		
		//! template to read a record from stringstream 
		//! shoud works for all build-in data types
		//! but to be sure...
		//! @return stringstream.good() 
		template < typename T > bool getVal( T &val ) {
			if ( m_init ) {
				bool state = rdstate();
				if ( state ) m_inout >> val;
				return state;
			}
			return false;
		};
		
		//! special method to convert string to index of array
		//! @param index a reference to the storing variable
		//! @return true if conversion is sucessful
		//! @warning index follow the Fortran 77 convention
		bool getIndex( int & index );
		
		//! get the possible error communicate
		//! @return a message packed in std::string
		std::string what();
		
		//! label getter
		//! @return a label
		std::string  getLabel( ) { return m_label; }
		
		//! tells the position of last get from stream
		//! @return position of last get in stream
		std::streampos where() { return m_inout.tellg(); }
		
		//! @deprecated 
		//! old interface
		//! @param num an index of word in command
		//! @return a word 
		std::string piece(const int & num);

		//! @deprecated 
		//! old interface
		//! @param num an index of word in command
		//! @return word converted to int, -1 if conversion is impossible
		int intpiece(const int & num); 
		
		//! @deprecated
		//! old interface
		//! @param num an index of word in command
		//! @return word converted to long, -1 if conversion is impossible
		long longpiece(const int & num); 

		//! @deprecated
		//! old interface
		//! @param num an index of word in command
		//! @return word converted to double, -1.1 if conversion is impossible
		double numpiece(const int & num);
		
		//! string size getter
		unsigned string_size(void) const;


	private:
		
		//! initalise StringParse
		bool initStringParser( std::string command );  
		
		//!  state of stringstream
		bool rdstate();
		
		//! flag to mark initalisation
		bool m_init;
		
		//! internally used status
		unsigned int m_status;
		
		//! basic input-output string
		std::stringstream m_inout;
		
		//! a command passed to StringParser object
		std::string m_command;
		
		//! a command label
		std::string m_label;
		
		//! possible errors 
		enum ParseBit {
			OK             =  0, //< OK (%0000  0x0)
			EmptyCommand   =  1, //< string to parse is empty (%0001  0x1)
			LabelNotAlpha  =  2, //< first letter of label is not an alpha (%0010  0x2)
			ValueNotSet    =  3, //< the label is not followed by its value (%0011  0x3)
			NotAnIndex     =  4, //< error trying to get int from stream (%0100  0x4)
			EndOfInput     =  5, //< reading from empty stream (%0101  0x5)
			FailBitSet     =  6, //< failbit set in stream (%0110  0x6)
			BadBitSet      =  7  //< badbit set in  stream (%0111  0x7)
		};
		
		/// OLD MEMBERS
		
		//! number of words in string
		int m_nword;
		
		//! vector of words
		std::vector <std::string> m_words;
		
	}; // end of StringParse class

}; // end of chunk of namespace CLUE


#endif // end of STRINGPARSE_H
