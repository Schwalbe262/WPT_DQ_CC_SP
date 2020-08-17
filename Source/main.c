#include "F28x_Project.h"
#include "cla_test.h"
#include "math.h"
#include "SFO_V8.h"
#include "dac.h"
#include "mathtable.h"

void init_param(void);
void update_param(void);
void init_7_segment(void);
void Configure_HW_Fault_Pin(void);
void Comp_Fsw(void);
void Init_Var(void);
void Init_Parameter(void);
void Init_Controller(void);
void Init_Observer(void);
void cla_Init(void);
void Configure_GPIO_CLA(void);
__interrupt void current_controller(void);
__interrupt void dac_cpu_isr(void);
__interrupt void dac_cla_isr(void);
__interrupt void cla1Isr1();
__interrupt void cpu_timer0_isr(void);

int MEP_ScaleFactor; // Global variable used by the SFO library
                     // Result can be used for all HRPWM channels
                     // This variable is also copied to HRMSTEP
                     // register by SFO(0) function.

volatile struct EPWM_REGS *ePWM[PWM_CH] =
{  &EPwm1Regs, &EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs, &EPwm5Regs,
   &EPwm6Regs, &EPwm7Regs, &EPwm8Regs};



#pragma DATA_SECTION(dq, "Cla1ToCpuMsgRAM");
struct DQ dq;

#pragma DATA_SECTION(cpu, "CpuToCla1MsgRAM");
struct CPU cpu;

float VdqRL = 0.;
float phase_rx = 0.;
int dac_flag = 0;

void main(void)
{
//	Step 1
//	Disable Global Interrupt & Interrupt Flag Clear
 	DINT;
	IER = 0x0000;
	IFR = 0x0000;

//	Step 2
	InitSysCtrl();
	InitGpio();
    InitEPwm1Gpio();
    InitEPwm2Gpio();
	Configure_GPIO_CLA();
	Configure_PWM_EN_GPIO();
	Configure_LED_GPIO();

//	Step 3
//	3.1 Initialize Peripheral Interrupt Expansion circuit
	InitPieCtrl();

//	Step 4
//	4.1 Pie Vector Table Re-allocation
    InitPieVectTable();

//	Step 5
//	5.1 Interrupt Service routine re-mapping and Interrupt vector enable
    EALLOW;
    	PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    	PieVectTable.EPWM1_INT = &dac_cpu_isr;
    	PieVectTable.EPWM2_INT = &current_controller;
    	PieVectTable.EPWM7_INT = &dac_cla_isr;
    EDIS;

    Configure_HW_Fault_Pin();

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	// PIE1.7 - TIMER 0
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	// PIE3.1 - EPWM 1
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;	// PIE3.2 - EPWM 2
    PieCtrlRegs.PIEIER3.bit.INTx7 = 1;	// PIE3.7 - EPWM 7

//	Step 6
//	6.1 Initialize Peripherals for User Application
	EALLOW;
		CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
    EDIS;

	init_param();
	ConfigureDAC();
	ConfigureEPWM1();
	ConfigureEPWM2();
    ConfigureADC();
	ConfigureEPWM7();

	EALLOW;
		CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // Run all the TB clocks
		EPwm1Regs.TBCTL.bit.SWFSYNC = 1;
		EPwm2Regs.TBCTL.bit.SWFSYNC = 1;
		EPwm7Regs.TBCTL.bit.SWFSYNC = 1;
    EDIS;

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 180, 200);
    CpuTimer0Regs.TCR.all = 0x4000;

//	Step 7
//	Initialize CLA module
	cla_Init();

//	Step 8
//	8.1 Enable Global real-time interrupt DBGM
//	8.2 Enable Global Interrupt
    IER |= M_INT3;	//Enable CPU INT3 for EPWM3_INT
    IER |= M_INT1; //Enable group 1 interrupts
    IER |= M_INT13; //Enable group 1 interrupts

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

	for(;;){
		update_param();
	}
}

void update_param(void){

	if(FLAG_PWM_ENABLE == 1){
		if(Itx_mag_cmd == 0) Vtx_cmd_integ = 0;
		if(PWM_ENABLE_CNT == 0){
			GPIO_WritePin(PWM_EN0, 0);
			GPIO_WritePin(PWM_EN1, 0);
			EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
			EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
			EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
			PWM_ENABLE_CNT = 1;
		}
		SFO();

		Itx_mag = sqrtf(dq.Itx_de * dq.Itx_de + dq.Itx_qe * dq.Itx_qe); //measured magnitude of transmitter current
		Irx_mag = sqrtf(dq.Irx_de * dq.Irx_de + dq.Irx_qe * dq.Irx_qe); //measured magnitude of transmitter current
		Irx_magh = sqrtf(Idrh * Idrh + Iqrh * Iqrh);
		VdqRL = sqrtf(VdrL * VdrL + VqrL * VqrL);
		phase_rx = 57.3 * ATAN(Iqrh/Idrh);

	}
	else{
		GPIO_WritePin(PWM_EN0, 1);
		GPIO_WritePin(PWM_EN1, 1);
		Init_Var();
		EPwm1Regs.CMPA.bit.CMPA = (Uint16) (EPWM1_TBPRD);
		EPwm2Regs.CMPA.bit.CMPA = (Uint16) (EPWM1_TBPRD);
		EPwm7Regs.CMPA.bit.CMPA = (Uint16) (EPWM3_TBPRD);
		EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		EPwm7Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
		PWM_ENABLE_CNT = 0;
	}
}

__interrupt void cla1Isr1 ()
{
 	PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;	// Acknowledge interrupt to PIE
}

//============================================================================================

void cla_Init(void){

	EALLOW;
		MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;	//Initialization CLA1toCPUMsgRam and wait for completion
		while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

		MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1; //Initialization CPUToCLA1MsgRAM and wait for completion
		while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

		MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;		//Set LS4RAM as CLA program memory
		MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;

		MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;		//Set LS5RAM as CLA program memory
		MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

		MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;		//Set LS0RAM as CLA data memory
		MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

		MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;		//Set LS1RAM as CLA data memory
		MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;
	EDIS;

	EALLOW;
		Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
		Cla1Regs.MCTL.bit.IACKE = CLA_IACK_DISABLE;
		Cla1Regs.MIER.all = M_INT1;
		PieVectTable.CLA1_1_INT = &cla1Isr1;
		DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = CLA_TRIG_EPWM7INT;

		Cla1Regs.MIER.bit.INT1 = 1; // Enable CLA Task 1 by interrupt
	    PieCtrlRegs.PIEIER11.bit.INTx1 = 1;				// Enable INT 11.1 in the PIE (CLA Task1)

	    IER |= M_INT11;

	EDIS;

}

void init_param(void){

	FLAG_PWM_ENABLE = 0;
	Vtx_duty = 0.;
	Init_Var();
	Comp_Fsw();
	Tsamp = 1/fsw;
	Tsamp2 = 1/fsw2;
	EPWM1_TBPRD = (Uint16) (180.E6*Tsamp)/2;
	EPWM3_TBPRD = (Uint16) (180.E6*Tsamp2)/2;

	EPWM1_TBPRDHR = (Uint16)(((180.E6*Tsamp-EPWM1_TBPRD*2)*32768));
	EPWM7_TBPRDHR = (Uint16)(((180.E6*Tsamp2-EPWM3_TBPRD*2)*32768));

	EPWM1_DB = 10;
	EPWM2_DB = 10;

	EALLOW;
		EPwm1Regs.TBPRD = EPWM1_TBPRD;
		EPwm1Regs.TBPRDHR = EPWM1_TBPRDHR;

		EPwm2Regs.TBPRD = EPWM1_TBPRD;
		EPwm2Regs.TBPRDHR = EPWM1_TBPRDHR;

		EPwm2Regs.TBPHS.bit.TBPHS = EPWM2_TBPHS;           // Phase is 180 DEGREE
		EPwm2Regs.TBPHS.bit.TBPHSHR = 100000;

		EPwm1Regs.HRCNFG.bit.EDGMODEB = HR_BEP;
		EPwm1Regs.HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges.
		EPwm1Regs.HRCNFG.bit.CTLMODE = HR_CMP;
		EPwm1Regs.HRCNFG.bit.CTLMODEB = HR_CMP;
		EPwm1Regs.HRCNFG.bit.HRLOAD = HR_CTR_PRD ;  // load on CTR = 0 and CTR = TBPRD
		EPwm1Regs.HRCNFG.bit.HRLOADB = HR_CTR_ZERO_PRD;
		EPwm1Regs.HRCNFG.bit.AUTOCONV = 1;        // Enable autoconversion for HR period
		EPwm1Regs.HRPCTL.bit.TBPHSHRLOADE = 1;    // Enable TBPHSHR sync (required for updwn count HR control)
		EPwm1Regs.HRPCTL.bit.HRPE = 1;            // Turn on high-resolution period control.

		EPwm2Regs.HRCNFG.bit.EDGMODE = HR_BEP;
		EPwm2Regs.HRCNFG.bit.EDGMODEB = HR_BEP;
		EPwm2Regs.HRCNFG.bit.CTLMODE = HR_CMP;
		EPwm2Regs.HRCNFG.bit.CTLMODEB = HR_CMP;
		EPwm2Regs.HRCNFG.bit.HRLOAD = HR_CTR_PRD;  // load on CTR = 0 and CTR = TBPRD
		EPwm2Regs.HRCNFG.bit.HRLOADB = HR_CTR_ZERO_PRD;
		EPwm2Regs.HRCNFG.bit.AUTOCONV = 1;        // Enable autoconversion for HR period
		EPwm2Regs.HRPCTL.bit.TBPHSHRLOADE = 1;    // Enable TBPHSHR sync (required for updwn count HR control)
		EPwm2Regs.HRPCTL.bit.HRPE = 1;            // Turn on high-resolution period control.

		EPwm3Regs.HRCNFG.bit.EDGMODE = HR_BEP;
		EPwm3Regs.HRCNFG.bit.EDGMODEB = HR_BEP;
		EPwm3Regs.HRCNFG.bit.CTLMODE = HR_CMP;
		EPwm3Regs.HRCNFG.bit.CTLMODEB = HR_CMP;
		EPwm3Regs.HRCNFG.bit.HRLOAD = HR_CTR_PRD;  // load on CTR = 0 and CTR = TBPRD
		EPwm3Regs.HRCNFG.bit.HRLOADB = HR_CTR_ZERO_PRD;
		EPwm3Regs.HRCNFG.bit.AUTOCONV = 1;        // Enable autoconversion for HR period
		EPwm3Regs.HRPCTL.bit.TBPHSHRLOADE = 1;    // Enable TBPHSHR sync (required for updwn count HR control)
		EPwm3Regs.HRPCTL.bit.HRPE = 1;            // Turn on high-resolution period control.

		EPwm7Regs.HRCNFG.bit.EDGMODEB = HR_BEP;
		EPwm7Regs.HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges.
		EPwm7Regs.HRCNFG.bit.HRLOAD = HR_CTR_PRD;  // load on CTR = 0 and CTR = TBPRD
		EPwm7Regs.HRCNFG.bit.HRLOADB = HR_CTR_PRD;
		EPwm7Regs.HRCNFG.bit.AUTOCONV = 1;        // Enable autoconversion for HR period
		EPwm7Regs.HRPCTL.bit.TBPHSHRLOADE = 1;    // Enable TBPHSHR sync (required for updwn count HR control)
		EPwm7Regs.HRPCTL.bit.HRPE = 1;            // Turn on high-resolution period control.

	EDIS;
}

void Configure_GPIO_CLA(void){
	//GPIO hand over to CLA
	EALLOW;
	GpioCtrlRegs.GPAGMUX2.bit.GPIO31 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1; // 1=OUTput, 0=INput
	GpioDataRegs.GPASET.bit.GPIO31 = 0; // Set High initially
	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0; // 0= enables pullup, 1= disables pullup
	GpioCtrlRegs.GPACSEL4.bit.GPIO31 = 1; // 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA

	GpioCtrlRegs.GPBGMUX1.bit.GPIO34 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1; // 1=OUTput, 0=INput
	GpioDataRegs.GPBSET.bit.GPIO34 = 0; // Set High initially
	GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0; // 0= enables pullup, 1= disables pullup
	GpioCtrlRegs.GPBCSEL1.bit.GPIO34 = 0; // 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA

	GpioCtrlRegs.GPBGMUX1.bit.GPIO44 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1; // 1=OUTput, 0=INput
	GpioDataRegs.GPBSET.bit.GPIO44 = 0; // Set High initially
	GpioCtrlRegs.GPBPUD.bit.GPIO44 = 0; // 0= enables pullup, 1= disables pullup
	GpioCtrlRegs.GPBCSEL2.bit.GPIO44 = 1; // 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA

	GpioCtrlRegs.GPCGMUX2.bit.GPIO88 = 0;
	GpioCtrlRegs.GPCDIR.bit.GPIO88 = 1; // 1=OUTput, 0=INput
	GpioDataRegs.GPCSET.bit.GPIO88 = 0; // Set High initially
	GpioCtrlRegs.GPCPUD.bit.GPIO88 = 0; // 0= enables pullup, 1= disables pullup
	GpioCtrlRegs.GPCCSEL4.bit.GPIO88 = 0; // 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA

	GpioCtrlRegs.GPCGMUX2.bit.GPIO89 = 0;
	GpioCtrlRegs.GPCDIR.bit.GPIO89 = 1; // 1=OUTput, 0=INput
	GpioDataRegs.GPCSET.bit.GPIO89 = 0; // Set High initially
	GpioCtrlRegs.GPCPUD.bit.GPIO89 = 0; // 0= enables pullup, 1= disables pullup
	GpioCtrlRegs.GPCCSEL4.bit.GPIO89 = 0; // 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA
	EDIS;
}

void Configure_HW_Fault_Pin(void){

    EALLOW;
		InputXbarRegs.INPUT4SELECT = 47; 	// Highest PRIORITY FAULT INTERRUPT
		InputXbarRegs.INPUT5SELECT = 50;	// Highest PRIORITY FAULT INTERRUPT
		InputXbarRegs.INPUT6SELECT = 53;	// Lowest PRIORITY FAULT INTERRUPT
		InputXbarRegs.INPUT13SELECT = 56;	// Lowest PRIORITY FAULT INTERRUPT
		InputXbarRegs.INPUT14SELECT = 59;	// Lowest PRIOIRTY FAULT INTERRUPT

    	XintRegs.XINT1CR.bit.POLARITY = 0;
    	XintRegs.XINT2CR.bit.POLARITY = 0;
    	XintRegs.XINT3CR.bit.POLARITY = 0;
    	XintRegs.XINT4CR.bit.POLARITY = 0;
    	XintRegs.XINT5CR.bit.POLARITY = 0;

    	XintRegs.XINT1CR.bit.ENABLE = 1;
    	XintRegs.XINT2CR.bit.ENABLE = 1;
    	XintRegs.XINT3CR.bit.ENABLE = 1;
    	XintRegs.XINT4CR.bit.ENABLE = 1;
    	XintRegs.XINT5CR.bit.ENABLE = 1;
    EDIS;
}

__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if(FLAG_PWM_ENABLE == 1){
		if(CpuTimer0.InterruptCount > 4000){
			GPIO_WritePin(31,0);
			GPIO_WritePin(34,1);
			CpuTimer0.InterruptCount = 0;
		}
		else if(CpuTimer0.InterruptCount > 2000){
			GPIO_WritePin(31,1);
			GPIO_WritePin(34,0);
		}
    }
    else{
		GPIO_WritePin(31,1);
		GPIO_WritePin(34,1);
		CpuTimer0.InterruptCount = 0;
    }

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}


float temp_VdrL;
float temp_VqrL;
float temp_VdqrL;
__interrupt void dac_cpu_isr(void)
{

	if(dac_flag == 0){

		//DAC_PTR[3]->DACVALS.all = (Uint16)((Irx_magh) * dac_factor * 1.02);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((Irx_mag) * dac_factor);

		//DAC_PTR[3]->DACVALS.all = (Uint16)((-Idrh) * dac_factor * 1.02);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Irx_de) * dac_factor);

		//DAC_PTR[3]->DACVALS.all = (Uint16)((-Iqrh) * dac_factor);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Irx_qe) * dac_factor * 1.1);

		temp_VdrL = (VdrL) * 0.5 * 0.9;
		temp_VqrL = (VqrL) * 0.5 * 0.95;
		temp_VdqrL = sqrtf(temp_VdrL * temp_VdrL + temp_VqrL * temp_VqrL);
		//DAC_PTR[3]->DACVALS.all = (Uint16)(temp_VdrL * dac_factor);
		//DAC_PTR[2]->DACVALS.all = (Uint16)(temp_VqrL * dac_factor);

		//DAC_PTR[1]->DACVALS.all = (Uint16)(temp_VdqrL * dac_factor);



		//temp_sin = 0.1*sin(62831.853 * op_time);
		DAC_PTR[1]->DACVALS.all = (Uint16)((dq.Itx_de) * dac_factor);
		DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Itx_qe) * dac_factor);
		DAC_PTR[3]->DACVALS.all = (Uint16)((dq.Itx_mag) * dac_factor);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Irx_de) * dac_factor);
		//DAC_PTR[3]->DACVALS.all = (Uint16)((-Idrh) * dac_factor);


		//observer
		//DAC_PTR[1]->DACVALS.all = (Uint16)((VdqRL) * dac_factor * 0.25);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((VdrL) * dac_factor * 0.5);
		//DAC_PTR[3]->DACVALS.all = (Uint16)((VqrL) * dac_factor * 0.5);

		//DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Irx_qe) * dac_factor);
		//DAC_PTR[3]->DACVALS.all = (Uint16)((-Iqrh) * dac_factor);

		//DAC_PTR[2]->DACVALS.all = (Uint16)((-dq.Irx_qe) * dac_factor);
		//DAC_PTR[3]->DACVALS.all = (Uint16)((-Iqrh) * dac_factor);
	}

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}

void init_7_segment(void){
	GPIO_WritePin(71, 1);
	GPIO_WritePin(74, 1);
	GPIO_WritePin(77, 1);
	GPIO_WritePin(80, 1);
	GPIO_WritePin(81, 1);
	GPIO_WritePin(83, 1);
	GPIO_WritePin(86, 1);
	GPIO_WritePin(87, 1);
}

interrupt void dac_cla_isr(void)
{

	if(dac_flag == 1){
		//temp_sin = 0.1*sin(6283.1853 * op_time);
		//DAC_PTR[1]->DACVALS.all = (Uint16)((Itx_de_ref) * dac_factor);
		//DAC_PTR[1]->DACVALS.all = (dq.theta + 3.141592) * 200;
		//DAC_PTR[2]->DACVALS.all = (dq.theta + 3.141592) * 100;
		DAC_PTR[2]->DACVALS.all = dq.Ids_cnt * 300;
		//DAC_PTR[3]->DACVALS.all = EPwm1Regs.TBCTR * 2;

		//DAC_PTR[1]->DACVALS.all = (Uint16)((temp_sin + 1)*dac_factor);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((dq.Itx_de) * dac_factor);
		//DAC_PTR[2]->DACVALS.all = (Uint16)((dq.Ids[dq.Ids_cnt] + 1)*0.5*1241);
		//DAC_PTR[1]->DACVALS.all = (Uint16)((dq.Idss[dq.Ids_cnt] + 1)*0.5*1241);
	}
    EPwm7Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void Comp_Fsw(void){

	int fsam_TBPRD;
	int fsw_TBPRD;
	float temp_fsam;

	fsw2 = fsw * 8;
	temp_fsam = fsw2;
	fsam_TBPRD = 90E6 / temp_fsam;
	fsw_TBPRD = (int)(fsam_TBPRD * 8);
	fsw = 90E6 / fsw_TBPRD;

}

void Init_Var(void){
	Init_Parameter();
	Init_Controller();
	Init_Observer();
}

void Init_Parameter(void){

	fsw = 62e3;
	Vtx_duty = 0.;
	w_sw = 2 * PI * fsw;
	w_cc = 2 * PI * 100;
	//Ltx  = 110e-6;
	//Ltx  = 110e-6*1.05;
	//Ctx = 70e-9;
	Ltx  = 151e-6;
	Ctx = 48e-9;
	fr_tx = 1 / (2 * PI * sqrt(Ltx * Ctx));
	Rtx = 0.26;
	Rrx = 0.067;
	Lrx = 28e-6;
	Crx = 263e-9;
	fr_rx = 1 / (2 * PI * sqrt(Lrx * Crx));
	Leq_tx = 1/(Ltx + (1.f / powf(w_sw, 2) / Ctx));
	Leq_rx = 1/(Lrx + (1.f / powf(w_sw, 2) / Crx));

	Vdc_tx = 10.f;
	Vdc_tx_inv = 1 / Vdc_tx;
	Zeq_tx = sqrtf(powf(w_sw, 2) * powf(Ltx, 2) + powf(Rtx, 2));
	Vtx_factor = PI * 0.25 * Vdc_tx_inv;
	Itx_qe_factor = w_sw * Ltx - 1 / (w_sw * Ctx);
	Irx_qe_factor = w_sw * Lrx - 1 / (w_sw * Crx);
	Vdq_ref = Vdc_tx * 4 / PI;
	//M = 10.82e-6*1.05;
	M = 16e-6;
	OmegaM = w_sw*M;
	inv_OmegaM = 1 / OmegaM;
	Irx_mag = 0.;
	Irx_magh = 0.;
}

void Init_Controller(void){
	Kp_cc = 5;
	Ki_cc = 10000;
	Ka_cc = 1 / Kp_cc;

	Itx_mag_err = 0.f;
	Itx_mag_errsum = 0.f;
	Itx_mag_cmd = 0.f;
	Vtx_cmd = 0.;
	Vtx_cmd_integ = 0.;
	Itx_mag = 0.;
	phase = 0;
	Itx_de_ref = 0;
	//Ide Controller
	//ctr.theta = atanf(ctr.Xeq/ctr.Rtx);	//radian
	//SS Controller
	/*
	ctr.Nbar = 11.9763f;
	ctr.Vdc_inv = 1 / ctr.Vdc;

	{
		ctr.K[0][0] = 8.9654f;
		ctr.K[0][1] = 0.5002f;
	}

	ctr.r = ctr.target_mag * cos(ctr.theta) * ctr.Nbar;
	ctr.Vref = ctr.r - (ctr.K[0][0] * dq.Ide + ctr.K[0][1] * dq.Iqe);
	ctr.target_duty = ctr.Vref * ctr.Vdc_inv;
	 */
}

void Init_Observer(void){
	Kp_dto = 5;	//1500
	Ki_dto = 1000;	//63000
	Kp_qto = 5;	//1500
	Ki_qto = 1000;	//63000
	//Kp_qro = 0.06;	//0.3
	Kp_qro = 0.5;	//0.3
	//Ki_qro = 6300;	//630
	Ki_qro = 100;	//630
	Kp_dro = 0.5;	//0.3
	Ki_dro = 100;	//630

	Idth_err = 0;
	Idth_cmd_integ = 0;
	Idth_cmd = 0;
	Iqrh = 0;
	Iqth_ff = 0;
	Vdth = 0;
	Idth = 0;

	Iqth_err = 0;
	Iqth_cmd_integ = 0; //error integration
	Iqth_cmd = 0;
	Idrh = 0;
	Idth_ff = 0; //Iqt feed forward
	Vqth = 0; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Iqth = 0;

	Iqrh_err = 0;	//check
	Iqrh_cmd_integ = 0; //check
	VqrL = 0; //check
	RLq = 0;
	Idth_ff2 = 0; //Iqt feed forward
	Idrh_ff = 0;
	Vqrhh = 0; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Iqrhh = 0;

	Idrh_err = 0;	//check
	Idrh_cmd_integ = 0; //check
	VdrL = 0; //check
	RLd = 0;
	Iqrh_ff = 0;
	Vdrhh = 0; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	//Vdth = Kp_ob * Idth_err + Idth_cmd_integ; //Vtx command addition (Vtx_cmd_prop + Vtx_cmd_integ)
	Idrhh = 0;

}

