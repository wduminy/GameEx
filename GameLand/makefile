# This is the application makefile.
# It provides these targets:
#	- 'all' compiles sources in each subsystem  
#	- 'clean' cleans all artifacts
#	- 'test' runs all unit tests

include src/makerecipes

# all the directories with game code
SRCS_DIRS=src/systemex src/network src/game src/duality

# wildcard reference to those objects
OBJ_DIRS=$(foreach dir,$(SRCS_DIRS), $(dir)/*.o) 

# all the sources
SRCS_ALL=$(foreach dir,$(SRCS_DIRS), $(wildcard $(dir)/*.cpp)) 
DEPS_ALL=$(SRCS_ALL:.cpp=.depends) 

# objects for all run.
OBJS_ALL=$(SRCS_ALL:.cpp=.o)
all: $(OBJS_ALL)

# those sources that have unit tests in them
SRCS_UNIT_TEST=$(foreach dir,$(SRCS_DIRS), $(wildcard $(dir)/*test.cpp)) 
OBJS_UNIT_TEST=$(SRCS_UNIT_TEST:.cpp=.o)
# the sources needed to run unit tests
SRCS_TEST=$(wildcard src/testex/*.cpp) 
# all objects that are test specific 
OBJS_TEST=$(SRCS_TEST:.cpp=.o) 
# all object, but excludes main method other than test main
OBJS_TEST_RUN=$(OBJS_ALL:src/duality/duality_main.o=) $(OBJS_TEST) 
test: $(OBJS_TEST_RUN)  
	$(CXX) -o test.exe $(OBJS_TEST_RUN) $(LINKFLAGS) $(LIBS)
	.\test.exe

run_test:
	.\test.exe
	
# I generate unit test first and then delete them :()
run: $(DEPS_ALL) $(OBJS_ALL)
	rm -f $(OBJS_TEST) $(OBJS_UNIT_TEST)
	$(CXX) -o game.exe $(OBJ_DIRS) $(LINKFLAGS) $(LIBS)
	.\game.exe

clean:
	rm -f $(foreach dir,$(SRCS_DIRS), $(wildcard $(dir)/*.o)) 
	rm -f $(foreach dir,$(SRCS_DIRS), $(wildcard $(dir)/*.depends))
	rm -f $(foreach dir,$(SRCS_DIRS), $(wildcard $(dir)/*.exe))

# unfortunately dependencies are generated, even when I run clean :()
-include $(DEPS_ALL)