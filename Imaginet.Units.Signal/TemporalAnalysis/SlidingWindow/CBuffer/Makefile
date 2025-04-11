CC			?= gcc
CFLAGS		+= -O3 -Wall -lm -I../Assert -Wno-unknown-pragmas -Wno-unused-function

all: cbuffer_test_runner
	./cbuffer_test_runner 

cbuffer_test_runner: cbuffer.h cbuffer_tests.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf cbuffer_test_runner __pycache__

.PHONY: all clean test