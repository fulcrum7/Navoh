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
	g++ -I/usr/include/gstreamermm-0.10 -I/usr/lib/gstreamermm-0.10/include -I/usr/include/giomm-2.4 -I/usr/lib/giomm-2.4/include -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/include/gstreamer-0.10 -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/libxml2 -I/usr/include/gtkmm-2.4 -I/usr/lib/gtkmm-2.4/include -I/usr/include/pangomm-1.4 -I/usr/lib/pangomm-1.4/include -I/usr/include/gtk-2.0 -I/usr/include/gtk-unix-print-2.0 -I/usr/include/atkmm-1.6 -I/usr/include/gdkmm-2.4 -I/usr/lib/gdkmm-2.4/include -I/usr/include/cairomm-1.0 -I/usr/lib/cairomm-1.0/include -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/directfb -I/usr/include/libpng12 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/gio-unix-2.0/ -O0 -g3 -Wall $(shell pkg-config gtkmm-2.4 gstreamermm-0.10 --cflags) -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


