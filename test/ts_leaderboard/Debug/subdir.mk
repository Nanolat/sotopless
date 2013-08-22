################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test_common.cpp \
../test_leaderboard.cpp \
../test_scenarios.cpp \
../test_score.cpp \
../test_user.cpp \
../test_util.cpp 

OBJS += \
./test_common.o \
./test_leaderboard.o \
./test_scenarios.o \
./test_score.o \
./test_user.o \
./test_util.o 

CPP_DEPS += \
./test_common.d \
./test_leaderboard.d \
./test_scenarios.d \
./test_score.d \
./test_user.d \
./test_util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__stdcall='' -I../../../deps/include -I../../../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


