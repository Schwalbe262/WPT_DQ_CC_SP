#include "F2837xS_device.h"
#include "F28x_Project.h"
#include "math.h"
#include "cla_test.h"
#include "mathtable.h"

//tx mag control
/*
interrupt void current_controller(void){
	float32 asintemp; //local variable for asin
	float32 temp_Itx_de, temp_Itx_qe; //local variable to save Itx_dq temporarily
	//GPIO_WritePin(81, 1); //for debugging
	temp_Itx_de = dq.Itx_de; //preventing value variation while cc is running
	temp_Itx_qe = dq.Itx_qe; //preventing value variation while cc is running

	//Itx current controller

	Itx_mag = sqrtf(temp_Itx_de * temp_Itx_de + temp_Itx_qe * temp_Itx_qe); //measured magnitude of transmitter current
	Itx_mag_err = Itx_mag_cmd - Itx_mag; //error between magnitude command and measured magnitude
	Vtx_cmd_integ += Ki_cc * Tsamp * (Itx_mag_err - Ka_cc * (Vtx_cmd - Vtx_cmd_sat)); //error integration and anti wind-up
	Itx_qe_ff = temp_Itx_qe * Itx_qe_factor; //Iqt feed forward
	Vtx_cmd = Kp_cc * Itx_mag_err + Vtx_cmd_integ + Itx_qe_ff; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Vtx_cmd_sat = LIMIT(Vtx_cmd, 0, Vdc_tx); //Vtx saturation (0 <= Vtx_cmd <= Vdc_tx)
	Vdq_ref = Vtx_cmd_sat * 0.63662f; //Vdq_ref = Vref * 2 / PI
	asintemp = Vtx_cmd_sat * Vtx_factor; //asin for getting duty
	asintemp = LIMIT(asintemp, -1., 1.); //asin saturation (-1 <= asintemp <= 1)
	//Vtx_duty = 0.6366f * asinf(asintemp); //Vtx duty
	Vtx_duty = 0.6366f * asin_val[(int)((asintemp * 100) + 100)];
	Vtx_duty = LIMIT(Vtx_duty, 0., 0.5); //Vtx duty saturation (0 <= Vtx_duty <= 0.5)

	EPwm1Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD); //apply duty
	EPwm2Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD); //apply duty

	EPwm2Regs.ETCLR.bit.INT = 1; //set interrupt clear bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //acknowledge pie group 3
	//GPIO_WritePin(81, 0); //for debugging

}
*/
//Ide = mag * cos(theta) control


float32 asintemp; //local variable for asin
int cc_flag = 0;
float temp_RL = 0.;
//float phase_rx = 0.;
interrupt void current_controller(void){

	GPIO_WritePin(86, 1); //for debugging

    //uint16_t TempPIEIER;
	//TempPIEIER = PieCtrlRegs.PIEIER3.all; // Save PIEIER register for later
	IER |= M_INT3;                         // Set global priority by adjusting IER
	//IER &= M_INT3;
	PieCtrlRegs.PIEIER3.all |= 0x0040;    // Set group priority by adjusting PIEIER2 to allow INT2.2 to interrupt current ISR
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;      // Enable PIE interrupts
	asm("       NOP");                    // Wait one cycle
	EINT;

	phase = ATAN(dq.Itx_qe/dq.Itx_de);
	Itx_de_ref = Itx_mag_cmd * (float)(COS(phase)); //Itx_de_ref = Itx_mag_cmd + temp_sin;
	//Itx_de_ref = Itx_mag_cmd; //Itx_de_ref = Itx_mag_cmd + temp_sin;
	//Itx_mag_err = Itx_de_ref - dq.Itx_de; //error between magnitude command and measured magnitude
	Itx_mag_err = Itx_mag_cmd - Itx_mag; //error between magnitude command and measured magnitude
	Vtx_cmd_integ += Ki_cc * Tsamp * Itx_mag_err;
	//if(Vtx_cmd_integ > 1000) Vtx_cmd_sat = 1000;
	Itx_qe_ff = dq.Itx_qe * Itx_qe_factor; //Iqt feed forward
	Vtx_cmd = Kp_cc * Itx_mag_err + Vtx_cmd_integ - Itx_qe_ff; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Vtx_cmd_sat = LIMIT(Vtx_cmd, 0, Vdc_tx); //Vtx saturation (0 <= Vtx_cmd <= Vdc_tx)
	asintemp = Vtx_cmd_sat * Vtx_factor; //asin for getting duty
	asintemp = LIMIT(asintemp, -1., 1.); //asin saturation (-1 <= asintemp <= 1)
	Vtx_duty = 0.3183f * ASIN(asintemp);
	//Vtx_duty = Vtx_cmd_sat / Vdc_tx;
	Vtx_duty = LIMIT(Vtx_duty, 0., 0.5); //Vtx duty saturation (0 <= Vtx_duty <= 0.5)
	EPwm1Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD); //apply duty
	EPwm2Regs.CMPA.bit.CMPA = EPwm1Regs.CMPA.bit.CMPA; //apply duty
	/*
	//state_observeVdc_tx_inv
	//Idth
	//Idth_err = dq.Itx_de - Idth - 0.2;
	Idth_err = dq.Itx_de - Idth;
	Idth_cmd_integ += Ki_dto * Tsamp * Idth_err; //error integration
	Idth_cmd = Kp_dto * Idth_err + Idth_cmd_integ;
	//Iqrh = Idth_cmd * inv_OmegaM * 0.82; //4ohm
	Iqrh = Idth_cmd * inv_OmegaM;
	//Iqrh = Idth_cmd * inv_OmegaM * 1.2;//2ohm
	Itx_qe_ff = dq.Itx_qe * Itx_qe_factor; //Iqt feed forward
	Vdth = Idth_cmd + Itx_qe_ff + 19.09 * sin(PI * Vtx_duty) - Idth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Idth_cmd + Itx_qe_ff + 15 - Idth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Idth_cmd + Iqth_ff + Vdq_ref - Idth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Idth += Vdth * Tsamp * Leq_tx;

	//Iqth
	Iqth_err = dq.Itx_qe - Iqth;	//check
	Iqth_cmd_integ += Ki_qto * Tsamp * Iqth_err; //check
	Iqth_cmd = Kp_qto * Iqth_err + Iqth_cmd_integ; //check
	//Idrh = Iqth_cmd * -inv_OmegaM * 1.32; //4ohm
	Idrh = Iqth_cmd * -inv_OmegaM;
	//Idrh = Iqth_cmd * -inv_OmegaM * 0.8; //2ohm
	Idth_ff = dq.Itx_de * Itx_qe_factor; //Iqt feed forward
	Vqth = Iqth_cmd - Idth_ff - Iqth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Iqth += Vqth * Leq_tx * Tsamp;

	//Iqrhh

	Iqrh_err = Iqrh - Iqrhh;	//check
	Iqrh_cmd_integ += Ki_qro * Tsamp * Iqrh_err; //check
	VqrL = Kp_qro * Iqrh_err + Iqrh_cmd_integ; //check
	RLq = -VqrL / Iqrh;
	//Idth_ff2 = dq.Itx_de * OmegaM * 0.98; //Iqt feed forward
	Idth_ff2 = dq.Itx_de * OmegaM * 0.9; //Iqt feed forward
	Idrh_ff = -Idrh * Irx_qe_factor;
	Vqrhh = VqrL - Idth_ff2 - Idrh_ff - Iqrhh * Rrx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Iqrhh += Vqrhh * Leq_rx * Tsamp;

	//Idrhh
	Idrh_err = Idrh - Idrhh;	//check
	Idrh_cmd_integ += Ki_dro * Tsamp * Idrh_err; //check
	VdrL = Kp_dro * Idrh_err + Idrh_cmd_integ; //check
	RLd = -VdrL / Idrh;
	//Iqth_ff = dq.Itx_qe * OmegaM * 0.98; //Iqt feed forward
	Iqth_ff = dq.Itx_qe * OmegaM * 0.85; //Iqt feed forward
	Iqrh_ff = Iqrh * Irx_qe_factor;
	Vdrhh = VdrL + Iqth_ff + Iqrh_ff - Idrhh * Rrx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Idrhh += Vdrhh * Leq_rx * Tsamp;

	temp_RL = (RLq + RLd) * 0.5;
	*/
	DINT;
	//PieCtrlRegs.PIEIER3.all = TempPIEIER;
	EPwm2Regs.ETCLR.bit.INT = 1; //set interrupt clear bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //acknowledge pie group 3

	GPIO_WritePin(86, 0); //for debugging
}


//rx mag control
/*
interrupt void current_controller(void){
	float32 asintemp; //local variable for asin
	float32 temp_Itx_de, temp_Itx_qe; //local variable to save Itx_dq temporarily
	//GPIO_WritePin(81, 1); //for debugging
	temp_Itx_de = dq.Itx_de; //preventing value variation while cc is running
	temp_Itx_qe = dq.Itx_qe; //preventing value variation while cc is running

	//Itx current controller
	Itx_mag = sqrtf(Irx_de_hat * Irx_de_hat + Irx_qe_hat * Irx_qe_hat); //measured magnitude of transmitter current
	Itx_mag_err = Itx_mag_cmd - Irx_mag; //error between magnitude command and measured magnitude
	//Vtx_cmd_integ += Ki_cc * Tsamp * (Itx_mag_err - Ka_cc * (Vtx_cmd - Vtx_cmd_sat)); //error integration and anti wind-up
	Vtx_cmd_integ += Ki_cc * Tsamp * Itx_mag_err; //error integration and anti wind-up
	//Itx_qe_ff = temp_Itx_qe * Itx_qe_factor; //Iqt feed forward
	//Vtx_cmd = Kp_cc * Itx_mag_err + Vtx_cmd_integ + Itx_qe_ff; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Vtx_cmd = Kp_cc * Itx_mag_err + Vtx_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Vtx_cmd_sat = LIMIT(Vtx_cmd, 0, Vdc_tx); //Vtx saturation (0 <= Vtx_cmd <= Vdc_tx)
	Vdq_ref = Vtx_cmd_sat * 0.63662f; //Vdq_ref = Vref * 2 / PI
	asintemp = Vtx_cmd_sat * Vtx_factor; //asin for getting duty
	asintemp = LIMIT(asintemp, -1., 1.); //asin saturation (-1 <= asintemp <= 1)
	Vtx_duty = 0.6366f * asinf(asintemp); //Vtx duty
	Vtx_duty = LIMIT(Vtx_duty, 0., 0.5); //Vtx duty saturation (0 <= Vtx_duty <= 0.5)

	EPwm1Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD); //apply duty
	EPwm2Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD); //apply duty

	EPwm2Regs.ETCLR.bit.INT = 1; //set interrupt clear bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //acknowledge pie group 3
	//GPIO_WritePin(81, 0); //for debugging

}
*/

int observer_cnt = 0;
//float temp_sinn = 0;
interrupt void state_observer(void){
	GPIO_WritePin(83, 1); //for debugging
	float32 temp_Itx_de, temp_Itx_qe; //local variable to save Itx_dq temporarily

	//observer_cnt ++;
	temp_Itx_de = dq.Itx_de; //preventing value variation while cc is running
	temp_Itx_qe = dq.Itx_qe; //preventing value variation while cc is running

	//Idth Iqrh
	//if(Itx_mag_cmd != 0){
	//if(cc_flag == 1){

		Idth_err = temp_Itx_de - Idth;
		Idth_cmd_integ += Ki_dto * Tsamp * Idth_err; //error integration
		Idth_cmd = Kp_dto * Idth_err + Idth_cmd_integ;
		Iqrh = Idth_cmd * inv_OmegaM;
		Iqth_ff = temp_Itx_qe * Itx_qe_factor; //Iqt feed forward
		Vdth = Idth_cmd + Iqth_ff + 19.1 * sin(3.14 * Vtx_duty) - Idth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		//Vdth = Idth_cmd + Iqth_ff + Vdq_ref - Idth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		Idth += Vdth * Leq_tx * Tsamp;

	//}
	//Iqth Idrh
	//if(Itx_mag_cmd != 0){
		Iqth_err = temp_Itx_qe - Iqth;	//check
		Iqth_cmd_integ += Ki_qto * Tsamp * Iqth_err; //check
		Iqth_cmd = Kp_qto * Iqth_err + Iqth_cmd_integ; //check
		Idrh = Iqth_cmd * -inv_OmegaM; //check
		Idth_ff = temp_Itx_de * Itx_qe_factor; //Iqt feed forward
		Vqth = Iqth_cmd - Idth_ff - Iqth * Rtx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		Iqth += Vqth * Leq_tx * Tsamp;

	//}
/*
		Iqrh_err = Iqrh - Iqrhh;	//check
		Iqrh_cmd_integ += Ki_qro * Tsamp * Iqrh_err; //check
		VqrL = Kp_qro * Iqrh_err + Iqrh_cmd_integ; //check
		RLq = VqrL / Iqrh;
		Idth_ff2 = temp_Itx_qe * OmegaM; //Iqt feed forward
		Idrh_ff = Idrh * Irx_qe_factor;
		Vqrhh = VqrL + Idth_ff2 + Idrh_ff - Iqrhh * Rrx; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
		Iqrhh += Vqrhh * Leq_rx * Tsamp;
*/
	//}
	//temp_sinn = sin(3.141592/2);

	/*
	temp_Itx_de = dq.Itx_de; //preventing value variation while cc is running
	temp_Itx_qe = dq.Itx_qe; //preventing value variation while cc is running

	Idth_err = temp_Itx_de - Idth;
	Idth_cmd_integ += Ki_ob * Tsamp * Idth_err; //error integration
	Iqth_ff = temp_Itx_qe * Itx_qe_factor; //Iqt feed forward
	Idth = Vdq_ref + Kp_ob * Idth_err + Idth_cmd_integ + Iqth_ff; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Iqrh = -Idth * inv_OmegaM;

	Iqth_err = temp_Itx_qe - Iqth;
	Iqth_cmd_integ += Ki_ob * Tsamp * Iqth_err; //error integration
	Idth_ff = temp_Itx_de * Itx_qe_factor; //Iqt feed forward
	Iqth = Kp_ob * Idth_err + Iqth_cmd_integ + Idth_ff; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Idrh = Iqth * inv_OmegaM;

	Irx_mag = sqrtf(Idrh * Idrh + Iqrh * Iqrh);
*/
	//cc_flag = 0;
    EPwm3Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    GPIO_WritePin(83, 0); //for debugging

}
/*
interrupt void current_controller(void)
{
	//float a = 0., b=0.;


//	Itx_phase = atan(Itx_de_avg/Itx_qe_avg);

//	LPF1(Itx_phase, Itx_phase_flt, IDX_ITX_PHASE_FLT);
//	LPF1(Vdc_tx, Vdc_tx_flt, IDX_VDC_TX_FLT);
//	LPF1(Vdc_rx, Vdc_rx_flt, IDX_VDC_RX_FLT);
//	LPF1(Itx_mag, Itx_mag_flt, IDX_ITX_MAG_FLT);

//	Itx_mag_cmd = 1;
	//Vdc_tx 	= 	(((float) AdccResultRegs.ADCRESULT0)*0.0007539258 - VADC_OFFSET)*40.;  // *0.0006762695 = VREF_HI/4096.
	//Itx 	= 	(((float) AdccResultRegs.ADCRESULT1)*0.0007539258)*8.8;
	//Idc_tx 	= 	(((float) AdcaResultRegs.ADCRESULT0)*0.0007539258 - VADC_OFFSET)*10.;
	//Irx 	= 	(((float) AdcaResultRegs.ADCRESULT1)*0.0007539258 - VADC_OFFSET);


	if(FLAG_PWM_ENABLE){
		GPIO_WritePin(PWM_EN0, 0);
		GPIO_WritePin(PWM_EN1, 0);

		if(FLAG_STARTUP){
			LPF1(Itx_mag_cmd, Itx_mag_cmd_flt, IDX_ITX_CMD_FLT);
			Itx_mag_err = Itx_mag_cmd_flt - Itx_mag;
			Vtx_cmd_integ += Ki_cc*Tsamp*(Itx_mag_err - Ka_cc * (Vtx_cmd - Vtx_cmd_sat));
			Vtx_cmd = Kp_cc*Itx_mag_err + Vtx_cmd_integ;
		}
		else {
			Itx_mag_err = Itx_mag_cmd - Itx_mag;
			Vtx_cmd_integ += Ki_cc*Tsamp*(Itx_mag_err - Ka_cc * (Vtx_cmd - Vtx_cmd_sat));
			Vtx_cmd = Kp_cc*Itx_mag_err + Vtx_cmd_integ;
		}

		Vtx_cmd_sat = LIMIT(Vtx_cmd, 0, Vdc_tx);
//		Vtx_duty = Vtx_cmd_sat/Vdc_tx;

//		Vtx_duty = 0.3;

		EPwm1Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD);
		EPwm2Regs.CMPA.bit.CMPA = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD);
		//EPwm2Regs.TBPHS.bit.TBPHS = EPWM1_TBPRD;

	}
	else{
		GPIO_WritePin(PWM_EN0, 1);
		GPIO_WritePin(PWM_EN1, 1);
	}

 //   Itx_mag_avg = sqrt(Itx_de*Itx_de + Itx_qe*Itx_qe);

//	DA12(1);
//	DA12(2);

	cc_cnt++;
	if (cc_cnt > 1000) {
		cc_cnt = 0;
	}

	
	
	EPwm3Regs.ETCLR.bit.INT = 1; //clear INT1 flag
//	EPwm2Regs.ETCLR.bit.INT = 1; //clear INT1 flag
//	AdcaRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1/
//	AdcaRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag.
	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

*/

//Ide control
//float Ide = 0;
/*
interrupt void PI_Controller(void){

	magnitude = sqrtf(dq.Ide * dq.Ide + dq.Iqe * dq.Iqe);
	//Ide = dq.Ide;
	magnitude2 = magnitude;
	ctr.err = ctr.target_Ide - dq.Ide;
	ctr.errsum = ctr.errsum + (ctr.err * Tsamp);
	Vtx_duty = (ctr.p_gain * ctr.err) + (ctr.i_gain * ctr.errsum);

	if(Vtx_duty > 0.5f){
		Vtx_duty = 0.5f;
	}
	else if(Vtx_duty < 0.f){
		Vtx_duty = 0.f;
	}

    EPwm5Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
*/



/*
float temp_theta;
interrupt void State_Space_Controller(void){

	magnitude = sqrtf(dq.Ide * dq.Ide + dq.Iqe * dq.Iqe);
	ctr.err = ctr.target_mag - magnitude;
	//ctr.errsum = ctr.errsum + (ctr.err * Tsamp);
	//Ide, Iqe 제한, software flag, interrupt out, TBfreeze로 게이팅 전부 끄기
	if((abs(dq.Ide) > 0.7f) || (abs(dq.Iqe) > 0.7f)){
		GPIO_WritePin(PWM_EN0, 1);
		GPIO_WritePin(PWM_EN1, 1);

		EALLOW;
			EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
			EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
			EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EDIS;
	}

	ctr.r = ctr.target_mag * cosf(ctr.theta) * ctr.Nbar * 1.37f;
	temp_theta = cos(1.57);
	//ctr.r = ctr.target_mag * cos(ctr.theta);
	ctr.Vref = ctr.r - (ctr.K[0][0] * dq.Ide + ctr.K[0][1] * dq.Iqe);
	ctr.target_duty = 0.5f * ctr.Vref * ctr.Vdc_inv; //+ (ctr.i_gain * ctr.errsum);
	Vref = ctr.target_duty;
	if(ctr.target_duty > 0.5f){
		ctr.target_duty = 0.5f;
	}
	else if(ctr.target_duty < 0.f){
		ctr.target_duty = 0.f;
	}
	Vtx_duty = ctr.target_duty;

	EPwm5Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
*/




