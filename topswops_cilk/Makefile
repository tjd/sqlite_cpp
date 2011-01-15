# Makefile for Cilk++ example.  See source code for details.

CILKPP	= cilk++
LIBARG	= -O3 -g -lcilkutil
TARGET	= topswop_cilk
SRC	= $(addsuffix .cilk,$(TARGET))

all: $(TARGET)

$(TARGET): $(SRC)
	$(CILKPP) $(SRC) $(LIBARG) -o $@_ex

clean:
	rm -f $(TARGET)_ex
