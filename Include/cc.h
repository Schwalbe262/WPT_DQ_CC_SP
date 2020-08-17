/*
 * cc.h
 *
 *  Created on: 2017. 1. 23.
 *      Author: UoS_PEETS
 */

#ifndef INCLUDE_CC_H_
#define INCLUDE_CC_H_


extern interrupt void current_controller(void);
extern interrupt void state_observer(void);
extern interrupt void adc_isr(void);


#endif /* INCLUDE_CC_H_ */
