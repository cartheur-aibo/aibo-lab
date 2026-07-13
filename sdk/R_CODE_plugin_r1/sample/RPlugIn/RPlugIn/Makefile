#
# Copyright 2002,2003,2006 Sony Corporation 
#
# Permission to use, copy, modify, and redistribute this software for
# non-commercial use is hereby granted.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#

OPENRSDK_ROOT?=/usr/local/OPEN_R_SDK
INSTALLDIR=../MS
CXX=$(OPENRSDK_ROOT)/bin/mipsel-linux-g++
STRIP=$(OPENRSDK_ROOT)/bin/mipsel-linux-strip
MKBIN=$(OPENRSDK_ROOT)/OPEN_R/bin/mkbin
STUBGEN=$(OPENRSDK_ROOT)/OPEN_R/bin/stubgen2
MKBINFLAGS=-p $(OPENRSDK_ROOT)
LIBS=-L$(OPENRSDK_ROOT)/OPEN_R/lib -lObjectComm -lOPENR -lInternet -lantMCOOP
CXXFLAGS= \
	-O2 \
	-g \
	-I. \
	-I../../../OPEN_R_SDK/OPEN_R/include \
	-I$(OPENRSDK_ROOT)/OPEN_R/include/R4000 \
	-I$(OPENRSDK_ROOT)/OPEN_R/include/MCOOP \
	-I$(OPENRSDK_ROOT)/OPEN_R/include

#
# When OPENR_DEBUG is defined, OSYSDEBUG() is available.
#
CXXFLAGS+= -DOPENR_DEBUG

.PHONY: all install clean

all: rPlugIn.bin

SRCS= \
	RPlugInStub.cc \
	RPlugIn.cc \
	RPlugInImage.cc \
	RPlugInAudio.cc \
	RPlugInNetwork.cc \
	NTP/NTP.cc \
	EchoServer/EchoServer.cc

OBJS = $(patsubst %.c,%.o, $(patsubst %.cc,%.o,$(SRCS)))

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $^

RPlugInStub.cc: stub.cfg
	$(STUBGEN) $^

rPlugIn.bin: $(OBJS) RPlugIn.ocf
	$(MKBIN) $(MKBINFLAGS) -o $@ $^ $(LIBS)
	$(STRIP) $@

install: rPlugIn.bin
	gzip -c rPlugIn.bin > $(INSTALLDIR)/OPEN-R/APP/DATA/P/RCODE/RPLUGIN.BIN

clean:
	find . -name "*.o" -exec rm {} \;
	rm -f *.bin *.elf *.snap.cc
	rm -f RPlugInStub.h RPlugInStub.cc def.h entry.h
	rm -f $(INSTALLDIR)/OPEN-R/APP/DATA/P/RPLUGIN.BIN
