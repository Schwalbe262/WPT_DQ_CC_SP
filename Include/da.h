// File name : da.c
// function :  DA output control

// variable explanation(#=1,2,3,4)
//	da# : address of variable
//  da#_type = 0 ; the variable is float
//	         = 1 ; the variable is integer
//	da#_mid : mid value
//	da#_rng : da scale

// use this routine in EasyDSP as below
//	da1=&var_float
//	da1_type=0
//	da1_mid=0
//	da1_rng=20
//	da2 = &var_int
//	da2_type = 1
//	da2_mid= 0
//	da2_rng = 20

#ifndef _DA_EasyDSP
#define _DA_EasyDSP

// you should specify the da address of your own
#define	DA1_ADDR			(*(int *)0x00005C03)
#define	DA2_ADDR			(*(int *)0x00005C13)
#define	DA3_ADDR			(*(int *)0x00005C23)

extern unsigned int da1, da2, da3, da1_type, da2_type, da3_type;
extern float da1_rng, da1_val, da1_mid;
extern float da2_rng, da2_val, da2_mid;
extern float da3_rng, da3_val, da3_mid;

// 12 bit DA
#define DA12(num) 	\
	da##num##_val = (da##num##_type == 0 ? *(float *)da##num : (float)(*(int *)da##num)) ; \
    DA##num##_ADDR = (int)((da##num##_val-da##num##_mid )* 0x7ff/da##num##_rng) + 0x800 ;

// 8 bit DA
#define DA8(num) 	\
	da##num##_val = (da##num##_type == 0 ? *(float *)da##num : (float)(*(int *)da##num)) ; \
    DA##num##_ADDR = (int)((da##num##_val-da##num##_mid )*0x7f/da##num##_rng) + 0x80 ;

#endif

// ref 1:
// if da1_val = 20, da1_mid=0, da1_rng=20 => DA1_ADDR = 0xFFF (12 bit)
//	20*0x7ff/20 + 0x800 = 0xfff

// ref 2:
// 0x7ff/da##num##_rng => may cause time consumption
