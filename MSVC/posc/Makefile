# Makefile for POSC for nmake/MSVC

CFLAGS  = /O2 /nologo $(OPTS)
AR      = lib
ARFLAGS = /nologo
RM      = -del /s /q

OBJECTS = $(SOURCES:.c=.obj)
SETUP   = findhdrdir.exe
POSCLIB = $(OUTDIR)posc.lib
POSCHDR = posc_hdrdir.h

SOURCES = posc_getopt.c posc_snprintf.c posc_strdup.c

all: $(POSCHDR) $(POSCLIB)

$(POSCHDR):
	$(MAKE) $(SETUP)
	@echo ** Seeking standard header locations ... ***************
	@$(SETUP)
	@echo ** Done. ***********************************************

$(POSCLIB): $(SOURCES)
	-mkdir $(OUTDIR)
	$(RM) $(OBJECTS)
	$(MAKE) $(OBJECTS)
	$(AR) $(ARFLAGS) /OUT:$(POSCLIB) $(OBJECTS)

clean:
	$(RM) $(SETUP) $(SETUP:.exe=.obj) $(OBJECTS)

distclean: clean
	$(RM) $(POSCLIB) $(POSCHDR)
