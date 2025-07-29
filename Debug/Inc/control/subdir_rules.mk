################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Inc/control/%.o: ../Inc/control/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Debug" -I"C:/ti/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_00_05/source" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Debug/Inc/encoder" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/INC" -I"CAR-V2/Inc/control" -I"CAR-V2/Inc/encoder" -gdwarf-3 -MMD -MP -MF"Inc/control/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


