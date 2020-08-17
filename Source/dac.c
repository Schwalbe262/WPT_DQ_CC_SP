
#include "F2837xS_device.h"
#include "F28x_Project.h"

void ConfigureDAC(void)
{
    EALLOW;

    DAC_PTR[1]->DACCTL.bit.DACREFSEL = 1;  // reference = VREFHI
    DAC_PTR[1]->DACCTL.bit.LOADMODE = 0;
    DAC_PTR[1]->DACOUTEN.bit.DACOUTEN = 1;
    DAC_PTR[1]->DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up

    DAC_PTR[2]->DACCTL.bit.DACREFSEL = 1;  // reference = VREFHI
    DAC_PTR[2]->DACCTL.bit.LOADMODE = 0;
    DAC_PTR[2]->DACOUTEN.bit.DACOUTEN = 1;
    DAC_PTR[2]->DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up

    DAC_PTR[3]->DACCTL.bit.DACREFSEL = 1;  // reference = VREFHI
    DAC_PTR[3]->DACCTL.bit.LOADMODE = 0;
    DAC_PTR[3]->DACOUTEN.bit.DACOUTEN = 1;
    DAC_PTR[3]->DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up


    EDIS;
}


