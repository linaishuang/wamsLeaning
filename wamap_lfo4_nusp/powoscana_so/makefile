.SUFFIXES: .cpp .o

VERSION     = 1.0.0
PROGRAM     = libpowoscana.so
OBJECTS     = PronyAnalysis.o \
              LFOAnaByCurve.o  

CC       = `makeopts.sh cc` -D_CXX -D_UNIX -D_WAMSDATA_ -D_D5000 -D_WAMAP_  -D_PODB_WARN_ -D_GUANGDONG -D_NUSP
CCFLAGS  = `makeopts.sh ccflag corba_inc cppdollar` -I../../pub -I$(D5000_HOME)/src/include/db_api -I$(D5000_HOME)/src/advanced/wams/wams_lib/inc -I../../tmd/work/i_lfo/save -I$(D5000_HOME)/src/advanced/wams/wams_fes/fes_warn
LDFLAGS  = `makeopts.sh ldflag`
LIBS     = `makeopts.sh syslib pub odb_api sam msg ` -lpub -lodb_apiall_lib -lpthread -lwamap_dbio  -L/opt/intel/Compiler/11.1/069/lib/intel64 -lifcore -lifport # -ltriggersampleclient
MAKE_SO  = `makeopts.sh mk_so`
LIB_PATH = `makeopts.sh lib_path`

.f.o:
	@echo $(FC) $(FCFLAGS) -o $@ $<
	@       $(FC) $(FCFLAGS) -o $@ $<


.cpp.o:
	@echo	$(CC) $(CCFLAGS) $<
	@     $(CC) $(CCFLAGS) $<

all : $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@echo	 $(MAKE_SO) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) $(LIBS)
	@	     $(MAKE_SO) -o $(PROGRAM) $(OBJECTS) $(LDFLAGS) $(LIBS)

#	mkdir -p ../../include_eaccs
#	cp -f ../*.h ../../include_eaccs
	cp -f $(PROGRAM) ../../
	cp -f $(PROGRAM) ../

RM 	= rm -f

clean:
	$(RM) $(PROGRAM)  $(OBJECTS)

cleano:
	$(RM) $(OBJECTS) 

cleanp:
	$(RM) $(PROGRAM)

config:
	@echo    rm -f $(SRC_HOME)/lib/$(PROGRAM)
	@        rm -f $(SRC_HOME)/lib/$(PROGRAM)
	
	@echo    cp -f $(PROGRAM) $(SRC_HOME)/lib/$(PROGRAM)
	@	     cp -f $(PROGRAM) $(SRC_HOME)/lib/$(PROGRAM)
	
	@echo    rm -rf  $(D5000_HOME)/lib/$(PROGRAM)
	@        rm -rf  $(D5000_HOME)/lib/$(PROGRAM)
	
	@echo	 cp -f $(PROGRAM) $(D5000_HOME)/lib/$(PROGRAM)
	@	     cp -f $(PROGRAM) $(D5000_HOME)/lib/$(PROGRAM)
