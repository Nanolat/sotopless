################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Connection.cpp \
../DatabaseService.cpp \
../DatabaseService_constants.cpp \
../DatabaseService_types.cpp \
../SyncClient.cpp \
../Util.cpp 

OBJS += \
./Connection.o \
./DatabaseService.o \
./DatabaseService_constants.o \
./DatabaseService_types.o \
./SyncClient.o \
./Util.o 

CPP_DEPS += \
./Connection.d \
./DatabaseService.d \
./DatabaseService_constants.d \
./DatabaseService_types.d \
./SyncClient.d \
./Util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../include -I../../../deps/include -O3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


