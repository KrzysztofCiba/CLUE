/**
  * @file DUMMYGEN_map.cxx
  * @brief This is automaticly generated C++ header file
  * from Fortran inc file(s):
  * <ul>
  * <li> ../../src/common.inc
  * </ul>
  * using:
  * CLUE 0.0.1 by Krzysztof.Ciba@NOSPAMagh.edu.pl
  * @date Mon, 04 Dec 2006 15:15:06 CET
  *
  * @warning <i>DO NOT EDIT! ALL CHANGES will be lost
  * during next (re)generation</i>.
  */


#include "DUMMYGEN_extern_C.h"
#include "DUMMYGEN_map.h"

// begin of namespace CLUE

namespace CLUE {
/** map initalisation for generator DUMMYGEN */
void DUMMYGEN_LUT::init()
{

	 m_command_map[std::string("D1")] =
	 new CLUE::Command<double>( std::string("DUMMYGEN"),
	 std::string("D1"),
	 std::string("D1 %f"),
	 global_.d1 );

	 m_command_map[std::string("D2")] =
	 new CLUE::Command<double>( std::string("DUMMYGEN"),
	 std::string("D2"),
	 std::string("D2 %f"),
	 global_.d2 );

	 m_command_map[std::string("DTAB")] =
	 new CLUE::Command<double>( std::string("DUMMYGEN"),
	 std::string("DTAB"),
	 std::string("DTAB %d(1#7) %d(1#5) %d(-2#2) %f"),
	 (***global_.dtab) );

	 m_command_map[std::string("RTAB")] =
	 new CLUE::Command<float>( std::string("DUMMYGEN"),
	 std::string("RTAB"),
	 std::string("RTAB %d(1#10) %f"),
	 (*global_.rtab) );

	 m_command_map[std::string("I1")] =
	 new CLUE::Command<int>( std::string("DUMMYGEN"),
	 std::string("I1"),
	 std::string("I1 %d"),
	 global_.i1 );

	 m_command_map[std::string("I2")] =
	 new CLUE::Command<int>( std::string("DUMMYGEN"),
	 std::string("I2"),
	 std::string("I2 %d"),
	 global_.i2 );

	 m_command_map[std::string("ITAB")] =
	 new CLUE::Command<int>( std::string("DUMMYGEN"),
	 std::string("ITAB"),
	 std::string("ITAB %d(1#2) %d(1#5) %d"),
	 (**global_.itab) );

	 m_command_map[std::string("DAY")] =
	 new CLUE::Command<char*>( std::string("DUMMYGEN"),
	 std::string("DAY"),
	 std::string("DAY %d(1#7) %s"),
	 &(**global_.day), 3 );

	 m_command_map[std::string("PLANET")] =
	 new CLUE::Command<char*>( std::string("DUMMYGEN"),
	 std::string("PLANET"),
	 std::string("PLANET %s"),
	 &(*global_.planet), 10 );

 setInitialised();

};

}; // end of namespace CLUE
