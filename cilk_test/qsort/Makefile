# Makefile for Cilk++ example.  See source code for details.

CILKPP	= cilk++
LIBARG	= -O1 -g -lcilkutil
TARGET	= qsort
SRC	= $(addsuffix .cilk,$(TARGET))

all: $(TARGET)

$(TARGET): $(SRC)
	$(CILKPP) $(SRC) $(LIBARG) -o $@_ex

clean:
	rm -f $(TARGET)
