CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -g -O0 -MMD -MP

rt: src/main.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f rt rt.d 

.PHONY: clean

-include rt.d
