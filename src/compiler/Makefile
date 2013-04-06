SRCS := $(wildcard *.cpp)
OBJS := ${SRCS:.cpp=.o}
INCLUDE_DIRS := ../
BIN_DIR := ../../bin/
NAME := $(BIN_DIR)compiler 

CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

.PHONY: all clean distclean

all: $(NAME)

$(NAME): $(OBJS)
    mkdir -p $(BIN_DIR)
    $(LINK.cc) $(OBJS) -o $(NAME)

clean:
    @- $(RM) $(NAME)
    @- $(RM) $(OBJS)
