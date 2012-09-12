#! /usr/bin/env python

#############################################################
# Name:     clugger.py                                      #
# Purpose:  CLUG logger                                     #
# Author:   Krzysztof CIBA                                  #
# Created:  10/11/2006                                      #
#############################################################
 

import logging
"""
Logger wrapper.
"""

# default file name
__defaultFn  = 'clue.log'
# default log level
__defaultLvl = logging.INFO


def clugger(fn=None, lvl=None):
    # check parameters
    if ( fn == None ): fn = __defaultFn
    if ( lvl == None ): lvl = __defaultLvl
            
    # start logger
    logging.basicConfig(level=lvl,
                        format='%(levelname)-8s %(message)s',
                        filename=fn,
                        filemode='w')
    # return logger
    return logging.getLogger("clue") 
