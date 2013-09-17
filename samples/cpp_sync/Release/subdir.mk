################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../common.cpp \
../database.cpp \
../database_perf.cpp \
../leaderboard.cpp \
../main.cpp 

OBJS += \
./common.o \
./database.o \
./database_perf.o \
./leaderboard.o \
./main.o 

CPP_DEPS += \
./common.d \
./database.d \
./database_perf.d \
./leaderboard.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DNDEBUG -I../../../include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


