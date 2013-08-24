################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AsyncClient.cpp \
../Connection.cpp \
../SyncClient.cpp \
../Util.cpp 

OBJS += \
./AsyncClient.o \
./Connection.o \
./SyncClient.o \
./Util.o 

CPP_DEPS += \
./AsyncClient.d \
./Connection.d \
./SyncClient.d \
./Util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../include -I../../../deps/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


