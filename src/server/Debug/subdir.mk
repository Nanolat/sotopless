################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DatabaseServer.cpp \
../DatabaseServerImpl.cpp \
../DatabaseService.cpp \
../DatabaseService_constants.cpp \
../DatabaseService_types.cpp \
../LeaderboardServer.cpp \
../LeaderboardService.cpp \
../LeaderboardService_constants.cpp \
../LeaderboardService_types.cpp 

OBJS += \
./DatabaseServer.o \
./DatabaseServerImpl.o \
./DatabaseService.o \
./DatabaseService_constants.o \
./DatabaseService_types.o \
./LeaderboardServer.o \
./LeaderboardService.o \
./LeaderboardService_constants.o \
./LeaderboardService_types.o 

CPP_DEPS += \
./DatabaseServer.d \
./DatabaseServerImpl.d \
./DatabaseService.d \
./DatabaseService_constants.d \
./DatabaseService_types.d \
./LeaderboardServer.d \
./LeaderboardService.d \
./LeaderboardService_constants.d \
./LeaderboardService_types.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../../include -I../../../deps/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


