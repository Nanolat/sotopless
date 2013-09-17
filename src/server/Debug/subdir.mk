################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BoostLogger.cpp \
../DatabaseServer.cpp \
../DatabaseServerError.cpp \
../DatabaseService.cpp \
../DatabaseService_constants.cpp \
../DatabaseService_types.cpp \
../LeaderboardImpl.cpp \
../LeaderboardProtocolPrinter.cpp \
../LeaderboardServer.cpp \
../LeaderboardService.cpp \
../LeaderboardService_constants.cpp \
../LeaderboardService_types.cpp \
../NanolatDatabaseError.cpp \
../StackDumper.cpp 

OBJS += \
./BoostLogger.o \
./DatabaseServer.o \
./DatabaseServerError.o \
./DatabaseService.o \
./DatabaseService_constants.o \
./DatabaseService_types.o \
./LeaderboardImpl.o \
./LeaderboardProtocolPrinter.o \
./LeaderboardServer.o \
./LeaderboardService.o \
./LeaderboardService_constants.o \
./LeaderboardService_types.o \
./NanolatDatabaseError.o \
./StackDumper.o 

CPP_DEPS += \
./BoostLogger.d \
./DatabaseServer.d \
./DatabaseServerError.d \
./DatabaseService.d \
./DatabaseService_constants.d \
./DatabaseService_types.d \
./LeaderboardImpl.d \
./LeaderboardProtocolPrinter.d \
./LeaderboardServer.d \
./LeaderboardService.d \
./LeaderboardService_constants.d \
./LeaderboardService_types.d \
./NanolatDatabaseError.d \
./StackDumper.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DBOOST_ALL_DYN_LINK -I../../../include -I../../../deps/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


