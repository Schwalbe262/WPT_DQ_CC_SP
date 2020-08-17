/*
 * cla_test.h
 *
 *  Created on: 22 Sep 2017
 *      Author: Polar
 */

#ifndef INCLUDE_CLA_TEST_H_
#define INCLUDE_CLA_TEST_H_

#include "F28x_Project.h"
#include "F2837xS_Cla_defines.h"
#include "F2837xS_gpio.h"
#include "CLAmath.h"
#include "variable.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

//Task 1 (C) Variables

struct DQ {
	float32 Ids[12]; //[0] = 0, [1] = 2pi/12, [2] = 4pi/12, [3] = 6pi/12, [4] = 8pi/12, [5] = 10pi/12
	float32 Iqs[12]; //[0] = 6pi/12
	float32 Idss[12];
	float32 Iqss[12];
	Uint16 Ids_cnt;
	Uint16 Idss_cnt;
	Uint16 Iqs_cnt;
	Uint16 Iqss_cnt;
	float32 Itx_de;
	float32 Irx_de;
	float32 Itx_qe;
	float32 Irx_qe;
	//float32 Itx_de_hat;
	//float32 Itx_qe_hat;
	//float32 Irx_de_hat;
	//float32 Irx_qe_hat;
	float32 theta;
	float32 thetaa;
	float32 phase;
	float32 Itx_mag;
};

struct CPU {
	float32 adc_offset;
};

extern struct CPU cpu;
extern struct DQ dq;

//Task 2 (C) Variables

//Task 3 (C) Variables

//Task 4 (C) Variables

//Task 5 (C) Variables

//Task 6 (C) Variables

//Task 7 (C) Variables

//Task 8 (C) Variables

struct OBR {
	float Iqrh_err;
	float Iqrhh;
	float Iqrh_cmd_integ;
	float VqrL;
	float RLq;
	float Idth_ff2;
	float Idrh_ff;
	float Vqrhh;
	float Kp_qro;
	float Ki_qro;
};

extern struct OBR Obr;
//Common (C) Variables

//*****************************************************************************
// function prototypes
//*****************************************************************************
// The following are symbols defined in the CLA assembly code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.

//CLA C Tasks
__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

#ifdef __cplusplus
}
#endif // extern "C@

#endif /* INCLUDE_CLA_TEST_H_ */
