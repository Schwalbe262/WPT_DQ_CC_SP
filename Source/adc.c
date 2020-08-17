#include "F2837xS_device.h"
#include "F2837xS_Examples.h"

void ConfigureADC(void)
{
	//Uint16 acqps;

    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    //power up the ADC
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 3;  //SOC0 will convert pin ADCINA3
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0x11; //trigger on ePWM7 SOCA/C

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 4;  //SOC1 will convert pin ADCINA4
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
	AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0x11; //trigger on ePWM7 SOCA/C

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    EDIS;
    /*
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    //
    //Select the channels to convert and end of conversion flag
    //
    EALLOW;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin ADCINA3
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 9; //trigger on ePWM7 SOCA/C

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 4;  //SOC1 will convert pin ADCINA4
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
	AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 9; //trigger on ePWM7 SOCA/C

    AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;  //SOC2 will convert pin ADCINC2
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 9; //trigger on ePWM7 SOCA/C

    AdccRegs.ADCSOC1CTL.bit.CHSEL = 3;  //SOC3 will convert pin ADCINC3
	AdccRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
	AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 9; //trigger on ePWM7 SOCA/C

	AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC1 will set INT1 flag
	AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
	*/
    //EDIS;

}



