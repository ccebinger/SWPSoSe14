################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/frontend/parser/Parser.cpp 

OBJS += \
./src/frontend/parser/Parser.o 

CPP_DEPS += \
./src/frontend/parser/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/frontend/parser/%.o: ../src/frontend/parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/sandra/Dokumente/Uni/SWPSoSe14/projekt-compiler/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wall -Wextra -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


