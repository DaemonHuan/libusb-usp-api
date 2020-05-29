################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/libusp_bpm22.cpp 

OBJS += \
./src/libusp_bpm22.o 

CPP_DEPS += \
./src/libusp_bpm22.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"D:\workspace\ew-libusp-bpm-api" -I"D:\workspace\ew-libusp-bpm-api\usbBasic" -I"D:\workspace\ew-libusp-bpm-api\libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


