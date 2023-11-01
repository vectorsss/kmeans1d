OBJS := kmeans_dp.o kmeans_slow.o kmeans_linear.o kmeans_monotone.o \
	kmeans_lloyd.o kmeans_hirschberg_larmore.o kmeans_wilber.o \
	interval_sum.o
CXXFLAGS_RELEASE := -Wall -Wextra -fPIE -O2
CXXFLAGS_DEBUG := -g -Wall -Wextra -fPIE -fsanitize=undefined -DDEBUG #-fsanitize=address
T ?= RELEASE
CXXFLAGS = $(CXXFLAGS_$T) -std=c++11
EXEC := run
TIME := timing
CXX = g++
TEST = test-$T

all: $(EXEC)

test : $(TEST)

clean:
	$(RM) $(EXEC) $(TEST) $(OBJS)

$(EXEC): $(OBJS) run.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TEST) : $(OBJS) test.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TIME) : $(OBJS) timing.cpp
	$(CXX) -fopenmp $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY : all test clean
