# makefile for network
include ..\makevars

 # dirs on which these tests depend
DEP_DIRS= ../systemex ../testex
DEP_SRCS=$(foreach dir,$(DEP_DIRS), $(wildcard $(dir)/*.cpp))
DEP_OBJS=$(DEP_SRCS:.cpp=.o)
DEP_OBJS_NO_TEST=$(patsubst %_test.o,,$(DEP_OBJS))

# objects derived from dirs
OBJS_TEST_RUN=$(OBJS) $(DEP_OBJS_NO_TEST)

$(TEST): $(OBJS_TEST_RUN)  
	$(CXX) -o $(TEST) $(OBJS_TEST_RUN) $(LINKFLAGS) $(LIBS)