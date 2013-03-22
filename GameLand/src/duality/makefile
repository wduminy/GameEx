# makefile for duality 
#	- rule for making the test file

include ../makevars

# dirs on which these tests depend
SRCS_DIRS=../systemex ../game ../testex
# objects derived from dirs
OBJ_DIRS=$(foreach dir,$(SRCS_DIRS), $(dir)/*.o) 
# object in this dir -- excluding files with a main function
OBJS_TEST_RUN=$(OBJS:duality_main.o=) $(OBJ_DIRS)

$(TEST): all  
	$(CXX) -o $(TEST) $(OBJS_TEST_RUN) $(LINKFLAGS) $(LIBS)



