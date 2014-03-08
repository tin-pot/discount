# Microsoft Developer Studio Project File - Name="posc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=posc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "posc-vc6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "posc-vc6.mak" CFG="posc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "posc - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "posc - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "posc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f posc.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "posc.exe"
# PROP BASE Bsc_Name "posc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "cd posc && nmake CRT_D=/MLd CRT_R=/ML conf=Release"
# PROP Rebuild_Opt "distclean"
# PROP Target_File "Win32\Release\posc.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "posc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f posc.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "posc.exe"
# PROP BASE Bsc_Name "posc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "cd posc && nmake CRT_D=/MLd CRT_R=/ML conf=Debug"
# PROP Rebuild_Opt "distclean"
# PROP Target_File "Win32\Debug\posc.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "posc - Win32 Release"
# Name "posc - Win32 Debug"

!IF  "$(CFG)" == "posc - Win32 Release"

!ELSEIF  "$(CFG)" == "posc - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\posc\findhdrdir.c
# End Source File
# Begin Source File

SOURCE=.\posc\getopt.c
# End Source File
# Begin Source File

SOURCE=.\posc\posc_getopt.c
# End Source File
# Begin Source File

SOURCE=.\posc\posc_snprintf.c
# End Source File
# Begin Source File

SOURCE=.\posc\posc_strdup.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\posc\getopt.h
# End Source File
# Begin Source File

SOURCE=.\posc\inttypes.h
# End Source File
# Begin Source File

SOURCE=.\posc\posc.h
# End Source File
# Begin Source File

SOURCE=.\posc\stdarg.h
# End Source File
# Begin Source File

SOURCE=.\posc\stdbool.h
# End Source File
# Begin Source File

SOURCE=.\posc\stddef.h
# End Source File
# Begin Source File

SOURCE=.\posc\stdint.h
# End Source File
# Begin Source File

SOURCE=.\posc\stdio.h
# End Source File
# Begin Source File

SOURCE=.\posc\string.h
# End Source File
# Begin Source File

SOURCE=.\posc\strings.h
# End Source File
# Begin Source File

SOURCE=.\posc\unistd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
