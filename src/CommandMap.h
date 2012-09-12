/**
 * @file CommandMap.h
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 04/12/2006
 * @brief class CommandMap declaration
 */

#ifndef COMMANDMAP_H
#define COMMANDMAP_H

// includes from std libraries
#include <map>
#include <complex>
#include <string>
#include <cctype>
#include <algorithm>
#include <iostream>

#ifndef COMMANDBASE_H
#include "CommandBase.h"
#endif

#ifndef CLUTILS_H
#include "CLUtils.h"
#endif


// begin of namespace CLUE chunk

namespace CLUE {

	/**
	 * @class CommandMap
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 04/12/2006 
	 * @brief Wrapper class for std::map<std::string, CommandBase*>
	 */
	class CommandMap {

	public:
	 
		/** generator name getter */
		std::string generator() const { return m_generator; }; 

		/** command map getter */
		std::map<std::string, CLUE::CommandBase*>* map() 
			{
				if ( ! m_initialised ) init();
				return &m_command_map;
			};

		/**
		 * main parsing function
		 * @param commands a vector of strings
		 * @return true if parsing was succesfull
		 * @throws all possible CLUE exceptions
		 */
		void parse( std::vector<std::string > commands )
			{
				
				if ( ! m_initialised ) init();
				
				CLUE::StringParse sp;
				std::vector< std::string >::iterator it;
				for ( it = commands.begin(); it != commands.end(); ++it ) {
					sp.setCommand( (*it) );
					std::string label = upper( sp.getLabel() );
					CLUE::CommandBase* com = command( label );
					com->parse(*it);
				}
			}
		
		/**
		 * command getter
		 * @param name  a name of command
		 * @return pointer to CommandBase if command of name is present in map
		 * @throws CLUE::NotKnownCommand
		 */
		CLUE::CommandBase* command( std::string name ) 
			throw (CLUE::NotKnownCommand ) {
			
			if ( ! m_initialised ) init();
			name = upper(name);
			std::map<std::string, CommandBase*>::iterator it;
			it = m_command_map.find( name );
			if ( it == m_command_map.end() ) 
				throw CLUE::NotKnownCommand(m_generator, name);

			return it->second;
		}

		/** map initalisation */
		virtual void init() = 0;

		/** check if map is initialised */
		bool initialised() const { return m_initialised; };
		
		friend std::ostream& operator<<( std::ostream& stream, CommandMap& cm) {
			if ( ! cm.initialised() ) cm.init();
			stream << " CommandMap for generator " << cm.generator() 
						 << " defined commands are: "<< std::endl;
			std::map<std::string, CommandBase*>::iterator it;
			std::map<std::string, CommandBase*>* map = cm.map();
			
			unsigned int i = 1;
			for (it = map->begin(); it != map->end(); ++it, ++i ) {

				CLUE::CommandBase* cb = it->second;
				stream << " [" << i << "] ";  

				CLUE::Command<int>* icmd = dynamic_cast< CLUE::Command<int>* >( cb );
				if ( NULL != icmd ) {
					stream << "int type" << (*icmd) << std::endl;
					continue;
				}
				CLUE::Command<double>* dcmd = dynamic_cast< CLUE::Command<double>* >( cb );
				if ( NULL != dcmd ){
					stream << "double type" << (*dcmd) << std::endl;
					continue;
				}
				CLUE::Command<float>* fcmd = dynamic_cast< CLUE::Command<float>* >( cb );
				if ( NULL != fcmd ) {
					stream << "float type" <<(*fcmd) << std::endl;
					continue;
				}
				CLUE::Command<char*>* ccmd = dynamic_cast< CLUE::Command<char*>* >( cb );
				if ( NULL != ccmd ) {
					stream << "char* type" << (*ccmd) << std::endl;
					continue;
				}
				CLUE::Command< std::complex<double> >* cdcmd = 
					dynamic_cast< CLUE::Command< std::complex<double> >* >( cb );
				if ( NULL != cdcmd ) {
					stream << "complex<double> type" <<(*cdcmd) << std::endl;
					continue;
				}
				CLUE::Command< std::complex<float> >* cfcmd = 
					dynamic_cast< CLUE::Command< std::complex<float> >* >( cb );
				if ( NULL != cfcmd ) {
					stream << "complex<float> type" << (*cfcmd) << std::endl;
				}
			}
				
			return stream;
		}

	protected:
	 

		/** generator name */
		std::string m_generator;

		/** flag to mark map initalisation */
		bool m_initialised;

		/** the command map itself */
		std::map<std::string, CLUE::CommandBase*> m_command_map;

		/** command map initalisation marking */
		void setInitialised() { m_initialised = true; }

		
		/** 
		 * helper function to uppercase a string 
		 * @param str a string
		 * @return an uppercase string
		 */
		std::string upper(std::string str)
			{
				std::transform(str.begin(),str.end(),str.begin(),(int(*)(int)) std::toupper);
				return str;
			};

	}; // end of class CommandMap


}; // end of namespace CLUE chunk


#endif // COMMANDMAP_H
