CXX = g++
CXXFLAGS = -std=c++17

assembler: orchestrator.cpp pass1/pass1.cpp path_helper.cpp
	$(CXX) $(CXXFLAGS) orchestrator.cpp pass1/pass1.cpp path_helper.cpp -o assembler

clean:
	rm -f assembler