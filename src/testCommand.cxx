/**
 * @file testCommand.cxx
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 23/11/2006
 * @brief Just a test program for Command interface.
 */

// includes from standard libraries
#include <string>
#include <iostream>

// local includes
#ifndef DUMMYGEN_EXTERN_C_H
#include "DUMMYGEN_extern_C.h"
#endif


#ifndef COMMANDBASE_H
#include "CommandBase.h"
#endif

#ifndef COMMAND_H
#include "Command.h"
#endif

#ifndef CLUTILS_H
#include "CLUtils.h"
#endif

extern "C" {
void init_();	
void print_i1_();
void print_i2_();
void print_d1_();
void print_d1_();
void print_planet_();
void print_rtab_();
void print_itab_();
void print_dtab_();
void print_day_();
void writeout_();
};


// namespaces used
using namespace std;
using namespace CLUE;

ostream& cpp( ostream& stream ) 
{ 
	stream << " C++ ";
	return stream;
} 


int main() {
	
	cout << cpp << "testCommand# initalize Fortran 77 common" << endl;
	init_();
	writeout_();
	
	try {
	
		CLUE::Command<int> I2( std::string("DUMMYGEN"),
													 std::string("I2"),
													 std::string("I2 %d"),
													 global_.i2 );
		cout << I2 << endl;
		
		print_i2_();
		
		cout << cpp << " I2.get() = " << I2.get() << endl;
		cout << cpp << " calling I2.set(999)..." << endl;
		I2.set(999);
		cout << cpp << " I2.get() = " << I2.get() << endl;
		
		print_i2_();
		
	}
	catch ( CLUException& e ) {
		cout << e.what() << endl;
	}


	
	
	
	try {
		CLUE::Command<char*> PLANET( std::string("DUMMYGEN"),
																 std::string("PLANET"),
																 std::string("PLANET %s"),
																 &(*global_.planet), 10 );
		cout << cpp << PLANET << endl;
		print_planet_();
		cout << cpp << "PLANET.get() = " << PLANET.get() << endl;
		cout << cpp << "setting PLANET to MARS..." << endl;
		PLANET.set("MARS");
		cout << cpp << "PLANET.get() = " << PLANET.get() << endl;
		print_planet_();
	}
	catch ( CLUException &  e ) {
		cout << e.what() << endl;
	} 

 
	try {
		CLUE::Command<double> DTAB( std::string("DUMMYGEN"),
																std::string("DTAB"),
																std::string("DTAB %d(1#7) %d(1#5) %d(-2#2) %f"),
																(***global_.dtab) );

		cout << cpp << DTAB << endl;
		
		print_dtab_();
		
		
		cout << cpp << " BEFORE DTAB.set()" << endl;
		cout << cpp << " DTAB.get(-2,1,1) = " <<  DTAB.get(-2,1,1) << endl;
		cout << cpp << " DTAB.get( 2,5,7) = " <<  DTAB.get(2,5,7) << endl;
		cout << cpp << " setting 1st and last element of DTAB to zero" << endl; 	
		DTAB.set(0.0, -2, 1, 1);
		DTAB.set(0.0,  2, 5, 7);
		cout << cpp << " AFTER DTAB.set()" << endl;
		cout << cpp << " DTAB.get(-2,1,1) = " <<  DTAB.get(-2,1,1) << endl;
		cout << cpp << " DTAB.get( 2,5,7) = " <<  DTAB.get(2,5,7) << endl;

    print_dtab_();


	}
	catch ( CLUException& e ) {
		cout << e.what() << endl;
	} 


	try {
		
		CLUE::Command<char*> DAY(std::string("DUMMYGEN"),
														 std::string("DAY"),
														 std::string("DAY %d(1#7) %s"),
														 &(**global_.day), 3 );
		cout << cpp << DAY << endl;
		
		print_day_();
		
		cout << cpp << " DAY table BEFORE DAY.set()..." << endl;
		for (int i = 1; i <= 7; ++i) {
			cout << cpp << "DAY(" << i << ") = " << DAY.get(i) << endl;
		}
		cout << cpp << " setting 2nd day to XXX" << endl;
		DAY.set("XXX", 2);
		cout << cpp << " DAY table AFTER DAY.set()..." << endl;
		for (int i = 1; i <= 7; ++i) {
			cout << cpp << "DAY(" << i << ") = " << DAY.get(i) << endl;
		}
		print_day_();
	}
	catch ( CLUException& e ) {
		cout << e.what() << endl;
	} 
	
	return 0;
}
