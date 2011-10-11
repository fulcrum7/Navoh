################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GstPlayerBase.cpp \
../GstVideoClient.cpp \
../main.cpp 

OBJS += \
./GstPlayerBase.o \
./GstVideoClient.o \
./main.o 

CPP_DEPS += \
./GstPlayerBase.d \
./GstVideoClient.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(shell pkg-config gstreamermm-0.10 gtkmm-2.4 --cflags) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


