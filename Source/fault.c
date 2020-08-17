#include "F2837xS_device.h"
#include "F28x_Project.h"


interrupt void hw_fault1(void){
	GPIO_WritePin(PWM_EN0, 1);
	GPIO_WritePin(PWM_EN1, 1);

	EALLOW;
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm3Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm6Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	EDIS;

	FLAG_HW_FAULT = 1;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void hw_fault2(void){
	GPIO_WritePin(PWM_EN0, 1);
	GPIO_WritePin(PWM_EN1, 1);

	EALLOW;
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm3Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm6Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	EDIS;

	FLAG_HW_FAULT = 2;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}


interrupt void hw_fault3(void){
	GPIO_WritePin(PWM_EN0, 1);
	GPIO_WritePin(PWM_EN1, 1);

	EALLOW;
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm3Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm6Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	EDIS;

	FLAG_HW_FAULT = 3;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;


}


interrupt void hw_fault4(void){
	GPIO_WritePin(PWM_EN0, 1);
	GPIO_WritePin(PWM_EN1, 1);

	EALLOW;
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm3Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm6Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	EDIS;

	FLAG_HW_FAULT = 4;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

}

interrupt void hw_fault5(void){
	GPIO_WritePin(PWM_EN0, 1);
	GPIO_WritePin(PWM_EN1, 1);

	EALLOW;
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm3Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	//	EPwm6Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
	EDIS;

	FLAG_HW_FAULT = 5;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

}



