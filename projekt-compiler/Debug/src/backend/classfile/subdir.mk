################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/backend/classfile/Bytecode_writer.cpp \
../src/backend/classfile/Lambda_classfile_writer.cpp 

CC_SRCS += \
../src/backend/classfile/classfile_writer.cc \
../src/backend/classfile/constant_pool.cc 

OBJS += \
./src/backend/classfile/Bytecode_writer.o \
./src/backend/classfile/Lambda_classfile_writer.o \
./src/backend/classfile/classfile_writer.o \
./src/backend/classfile/constant_pool.o 

CC_DEPS += \
./src/backend/classfile/classfile_writer.d \
./src/backend/classfile/constant_pool.d 

CPP_DEPS += \
./src/backend/classfile/Bytecode_writer.d \
./src/backend/classfile/Lambda_classfile_writer.d 


# Each subdirectory must supply rules for building sources it contributes
src/backend/classfile/%.o: ../src/backend/classfile/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/sandra/Dokumente/Uni/SWPSoSe14/projekt-compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wall -Wextra -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/backend/classfile/%.o: ../src/backend/classfile/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/sandra/Dokumente/Uni/SWPSoSe14/projekt-compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wall -Wextra -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


