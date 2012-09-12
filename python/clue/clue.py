#! /usr/bin/env python

#############################################################
# Name:     clue.py                                         #
# Purpose:  COMMON Look Up tablE                            #
# Author:   Krzysztof CIBA                                  #
# Created:  10/11/2006                                      #
#############################################################
 
import os, sys
import os.path
from types import *
import datetime
from  time import strftime, localtime
import re, string
import getopt
import fnmatch
import logging 

from clutils import clugger

_clue_version = "0.0.1"
_clue_author  = "Krzysztof.Ciba@NOSPAMagh.edu.pl"
_log_path = os.path.curdir

class clue:
    """
     Clue Clue (kl=u), n. See Clew, n.
     Clew Clew (kl=u), Clue Clue, n. OE. clewe, clowe, clue,
     AS. cleowen, cliwen, clywe ball of thread; akin to D. kluwen,
     OHG. chliwa, chliuwa, G. dim. kleuel, knaumuel, and perch.
     to L. gluma hull, husk, Skr. glaus sort of ball or tumor.
     Perch. akin to E. claw. root26. Cf. Knawel.
     1. A ball of thread, yarn, or cord; also, The thread itself.
     1913 Webster

     Untwisting his deceitful clew.        --Spenser.
     1913 Webster

     2. That which guides or directs one in anything of a doubtful
     or intricate nature; that which gives a hint in the
     solution of a mystery.
     1913 Webster

     The clew, without which it was perilous to enter the
     vast and intricate maze of countinental politics,
     was in his hands.                     --Macaulay.
     1913 Webster

     3. (Naut.)
     (a.) A lower corner of a square sail, or the after corner
     of a fore-and-aft sail.
     (b.) A loop and thimbles at the corner of a sail.
     (c.) A combination of lines or nettles by which a hammock
     is suspended.
     1913 Webster

     4. (ATLAS exp. @ CERN)
     CLUE (kl=u) A 'litte' python script to parse Fortran includes
     and automaticly generate C++ files. syn: Common Look Up tablE 

     What's the CLUE?                       --Muriel.
     

    """


    # mapping types to extren C block
    extern_C_types = { "INTEGER"          : "int",
                       "LOGICAL"          : "int",
                       "DOUBLEPRECISION"  : "double",
                       "REAL*8"           : "double",
                       "REAL*4"           : "float",
                       "REAL"             : "float",
                       "COMPLEX"          : "compex<float>",
                       "DOUBLECOMPLEX"    : "complex<double>",
                       "CHARACTER"        : "char" }

    fmtstr = { "int"             : "%d",
               "float"           : "%f",
               "double"          : "%f",
               "complex<float>"  : "%f %f",
               "complex<double>" : "%lf %lf",
               "char"            : "%s" }

    # Dictionary for PARAMETERs
    # format of record in dictionary: "PARMANE_STRING" -> "PARVALUE_STRING"
    # e.g.:
    #   parametersDict["PAR"] = "500"
    parametersDict = None

    # lines from input with PARAMETER statements
    parametersLines = []

    # Dictionary with common blocks entries
    # commonDict["COMMONNAME"] = ( var0, var1, var2)
    # varn = ( "varname", "vartype" )
    #           name       type OR    None OR 
    #                      None       tuple with sizes in C++ order
    commonDict = None

    # lines from input with COMMON keywords
    commonLines = []

    # Dictionary for implicit rules
    # format of record: "BIG_LETTER_STRING" -> "FORTRAN_TYPES"
    # e.g.:
    #   implicitDict["A"] = "DOUBLEPRECISION"
    implicitDict = None

    # lines from input with IMPLICIT keyword
    # at the beginning holds default Fortran 77 rule
    implicitLine = "IMPLICITREAL(A-H,O-Z),INTEGER(I-N)"

    # Dictionary for defined Fortran variables
    # 
    varDict = None

    # lines from input with variable declarations
    varLines = []

    # Dictionary with DIMESION
    dimensionDict = None

    # lines from input with DIMENSION keywords
    dimensionLines = []

    # Dictionary with Fortran 77 keywords
    # format of record "KEYWORD" -> "REGEXP_KEYWORD"
    # e.g.:
    #  fortran_keywords["REAL"] = "^REAL"
    #
    # TODO:
    # Check if there are any other keywords used in inc files.
    fortran_keywords = { "IMPLICIT"         : re.compile("^IMPLICIT"), 
                         "PARAMETER"        : re.compile("^PARAMETER"),
                         "INTEGER"          : re.compile("^INTEGER"),
                         "LOGICAL"          : re.compile("^LOGICAL"),
                         "DOUBLEPRECISION"  : re.compile("^DOUBLEPRECISION"),
                         "REAL*8"           : re.compile("^REAL\*8"),
                         "REAL*4"           : re.compile("^REAL\*4"),
                         "REAL"             : re.compile("^REAL"),
                         "DOUBLECOMPLEX"    : re.compile("^DOUBLECOMPLEX"),
                         "COMPLEX"          : re.compile("^COMPLEX"),
                         "CHARACTER"        : re.compile("^CHARACTER"),
                         "COMMON"           : re.compile("^COMMON"),
                         "DIMENSION"        : re.compile("^DIMENSION"),
                         "EXTERNAL"         : re.compile("^EXTERNAL"),
                         "EQUIVALENCE"      : re.compile("^EQUIVALENCE")}

    # Dictionary with Fortran 77 data types
    # A subset of fortran_keywords dictionary, that
    # holds only data types. Format of entry as above.
    fortran_types = { "INTEGER"         : re.compile("^INTEGER"),
                      "LOGICAL"         : re.compile("^LOGICAL"),
                      "REAL*4"          : re.compile("^REAL\*4"),
                      "REAL*8"          : re.compile("^REAL\*8"),
                      "REAL"            : re.compile("^REAL"),
                      "DOUBLEPRECISION" : re.compile("^DOUBLEPRECISION"),
                      "CHARACTER"       : re.compile("^CHARACTER"),
                      "DOUBLECOMPLEX"   : re.compile("^DOUBLECOMPLEX"),
                      "COMPLEX"         : re.compile("^COMPLEX") }
                               
    # Openning modes for files.
    # * modeR -> Open file for reading only.
    # * modeA -> Open file for writting (append new lines just after the EOF)
    # * modeW -> Open file for writting and truncate.
    modeR = "r"
    modeW = "w+"
    modeA = "a"

    # Default name for generator
    # Probably user should avoid of using default name. Only for testing
    # purposes.
    generator = "DUMMY"

    # List of additional parameters, as specified in command line
    param = []

    # List of Fortrtan 77 files to parse
    inc_list = [] 

    # Name of currnet Fortran 77 parsed file.
    cur_inc = None

    # A list of Command<TYPE> objects. 
    commands = [ ]

    # List of C++ variables
    cvars = []

    # List with names of C++ generated files.
    # Now holds three entries:
    # * <GENERATORNAME>_extern_C.h
    # * <GENERATORNAME>_map.h
    # * <GENERATORNAME>_map.cxx
    out_list = [] 

    def __init__(self, list, gen = None, param = None, lvl = logging.INFO ):
        """
        clue constructor
        
        parameters:
        list  - a list of include Fortran 77 files
        param - a list of additional parameters 
        gen   - a name of generator
        lvl   - a logging level
        """
        
        # get current time
        now = strftime("%d-%b-%Y-%H-%M", localtime())
        
        # start logger        
        logFile = _log_path + os.sep + "log-" + now +  ".log"

        # get clugger (logger wrapper)
        self.log = clugger.clugger(logFile, lvl)

        # say hello
        self.info("Welcome to COMMON Look Up tablE")
        now = strftime("%a, %d %b %Y %H:%M:%S %Z" , localtime())
        self.info( now )
        self.info("clue# checking configuration...")

        # check configuration
        if ( gen ):
            self.generator = string.upper(gen)
            self.info( "clue# generator set to " + str(self.generator) ) 
        else:
            self.warning( "clue# generator no set - using defualt " + str( self.generator ) )

        
        self.inc_list = list
        self.info( "clue# file list to parse " + str(self.inc_list) )

        self.out_list.append( self.generator + "_extern_C.h" )
        self.out_list.append( self.generator + "_map.h" )
        self.out_list.append( self.generator + "_map.cxx" )

        for entry in self.out_list:
            msg = "clue# will produce output file # " + entry
            self.info( msg )

        
        self.namespace = "Atlas_" + string.upper(self.generator) 
        self.info( "clue# setting namespace to " + str(self.namespace) )


        # build default implicit rules dictinary
        self.debug("clue# building default implicit dictionary")
        self.build_implicit_dict()
        self.implicitLine = ""

        # for every file in list read the content and parse
        lines = []

        # add parameters from command line
        if (param != None):
            for p in param:
                self.info("clue# got additional parameter " + p )
                lines.append( "      PARAMETER(" + p + ")" )

        for entry in self.inc_list:
            self.cur_inc = entry
            inc_file = self.getFile( entry, self.modeR )
            lines += inc_file.readlines()
            inc_file.close()
            self.debug("clue# read " + str(len(lines)) + " lines from file " + entry )

        status = self.parse_fortran( lines )

        status = self.generate()

        self.info("clue#  Work complete!")
        pass

        
    def verbose( self, msg ):
        self.log.verbose( msg )

    def debug( self, msg ):
        self.log.debug( msg )

    def info( self, msg ):
        self.log.info( msg )

    def warning( self, msg ):
        self.log.warning( msg )

    def error( self, msg ):
        self.log.warning( msg )

    def critical( self, msg ):
        self.log.critical( msg )

    def getFile(self, name, mode=None ):
        """
        Returns opened file of a name \"name\" in mode \"mode\".
        Parameters:
          name - string with full path to file
          mode - string with opening mode (e.g. \"r\", \"w\" )
        
        If mode is not specified, the file is opened only for READING.
        """

        thefile = None
        if ( mode == None ):
            mode = self.modeR
        try:
            thefile = open( name , mode)
            self.debug("getFile# file " + name + " opened in mode " + mode )
            return thefile
        except IOError:
            self.critical("getFile# opening of file " + name + " in mode " + mode +
                          " failure - exiting")
            sys.exit("error opening file " + name + " in mode " + mode)
       

    def parse_fortran( self, inc_lines ):
        """
        Main parsing function:

        Parameters:
         inc_lines - a raw list of lines read form Fortran 77 inc file.

        Parsing chain:
        1. remove_fortran_com_and_cont -> removing of empty and comment lines etc.
        2. cutting the rest into lexigraphic slices (internally)
        3. build_dictos                -> building dictionaries:
          3a. build_implicit_dict         -> building dict of implicit rules 
          3b. build_parameters_dict       -> building dict of parameters
          3c. build_vars_dict             -> building dict of variables
          3d. build_dimension_dict        -> building dict of dimensions
          3e. build_common_dict           -> building dict of commons

        Returns True, but could also exit the execution in case of errors.

        """

        inc_lines = self.remove_fortran_com_and_cont( inc_lines )
        
        all_slices = []

        for i in range(len(inc_lines)):
            for key in self.fortran_keywords.keys():
                if ( self.fortran_keywords[key].search(inc_lines[i]) ):
                    self.debug("parse_fortran# found " + key + " in line " + str(i) )
                    all_slices.append( ( i, key ) )
                    
        if ( self.build_dictos( inc_lines , all_slices) ): pass
        else:
            msg = "parse_fortran# error while building dictionaries"
            self.critical(msg)
            sys.exit(msg)
                    
        return True

                    
    def build_dictos( self, lines, slices ):
        """
        Build dictionaries of IMPLICIT rules, PARAMETERs, variables,
        DIMENSIONs and COMMONs.

        Parameters:
         lines  - a list of lines w/o whitespaces and EOL chars
         slices - a list of tuples ( chunk_beg, chunk_end ), marking
                  the begginig and ending line in lines, holding
                  the chunk of Fortran 77 code which begins with one of
                  Fortran keyword

        Returns True, but could also exit when some error occures.
        """

        for ik in slices:
            index = ik[0]
            key = ik[1]
            next_index = slices.index(ik)+1
            if (next_index >= len(slices)):
                if ( self.get_lines( lines[index:] ) ) : pass
            else:
                next_index_slice = slices[next_index][0]
                if ( self.get_lines( lines[index:next_index_slice] ) ) : pass


        if ( len(self.implicitLine) ):
            self.debug("************************************************************")
            self.debug("****** found IMPLICIT statement, building dictionary  ******")
            if ( self.build_implicit_dict() ) : pass
            else:
                self.critical("Error occured when parsing IMPLICIT")
                sys.exit("Error occured when parsing IMPLICIT")
                
        else:
            self.info("no IMPLICIT statements found, using Fortran77 default")
            
        if ( len(self.parametersLines) ):
            self.debug("************************************************************")
            self.debug("****** found PARAMETER statement, building dictionary ******")
            if ( self.build_parameters_dict() ) : pass
            else:
                self.critical("Error occured when parsing PARAMETERs")
                sys.exit("Error occured when parsing PARAMETERs")
        else:
            self.info("no PARAMETERS statements found")

     
        if ( len(self.varLines) ):
            self.debug("************************************************************")
            self.debug("****** found variables defnition, building dictionary ******")
            if ( self.build_var_dict() ) : pass
            else :
                self.critical("Error occured when parsing variables")
                sys.exit("Error occured when parsing variabless")
        else:
            self.warning("no variables definition found")

        if ( len(self.dimensionLines) ):
            self.debug("************************************************************")
            self.debug("****** found DIMENSION statement, building dictionary ******")
            if ( self.build_dimension_dict() ) : pass
            else:
                self.critical("Error occured when parsing DIMENSIONs")
                sys.exit("Error occured when parsing DIMENSIONs")
        else:
            self.info("no DIMENSION statements found")
            
            
        if ( len(self.commonLines) ):
            self.debug("************************************************************")
            self.debug("****** found COMMON statement, building dictionary... ******")
            if ( self.build_common_dict() ) : pass
            else:
                self.critical("Error occured when parsing COMMONs")
                sys.exit("Error occured when parsing COMMONs")
        else:
            self.critical("!!! no COMMON blocks definitions in file !!!")
            sys.exit("Probably wrong file (not Fortran 77 include), no COMMONs inside!")        
        
        return True

    def get_lines( self, list ):
        """
        Slices the Fortran 77 input lines into lexigraphic chunks. 
        """
        line = string.join(list, "")
        line = line.strip(string.whitespace)

        for key in self.fortran_keywords.keys():
            if ( self.fortran_keywords[key].search(line) ):

                # skip external 
                if ( key == "EXTERNAL" ): continue

                # slice implicit
                if ( key == "IMPLICIT" ):
                    self.implicitLine = line
                    continue

                # slice PARAMETER
                if ( key == "PARAMETER" ):
                    self.parametersLines.append( line )
                    continue

                # slice COMMON
                if ( key == "COMMON"):
                    self.commonLines.append( line )
                    continue

                # slice DIMENSION
                if ( key == "DIMENSION"):
                    self.dimensionLines.append( line )
                    continue

                # other are possible variables declarations, slice it
                if ( key not in ( "IMPLICIT", "PARAMETER", "COMMON", "DIMENSION", "EXTERNAL" ) ):
                    self.varLines.append( line )

        return True

    def build_implicit_dict(self):
        """
        Builds dictionary with implicit names for certain types
        Parses Fortran77 IMPLICIT statements.
        """
        
        line = self.implicitLine
        implicit_rules = [ ]

        self.info("buildIMPdict# got line \"" + line + "\"" )
        # remove IMPLICIT keyword
        line = line.replace("IMPLICIT", "")
        # build empty dictionary
        if (line == "NONE"):
            self.debug("buildIMPdict# setting IMPLICIT rules to \"None\" for all letters")
            for char in list(string.ascii_uppercase):
                implicit_rules.append( (char, None ) )
        else:
            raw_implicit_rules = [ ]
            # slice it using ) delimiter
            raw_chunks = line.split(")")
            chunks = []
            for chunk in raw_chunks:
                if ( len(chunk) > 0 ):
                    if ( chunk[0] == "," ):
                        chunk = chunk[1:].split("(")    
                    else:
                        chunk = chunk.split("(")

                    type = chunk[0]

                    raw_rules = chunk[1].split(",")
                    rules = []
                    rule = ""
                    for raw_rule in raw_rules:
                        raw_rule = raw_rule.split("-")
                        if ( len( raw_rule ) == 1 ):
                            rule = rule + raw_rule[0]
                        else:
                            upper = string.ascii_uppercase
                            lo = upper.find( raw_rule[0] )
                            up = upper.find( raw_rule[1] )
                            rule = rule + upper[lo:up+1]
                                    
                            
                    implicit_rule = ( type, rule )
                    raw_implicit_rules.append( implicit_rule )

            for ( type , letters ) in raw_implicit_rules:
                self.info("buildIMPdict# changing IMPLICIT rules for type " + type + 
                          " letters: " + letters )
                for char in letters:
                    implicit_rules.append( ( char, type ) )

        if ( self.implicitDict == None ):
            self.implicitDict = dict( implicit_rules )
        else:
            for ( char, type ) in implicit_rules:
                self.implicitDict[char] = type

        keys = self.implicitDict.keys()
        self.debug("buildIMPdict# IMPLICIT rules are:")
        for key in string.ascii_uppercase:
            self.debug("buildIMPdict# ...key " + key + " type " + str(self.implicitDict[key]))

        return True
            
    def build_parameters_dict(self):
        """
        Builds dictionary with parameters and their values.
        Parses Fortran 77 PARAMETER statements.

        """
        
        lines = self.parametersLines

        parameters = []
        for line in lines:
            self.info("buildPARdict# got line \"" + line + "\"")
            # remove PARAMETER keyword
            parameters_line = line.replace("PARAMETER", "").strip()
            # remove brackets, split string using comma
            parameters_chunks = parameters_line.lstrip("(").rstrip(")").split(",")
            for chunk in parameters_chunks:

                # split PARAMETER entry into name and value
                chunk = chunk.split("=")
                pkey = chunk[0].strip()
                pval = chunk[1].strip()

                # search for DOUBLE PRECISION constant
                # and replace "D" with "E" in exponent
                ReDOUBLEPREC = re.compile("[-+]?[0-9]*\.?[0-9]+([D][-+]?[0-9]+)?")
                if ( ReDOUBLEPREC.search( pval ) ):      
                    pval = pval.replace("D", "E", 100)
                    
                # append name and its value to list
                parameters.append( [ pkey, pval ] )            
                # rebuild dictionary
            self.parametersDict = dict(parameters)

        # print whole dictionary to debug logger
        for key in self.parametersDict.keys():
            val = self.parametersDict[key]
            if ( val not in string.digits ): 
                val = self.replace_params( val )
                self.parametersDict[key] = val
            
            self.debug("buildPARdict# parameter " + key + " of value " +  val )

        return True
        

    def build_common_dict(self):
        """
        Builds dictionary of common blocks.
        Parses Fortran 77 COMMON statements.

        """
        lines = self.commonLines
        
        list = [ ]
        for line in lines:
            self.info("buildCOMdict# got line \"" + line + "\"")

            common = line.replace("COMMON", "").strip("/")
            common_name = common[:common.find("/")]
            common = common.replace(common_name+"/", "")

            self.debug("buildCOMdict# common name " + common_name)

            common = self.search_and_replace_brackets(common)
            common_vars =  common.split(",")


            # add information about table sizes
            vars = []
            for var in common_vars:
                vname = var
                vsize = ""
                if ( vname.find("[") != -1 ):
                    vname = var[:var.find("[")]
                    vsize = var[var.find("["):]                       
                
                vars.append( ( vname, vsize )  )
                
            list.append( (common_name, vars ) )

        self.commonDict = dict( list )

        for key in self.commonDict.keys():
            self.debug("buildCOMdict# found common " + key + " with variables:")
            for ( vname, vsize ) in  self.commonDict[key]:
                msg = "buildCOMdict# " + vname
                if ( vsize != "" ): msg += " of size " + vsize 
                self.debug(msg)

        return True

    


    def stars_and_stripes( self, star_line ):
        """
        Ladies and Gentelmen! Attention!
        Pa pa-pa pa pa pa! Pa pa-pa pa pa ra!
        Stand at easy!

        But also remove stars (*) and strip spaces:
        e.g:
        line like this: 
               CHARACTER*5 A
        will be transformed to: 
               CHARACTERA[5]

               character b*10
               characterb[10]

        Parameters:
        star_line - a line containing * character

        Returns:
        A line transformed in a way as above. 
        
        """
        line = star_line
        
        new_line = ""
        star_pos = line.find("*")

        self.info("stars@stripes# got line \"" + star_line + "\"")
        
        if ( star_pos != -1):
            self.debug("stars@stripes# found star in line")

            # star at the beginning
            if ( star_pos == 0 ):
                # remove star from line
                line = line[1:]

                # get size 
                global_size = ""
                for char in line:
                    if ( char in string.digits ):
                        global_size = global_size + char
                    else:
                        break

                self.debug("stars@stripes# global size = " + global_size)

                # remove size from line
                line = line.replace(global_size,"", 1)

                # split rest
                raw_chunks = line.split(",")
                
                chunks = []
                chunk_size = ""
                for raw_chunk in raw_chunks:
                    self.debug("stars@stripes# got chunk " + raw_chunk )
                    new_star_pos = raw_chunk.find("*")
                    if ( new_star_pos != -1 ):
                        chunk_size = raw_chunk[new_star_pos+1:]
                        chunk = raw_chunk[:new_star_pos]
                    else:
                        chunk_size = global_size
                        chunk = raw_chunk
                    self.debug("stars@stripes# new chunk " + chunk + " of size " + chunk_size )
                    chunks.append( chunk + "(" + chunk_size + ")" )

                for chunk in reversed(chunks):
                    new_line = new_line + "," + chunk
                new_line = new_line.strip(",")
            else:
                # no global size 
                # all sizes are individual
                raw_chunks = line.split(",")
                chunks = []
                chunk_size = ""
                for raw_chunk in raw_chunks:
                    self.debug("stars@stripes# got chunk " + raw_chunk )
                    new_star_pos = raw_chunk.find("*")
                    if ( new_star_pos != -1 ):
                        chunk_size = raw_chunk[new_star_pos+1:]
                        chunk = raw_chunk[:new_star_pos]
                    else:
                        chunk_size = global_size
                        chunk = raw_chunk
                    self.debug("stars@stripes# new chunk " + chunk + " of size " + chunk_size )
                    chunks.append( chunk + "(" + chunk_size + ")" )

                for chunk in reversed(chunks):
                    new_line = new_line + "," + chunk
                new_line = new_line.strip(",")

                
        self.debug("stars@stripes# return line \"" + new_line + "\"" )
        return new_line
    

    def build_var_dict(self):
        """
        A little messy building variable list and dictionary.

        Returns: True
        """

        all_vars = []
        lines = self.varLines

        for line in lines:
            self.info("buildVARdict# got line \"" + line + "\"")

            vtype = None
            var_names_list = None
           
            # find type
            for key in self.fortran_types.keys():
                if ( self.fortran_types[key].search(line) ):
                    vtype = key                    
               
            var_names_list = line.replace(vtype, "")

            # replace REAL*8 with DOUBLEPRECISION
            if ( vtype == "REAL*8" ): vtype = "DOUBLEPRECISION"
            # replace REAL*4 with REAL
            if ( vtype == "REAL*4" ): vtype = "REAL"

            # a little messy CHARACTER size parsing 
            if ( vtype == "CHARACTER" ):
                var_names_list = self.stars_and_stripes( var_names_list )
            
            if ( vtype == None ):
                self.critical("buildVARdict# not known type in line \"" + line + "\"" )
                sys.exit("buildVARdict# not known type in line \"" + line + "\"")
            
            # replace brackets () with [], revert the indexes
            var_names = self.search_and_replace_brackets( var_names_list )
            # split variables
            var_names = var_names.split(",")

            # TODO: add information about table sizes
            for var_name in var_names:
        
                vname = var_name
                vsize = ""
                if ( var_name.find("[") != -1 ):
                    vname = var_name[:var_name.find("[")]
                    vsize = var_name[var_name.find("["):]
                
                    
                all_vars.append( ( vname, ( vsize, vtype ) ) )
            
        if ( self.varDict == None ):
            self.varDict = dict( all_vars )

        self.debug("buildVARdict# variable dictionary entries:")
        for vname in sorted(self.varDict.keys()):
            ( vsize, vtype ) = self.varDict[vname]
            msg = "buildVARdict# " + vname + " of type " + vtype
            if ( vsize != "" ):
                msg += " and size " + vsize
            self.debug(msg)

        return True

    def build_dimension_dict(self):
        """
        Builds dictionary from DIMENSION statements.
        Returns True.
        """

        lines = self.dimensionLines
        dimension_vars = []

        for line in lines:
            self.info("buildDIMdict# got line \"" + line + "\"")
            
            # remove DIMENSION keyword
            line = line.replace("DIMENSION", "", 1)

            # replace () with []
            line = self.search_and_replace_brackets( line )

            # split variables with comma
            raw_chunks = line.split(",")

            var_name = ""
            for raw_chunk in raw_chunks:
                open_bracket = raw_chunk.find("[")
                var_name = raw_chunk[0:open_bracket]
                dimension_vars.append( ( var_name, raw_chunk ) )

        if ( self.dimensionDict == None ):
            self.dimensionDict = dict( dimension_vars )
        self.debug("buildDIMdict# dimension dictionary entries:")
        for key in self.dimensionDict.keys():
            self.debug("buildDIMdict# " + key + " of dimension " + self.dimensionDict[key])

        return True


    def search_and_replace_brackets( self, var_line ):
        """
        Replaces Fortran 77 brackets with C/C++ brackets in array declarations
        e.g.
         (a,b,-d:d) with [2d+1][b][a].

        Parameters:
        var_line - a string

        Returns:
        A copy of var_line with all brackets replaced.

        """
        
        line = var_line
        self.info("search&replace_()# got line \"" + var_line + "\"")

        # search for ( ) bracket pairs
        open_bra = line.find("(")
        brackets = []
        cur_pos = 0
        while ( open_bra  != -1 ):
            cur_pos = cur_pos + open_bra + 1
            close_bra = line[cur_pos:].find(")")
            if ( close_bra != -1):
                brackets.append( ( cur_pos + 1, cur_pos + close_bra + 1 ) )
                cur_pos = cur_pos + close_bra + 1
            open_bra = line[cur_pos:].find("(")

        # find old and build new chunks
        chunks = []
        for ( open_bra, close_bra ) in brackets:
            dim_line =  line[open_bra-1:close_bra-1]

            dims = dim_line.split(",")
            dims.reverse()

            new_chunk = ""
            for dim in dims:

                colon_pos = dim.find(":")
                lo = None
                up = None
                if ( colon_pos != -1):
                    dim = dim.split(":")
                    lo = self.replace_params( dim[0] )
                    up = self.replace_params( dim[1] )
                    dim = str(eval( up + " - " + lo + "+ 1"))
                    self.debug("search&replace_()# found : in chunk, lo=" + lo + ", up=" + up +
                               " new dim= " + dim)
                else:
                    dim = self.replace_params( dim )


                # build up a new chunk
                new_chunk = new_chunk + "[" + str(eval(dim))
                if ( lo != None and up != None ):
                    new_chunk = new_chunk + "#" + lo + "#" + up
                new_chunk = new_chunk + "]"
            # append new chunk to all chunks
            chunks.append( ( "(" + dim_line + ")" , new_chunk ) )

        # replace old dimension chunks with a new one
        for ( old, new ) in chunks:
            self.debug("search&replace_()# replacing " + old + " with " + new + " chunk") 
            line = line.replace(old, new)
            
        self.info("search&replace_()# return line \"" + line + "\"")
        return line
            

    def replace_params(self, chunk):
        """
        Replaces all occurences of PARAMETER with its value.        
        Returns a copy of string with all parameters replaced by their values.
        """
        if ( self.parametersDict != None ):
            # replace PARAMETERs by constants
            for key in self.parametersDict.keys():
                RePAR = re.compile(key)
                if ( RePAR.search(chunk) ):
                    val = self.parametersDict[key]
                    self.debug("replacePAR# found " + key + 
                                   " in parameters dictionary, replacing it with " + val)
                    chunk = chunk.replace( key, val )
            # evaluate a new value and convert it to string
            chunk = str( eval(chunk) )
        return chunk


                    
    def remove_fortran_com_and_cont(self,  inc_lines ):
        """
        A lot of working here:
        1. removing of empty lines,
        2. removing of fortran comments,
        3. removing of 6 first columns,
        4. removing of inline comments (after excalmation mark)
        5. removing of EOLs
        6. replacing \"DOUBLE FOO\" with \"DOUBLEFOO\" 

        """
        self.info("remF77comments&cont# lines before removing of Fortran coments " +
                  str(len(inc_lines) ) )
        self.log_lines( inc_lines )
        
        lines = []
        cont_line = ""
        continuation = False
        
        # strip coments and continuation marks
        for line in inc_lines:
                
            # empty line
            if line.isspace(): continue
            
            # 1st char is space ?
            if ( not line[0].isspace() ): continue
            
            # remove comment after exclamation mark
            cut = line.find("!")
            if ( cut != -1 ): line = line[:cut]        

            # cut to 6th column
            line = line[6:].strip("\n").upper()
            words = line.split()
            line = ""
            # remove spaces, tabs etc.
            for word in words:
                line += word

            lines.append( line ) 
        
        
        
        self.debug("remF77comments&cont# lines after removing of Fortran comments " + str(len(lines)) )
        self.log_lines( lines )
        return lines
        

    def generate( self ):
        """
        Main C++ code generation function.
        
        """


        # open and truncate files
        extern_C_header_file = self.getFile( self.out_list[0], self.modeW )
        map_header_file = self.getFile( self.out_list[1], self.modeW )
        map_body_file = self.getFile( self.out_list[2], self.modeW ) 

        # write headers
        extern_C_header_file.write( self.header_of_file( self.out_list[0] ) )
        macrovar =  str.upper(self.generator + "_EXTERN_C_H\n")
        extern_C_header_file.write( "#ifndef " + macrovar )
        extern_C_header_file.write( "#define " + macrovar )

    
        
        map_header_file.write( self.header_of_file( self.out_list[1] ) )
        macrovar = str.upper(self.generator + "_MAP_H\n")
        map_header_file.write( "#ifndef " + macrovar )
        map_header_file.write( "#define " + macrovar )

        
        map_body_file.write(  self.header_of_file( self.out_list[2] ) )

        
        extern_C_header_file.write( self.std_includes() )
        extern_C_header_lines = self.generate_extern_C_body()
        extern_C_header_file.write ( extern_C_header_lines  )
        extern_C_header_file.write("#endif\n")

        includes = self.std_includes()
        includes += "\n// local includes\n"
        includes += "#ifndef COMMANDBASE_H\n"
        includes += "#include \"CommandBase.h\"\n"
        includes += "#endif\n\n"
        includes += "#ifndef COMMAND_H\n"
        includes += "#include \"Command.h\"\n"
        includes += "#endif\n\n"
        includes += "#ifndef COMMANDMAP_H\n"
        includes += "#include \"CommandMap.h\"\n"
        includes += "#endif\n\n\n"
        
        map_header = self.wrap_namespace( self.generate_map_header() )

        map_header = includes + map_header

        map_header_file.write( map_header )

        map_header_file.write("#endif\n")

        includes1 = "#include \"" + self.out_list[0] + "\"\n"
        includes1 += "#include \"" + self.out_list[1] + "\"\n"
        
        map_body = self.wrap_namespace( self.generate_map_body() ) 

        map_body = includes1 + map_body

        map_body_file.write( map_body )

        pass

        

    def generate_extern_C_body( self ):
        """
        
        """

        colend = ";\n"

        header = ""
        
        common_names = [ ]
        for common in self.commonDict.keys():

            loc_common_name = "Atlas_" + self.generator + "_" + common
            ext_common_name = string.lower(common) + "_"

            common_names.append( ( loc_common_name, ext_common_name ) )
            
            # begin struct
            header += "\n"
            header += " /** @class " + loc_common_name + "\n" 
            header += "   * @brief definition of struct for common " + ext_common_name + "\n"
            header += "   *  from generator " + self.generator + "\n" 
            header += "   */\n"
            header += " struct " + loc_common_name +  " {\n"
            variables = self.commonDict[common]

            # get all the names 
            for ( vname, vsize ) in variables:
                
                vtype = None
                
                # variable defined?
                if ( self.varDict != None ):
                    if ( vname in self.varDict.keys() ):
                        ( size, type ) = self.varDict[vname]
                        vtype = type
                        if ( size != vsize ):
                            vsize = vsize + size
                            # was reverted vsize = size + vsize
                            
                                

                # not defined? get the implicit type                             
                if ( ( vtype == None ) and
                     ( self.implicitDict != None ) ):
                    vtype = self.implicitDict[vname[0]]

                if ( vtype == None ):
                    msg = "generate_extern_C_body# *** cannot set type for variable " + vname + " ***"
                    self.critical(msg)
                    sys.exit(msg)
                    
                    

                # variable has dimension defined?
                if ( ( self.dimensionDict != None ) and
                     ( vname in self.dimesionsDict.keys() ) ):
                    size = self.dimesionsDict[val]
                    if ( size != vsize ):
                        # was reverted size + vsize
                        vsize =  vsize + size 

                # check the size
                recipe = "\"" + vname
                dim_table = []
                lenght = None
                if ( vsize != "" ):
                    sizes = vsize.split("[")
                    for size in sizes:
                        if ( size != ""): 
                            size = size.strip("]")
                            size = size.split("#")
                            if ( len(size) == 3 ):
                                dim = size[0]
                                lo = size[1]
                                up = size[2]
                                dim_table.append( (dim, lo, up) )
                            else:
                                dim = size[0]
                                dim_table.append( (dim, "1", dim ) )
                    vsize = ""
                    for i in range(len(dim_table)):
                        ( dim, lo, up ) = dim_table[i]
                        if (  vtype == "CHARACTER"  ):
                            if ( i == len(dim_table)-1 ):
                                lenght = dim
                                vsize += "[" + dim + "]"
                                continue
                        vsize += "[" + dim + "]"
                        recipe += " %d(" + lo + "#" + up + ")"
                             

               
                
                # get C++ type
                ctype = self.extern_C_types[vtype]
                cname = string.lower(vname)

                
                recipe += " " + self.fmtstr[ctype] + "\""
                                                       
                header += "\t " + ctype + " " + cname + vsize + colend
                command = "Command<" + ctype + "> " + vname + "(&" + \
                          ext_common_name + "." + cname + ", " + recipe + ")" + colend
                self.commands.append( command )

                ptr = ext_common_name+"."+cname
                if ( len(dim_table) > 0 ):
                    if ( ctype == "char") :
                        ctype = ctype + "*"
                        ptr = "&(" + "*"*(len(dim_table)) + ptr + ")"
                    else:
                        ptr = "(" + "*"*len(dim_table) + ptr + ")"
                        
                self.cvars.append( ( vname, ptr, ctype , vsize ,  recipe , dim_table, lenght) )
                                   
            # end struct
            header += " };\n"
                                   
        header = self.wrap_namespace( header )

        extC = self.begin_of_extern_C()
        for ( loc, ext ) in common_names:
            extC += "\textern struct CLUE::" + loc + " " + ext + colend
        
        extC += self.end_of_extern_C()

        header = header + extC
        return header

    def generate_map_header( self ):
        colend = ";\n"
        mapname = self.generator + "_LUT"
        line =  "\n  /**\n"
        line += "   * @class " + mapname + "\n"
        line += "   * @brief " + mapname + " declaration\n"
        line += "   */\n"
        line += "  class " +  mapname + " : public CLUE::CommandMap {\n\n"
        line += "   public:\n\n"
        line += "    /** constructor */\n"
        line += "    " + mapname + "(std::string generator = std::string(\"" + self.generator  + "\"))\n"
        line += "     {\n"
        line += "       m_initialised = false" + colend
        line += "       m_generator = generator" + colend
        line += "     }" + colend
        line += "    /** initalising */\n"
        line += "    void init()" + colend
        line += "\n  }; // end of class " + mapname + "\n"
        return line

    def generate_map_body( self ):
        colend = ";\n"
        mapname = self.generator + "_LUT"
        lines = "/** map initalisation for generator " + self.generator + " */\n"
        lines += "void " + mapname + "::init()\n{\n" 
        for ( vname, cname, ctype, csize, recipe, dim_table, lenght ) in self.cvars:
            lines += "\n\t m_command_map[std::string(\"" + vname + \
                     "\")] =\n\t new CLUE::Command<" + ctype + ">( " + \
                     "std::string(\"" + self.generator + "\"),\n\t " + \
                     "std::string(\"" + vname + "\"),\n\t " + \
                     "std::string(" + recipe + "),\n\t " + cname
            if ( lenght != None ): lines += ", " + str(lenght) + " )" +  colend
            else: lines +=  " )" +  colend
        lines += "\n setInitialised()" + colend
        lines += "\n};\n"
        return lines

    def wrap_namespace( self, lines ):
        spacename = "CLUE"
        begin = "\n// begin of namespace " + spacename + "\n\n"
        begin += "namespace " + spacename + " {\n"
        end = "\n}; // end of namespace " + spacename + "\n"
        lines =  begin + lines + end
        return lines
 

    def print_lines( self, lines ):
        """
        Helper function. Prints out the list on stdout.
        """
        for line, i in zip(lines, range(len(lines) ) ):
            print str(i) + " " + line.strip("\n")
                           
    def log_lines( self, lines):
        """
        Helper function. Prints out the list to logger.
        """
        for line, i in zip(lines, range( len(lines) ) ):
            self.info(str(i) + " " + line.strip("\n"))

    def std_includes(self):
        msg = "\n// includes from stdandard libraries\n"
        msg += "#include <map>\n"
        msg += "#include <string>\n"
        msg += "#include <complex>\n\n"
        return msg
        
    def header_of_file(self, fname):
        """
        Header of extern C header.

        """
        msg  = "/**\n"
        msg += "  * @file " + fname + "\n"
        msg += "  * @brief This is automaticly generated C++ header file\n"
        msg += "  * from Fortran inc file(s):\n"
        msg += "  * <ul>\n"
        for entry in self.inc_list:
            msg += "  * <li> " + entry + "\n"
        msg += "  * </ul>\n" 
        msg += "  * using:\n"
        msg += "  * CLUE " + str(_clue_version) + " by "+ str(_clue_author) + "\n"
        msg += "  * @date " + strftime("%a, %d %b %Y %H:%M:%S %Z" , localtime()) + "\n"
        msg += "  *\n"
        msg += "  * @warning <i>DO NOT EDIT! ALL CHANGES will be lost\n"
        msg += "  * during next (re)generation</i>.\n"
        msg += "  */\n"
        msg += "\n\n"
        
        return msg

    def end_of_extern_C_header( self ):
        """
        End of header.
        """
        define =  str.upper(self.namespace + "_H")
        msg = "#endif //! " + define + "\n" 
        return msg

    def begin_of_extern_C( self ):
        """
        Begin of extern C
        """
        msg  = "\n// begin of extern C struct declarations\n\n"
        msg += "extern \"C\" {\n" 
        return msg

    def end_of_extern_C( self ):
        """
        End of extern C
        """
        msg = "\n}; // end of extern C block\n"
        return msg

    

    

def usage():
    print
    version()
    print
    print " usage:"
    print " [python] clue [-huvd] [-p FOO=BAR] [-g GNAME] FILE1.INC [FILE2.INC ...]"
    print
    print " where:"
    print " -g, --generator GNAME     the name of generator wrapper (if not set, the name of"
    print "                           generator will be set to Generator_i)"
    print " -p, --parameter FOO=BAR   additional parameters"
    print " FILE1.INC ...             Fortran 77 files with common blocs declarations"
    print " -h, --help                RU need more help?" 
    print " -u, --usage               prints this help and exit"
    print " -v, --version             version and author info"
    print " -d, --debug               logging level (default >= info)"
    print
    

def help():
    print
    version()
    print
    print "1. Generator name"
    print
    print "   If not present, the default generator name (\"DUMMY\") will be used,"
    print "   but you should avoid it. The generator name will be used (in uppercase)"
    print "   to C++ object definition, e.g.:"
    print "         clue.py -g pythia ../foo.inc ../bar.inc"
    print "   will produce namespace:"
    print "            namespace Atlas_PYTHIA_COMMONS { ... }"
    print
    print "2. Logging level"
    print
    print "   By using -d switch the log file will be much bigger than in a "
    print "   default case. Please, use it if something goes wrong and send the"
    print "   log file together with Fortran inc file to me. My mail address is"
    print "   Krzysztof.Ciba@NOSPAM.agh.edu.pl (do not forget to remove NOSPAM)."
    print 
    print "3. Additional parameters"
    print
    print "   You can pass additional PARAMETER statements using -p switch e.g.:"
    print "           -p MAX=12"
    print "   Such parameter will be included into Fortran 77 lines in a way: "
    print "            PARAMETER (MAX=12)"
    print "   But DO NOT forget to place quotation marks around them if your"
    print "   parameter definition doing some calculation e.g.:"
    print "           -p \"MIN=MAX-12\""
    print "   to avoid some shell wildcards substitutions."
    print
    print "4. Output files"
    print 
    print "   For each run the CLUE generates three files:"
    print "   1. <GENERATOR>_extern_C.h -- header file with C++ struct"
    print "   declaration and extern C definitions"
    print "   2. <GENERATOR>_map.h  -- declaration of CLUE::Command map"
    print "   3. <GENERATOR>_map.cxx -- definition of CLUE::Command map"
    print
    

def version():
    print " COMMON Look Up tablE " + str(_clue_version) + " by "+ str(_clue_author)
    


def run():
    """The main function.
    Parses command line arguments and invokes clue. 
    """

    # parsing command line 
    try:
        opts, args = getopt.getopt(sys.argv[1:],
                                   "vhug:dp:",
                                   [ "version", "help", "usage", "generator", "debug", "param"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)

    # predefined variables
    file_list = [ ] 
    lvl = logging.INFO
    gen = None
    param = []


    if ( len(opts) == 0 ):
        usage()
        sys.exit()
    
    
    for o, a in opts:

        if o in ("-v","--version"):
            version()
            sys.exit(0)

        if o in ( "-u", "--usage"):
            usage()
            sys.exit(0)

        if o in ( "-h", "--help"):
            help()
            sys.exit(0)

        if  o in ( "-g", "--generator"):
            gen = a

        if o in ( "-p", "--param"):
            param.append( a ) 
            
        if o in ( "-d", "--debug" ):
            lvl = logging.DEBUG

    # constructing list of files
    for i in range(0, len(args)): 
        file_list.append( args[i] )

    # calling clue :)
    aClue = clue( file_list , gen, param, lvl)
        
if __name__ == "__main__":
    run()
