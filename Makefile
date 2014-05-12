# c.ebinger, 28-04-2014
# Usage: make fu-rail 

all: rail

fu-rail: projekt-compiler/src/backend/main.cc  
	g++ -o fu-rail projekt-compiler/src/backend/main.cc

rail: rail-interpreter/src/*.cpp
	g++ -o rail rail-interpreter/src/*.cpp  
