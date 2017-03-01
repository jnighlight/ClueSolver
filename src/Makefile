program_NAME := ClueSolver
program_C_SRCS := $(wildcard *.c)
program_C_OBJS := ${program_C_SRCS:.c=.o}

program_TEST_FILES := $(wildcard t_*)
program_CXX_SRCS := $(wildcard *.cpp)
program_TEST_SRCS := $(filter-out Main.cpp,$(program_CXX_SRCS))
program_CXX_SRCS := $(filter-out t_all.cpp,$(program_CXX_SRCS))

program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_TEST_OBJS := ${program_TEST_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS :=
program_LIBRARY_DIRS :=
program_LIBRARIES := 

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir)) -std=c++11
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	    $(LINK.cc) $(program_OBJS) -o $(program_NAME)

gtest: $(program_TEST_OBJS) $(program_TEST_FILES)
	    $(LINK.cc) $(program_TEST_OBJS) -o gtest

test: LDFLAGS += -lgtest -lgcov
test: gtest

clean:
	    @- $(RM) $(program_NAME)
	    @- $(RM) gtest
	    @- $(RM) $(program_OBJS)
	    @- $(RM) $(program_TEST_OBJS)

distclean: clean
