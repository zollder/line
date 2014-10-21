################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlobDetector.cpp \
../src/BlobsInfoDao.cpp \
../src/blob.cpp 

OBJS += \
./src/BlobDetector.o \
./src/BlobsInfoDao.o \
./src/blob.o 

CPP_DEPS += \
./src/BlobDetector.d \
./src/BlobsInfoDao.d \
./src/blob.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


