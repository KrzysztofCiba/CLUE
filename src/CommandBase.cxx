/**
 * @file CommandBase.cxx
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 23/11/2006
 * @brief Definition of CommandBase class.
 */

#include "CommandBase.h"

void CLUE::CommandBase::parse_recipe() 
{
	std::stringstream inout(m_recipe);
	
	std::vector<std::string> sentence;
	std::string word;
	while ( inout >> word ) sentence.push_back(word);
	
	std::vector<std::string>::iterator it;
	for (it = sentence.begin(); it != sentence.end(); ++it ) {
		
		std::string chunk = (*it);
		
		if ( chunk[0] == '%' && ( (*it).length() > 3 ) ) { // we got an index
			
			// remove (,#,) chars 
			std::replace((*it).begin(), (*it).end(), '(', ' ');
			std::replace((*it).begin(), (*it).end(), ')', ' ');
			std::replace((*it).begin(), (*it).end(), '#', ' ');
			
			inout.clear();
			inout.seekg( std::ios_base::beg );
			inout.str( (*it) );
			int lo, up;
			inout >> word >> lo >> up;
			m_dims.push_back( CLUE::FORTRAN_DIM( lo, up ) );
		}
	}
	// reverting to get F77 indexes
	std::reverse(m_dims.begin(), m_dims.end());
	m_dim = m_dims.size();
};
