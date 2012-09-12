/**
 * @file CommandBase.h
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 22/11/2006
 * @brief Declaration of CommandBase class.
 */

#ifndef COMMANDBASE_H
#define COMMANDBASE_H

// includes from standard libraries
#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#ifndef CLUTILS_H
#include "CLUtils.h"
#endif



// begin of  namespace CLUE chunk 

namespace CLUE {

	/**
	 * @class CommandBase
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 22/11/2006
	 * @brief Declaration of CommandBase class - base class for all
	 * Command<> classes.
	 */
	class CommandBase {

	public:

		/** parsig the command */
		virtual void parse(std::string command) = 0;

		/** the command name getter */
		inline std::string name() const { return m_name; };
		
		/** the generator name getter */
		inline std::string generator() const { return m_generator; };

		/** the recipe getter */
		inline std::string recipe() const { return m_recipe; };
		
		/** the nb of dimensions getter */
		inline unsigned int nbdim( ) const { return m_dim; };
		
		/** F77 dimensions getter */
		inline std::vector<CLUE::FORTRAN_DIM> dimensions() const 
			{ 
				return m_dims;	
			};

	protected:

		/** raw recipe parsing */
		void parse_recipe();
		
		/** name of command (aka label) */
		std::string m_name;
	
		/** name of generator */
		std::string m_generator;

		/** read/write format */
		std::string m_format;

		/** original string */
		std::string m_recipe;

		/** number of dimensions */
		mutable unsigned int m_dim;

		/** F77 dimensions */
		std::vector<CLUE::FORTRAN_DIM> m_dims;

	}; // end of CommandBase class
	
}; //! end of chunk of namespace CLUE


#endif // COMMANDBASE_H
