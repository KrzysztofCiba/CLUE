/**
 * @file CLUtils.h
 * @author Krzysztof Ciba (krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 04/12/2006
 * @brief Variuos utility classes, exceptions etc.
 */

#ifndef CLUTILS_H
#define CLUTILS_H

// begin of CLUE chunk
namespace CLUE {

	/**
	 * @class CLUException
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 29/01/2007
	 * @brief a base class for all CLUE exceptions, pure virtual 
	 */
	class CLUException {
	public:
		/** tell to the world what's going on */
		virtual const char* what() throw() { };
	};


	/**
	 * @class ParsingError
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 05/12/2006
	 * @brief Exception thrown when parsing ends with error
	 */
	class ParsingError : public CLUException {

	public:
		
		/**
		 * a constructor
		 * @param com parsed string
		 * @param what possible error description from CLUE::StringParse
		 * @param where possible position of error
		 */
		ParsingError( std::string com, std::string what, std::streampos where ) throw() {
			m_com = com;
			m_what = what;
			m_where = where;
		}
		
		/** a destructor */
		~ParsingError() throw() { };
 
		/** tell to the world what's going on */
		virtual const char* what() throw() { 
			std::stringstream inout;
			inout << "*** CLUE::ParsigError exception - in command \"" 
						<< m_com << "\" on position=" << m_where << ","<< std::endl
						<< "CLUE::StringParser said \"" << m_what << "\" ***";
			std::string msg = inout.str();
			return msg.c_str();
		}

	private:
		
		/** string to parse */ 
		std::string m_com;

		/** error communicate */
		std::string m_what;

		/** position in stream, where error occured  */
		std::streampos m_where; 


	}; // end of ParsingError



	/**
	 * @class NotKnownCommand 	
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 04/12/2006
	 * @brief Exception thrown when command of name "NAME" is not
	 * defined.
	 */
	class NotKnownCommand : public CLUException {

	public:

		/**
 		 * a constructor
		 * @param gen name of generator
		 * @param com name of not known command
		 */
		NotKnownCommand( std::string gen, std::string com ) throw() { 
			m_generator = gen;
			m_command = com;
		};

		/** destructor */
		~NotKnownCommand() throw() { };

		/** tell the world what is going on */
		virtual const char* what() throw() {
			std::stringstream inout;
			inout << "*** CLUE::NotKnownCommand exception - undefined command " 
						<< m_command << " in generator " << m_generator 
						<< ". ***"<< std::endl;
			std::string msg = inout.str();
			return msg.c_str();
		};

	private:

		/** generator name */
		std::string m_generator;

		/** generator command name */
		std::string m_command;

	}; // end of NotKnownCommand

	/**
	 * @class F77OutOfRange
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @brief Exception class inherited from std::exception
	 * 
	 * This exception will be trown when user submit a wrong
	 * index (not in range of Fotran-defined table size).
	 *
	 * To understand what is the problem print out the communicate
	 * using CLUE::F77OutOfRange::what(). 
	 */
	class F77OutOfRange : public CLUException {

	public:

		/** a constructor */
		F77OutOfRange( const int lo, const int up, const int yours ) throw() {
			m_lo = lo;
			m_up = up;
			m_yours = yours;
		};

		/** a destructor */
		~F77OutOfRange() throw() { };

		/** telling to the world what goes wrong */
		virtual const char* what() throw() {
				std::stringstream inout;
				inout << "*** CLUE::F77OutOfRange exception - "
							<< "Fortran 77 array index out of range." 
							<< " Allowed range is [" << m_lo << ", " 
							<< m_up << "], but you specified "
							<< m_yours << ". ***" << std::endl;
				std::string com = inout.str();
				return com.c_str();
		};

	private:

		/** lowest allowed F77 index  */
		int m_lo;

		/** higest allowed F77 index */
		int m_up;

		/** user input out of range */
		int m_yours;

	}; // end of F77OutOfRange definition, declaration

	
	/**
	 * @class InvalidDim
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @brief Exception class for Command<T,char*> getters and setters.
	 *
	 * This exception is thrown when user has picked up wrong setter or 
	 * getter from Command and Command<char*> objects e.g.
	 * setter for 2D dimension array, when working with 5D array. 
	 *
	 * To see what goes wrong, please print out the message using 
	 * CLUE::InvalidDim::what()
	 */
	class InvalidDim : public CLUException {

	public:

		/**
		 * constructor 
		 * @param com command name
		 * @param wdim wrong dimension specifioed by user 
		 * @param gdim dimension of Command<> com
		 */
		InvalidDim(const std::string com,
							 const int wdim, 
							 const int gdim ) throw() {
			m_com = com;
			m_wdim = wdim;
			m_gdim = gdim;
		};
		
		~InvalidDim() throw() { };
		
		/**
		 * tells to the world what goes wrong
		 */
		virtual const char* what() throw() {

			std::stringstream inout;
			inout << "*** CLUE::InvalidDim exception - wrong get/set chosen." 
						<< " The Command " << m_com << " is a ";
			m_gdim == 0 ? inout << "scalar," : inout << m_gdim <<"D table,";
			inout << " but you choose get/set for ";
			m_wdim == 0 ? inout << "scalar" : inout << m_wdim << "D table";
			inout << ". ***";

			std::string com = inout.str();
			return com.c_str();
		};

	private:

		/** nb of dimensions of Command m_com  */
		int m_gdim;

		/** nb of dimensions chosen by user */
		int m_wdim;

		/** name of command */
		std::string m_com;

	}; // end of InvalidDim definition, declaration

	/**
	 * @class FORTRAN_DIM
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 27/11/2006
	 * @brief Helper class to convert Fortran 77 to C++ index
	 *
	 * This converts Fortran-based 1D index to the C/C++ one, e.g.:
	 * when in Fotran code you have a table of size:
	 * <pre>
	 * INTEGER A(-2:2)
	 * </pre>
	 * so the Fortran indexes are [-2, -1, 0, 1, 2],
	 * then in C/C++ allowed indexes for table A are [ 0, 1, 2, 3, 4 ].
	 * The C/C++ index is returned using CLUE::FORTRAN_DIM::index()
	 * 
	 * To find the allowed Fortran limits use:
	 * <ul>
	 * <li> CLUE::FORTRAN_DIM::lo() for the lowest allowed F77 index
	 * <li> CLUE::FORTRAN_DIM::up() for the higest allowed F77 index
	 * </ul>
	 */
	class FORTRAN_DIM {

	public:

		/** constructor */
		FORTRAN_DIM( int l, int u ) : m_lo(l), m_up(u) 
			{ m_size = m_up - m_lo + 1;  };

		/** the lowest F77 index getter */
		int lo() const { return m_lo; };

		/** the highest F77 index getter */
		int up() const { return m_up; };

		/** the size getter */
		int size() const { return m_size; };

		/** 
		 * Fortran77 to C++ index converter for one dimesion in array 
		 * @param fi a Fortran based index
		 * @return C/C++ index
		 * @throw CLUE::F77OutOfRange
		 */
		int index(const int fi) const throw (CLUE::F77OutOfRange) {
			if (( fi < m_lo ) || ( fi > m_up ) ) 
				throw CLUE::F77OutOfRange( m_lo, m_up, fi );
			return fi - m_lo;
		};

	private:

		/** dimension */
		int m_size;

		/** the lowest allowed Fortran 77 index */
		int m_lo;

		/** the highest allowed Fortran 77 index */
		int m_up;

	}; // end of FORTRAN_DIM

}; // end of namespace CLUE chunk

#endif // end of CLUTILS_H
