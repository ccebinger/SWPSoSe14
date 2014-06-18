# c.ebinger, 28-04-2014
# Usage: make fu-rail 

all: rail fu-rail tests compile run

tests: unittest_constantpool unittest_classfile_writer_test

fu-rail: projekt-compiler/src/main.cpp
	g++ -D STANDALONE_BACKEND -g -Wall -Wextra -Wtype-limits -pedantic -std=c++0x -o fu-rail projekt-compiler/src/backend/classfile/constant_pool.cc projekt-compiler/src/backend/classfile/classfile_writer.cc projekt-compiler/src/frontend/Graphs.cpp projekt-compiler/src/backend/classfile/Bytecode_writer.cpp projekt-compiler/src/backend/codegen/bytecode_generator.cpp projekt-compiler/src/frontend/adjacency_list.cpp projekt-compiler/src/frontend/lexer/Lexer.cpp projekt-compiler/src/frontend/parser/Parser.cpp projekt-compiler/src/backend/backend.cc projekt-compiler/src/main.cpp -Iprojekt-compiler/src

rail: rail-interpreter/src/*.cpp
	g++ -o rail -pedantic rail-interpreter/src/*.cpp

unittest_constantpool: projekt-compiler/src/backend/unittest/constant_pool_test.cc
	g++ -D TESTS -std=c++0x -Wall -Wextra -Wtype-limits -pedantic -o unittest_constantpool projekt-compiler/src/backend/codegen/bytecode_generator.cpp projekt-compiler/src/frontend/Graphs.cpp projekt-compiler/src/frontend/adjacency_list.cpp projekt-compiler/src/backend/classfile/constant_pool.cc projekt-compiler/src/backend/classfile/Bytecode_writer.cpp projekt-compiler/src/backend/classfile/classfile_writer.cc projekt-compiler/src/backend/unittest/constant_pool_test.cc -Iprojekt-compiler/src

unittest_classfile_writer_test: projekt-compiler/src/backend/unittest/classfile_writer_test.cc
	g++ -D TESTS -std=c++0x -Wall -Wextra -Wtype-limits -pedantic -o unittest_classfile_writer_test projekt-compiler/src/backend/codegen/bytecode_generator.cpp projekt-compiler/src/frontend/Graphs.cpp projekt-compiler/src/frontend/adjacency_list.cpp projekt-compiler/src/backend/classfile/constant_pool.cc projekt-compiler/src/backend/classfile/Bytecode_writer.cpp projekt-compiler/src/backend/classfile/classfile_writer.cc projekt-compiler/src/backend/unittest/classfile_writer_test.cc -Iprojekt-compiler/src

compile: 
	./fu-rail -i projekt-compiler/Tests/test-cases/pushConst.txt -s projekt-compiler/io/serialized.csv -g projekt-compiler/io/graphviz.dot

run: 
	java io/Main
