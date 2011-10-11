################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp 

OBJS += \
./main.o 

CPP_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/gstreamermm-0.10 -I/usr/include/giomm-2.4 -I/usr/include/libxml2 -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/lib/sigc++-2.0/include -I/usr/lib/glibmm-2.4/include -I/usr/lib/glib-2.0/include -I/usr/include/sigc++-2.0 -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/include/glibmm-2.4 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


