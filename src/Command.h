/**
 * @file Command.h
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 23/11/2006
 * @brief template Command class definition
 */


#ifndef COMMAND_H
#define COMMAND_H


#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <complex>


#ifndef STRINGPARSE_H
#include "StringParse.h"
#endif

#ifndef CLUTILS_H
#include "CLUtils.h"
#endif


#include <stdlib.h>

/** for friend operator<<() methods */
//class MsgStream;


class CLUE::CommandBase;

namespace CLUE {
	
	///////////////////////////////////////////////////////////////////////////////
	// template<typename T> class Command
  ///////////////////////////////////////////////////////////////////////////////
		
	/**
	 * @class Command
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 24/11/2006
	 * @brief A template class to interface variable from Fortran 77 common. 
	 */
	template<typename T> 
		class Command : public CommandBase  {
		public:
		
		/** 
		 * a constructor 
		 * @param gen - a name of generator
		 * @param name - a command name
		 * @param recipe - a command read/write format 
		 * @param ptr - a reference to common variable
		 */
		Command(std::string gen, 
						std::string name, 
						std::string recipe, 
						T& ptr) { 
			m_generator = gen;
			m_name = name;
			m_recipe = recipe;
			m_val = &ptr; 
			parse_recipe();
		};

		/**
		 * parsing function
		 * @param command a string
		 * @throws CLUE::F77OutOufRange, CLUE::InvalidDim or CLUE::ParsingError
		 */
		void parse(std::string command)
			throw( CLUE::F77OutOfRange, CLUE::InvalidDim , CLUE::ParsingError) 
			{
				
				CLUE::StringParse sp(command);

				int f0, f1, f2, f3, f4, f5, f6, f7;
				T val;
				
				switch ( nbdim() ) {
					
				case 0:
					if ( sp.getVal(val) ) {
						set( val );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 1:
					if ( sp.getIndex(f0) && sp.getVal(val) ) {
							set( val, f0 );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 2:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getVal(val) ) { 
						set( val, f0, f1 );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 3:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getVal(val) ) {
						set( val, f0, f1, f2);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 4:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getVal(val) ) {
						set( val, f0, f1, f2, f3);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 5:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getVal(val) ) {
						set( val, f0, f1, f2, f3, f4);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 6:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getIndex(f5) && 
							 sp.getVal(val) ) {
						set( val, f0, f1, f2, f3, f4, f5);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
					
				case 7:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getIndex(f5) && 
							 sp.getIndex(f6) && sp.getVal(val) ) {
						set( val, f0, f1, f2, f3, f4, f5, f6);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 8:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getIndex(f5) && 
							 sp.getIndex(f6) && sp.getIndex(f7) && 
							 sp.getVal(val) ) {
						set( val, f0, f1, f2, f3, f4, f5, f6, f7);
					} else  throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
				}
			}			
		

		/** 
		 * getter for scalar variable 
		 * @throws CLUE::InvalidDim 
		 */
		T get() const throw(CLUE::InvalidDim) 
			{
				if ( m_dim != 0 ) throw CLUE::InvalidDim( m_name, 0, m_dim );
				return *m_val; 
			}

		/**
		 * getter for 1D table
		 * @param f0 Fortran77 index 
		 * @warning f0 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0) const 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange) 
			{
				if ( m_dim != 1 ) throw CLUE::InvalidDim( m_name, 1, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c0 = (*it).index(f0);
				
				T* where = m_val + c0;
				return *(where);
				
			}
		

		/**
		 * getter for 2D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @warning f0 and f1 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 2 ) throw CLUE::InvalidDim( m_name, 2, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
	
				unsigned int c1 = (*it).index(f0);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f1);
				
				T* where = m_val + jump0*c0 + c1;
				return *(where);
			}
		
		/**
		 * getter for 3D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @warning f0,f1 and f2 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 3 ) throw CLUE::InvalidDim( m_name, 3, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c2 = (*it).index(f0);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f1);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f2);
				
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + c2; 
				
				return *(where);
			}

		
		/**
		 * getter for 4D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @warning f0-f3 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2,int f3) const 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 4 ) throw CLUE::InvalidDim( m_name, 4, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c3 = (*it).index(f0);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f1);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f2);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f3);
				
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + c3;
				return *(where);
			}

		/**
		 * getter for 5D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @warning f0-f4 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2,int f3,int f4) const
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 5 ) throw CLUE::InvalidDim( m_name, 5, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c4 = (*it).index(f0);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f1);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f2);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f3);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f4);
				
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + c4;
				return *(where);
			}

		
		/**
		 * getter for 6D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 5th F77 index
		 * @warning f0-f5 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2,int f3,int f4,int f5) const
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 6 ) throw CLUE::InvalidDim( m_name, 6, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c5 = (*it).index(f0);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f1);
				unsigned int jump3 = (*it++).size();
	
				unsigned int c3 = (*it).index(f2);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f3);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f4);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f5);
				
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + c5;
				return *(where);
			}
		
		
		/**
		 * getter for 7D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index
		 * @param f6 7th F77 index
		 * @warning f0-f6 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2,int f3,int f4,int f5,int f6) const
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 7 ) throw CLUE::InvalidDim( m_name, 7, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c6 = (*it).index(f0);
				unsigned int jump5 = (*it++).size();

				unsigned int c5 = (*it).index(f1);
				unsigned int jump4 = (*it++).size();
	
				unsigned int c4 = (*it).index(f2);
				unsigned int jump3 = (*it++).size();
	
				unsigned int c3 = (*it).index(f3);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f4);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f5);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f6);
				
				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + c6;
				return *(where);
			}

		
		/**
		 * getter for 8D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index
		 * @param f6 7th F77 index
		 * @param f7 8th F77 index
		 * @warning f0-f7 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		T get(int f0,int f1,int f2,int f3,int f4,int f5,int f6,int f7) const
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 8 ) throw CLUE::InvalidDim( m_name, 8, m_dim );

				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c7 = (*it).index(f0);
				unsigned int jump6 = (*it++).size();
				
				unsigned int c6 = (*it).index(f1);
				unsigned int jump5 = (*it++).size();
				
				unsigned int c5 = (*it).index(f2);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f3);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f4);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f5);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f6);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f7);
				
				jump5 *= jump6;
				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + jump6*c6 + c7;
				return *(where);
			}
		
		
		/** 
		 * setter for scalar variable
		 * @param val value to set
		 * @throws CLUE::InvalidDim 
		 */
		void set( const T val ) 
			throw(CLUE::InvalidDim)
			{
				if ( m_dim != 0 ) throw CLUE::InvalidDim( m_name, 0, m_dim );
				(*m_val) = val;
			}


		/** 
		 * setter for 1D table 
		 * @param val value to set
		 * @param f0 F77 index
		 * @warning f0 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 1 ) throw CLUE::InvalidDim( m_name, 1, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c0 = (*it).index(f0);
				
				T* where = m_val + c0;
				(*where) = val;
			}


		/** 
		 * setter for 2D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @warning f0 and f1 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 2 ) throw CLUE::InvalidDim( m_name, 2, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c0 = (*it).index(f0);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c1 = (*it).index(f1);
				
				T* where = m_val + jump0*c0 + c1; 
				(*where) = val;
			}

		/** 
		 * setter for 3D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @warning f0,f1 and f2 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 3 ) throw CLUE::InvalidDim( m_name, 3, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c2 = (*it).index(f0);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f1);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f2);

				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + c2; 
				(*where) = val;
			}

		
		/** 
		 * setter for 4D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @warning f0-f3 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2, int f3 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 4 ) throw CLUE::InvalidDim( m_name, 4, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c3 = (*it).index(f0);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f1);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f2);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f3);
				
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + c3;
				(*where) = val;
			}


		/** 
		 * setter for 5D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @warning f0-f4 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2, int f3, int f4 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 5 ) throw CLUE::InvalidDim( m_name, 5, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c4 = (*it).index(f0);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f1);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f2);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f3);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f4);

				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + c4;
				(*where) = val;
			}

		/** 
		 * setter for 6D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index 
		 * @warning f0-f5 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2, int f3, int f4, int f5 )
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 6 ) throw CLUE::InvalidDim( m_name, 6, m_dim );
				
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c5 = (*it).index(f0);
				unsigned int jump4 = (*it++).size();
	
				unsigned int c4 = (*it).index(f1);
				unsigned int jump3 = (*it++).size();
	
				unsigned int c3 = (*it).index(f2);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f3);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f4);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f5);
				
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + c5;
				(*where) = val;
			}
		


		/** 
		 * setter for 7D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index 
		 * @param f6 7th F77 index
		 * @warning f0-f6 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2, int f3, int f4, int f5, int f6 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange) 
			{
				if ( m_dim != 7 ) throw CLUE::InvalidDim( m_name, 7, m_dim );
	
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();

				unsigned int c6 = (*it).index(f0);
				unsigned int jump5 = (*it++).size();

				unsigned int c5 = (*it).index(f1);
				unsigned int jump4 = (*it++).size();
	
				unsigned int c4 = (*it).index(f2);
				unsigned int jump3 = (*it++).size();
	
				unsigned int c3 = (*it).index(f3);
				unsigned int jump2 = (*it++).size();
	
				unsigned int c2 = (*it).index(f4);
				unsigned int jump1 = (*it++).size();
	
				unsigned int c1 = (*it).index(f5);
				unsigned int jump0 = (*it++).size();
	
				unsigned int c0 = (*it).index(f6);

				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + c6;
				(*where) = val;
			}
		

		
		/** 
		 * setter for 8D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index 
		 * @param f6 7th F77 index
		 * @param f7 8th F77 index
		 * @warning f0-f7 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim 
		 * @throws CLUE::F77OutOfRange
		 */
		void set( const T val, int f0, int f1, int f2, int f3, int f4, int f5, int f6, int f7 ) 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 8 ) throw CLUE::InvalidDim( m_name, 8, m_dim );
	
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c7 = (*it).index(f0);
				unsigned int jump6 = (*it++).size();
				
				unsigned int c6 = (*it).index(f1);
				unsigned int jump5 = (*it++).size();
				
				unsigned int c5 = (*it).index(f2);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f3);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f4);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f5);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f6);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f7);
				
				jump5 *= jump6;
				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				T* where = m_val + jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + jump6*c6 + c7;
				(*where) = val;
			}
		
	 
	
		/**
		 * printouts the object to std::ostream
		 * @param stream an output stream 
		 * @param com an object to printout
		 * @return a reference to stream, as it should :)
		 */
		friend std::ostream& operator<<(std::ostream& stream, 
																		Command<T> & com) {
			stream << " command " << com.name() << " from generator " << com.generator()
						 << " which is a ";
			if ( com.nbdim() > 0 ) {
				std::vector< CLUE::FORTRAN_DIM>::iterator it;
				std::vector< CLUE::FORTRAN_DIM> dims = com.dimensions();
				stream << com.nbdim() << "D table of Fortran dimensions ";
				for (it = dims.begin(); it != dims.end(); ++it) 
					stream << "[" << (*it).lo() << ":" << (*it).up() << "]"; 
			}
			else stream << "scalar";
			
			return stream;
		};


		/**
		 * printouts the object to MsgStream
		 * @param stream an output stream 
		 * @param com an object to printout
		 * @return a reference to stream, as it should :)
		 
		friend MsgStream& operator<<(MsgStream& stream, 
																 Command<T> & com) {
			stream << " command " << com.name() 
						 << " from generator " << com.generator()
						 << " which is a ";
			if ( com.nbdim() > 0 ) {
				std::vector< CLUE::FORTRAN_DIM>::iterator it;
				std::vector< CLUE::FORTRAN_DIM> dims = com.dimensions();
				stream << com.nbdim() << "D table of Fortran dimensions ";
				for (it = dims.begin(); it != dims.end(); ++it) 
					stream << "[" << (*it).lo() << ":" << (*it).up() << "]"; 
			}
			else stream << "scalar";

			return stream;
		};
		
		*/
		private:		

		/** pointer to common variable  */
		T* m_val;
	
	}; // end of template Command<T> declaration



	///////////////////////////////////////////////////////////////////////////////
	// template<> class Command<char*>
  ///////////////////////////////////////////////////////////////////////////////

	/**
	 * @class Command<char*>
	 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
	 * @date 27/11/2006
	 * @brief Specialised template to cope with Fortran CHARACTER* (C/C++ char*)
 	 * variables.
	 */
	template <> 
		class Command<char*> : public CommandBase  {

	public:		

		Command(std::string gen, 
						std::string name, 
						std::string recipe, 
						char* ptr, 
						size_t len) {
			m_generator = gen;
			m_name = name;
			m_recipe = recipe;
			m_val = ptr; 
			m_len = len;
			parse_recipe();
		};
		
		/** 
		 * parsing command line
		 * @param command a string
		 * @throws CLUE::F77OutOufRange or CLUE::InvalidDim or CLUE::ParsingError
		 */
		void parse(std::string command) 
			throw( CLUE::F77OutOfRange, CLUE::InvalidDim, CLUE::ParsingError )
			{
	
				CLUE::StringParse sp(command);

				int f0, f1, f2, f3, f4, f5, f6;
				std::string val;
	
				switch ( nbdim() ) {
					
				case 0:
					if ( sp.getVal( val ) ) { 
						set( val.c_str() );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 1:
					if ( sp.getIndex( f0 ) && sp.getVal(val ) ) {
						set( val.c_str(), f0 );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 2:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getVal(val) ) {
						set( val.c_str(), f0 , f1);
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 3:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getVal(val) ) {
						set( val.c_str(), f0 , f1, f2);
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 4:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getVal(val) ) {
						set( val.c_str(), f0, f1, f2, f3);
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 5:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getVal(val) ) {
						set( val.c_str(), f0, f1, f2, f3, f4);
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 6:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getIndex(f5) &&
							 sp.getVal(val) ) {
						set( val.c_str(), f0, f1, f2, f3, f4, f5);
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
					
				case 7:
					if ( sp.getIndex(f0) && sp.getIndex(f1) && 
							 sp.getIndex(f2) && sp.getIndex(f3) && 
							 sp.getIndex(f4) && sp.getIndex(f5) &&
							 sp.getIndex(f6) && sp.getVal(val) ) {
						set( val.c_str(), f0, f1, f2, f3, f4, f5, f6 );
					} else throw CLUE::ParsingError( command, sp.what(), sp.where() );
					break;
				}
			}
		
		/** 
		 * getter for scalar variable 
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get() const 	
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 0 ) throw CLUE::InvalidDim( m_name, 0, m_dim );
				return m_val;
			};  

		/**
		 * getter for 1D table
		 * @param f0 Fortran77 index 
		 * @warning f0 could be signed, as in F77 code
		 */
		char* get(int f0) const 
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 1 ) throw CLUE::InvalidDim( m_name, 1, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c0 = (*it).index(f0);
				
				char* where = m_val + m_len*c0;
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  


		/**
		 * getter for 2D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @warning f0 and f1 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1) const
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange) 
			{
				if ( m_dim != 2 ) throw CLUE::InvalidDim( m_name, 2, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c1 = (*it).index(f0);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f1);
				
				char* where = m_val + m_len*(jump0*c0 + c1);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  

		
		/**
		 * getter for 3D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @warning f0,f1 and f2 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1,int f2) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 3 ) throw CLUE::InvalidDim( m_name, 3, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c2 = (*it).index(f0);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f1);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f2);
				
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + c2);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  
		
		
		/**
		 * getter for 4D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @warning f0-f3 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1,int f2,int f3) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 4 ) throw CLUE::InvalidDim( m_name, 4, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c3 = (*it).index(f0);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f1);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f2);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f3);
				
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + c3);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  
		
		/**
		 * getter for 5D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @warning f0-f4 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1,int f2,int f3,int f4) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 5 ) throw CLUE::InvalidDim( m_name, 5, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c4 = (*it).index(f0);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f1);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f2);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f3);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f4);
				
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + c4);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  
		
		
		/**
		 * getter for 6D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 5th F77 index
		 * @warning f0-f5 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1,int f2,int f3,int f4,int f5) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 6 ) throw CLUE::InvalidDim( m_name, 6, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c5 = (*it).index(f0);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f1);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f2);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f3);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f4);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f5);
				
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + c5);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  
		
		
		/**
		 * getter for 7D table
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index
		 * @param f6 7th F77 index
		 * @warning f0-f6 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		char* get(int f0,int f1,int f2,int f3,int f4,int f5,int f6) const
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 7 ) throw CLUE::InvalidDim( m_name, 7, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c6 = (*it).index(f0);
				unsigned int jump5 = (*it++).size();
				
				unsigned int c5 = (*it).index(f1);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f2);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f3);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f4);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f5);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f6);
				
				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + c6);
				char* val = new char[m_len];
				strncpy(val, where, m_len);
				return val;
			}  
		
		

		/** 
		 * setter for scalar variable
		 * @param val value to set
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 0 ) throw CLUE::InvalidDim( m_name, 0, m_dim );
				strncpy(m_val, val, m_len);
			}
		

		/** 
		 * setter for 1D table 
		 * @param val value to set
		 * @param f0 F77 index
		 * @warning f0 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 1 ) throw CLUE::InvalidDim( m_name, 1, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c0 = (*it).index(f0);
				
				char* where = m_val + m_len*c0;
				strncpy(where, val, m_len);
			}  

		/** 
		 * setter for 2D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @warning f0 and f1 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 2 ) throw CLUE::InvalidDim( m_name, 2, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c1 = (*it).index(f0);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f1);
				
				char* where = m_val + m_len*(jump0*c0 + c1);
				strncpy(where, val, m_len);
			}  


		/** 
		 * setter for 3D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @warning f0,f1 and f2 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1, int f2 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 3 ) throw CLUE::InvalidDim( m_name, 3, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c2 = (*it).index(f0);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f1);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f2);
				
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + c2);
				strncpy(where, val, m_len);
			}  
		
		
		/** 
		 * setter for 4D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @warning f0-f3 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1, int f2, int f3 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 4 ) throw CLUE::InvalidDim( m_name, 4, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c3 = (*it).index(f0);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f1);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f2);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f3);
				
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + c3);
				strncpy(where, val, m_len);
			}  
		

		/** 
		 * setter for 5D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @warning f0-f4 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1, int f2, int f3, int f4 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 5 ) throw CLUE::InvalidDim( m_name, 5, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c4 = (*it).index(f0);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f1);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f2);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f3);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f4);
				
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + c4);
				strncpy(where, val, m_len);
			}  
		
		

		/** 
		 * setter for 6D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index 
		 * @warning f0-f5 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1, int f2, int f3, int f4, int f5 )
				throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 6 ) throw CLUE::InvalidDim( m_name, 6, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c5 = (*it).index(f0);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f1);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f2);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f3);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f4);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f5);
				
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + c5);
				strncpy(where, val, m_len);
			}  
		
	 
		/** 
		 * setter for 7D table 
		 * @param val value to set
		 * @param f0 1st F77 index
		 * @param f1 2nd F77 index
		 * @param f2 3rd F77 index
		 * @param f3 4th F77 index
		 * @param f4 5th F77 index
		 * @param f5 6th F77 index 
		 * @param f6 7th F77 index
		 * @warning f0-f6 could be signed, as in F77 code
		 * @throws CLUE::InvalidDim or CLUE::F77OutOfRange
		 */
		void set( const char* val, int f0, int f1, int f2, int f3, int f4, int f5, int f6 )
			throw(CLUE::InvalidDim, CLUE::F77OutOfRange)
			{
				if ( m_dim != 7 ) throw CLUE::InvalidDim( m_name, 7, m_dim );
				std::vector<CLUE::FORTRAN_DIM>::const_iterator it;
				it = m_dims.begin();
				
				unsigned int c6 = (*it).index(f0);
				unsigned int jump5 = (*it++).size();
				
				unsigned int c5 = (*it).index(f1);
				unsigned int jump4 = (*it++).size();
				
				unsigned int c4 = (*it).index(f2);
				unsigned int jump3 = (*it++).size();
				
				unsigned int c3 = (*it).index(f3);
				unsigned int jump2 = (*it++).size();
				
				unsigned int c2 = (*it).index(f4);
				unsigned int jump1 = (*it++).size();
				
				unsigned int c1 = (*it).index(f5);
				unsigned int jump0 = (*it++).size();
				
				unsigned int c0 = (*it).index(f6);
				
				jump4 *= jump5;
				jump3 *= jump4;
				jump2 *= jump3;
				jump1 *= jump2;
				jump0 *= jump1;
				
				char* where = m_val + m_len*(jump0*c0 + jump1*c1 + jump2*c2 + jump3*c3 + jump4*c4 + jump5*c5 + c6);
				strncpy(where, val, m_len);
			}  
		

		/**
		 * print out the object to std::ostream
		 * @param stream reference to the std::ostream
		 * @param com object to print out
		 * @return reference to the std::ostream
		 */
		friend std::ostream& operator<<(std::ostream& stream, 
																		Command<char*> & com) {
			stream << " command " << com.name() 
						 << " from generator " << com.generator()
						 << " which is a ";
			if ( com.nbdim() > 0 ) {
				std::vector< CLUE::FORTRAN_DIM>::iterator it;
				std::vector< CLUE::FORTRAN_DIM> dims = com.dimensions();
				stream << com.nbdim() << "D table of Fortran dimensions ";
				for (it = dims.begin(); it != dims.end(); ++it) 
					stream << "[" << (*it).lo() << ":" << (*it).up() << "]"; 
			}
			else stream << "scalar ";
			stream << " of type char[" << com.len() << "]"; 

			return stream;
		};


		/**
		 * print out the object to MsgStream
		 * @param stream reference to the MsgStream 
		 * @param com object to print out
		 * @return reference to the MsgStream
		
		friend MsgStream& operator<<(MsgStream& stream, 
																 Command<char*> & com) {
			stream << " command " << com.name() 
						 << " from generator " << com.generator()
						 << " which is a ";
			if ( com.nbdim() > 0 ) {
				std::vector< CLUE::FORTRAN_DIM>::iterator it;
				std::vector< CLUE::FORTRAN_DIM> dims = com.dimensions();
				stream << com.nbdim() << "D table of Fortran dimensions ";
				for (it = dims.begin(); it != dims.end(); ++it) 
					stream << "[" << (*it).lo() << ":" << (*it).up() << "]"; 
			}
			else stream << " which is scalar";
			stream << " of type char[" << com.len() << "]"; 

			return stream;
		};
		*/


		/** lenght of char* table getter */
		size_t len() const { return m_len; };

	private:
		
		/** value */
		char* m_val;
		
    /** string lenght */
		size_t m_len;
		
	}; // end of Command<char*>





}; // end of chunk of namespace CLUE



///////////////////////////////////////////////////////////////////
// T Y P E D E F S 

//! typedef for INTEGER,LOGICAL \f$\rightarrow\f$ int 
typedef CLUE::Command< int >                     IntCmd;
//! typedef for REAL \f$\rightarrow\f$ float
typedef CLUE::Command< float >                   FloatCmd;
//! typedef for DOUBLE PRECISION \f$\rightarrow\f$ double
typedef CLUE::Command< double >                  DoubleCmd;
//! typedef for CHARACTER* \f$\rightarrow\f$ char*
typedef CLUE::Command< char* >                   CharCmd;
//! typedef for COMPLEX \f$\rightarrow\f$ complex< float > 
typedef CLUE::Command< std::complex < float > >  CmplxFCmd;
//! typedef for DOUBLE COMPLEX \f$\rightarrow$ complex< double >
typedef CLUE::Command< std::complex < double > > CmplxDCmd;



#endif //  COMMAND_H

