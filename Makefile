# c.ebinger, 28-04-2014
# Usage: make fu-rail 

all: rail fu-rail tests

tests: unittest_constantpool unittest_classfile_writer_test

fu-rail: projekt-compiler/src/backend/main.cc  
	g++ -D STANDALONE_BACKEND -std=c++0x -o fu-rail projekt-compiler/src/backend/backend.cc projekt-compiler/src/backend/main.cc -Iprojekt-compiler/src

rail: rail-interpreter/src/*.cpp
	g++ -o rail rail-interpreter/src/*.cpp  

unittest_constantpool: projekt-compiler/src/backend/unittest/constant_pool_test.cc
	g++ -D TESTS -std=c++0x -o unittest_constantpool projekt-compiler/src/backend/classfile/constant_pool.cc projekt-compiler/src/backend/classfile/classfile_writer.cc projekt-compiler/src/backend/unittest/constant_pool_test.cc -Iprojekt-compiler/src

unittest_classfile_writer_test: projekt-compiler/src/backend/unittest/classfile_writer_test.cc
	g++ -D TESTS -std=c++0x -o unittest_classfile_writer_test projekt-compiler/src/backend/classfile/constant_pool.cc projekt-compiler/src/backend/classfile/classfile_writer.cc projekt-compiler/src/backend/unittest/classfile_writer_test.cc -Iprojekt-compiler/src
