#include "F2837xS_device.h"
#include "F28x_Project.h"


Uint16 FLAG_HW_FAULT 	 = 0;

FILTER1	LPF1[N_LPF1],	HPF1[N_HPF1];
FILTER2	LPF2[N_LPF2],	HPF2[N_HPF2];
NF_BPF	BPF[N_BPF],		NF[N_NF];

//Uint16 AIN11 = 0, AIN12 = 0, AIN13 = 0, AIN14 = 0, AIN21 = 0, AIN22 = 0, AIN23 = 0, AIN24 = 0;
Uint16 FLAG_PWM_ENABLE = 0, PWM_ENABLE_CNT = 0;
Uint32 cc_cnt = 0, adc_cnt = 0;

Uint16 EPWM1_TBPRD = 0;
Uint32 HRPWM_TBPRD = 0;

double fsw = 0., fsw2 = 0., Tsamp = 0., Tsamp2 = 0., temp = 0., w_sw = 0.;
Uint16 TWO_EPWM1_TBPRD = 0, EPWM2_TBPRD = 0, EPWM3_TBPRD = 0, EPWM7_TBPRD = 0, EPWM2_TBPHS = 0, EPWM1_TBPRDHR = 0, EPWM7_TBPRDHR = 0;
Uint16 EPWM1_DB = 0, EPWM2_DB = 0, EPWM3_DB = 0, EPWM4_DB = 0, EPWM5_DB = 0, EPWM6_DB = 0, EPWM7_DB = 0;

float32 adc_offset;

volatile struct DAC_REGS* DAC_PTR[4] = {0x0,&DacaRegs,&DacbRegs,&DaccRegs};

float Vdc_tx = 0., Vdc_tx_inv = 0., Vdc_tx_flt = 0., Vdc_rx = 0., Vdc_rx_flt = 0., Itx = 0., Itx_1 = 0., Itx_2 = 0., Irx = 0., Itx_ds = 0., Itx_qs = 0., Itx_de = 0., Itx_qe = 0., Itx_de_sum = 0., Itx_de_avg = 0., Itx_qe_sum = 0., Itx_qe_avg = 0.;

float theta = 0.;
float Itx_phase = 0., Itx_phase_flt = 0.;

float Itx_mag_cmd = 0., Itx_mag = 0., Irx_mag = 0., Irx_magh = 0.;

//Current Controller
float Itx_mag_err = 0., Itx_mag_errsum = 0., Vtx_cmd_integ = 0., Vtx_cmd = 0., Vtx_cmd_sat = 0., Vtx_cmd_ff = 0., Vtx_duty = 0., Itx_qe_ff;

float w_cc = 0., Kp_cc = 0., Ki_cc = 0., Ka_cc = 0.;
float Ltx = 0., Rtx = 0., Ctx = 0., Lrx = 0., Rrx = 0., Crx = 0., Leq_tx = 0., Zeq_tx = 0., Leq_rx = 0.;
float dac_factor = 1241., Vtx_factor = 0., Itx_qe_factor = 0., Irx_qe_factor = 0.;
float fr_tx = 0., fr_rx = 0.;

float OmegaM = 0., inv_OmegaM, M = 0.;
float phase =0., inv_Itx_de=0.;

//observer
float Vdth = 0., Idth = 0., Iqrh = 0., Vdq_ref = 0., Idth_err = 0., Idth_cmd_integ = 0., Iqth_ff = 0., Idth_cmd = 0.;
float Vqth = 0., Iqth = 0., Idrh = 0., Iqth_err = 0., Iqth_cmd_integ = 0., Idth_ff = 0., Iqth_cmd = 0.;
float Iqrh_err = 0., Iqrhh = 0., Iqrh_cmd_integ = 0., VqrL = 0., RLq = 0., Idth_ff2 = 0., Idrh_ff = 0., Vqrhh = 0.;
float Idrh_err = 0., Idrhh = 0., Idrh_cmd_integ = 0., VdrL = 0., RLd = 0., Iqrh_ff = 0., Vdrhh = 0.;
float Ki_dto = 0., Kp_dto = 0., Ki_qto = 0., Kp_qto = 0., Kp_qro = 0., Ki_qro = 0., Kp_dro = 0., Ki_dro = 0.;
float temp_sin = 0.f, Itx_de_ref = 0.f;
