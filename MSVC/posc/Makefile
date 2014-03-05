#
# Makefile --
#
#	`NMAKE` makefile for POSC on Win32/MSVC.
#

### Environment and Macros #############################################
#
# We use replacements for the default CC and MAKE values that
# result in a less noisy build process.
MAKE	= $(MAKE) /NOLOGO RECURSIVE=1
CC	= $(CC) /nologo $(DEFINES) 
AS	= $(AS) /nologo
AR	= LIB /NOLOGO

# Some useful shorthands.
RM	= -DEL /Q >NUL: 2>&1
LS	= -DIR /B/L 2>NUL:
LN	= LINK /nologo
MKDIR   = -MKDIR >NUL: 2>&1

### Project definitions ################################################

PLATFORM      = Win32
PROJECT       = POSC

CFLAGS_Release  = /O1 /MT /D_CRT_SECURE_NO_WARNINGS /W3 /DNDEBUG 
ARFLAGS_Release =
LIBS_Release    =

CFLAGS_Debug	= /ZI /MTd /D_CRT_SECURE_NO_WARNINGS /W3 /DDEBUG /D_DEBUG
ARFLAGS_Debug	=
LIBS_Debug	=
    
### File groups

PRODUCTS = $(TARGETS) $(JUNK)			# All generated files.
JUNK = *.obj *.manifest *.idb *.ncb	# All "disposable" files.
CONF_FILE = build_env.bat		# File to store configuration.

### Configuration setup ################################################

# CONF=release:		Build "Release" project config (the default).
# CONF=debug:		Build "Debug" project config.
# CONF=env:		Use environment settings.

!IFDEF RECURSIVE
!UNDEF CONF
!ELSE # !RECURSIVE

!IFNDEF CONF
# No `CONF` given, use default.
CONF = release
# !MESSAGE Using default configuration: "$(CONF)".
!ENDIF # CONF

!IF "$(CONF)" == "release" || "$(CONF)" == "Release"
CFLAGS = $(CFLAGS_Release)
ARFLAGS = $(ARFLAGS_Release)
OUTDIR = ..\$(PLATFORM)\Release\ 
!ELSEIF "$(CONF)" == "debug" || "$(CONF)" == "Debug"
CFLAGS = $(CFLAGS_Debug)
ARFLAGS = $(ARFLAGS_Debug)
OUTDIR = ..\$(PLATFORM)\Debug\ 
!ELSEIF "$(CONF)" == "env" || "$(CONF)" == "Env"
!MESSAGE Using environment for configuration.
!ELSE
!ERROR Unknown configuration name: "$(CONF)".
!ENDIF
!MESSAGE OUTDIR="$(OUTDIR)"

SAVCONF = savcfg.bat $(JUNK)
!ENDIF # RECURSIVE

### Project Targets ####################################################

TARGETS = $(TGT1) $(TGT2) $(TGT3)

# Each target (TGT) is a `.exe`, `.dll`, or `.lib` built from source
# files (SRC), which in turn are compiled to object files (OBJ).
# Each target may also depend on other files (DEP), like headers.

TGT1 = findhdrdir.exe
SRC1 = findhdrdir.c
DEP1 =
OBJ1 = 

TGT2 = posc_hdrdir.h
DEP2 = $(TGT1)

TGT3 = "$(OUTDIR)posc.lib"
SRC3 = posc_getopt.c posc_snprintf.c posc_strdup.c
DEP3 = $(TGT2)
OBJ3 = $(SRC3:.c=.obj)

### Public Pseudotargets ###############################################

# `all`:	builds all targets, in the configuration given by the
#		environment, and this becomes the new current
#		configuration.

all: savconf $(TARGETS)

# `distclean`:	deletes all product files and the current configuration.

distclean:
	@ECHO Deleting configuration and products: (
	@$(LS) $(CONF_FILE) $(PRODUCTS) $(TGT1) $(TGT2)
	@$(RM) $(CONF_FILE) $(PRODUCTS) $(TGT1) $(TGT2)
	@ECHO )

# `clean`:	deletes all "disposable" product files, but not the
#		targets and the configuration.

clean:
	@ECHO Deleting intermediate files: (
	@$(LS) $(JUNK)
	@$(RM) $(JUNK)
	@ECHO )


### Project build descriptions #########################################

$(TGT1): savconf $(SRC1) $(DEP1)	

$(TGT2): savconf $(DEP2)
	@ECHO ** Seeking standard header locations ... ***************
	$(TGT1)
	@ECHO ********************************************************

$(TGT3): savconf $(SRC3) $(DEP3)
	@$(MKDIR) $(OUTDIR)
	$(MAKE) $(OBJ3)
	$(AR) $(ARFLAGS) /OUT:$(TGT3) $(OBJ3)


### Private pseudotargets ##############################################

$(TARGETS): savconf

savconf:
	@ECHO Using configuration "$(CONF)".
	@SET CFLAGS=$(CFLAGS)
	@SET ARFLAGS=$(ARFLAGS)
	@SET LIBS=$(LIBS)
	@SET CONF=$(CONF)
	@SET PROJECT=$(PROJECT)
	@SET OUTDIR=$(OUTDIR)
	-@$(SAVCONF)

### EOF ################################################################
# vim:noet:ts=8:sw=8:tw=0

