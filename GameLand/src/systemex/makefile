# makefile for systemex
include ..\makevars

# dirs on which these tests depend
SRCS_DIRS=../testex
# objects derived from dirs
OBJ_DIRS=$(foreach dir,$(SRCS_DIRS), $(dir)/*.o) 
OBJS_TEST_RUN=$(OBJS) $(OBJ_DIRS)

$(TEST): all  
	$(CXX) -o $(TEST) $(OBJS_TEST_RUN) $(LINKFLAGS) $(LIBS)

