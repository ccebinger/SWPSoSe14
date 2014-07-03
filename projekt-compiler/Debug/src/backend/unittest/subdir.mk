################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/backend/unittest/classfile_writer_test.cc \
../src/backend/unittest/constant_pool_test.cc 

OBJS += \
./src/backend/unittest/classfile_writer_test.o \
./src/backend/unittest/constant_pool_test.o 

CC_DEPS += \
./src/backend/unittest/classfile_writer_test.d \
./src/backend/unittest/constant_pool_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/backend/unittest/%.o: ../src/backend/unittest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/sandra/Dokumente/Uni/SWPSoSe14/projekt-compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wall -Wextra -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


