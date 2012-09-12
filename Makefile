
CXX=g++
COMP=-c -O2 -frtti -fexceptions
FCOMP=-c -O2

commandEXE=./bin/dummyCommand
mapEXE=./bin/dummyMap

work_dir=`pwd`
obj_dir=${work_dir}/obj

MYNAME="KrzysztofCiba"
PROJ_NAME=`basename  ${PWD} /`
DATE=`date +%d%m%y`
ZIP_NAME=${PROJ_NAME}_${MYNAME}_${DATE}

common_src=./src/common.F ./src/common.inc
common_obj=./obj/common.o

StringParse_src=./src/StringParse.cxx  ./src/StringParse.h
StringParse_obj=./obj/StringParse.o

CommandBase_src=./src/CommandBase.cxx ./src/CommandBase.h
CommandBase_obj=./obj/CommandBase.o

DummyGen_src=./src/DummyGen.cxx  ./src/DummyGen.h
DummyGen_obj=./obj/DummyGen.o

PYTHIA_map_src=./src/PYTHIA_map.cxx ./src/PYTHIA_map.h \
	./src/PYTHIA_extern_C.h ./src/CommandMap.h
PYTHIA_map_obj=./obj/PYTHIA_map.o

DUMMYGEN_map_src=./src/DUMMYGEN_map.cxx ./src/DUMMYGEN_map.h \
	 ./src/DUMMYGEN_extern_C.h ./src/CommandMap.h 
DUMMYGEN_map_obj=./obj/DUMMYGEN_map.o

objects=$(common_obj) $(StringParse_obj) $(CommandBase_obj) \
	$(CommandFactory_obj) $(DummyGen_obj)

testCommand_src=./src/testCommand.cxx
testCommandMap_src=./src/testCommandMap.cxx


all: map command

default: map command

common: $(common_src)
	gfortran $(FCOMP) ./src/common.F -o $(common_obj)

StringParse: $(StringParse_src)
	g++ $(COMP) ./src/StringParse.cxx -o $(StringParse_obj)

CommandBase: $(CommandBase_src)
	g++ $(COMP) ./src/CommandBase.cxx -o $(CommandBase_obj)

DUMMYGEN_map: $(DUMMYGEN_map_src)
	g++ $(COMP) ./src/DUMMYGEN_map.cxx -o $(DUMMYGEN_map_obj)

DummyGen: $(DummyGen_src)
	g++ $(COMP) ./src/DummyGen.cxx -o $(DummyGen_obj)

command:  common CommandBase StringParse $(testCommand_src)
		g++ -lgfortran -frtti -O2 -fexceptions ./src/testCommand.cxx  \
			$(CommandBase_obj) $(StringParse_obj) $(common_obj) \
			-o $(commandEXE)

map:  common CommandBase StringParse DUMMYGEN_map \
	$(testCommand_src)	
	g++ -lgfortran -frtti -O2 -fexceptions ./src/testCommandMap.cxx  \
			$(CommandBase_obj) $(StringParse_obj) $(DUMMYGEN_map_obj) $(common_obj) \
			-o $(mapEXE)

doc: Doxyfile
	doxygen 

clean:
	@rm -Rf ./obj/*.o
	@rm -Rf ./doc/html/*.*
	@rm -Rf ./bin/*.*

tar: 
	@echo creating tarball  ../${ZIP_NAME}
	cd .. && tar chf - ${PROJ_NAME} | gzip -f9 > ${ZIP_NAME}.tar.gz
	@echo ../${ZIP_NAME}.tar.gz created


help:
	@echo
	@echo "usage:"
	@echo "   make command          -> produce dummyCommand"
	@echo "   make map              -> produce dummyMap"
	@echo "   make all              -> the same as above"
	@echo "   make                  -> the same as above"
	@echo "   make doc              -> produce Doxygen documentation"
	@echo "   make clean            -> remove binaries+documantation"
	@echo "   make help             -> print this message"
	@echo
