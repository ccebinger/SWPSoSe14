################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/frontend/Graphs.cpp \
../src/frontend/adjacency_list.cpp \
../src/frontend/frontend.cpp 

OBJS += \
./src/frontend/Graphs.o \
./src/frontend/adjacency_list.o \
./src/frontend/frontend.o 

CPP_DEPS += \
./src/frontend/Graphs.d \
./src/frontend/adjacency_list.d \
./src/frontend/frontend.d 


# Each subdirectory must supply rules for building sources it contributes
src/frontend/%.o: ../src/frontend/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/sandra/Dokumente/Uni/SWPSoSe14/projekt-compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wall -Wextra -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


