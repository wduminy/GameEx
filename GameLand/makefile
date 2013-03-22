# This is the application makefile.
# It provides these targets:
#	- 'all' compiles sources in each subsystem  
#	- 'clean' cleans all artifacts
#	- 'test' runs all unit tests

include src/makerecipes

all:
	$(MAKE) -C src/systemex all
	$(MAKE) -C src/network all
	$(MAKE) -C src/game all
	$(MAKE) -C src/duality all


# all the directories with game code
DEP_DIRS= src/systemex src/network src/game src/duality
DEP_SRCS=$(foreach dir,$(DEP_DIRS), $(wildcard $(dir)/*.cpp))
DEP_OBJS=$(DEP_SRCS:.cpp=.o)
DEP_OBJS_NO_TEST=$(patsubst %_test.o,,$(DEP_OBJS))
DEP_OBJS_NO_MAIN=$(patsubst %_main.o,,$(DEP_OBJS))
TEST_SRCS=$(wildcard src/testex/*.cpp)
TEST_OBJS=$(TEST_SRCS:.cpp=.o)

DEP_OBJS_FOR_TEST=$(TEST_OBJS) $(DEP_OBJS_NO_MAIN)
test: all
	$(CXX) -o test.exe $(DEP_OBJS_FOR_TEST) $(LINKFLAGS) $(LIBS)
	.\test.exe

DEP_OBJS_FOR_RUN= $(DEP_OBJS_NO_TEST)	
run: all
	$(CXX) -o game.exe $(DEP_OBJS_FOR_RUN) $(LINKFLAGS) $(LIBS)
	.\game.exe

clean:
	rm -f $(foreach dir,$(DEP_DIRS), $(wildcard $(dir)/*.o)) 
	rm -f $(foreach dir,$(DEP_DIRS), $(wildcard $(dir)/*.depends))
	rm -f $(foreach dir,$(DEP_DIRS), $(wildcard $(dir)/*.exe))

# this target is called by the makefile in sub folders
# the idea is that the test runs from the same location
# as the program.
# prevents provlems related to the dlls and the meida file
# directory
run_test:
	.\test.exe
