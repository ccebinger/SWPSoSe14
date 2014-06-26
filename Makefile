SUBDIRS = io
# c.ebinger, 28-04-2014
# Usage: make fu-rail 

CC=g++
CFLAGS=-c -g -Wall -Wextra -Wtype-limits -pedantic -std=c++0x -Iprojekt-compiler/src
LDFLAGS=-lrt

all: rail fu-rail tests compile run

tests: unittest_constantpool unittest_classfile_writer_test

constant_pool.o:
	$(CC) $(CFLAGS) projekt-compiler/src/backend/classfile/constant_pool.cc 

classfile_writer.o:
	$(CC) $(CFLAGS) projekt-compiler/src/backend/classfile/classfile_writer.cc 

Graphs.o:
	$(CC) $(CFLAGS) projekt-compiler/src/frontend/Graphs.cpp 

Bytecode_writer.o:
	$(CC) $(CFLAGS) projekt-compiler/src/backend/classfile/Bytecode_writer.cpp 

local_variable_stash.o:
	$(CC) $(CFLAGS) projekt-compiler/src/backend/codegen/local_variable_stash.cpp

Bytecode.o: local_variable_stash.o
	$(CC) $(CFLAGS) projekt-compiler/src/backend/codegen/Bytecode.cpp local_variable_stash.o

adjacency_list.o:
	$(CC) $(CFLAGS) projekt-compiler/src/frontend/adjacency_list.cpp 

Lexer.o:
	$(CC) $(CFLAGS) projekt-compiler/src/frontend/lexer/Lexer.cpp 

Parser.o:
	$(CC) $(CFLAGS) projekt-compiler/src/frontend/parser/Parser.cpp

backend.o: Lexer.o Parser.o
	$(CC) $(CFLAGS) projekt-compiler/src/backend/backend.cc

Env.o:
	$(CC) $(CFLAGS) projekt-compiler/src/common/Env.cpp

main.o: constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o Bytecode.o\
	 adjacency_list.o Lexer.o Parser.o backend.o Env.o
	$(CC) $(CFLAGS) -D STANDALONE_BACKEND projekt-compiler/src/main.cpp

fu-rail: constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o Bytecode.o\
	 adjacency_list.o Lexer.o Parser.o backend.o Env.o main.o
	$(CC) constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o local_variable_stash.o Bytecode.o\
	 adjacency_list.o Lexer.o Parser.o backend.o Env.o main.o $(LDFLAGS) -o fu-rail

rail: rail-interpreter/src/*.cpp
	g++ -o rail -pedantic rail-interpreter/src/*.cpp

constant_pool_test.o: 
	$(CC) $(CFLAGS) -D TESTS projekt-compiler/src/backend/unittest/constant_pool_test.cc

unittest_constantpool: Env.o constant_pool.o Bytecode_writer.o Bytecode.o constant_pool_test.o
	$(CC) Env.o constant_pool.o Bytecode_writer.o local_variable_stash.o Bytecode.o constant_pool_test.o -o unittest_constantpool

classfile_writer_test.o: constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o Bytecode.o adjacency_list.o Lexer.o Parser.o backend.o Env.o
	$(CC) $(CFLAGS) -D TESTS projekt-compiler/src/backend/unittest/classfile_writer_test.cc

unittest_classfile_writer_test: constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o Bytecode.o adjacency_list.o Lexer.o Parser.o backend.o Env.o classfile_writer_test.o projekt-compiler/src/backend/unittest/classfile_writer_test.cc
	$(CC) constant_pool.o classfile_writer.o Graphs.o Bytecode_writer.o local_variable_stash.o Bytecode.o adjacency_list.o Lexer.o Parser.o backend.o Env.o classfile_writer_test.o -o unittest_classfile_writer_test

compile: 
	./fu-rail -i projekt-compiler/test/multiCharCommand/constant/success/s_pushConst.rail -s projekt-compiler/io/serialized.csv -g projekt-compiler/io/graphviz.dot -o s_pushConst.class

run: 
	java s_pushConst

clean:
	rm *.o fu-rail rail unittest_constantpool unittest_classfile_writer_test
