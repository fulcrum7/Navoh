################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GstPlayerBase.cpp \
../GstVideoServer.cpp \
../main.cpp 

OBJS += \
./GstPlayerBase.o \
./GstVideoServer.o \
./main.o 

CPP_DEPS += \
./GstPlayerBase.d \
./GstVideoServer.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(shell pkg-config gstreamermm-0.10 --cflags) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


