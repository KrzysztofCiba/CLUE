/**
  * @file DUMMYGEN_extern_C.h
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


#ifndef DUMMYGEN_EXTERN_C_H
#define DUMMYGEN_EXTERN_C_H

// includes from stdandard libraries
#include <map>
#include <string>
#include <complex>


// begin of namespace CLUE

namespace CLUE {

 /** @class Atlas_DUMMYGEN_GLOBAL
   * @brief definition of struct for common global_
   *  from generator DUMMYGEN
   */
 struct Atlas_DUMMYGEN_GLOBAL {
	 double d1;
	 double d2;
	 double dtab[7][5][5];
	 float rtab[10];
	 int i1;
	 int i2;
	 int itab[2][5];
	 char day[7][3];
	 char planet[10];
 };

}; // end of namespace CLUE

// begin of extern C struct declarations

extern "C" {
	extern struct CLUE::Atlas_DUMMYGEN_GLOBAL global_;

}; // end of extern C block
#endif
