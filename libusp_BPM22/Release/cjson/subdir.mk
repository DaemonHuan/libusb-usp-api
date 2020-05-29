################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/workspace/ew-libusp-bpm-api/cjson/cJSON.c \
D:/workspace/ew-libusp-bpm-api/cjson/cJSON_Utils.c 

OBJS += \
./cjson/cJSON.o \
./cjson/cJSON_Utils.o 

C_DEPS += \
./cjson/cJSON.d \
./cjson/cJSON_Utils.d 


# Each subdirectory must supply rules for building sources it contributes
cjson/cJSON.o: D:/workspace/ew-libusp-bpm-api/cjson/cJSON.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"D:\workspace\ew-libusp-bpm-api" -I"D:\workspace\ew-libusp-bpm-api\usbBasic" -I"D:\workspace\ew-libusp-bpm-api\libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cjson/cJSON_Utils.o: D:/workspace/ew-libusp-bpm-api/cjson/cJSON_Utils.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"D:\workspace\ew-libusp-bpm-api" -I"D:\workspace\ew-libusp-bpm-api\usbBasic" -I"D:\workspace\ew-libusp-bpm-api\libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


