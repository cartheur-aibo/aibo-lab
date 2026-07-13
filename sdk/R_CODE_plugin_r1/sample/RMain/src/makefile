#
# Copyright 2006 Sony Corporation 
#
# Permission to use, copy, modify, and redistribute this software for
# non-commercial use is hereby granted.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#

MSG=\
" Sample includes RMain (R-CODE main object) source code."\
" But you cannot make the RMain object by using the OPEN-R SDK."\
" Disclosing the RMain source code will help you to make"\
" R-CODE Plug-Ins."

all:
	@echo $(MSG)

# top directory of this module
MODULE_ROOT = $(shell pwd)

# target module name
TARGET_USER_OBJECT  = rMain

# C++ source files
CC_SRCS   = \
    RMainGlobals.cc \
	RMain.cc \
	RMainSemantics.cc \
	RMainStub.cc \
	RMainFiles.cc \

# C++ compile options
CC_FLAGS  = \
			-OMA \
			-D_MACRO_\
			-D_RCLOCK_MODE_ \
#			-D_RCODE_SCRIPT_DEBUG_ \
#			-DOPENR_DEBUG  \
#			-D_RMAIN_DEBUG_ \
#			-G \
#			-D_DEBUG_ \

# C++ include path
CC_INCLUDE	= \
  $(MODULE_ROOT) \
  $(OPENR_APP_ROOT)/include \
  $(OPENR_APP_ROOT)/SpeechRecognitionSearch/src \
  $(WLAN_ROOT)/include \
  $(WLAN_ROOT)/include/MCOOP \
  $(OPENR_ROOT)/include \

# C source files
C_SRCS		= \

# C compile options
C_FLAGS		= \

# C include path
C_INCLUDE	= \

# assembly source files
AS_SRCS		= \

# assembly compile options
AS_FLAGS	= \

# assembly include path
AS_INCLUDE	= \

# linker options
LD_FLAGS	= \

# libraries
LD_LIBRARIES  = \
	libObjectComm.a \
	libOUtil.a \
	libOPENR.a \
	libmcoopz.a \
    libWLANDriverProxy.a \
	$(MCOOP_LIBRARY) \
	libMiniStream.a \
