################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-974292641: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs2020/ccs/utils/sysconfig_1.24.0/sysconfig_cli.bat" --script "C:/Users/Walnut/workspace_ccstheia/CAR-V2/empty.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_05_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-974292641 ../empty.syscfg
device.opt: build-974292641
device.cmd.genlibs: build-974292641
ti_msp_dl_config.c: build-974292641
ti_msp_dl_config.h: build-974292641
Event.dot: build-974292641

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"Hardware/encoder" -I"Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Debug" -I"C:/ti/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_00_05/source" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/jy60" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/oled" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/pid" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/motor" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/uart" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/buzz" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MSPM0" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MPU6050" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/BNO08X_UART_RVC" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/delay" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/line" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/ole2d" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/encoder" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_2_05_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"Hardware/encoder" -I"Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Debug" -I"C:/ti/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_00_05/source" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/jy60" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/oled" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/pid" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/motor" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/uart" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/buzz" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MSPM0" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MPU6050" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/BNO08X_UART_RVC" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/delay" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/line" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/ole2d" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/encoder" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"Hardware/encoder" -I"Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Debug" -I"C:/ti/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_05_00_05/source" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/jy60" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/oled" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/pid" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/control" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/motor" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/uart" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/buzz" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MSPM0" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/MPU6050" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/BNO08X_UART_RVC" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/delay" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/line" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/ole2d" -I"C:/Users/Walnut/workspace_ccstheia/CAR-V2/Hardware/encoder" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


