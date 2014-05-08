cmake_minimum_required (VERSION 2.4)
project (Unittests)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR} 
  ../..
)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#-----------------------------------------------------------------------
# classfile_writer_test
#-----------------------------------------------------------------------
# .cxx sources
SET(CLASS_FILE_WRITER_CC
  ../../backend/classfile/classfile_writer.cc
  ../../backend/classfile/constant_pool.cc
  classfile_writer_test.cc
)

add_executable(classfileWriterTest ${CLASS_FILE_WRITER_CC} )
