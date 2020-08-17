#include "F2837xS_device.h"
#include "F28x_Project.h"

#define DB_UP          1
#define DB_DOWN        0


Uint16 EPwm1_DB_Direction;
Uint16 EPwm2_DB_Direction;
Uint16 EPwm3_DB_Direction;
//Uint16 EPwm4_DB_Direction;
//Uint16 EPwm5_DB_Direction;
//Uint16 EPwm6_DB_Direction;
//Uint16 EPwm7_DB_Direction;

//
// ConfigureEPWM - Configure EPWM SOC and compare values
//
void ConfigureEPWM1(void)
{

	EALLOW;

	//
	// Setup TBCLK
	//EPWMCLKDIV

	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	//EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		//for synchronization
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		//for synchronization
	EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;         // Disable phase loading
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

	EPwm1Regs.TBPRD = EPWM1_TBPRD; // Set timer period
	EPwm1Regs.TBPRDHR = EPWM1_TBPRDHR;
	EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                     // Clear counter

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	//
	// Setup compare
	//
	EPwm1Regs.CMPA.bit.CMPA = (Uint16)(EPWM1_TBPRD);
	//EPwm1Regs.CMPB.bit.CMPB = (Uint16) ((1-Vtx_duty)*EPWM1_TBPRD);
	//
	// Set actions
	//
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM2A on Zero
	EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;

	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm1Regs.DBRED.bit.DBRED = 3;
	EPwm1Regs.DBFED.bit.DBFED = 3;
	EPwm1_DB_Direction = DB_UP;

	//
	// Interrupt where we will modify the deadband
	//
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	EDIS;

}

void ConfigureEPWM2(void)
{
	EALLOW;

	//
	// Setup TBCLK
	//
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	//EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	//EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Slow just to observe on
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;

	//EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;          // Slow just to observe on the scope
	EPwm2Regs.TBPRD = EPWM1_TBPRD;                       // Set timer period
	EPwm2Regs.TBPRDHR = EPWM1_TBPRDHR;
	//EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 180 DEGREE
	EPwm2Regs.TBPHS.bit.TBPHS = EPWM1_TBPRD;           // Phase is 180 DEGREE
	EPwm2Regs.TBCTR = 0x0000;                     // Clear counter

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	//
	// Setup compare
	//
	//EPwm2Regs.CMPA.bit.CMPA = (Uint16) ((1 - Vtx_duty) * EPWM1_TBPRD);
	EPwm2Regs.CMPA.bit.CMPA = (Uint16)(EPWM1_TBPRD);
	//
	// Set actions
	//
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;

	EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM2A on Zero
	EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;
	//
	// Active Low complementary PWMs - setup the deadband
	//
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED.bit.DBRED = 3;
	EPwm2Regs.DBFED.bit.DBFED = 3;
	EPwm2_DB_Direction = DB_UP;

	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 3rd event

	EDIS;
}

void ConfigureEPWM3(void)
{

	EALLOW;
	//EPwm3Regs.ETSEL.bit.SOCAEN = 1;
	//EPwm3Regs.ETSEL.bit.SOCASEL = 1;
	//EPwm3Regs.ETPS.bit.SOCAPRD = 1;
	//
	// Setup TBCLK
	//
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
	//EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Slow just to observe on
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;          // Slow just to observe on
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBPRD = (Uint16)(EPWM1_TBPRD);                       // Set timer period
	EPwm3Regs.TBPHS.bit.TBPHS = 0;           // Phase is 180 DEGREE

	EPwm3Regs.TBCTR = 0x0000;                     // Clear counter

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	//
	// Setup compare
	//
	//EPwm3Regs.CMPA.bit.CMPA = (Uint16) ((1 - Vtx_duty) * EPWM1_TBPRD);
	EPwm3Regs.CMPA.bit.CMPA = 0.5 * EPWM1_TBPRD;
	//
	// Set actions
	//
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM2A on Zero
	EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;
	//
	// Active Low complementary PWMs - setup the deadband
	//
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBRED.bit.DBRED = 3;
	EPwm3Regs.DBFED.bit.DBFED = 3;
	EPwm3_DB_Direction = DB_UP;

	EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 3rd event
	EDIS;
}

void ConfigureEPWM5(void)
{

	EALLOW;

	//
	// Setup TBCLK
	//
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	//EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	//EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
	EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;          // Slow just to observe on
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;

	//EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;          // Slow just to observe on the scope
	EPwm5Regs.TBPRD = EPWM1_TBPRD;                       // Set timer period
	EPwm5Regs.TBPRDHR = EPWM1_TBPRDHR;
	//EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 180 DEGREE
	EPwm5Regs.TBPHS.bit.TBPHS = EPWM1_TBPRD;           // Phase is 180 DEGREE
	EPwm5Regs.TBCTR = 0x0000;                     // Clear counter

	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	//
	// Setup compare
	//
	//EPwm2Regs.CMPA.bit.CMPA = (Uint16) ((1 - Vtx_duty) * EPWM1_TBPRD);
	EPwm5Regs.CMPA.bit.CMPA = (Uint16)(EPWM1_TBPRD);
	//
	// Set actions
	//
	EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
	EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM2A on Zero
	EPwm5Regs.AQCTLB.bit.CAD = AQ_CLEAR;
	//
	// Active Low complementary PWMs - setup the deadband
	//
	//EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	//EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	//EPwm4Regs.DBRED.bit.DBRED = 10;
	//EPwm4Regs.DBFED.bit.DBFED = 10;
	//EPwm4_DB_Direction = DB_UP;

	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm5Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm5Regs.ETPS.bit.INTPRD = ET_10TH;           // Generate INT on 3rd event

	EDIS;

}

void ConfigureEPWM7(void)
{


	EALLOW;
	EPwm7Regs.ETSEL.bit.SOCAEN = 1;
	EPwm7Regs.ETSEL.bit.SOCASEL = 1;
	EPwm7Regs.ETPS.bit.SOCAPRD = 1;
	//
	// Setup TBCLK
	//
	EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	//EPwm7Regs.TBCTL.bit.PHSDIR = 1;
	EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // Slow just to observe on
	EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
	EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Slow just to observe on
	EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm7Regs.TBPRD = EPWM3_TBPRD;                       // Set timer period
	EPwm7Regs.TBPRDHR = EPWM7_TBPRDHR;
	//EPwm7Regs.TBPHS.bit.TBPHS = EPWM3_TBPRD*0.75;           // Phase is 180 DEGREE
	EPwm7Regs.TBPHS.bit.TBPHS = EPWM3_TBPRD*0.25;           // Phase is 180 DEGREE
	//EPwm7Regs.TBPHS.bit.TBPHS = 219;           // Synchronization with EPWM 1, 2

	EPwm7Regs.TBCTR = 0x0000;                     // Clear counter

	EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	//
	// Setup compare
	//
	EPwm7Regs.CMPA.bit.CMPA = (Uint16) (EPWM3_TBPRD);

	EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM2A on Zero
	EPwm7Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm7Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM2A on Zero
	EPwm7Regs.AQCTLB.bit.CAD = AQ_CLEAR;
	//
	// Interrupt where we will modify the deadband
	//
	EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm7Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;
	EPwm7Regs.DBRED.bit.DBRED = EPWM3_DB;
	EPwm7Regs.DBFED.bit.DBFED = EPWM3_DB;

	EPwm7Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm7Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm7Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

	EDIS;
}

/*

void ConfigureEPWM5(void)
{

	EALLOW;
	//
	// Setup TBCLK
	//
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV2;          // Slow just to observe on
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Slow just to observe on
	EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading


					   // the scope
	EPwm5Regs.TBPRD = EPWM1_TBPRD;                       // Set timer period
	EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
	EPwm5Regs.TBCTR = 0x0000;                     // Clear counter

	//
	// Setup compare
	//
	EPwm5Regs.CMPA.bit.CMPA = 416;

	//
	// Set actions
	//
	EPwm5Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM2A on Zero
	EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;

	EPwm5Regs.AQCTLB.bit.ZRO = AQ_SET;          // Set PWM2A on Zero
	EPwm5Regs.AQCTLB.bit.CAU = AQ_CLEAR;

	//
	// Active Low complementary PWMs - setup the deadband
	//
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm5Regs.DBRED.bit.DBRED = EPWM5_DB;
	EPwm5Regs.DBFED.bit.DBFED = EPWM5_DB;
	EPwm5_DB_Direction = DB_UP;

	//
	// Interrupt where we will modify the deadband
	//

	EDIS;
}

void ConfigureEPWM6(void)
{


	EALLOW;

	//
	// Setup TBCLK
	//
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV2;          // Slow just to observe on
	EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading

	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Slow just to observe on
//	EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;          // Slow just to observe on
			   // the scope
	EPwm6Regs.TBPRD = EPWM1_TBPRD;                       // Set timer period
	EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
	EPwm6Regs.TBCTR = 0x0000;                     // Clear counter

	//
	// Setup compare
	//
	EPwm6Regs.CMPA.bit.CMPA = 416;

	//
	// Set actions
	//
	EPwm6Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM2A on Zero
	EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;

	EPwm6Regs.AQCTLB.bit.ZRO = AQ_SET;          // Set PWM2A on Zero
	EPwm6Regs.AQCTLB.bit.CAU = AQ_CLEAR;

	//
	// Active Low complementary PWMs - setup the deadband
	//
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm6Regs.DBCTL.bit.IN_MODE = DBB_ALL;
	EPwm6Regs.DBRED.bit.DBRED = EPWM6_DB;
	EPwm6Regs.DBFED.bit.DBFED = EPWM6_DB;
	EPwm6_DB_Direction = DB_UP;

	//
	// Interrupt where we will modify the deadband
	//
//	EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
//	EPwm6Regs.ETSEL.bit.INTEN = 1;                // Enable INT
//	EPwm6Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event
	EDIS;
}
*/
