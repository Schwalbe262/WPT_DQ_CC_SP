#ifndef __Variable_h__
#define __Variable_h__


#define		PI					(float)3.14159265358979
#define		PI_OVER_4			(float)0.7853981
#define		TWO_PI_OVER_4			(float)1.570796
#define 	SQRT2				1.414213562
#define 	SQRT3				1.732050807
#define 	INV3				((float)0.333333333)
#define		TWO_PI				((float)6.28318530718)
#define 	MAX_OFFSET_CNT		10000

//#define 	EPWM1_TBPRD			1664
//#define 	EPWM1_TBPRD			6249
//#define 	EPWM1_TBPRD			832

//#define     EPWM7_TBPRD			(Uint16) EPWM1_TBPRD/4
//#define 	Tsamp 				((EPWM1_TBPRD+1)/100E6)*2
//#define 	EPWM1_TBPRD_OVER_4	(Uint16) EPWM1_TBPRD/4


#define 	N_LPF1		6
#define 	N_LPF2		4
#define 	N_HPF1		1
#define 	N_HPF2		1
#define 	N_BPF		1
#define 	N_NF		1

#define 	IDX_ITX_PHASE_FLT	0
#define 	IDX_ITX_CMD_FLT		1
#define 	IDX_VDC_TX_FLT		2
#define 	IDX_VDC_RX_FLT		3
#define 	IDX_ITX_MAG_FLT 	4
#define 	IDX_IL_REF_FLT	 	5

#define 	VDC_TX_FAULT_LEVEL	300
#define 	ITX_FAULT_LEVEL		20
#define 	VDC_RX_FAULT_LEVEL	300
#define 	IRX_FAULT_LEVEL		20

#define 	VREF_HI	 	2.68
//#define 	VREF_HI	 	2.48

#define 	VADC_OFFSET	1.56

typedef struct _FILTER1
{
	float		Wc;
	float		Kxn[2];
	float		Kyn[1];
	float 		xn[1];
	float		yn[1];
}FILTER1;

typedef struct _FILTER2
{
	float		Wc;
	float		Ze;
	float		Kxn[3];
	float		Kyn[2];
	float 	xn[2];
	float		yn[2];
}FILTER2;


typedef struct _NF_BPF
{
	float		Wc;
	float		Ze;
	float		InvKy;
	float		Kxn[3];
	float		Kyn[2];
	float 	xn[2];
	float		yn[2];
}NF_BPF;

typedef struct _Filter_CutOff
{
	float		Vdc_filter;
	float		Wrm_filter;
	float		Wrm_Inv_filter;
	float		Power_Con_Cutoff;
	float		Power_Inv_Cutoff;
	float		Power_Super_Cutoff;
	float		Vscap_filter;
	float		Power_feedfowrd;
 }Filter_CutOff;


extern FILTER1	LPF1[N_LPF1],	HPF1[N_HPF1];
extern FILTER2	LPF2[N_LPF2],	HPF2[N_HPF2];
extern NF_BPF	BPF[N_BPF],		NF[N_NF];

extern Uint16 FLAG_HW_FAULT;

extern Uint32 cc_cnt, adc_cnt;
extern Uint16 FLAG_PWM_ENABLE, PWM_ENABLE_CNT;
extern Uint16 AIN11, AIN12, AIN13, AIN14, AIN21, AIN22, AIN23, AIN24;
extern Uint16 TWO_EPWM1_TBPRD, EPWM1_TBPRD, EPWM2_TBPRD, EPWM3_TBPRD, EPWM7_TBPRD, EPWM2_TBPHS, EPWM1_TBPRDHR, EPWM7_TBPRDHR;
extern Uint16 EPWM1_DB, EPWM2_DB, EPWM3_DB, EPWM4_DB, EPWM5_DB, EPWM6_DB, EPWM7_DB;
extern float32 adc_offset;
extern double fsw, fsw2, Tsamp, Tsamp2, temp, w_sw;
extern volatile struct DAC_REGS* DAC_PTR[4];

extern float Vdc_tx, Vdc_tx_inv, Vdc_tx_flt, Vdc_rx, Vdc_rx_flt, Itx, Itx_1, Itx_2, Irx, Itx_ds, Itx_qs, Itx_de, Itx_qe, Itx_de_sum, Itx_de_avg, Itx_qe_sum, Itx_qe_avg;

extern float theta;
extern float Itx_phase, Itx_phase_flt;
extern float Itx_mag_cmd, Itx_mag, Irx_mag, Irx_magh;

extern float Itx_mag_err, Itx_mag_errsum, Vtx_cmd_integ, Vtx_cmd, Vtx_cmd_sat, Vtx_cmd_ff, Vtx_duty, Itx_qe_ff;

extern float w_cc, Kp_cc, Ki_cc, Ka_cc;
extern float Ltx, Rtx, Ctx, Lrx, Rrx, Crx, Leq_tx, Zeq_tx, Leq_rx;
extern float dac_factor, Vtx_factor, Itx_qe_factor, Irx_qe_factor;
extern float fr_tx, fr_rx;

extern float Vdth, Idth, Iqrh, Vdq_ref, Idth_err, Idth_cmd_integ, Iqth_ff, Idth_cmd;
extern float Vqth, Iqth, Idrh, Iqth_err, Iqth_cmd_integ, Idth_ff, Iqth_cmd;
extern float Iqrh_err, Iqrhh, Iqrh_cmd_integ, VqrL, RLq, Idth_ff2, Idrh_ff, Vqrhh;
extern float Idrh_err, Idrhh, Idrh_cmd_integ, VdrL, RLd, Iqrh_ff, Vdrhh;
extern float Ki_dto, Kp_dto, Ki_qto, Kp_qto, Kp_qro, Ki_qro, Kp_dro, Ki_dro;
extern float OmegaM, inv_OmegaM, M;
extern float phase, inv_Itx_de;
extern float temp_sin, Itx_de_ref;

#endif	
