/**
  * @file DUMMYGEN_map.h
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


#ifndef DUMMYGEN_MAP_H
#define DUMMYGEN_MAP_H

// includes from stdandard libraries
#include <map>
#include <string>
#include <complex>


// local includes
#ifndef COMMANDBASE_H
#include "CommandBase.h"
#endif

#ifndef COMMAND_H
#include "Command.h"
#endif

#ifndef COMMANDMAP_H
#include "CommandMap.h"
#endif



// begin of namespace CLUE

namespace CLUE {

  /**
   * @class DUMMYGEN_LUT
   * @brief DUMMYGEN_LUT declaration
   */
  class DUMMYGEN_LUT : public CLUE::CommandMap {

   public:

    /** constructor */
    DUMMYGEN_LUT(std::string generator = std::string("DUMMYGEN"))
     {
       m_initialised = false;
       m_generator = generator;
     };
    /** initalising */
    void init();

  }; // end of class DUMMYGEN_LUT

}; // end of namespace CLUE
#endif
