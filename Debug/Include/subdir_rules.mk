################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Include/CLASinCosTable.obj: ../Include/CLASinCosTable.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=strict --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/Users/PEETS/workspace_polar/wpt_cla_NEC/Include" --include_path="C:/Users/PEETS/workspace_polar/wpt_cla_NEC/Source" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/lib" --advice:performance=all --symdebug:coff --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="Include/CLASinCosTable.d" --obj_directory="Include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


