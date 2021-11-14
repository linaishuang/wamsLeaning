# Microsoft Developer Studio Project File - Name="POWOSCANA_HIS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=POWOSCANA_HIS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "POWOSCANA_HIS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "POWOSCANA_HIS.mak" CFG="POWOSCANA_HIS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "POWOSCANA_HIS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "POWOSCANA_HIS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "POWOSCANA_HIS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "POWOSCANA_HIS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "POWOSCANA_HIS - Win32 Release"
# Name "POWOSCANA_HIS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Group "pub"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\wamap_lfo4\pub\DataFile.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\FileOpSop.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\HisdbAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\lfo_ana_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\lfo_ana_obj_his.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\LFOAnaByCurve.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\PronyAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubfun.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\public_m.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubodbapi.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubwamsdata.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\trigger_sample_m.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\wam_lfo_ana.cpp
# End Source File
# Begin Source File

SOURCE=.\wam_lfo_ana_his.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\warn_service_message.cpp
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\warn_service_message_m.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\wamap_lfo4\pub\DataFile.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\FileOpSop.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\HisdbAccess.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\init_tmd_save_req.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\lfo_ana_obj.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\lfo_struct.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\LFOAnaByCurve.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\PronyAnalysis.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubfun.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\public_m.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubodbapi.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubodbstruct.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubodbstruct_bak.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\pubwamsdata.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\trigger_sample_m.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\wamtoolstruct.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\warn_service_message.h
# End Source File
# Begin Source File

SOURCE=..\wamap_lfo4\pub\warn_service_message_m.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
