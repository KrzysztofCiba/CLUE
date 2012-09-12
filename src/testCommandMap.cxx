/**
 * @file testCommandMap.cxx
 * @author Krzysztof Ciba (Krzysztof.Ciba@NOSPAMagh.edu.pl)
 * @date 23/11/2006
 * @brief Just a test program for CommandMap interface
 */

// includes from standard libraries
#include <string>
#include <iostream>

// local includes
#ifndef DUMMYGEN_EXTERN_C_H
#include "DUMMYGEN_extern_C.h"
#endif

#ifndef DUMMY_MAP_H
#include "DUMMYGEN_map.h"
#endif

#ifndef COMMAND_H
#include "Command.h"
#endif

#ifndef  CLUTILS_H
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
	
	cout << cpp << "testCommandMap: initalize Fortran 77 common..." << endl;
	init_();
	writeout_();
	
	DUMMYGEN_LUT dlut;
	cout << cpp << dlut << endl;
	
	try {
	string cname("i1");
	cout << cpp 
			 << "testCommandMap: testing command getter from CommandMap for command name: " 
			 << cname << endl; 
	CommandBase* I1 = dlut.command(cname);
	Command<int>* icmd = dynamic_cast< Command<int>* >( I1 );
	print_i1_();
	cout << (*icmd) << endl;
	icmd->set(999);
	print_i1_();

	vector<string> cms; //< CoMmandStrings - sorry CMS :)

	cout << cpp << "testCommandMap: testing parsing of command vector..." << endl;
	print_i1_();
	print_i2_();
	print_rtab_();	
	print_planet_();	
	print_day_();

	cms.push_back("I1 123");
	cms.push_back("I2 321");
	cms.push_back("rtab 2 0.999");
	cms.push_back("Planet  VENUS");
	cms.push_back("DAY 1                       WEE");
	cms.push_back("DAY 7 END");
	cms.push_back("FOO bar  baz");

	vector<string>::iterator it;
	unsigned int i = 1;
	cout << cpp << "testCommandMap:  commands to parse are:" << endl;
	for (it = cms.begin(); it != cms.end(); ++it, ++i) 
		cout << cpp << "[" << i << "] " << (*it) << endl;
	cout << cpp << "testCommandMap: invoking CommandMap parse..." << endl;

	dlut.parse( cms );
	
	cout << cpp << "testCommandMap: checking the new values..." << endl;

	print_i1_();
	print_i2_();
	print_rtab_();	
	print_planet_();	
	print_day_();

	}
	catch ( CLUException& e ) {
		cout << e.what() << endl;
	}

	return 0;
}
