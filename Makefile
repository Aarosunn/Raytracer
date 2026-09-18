CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -g -O0

rt: src/main.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f rt 

.PHONY: clean
