#ifndef __function_h__
#define __function_h__

#include "F2837xS_device.h"
//#include "math.h"
#include "CLAmath.h"
//#include "stdio.h"
//#include "stdlib.h"



/* macro functions **************************************** */

#define	BOUND_PI(x)		((x) - 2.*PI*(int)((x + PI)/(2.*PI)))
#define SIN(x,x2)		((x)*((float)1.-(x2)*(f3-(x2)*(f5-(x2)*(f7-(x2)*(f9-(x2)*(f11-(x2)*(f13-(x2)*f15))))))))
//#define COS(x2)			((float)1.-(x2)*(f2-(x2)*(f4-(x2)*(f6-(x2)*(f8-(x2)*(f10-(x2)*(f12-(x2)*f14)))))))
#define SIN_INV_X(x2)   (((float)1.-(x2)*(f3-(x2)*(f5-(x2)*(f7-(x2)*(f9-(x2)*(f11-(x2)*(f13-(x2)*f15))))))))
#define EXP(x)			((float)1.+(x)*((float)1.+(x)*(f2+(x)*(f3+(x)*(f4+(x)*(f5+(x)*(f6+(x)*f7)))))))

#define LIMIT(x,s,l)	(((x)>(l))?(l):((x)<(s))?((s)):(x))

#define	INIT_LPF1(Wcut,Ts,nf)	{LPF1[(nf)].Wc=(Wcut); a=1./(2.+(Ts)*LPF1[(nf)].Wc);\
						LPF1[(nf)].Kxn[0]=(Ts)*LPF1[(nf)].Wc*a;\
						LPF1[(nf)].Kxn[1]=LPF1[(nf)].Kxn[0];\
						LPF1[(nf)].Kyn[0]=(2.-(Ts)*LPF1[(nf)].Wc)*a;\
						LPF1[(nf)].yn[0]=0.;LPF1[(nf)].xn[0]=0.;}

#define	UPDATE_LPF1(Wcut,Ts,nf) {LPF1[(nf)].Wc=(Wcut); a=1./(2.+(Ts)*LPF1[(nf)].Wc);\
						LPF1[(nf)].Kxn[0]=(Ts)*LPF1[(nf)].Wc*a;\
						LPF1[(nf)].Kxn[1]=LPF1[(nf)].Kxn[0];\
						LPF1[(nf)].Kyn[0]=(2.-(Ts)*LPF1[(nf)].Wc)*a;}

#define	LPF1(x,y,nf)	{a=(x);\
					b=LPF1[(nf)].Kxn[0]*a+LPF1[(nf)].Kxn[1]*LPF1[(nf)].xn[0]+LPF1[(nf)].Kyn[0]*LPF1[(nf)].yn[0];\
					LPF1[(nf)].yn[0]=b;\
					LPF1[(nf)].xn[0]=a;\
					(y)=b;}

#define	INIT_LPF2(Wcut,Zeta,Ts,nf) { LPF2[(nf)].Wc=(Wcut);	LPF2[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*LPF2[(nf)].Ze*(Ts)*LPF2[(nf)].Wc+(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc);\
					LPF2[(nf)].Kxn[0]=(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc*a;\
					LPF2[(nf)].Kxn[1]=2.*LPF2[(nf)].Kxn[0];	LPF2[(nf)].Kxn[2]=LPF2[(nf)].Kxn[0];\
					LPF2[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc)*a;\
					LPF2[(nf)].Kyn[1]=(-4.+4.*LPF2[(nf)].Ze*(Ts)*LPF2[(nf)].Wc-(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc)*a;\
					LPF2[(nf)].yn[1]=0.; LPF2[(nf)].yn[0]=0.;LPF2[(nf)].xn[1]=0.; LPF2[(nf)].xn[0]=0.;}

#define	UPDATE_LPF2(Wcut,Zeta,Ts,nf)	{ LPF2[(nf)].Wc=(Wcut);	LPF2[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*LPF2[(nf)].Ze*(Ts)*LPF2[(nf)].Wc+(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc);\
					LPF2[(nf)].Kxn[0]=(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc*a;\
					LPF2[(nf)].Kxn[1]=2.*LPF2[(nf)].Kxn[0];	LPF2[(nf)].Kxn[2]=LPF2[(nf)].Kxn[0];\
					LPF2[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc)*a;\
					LPF2[(nf)].Kyn[1]=(-4.+4.*LPF2[(nf)].Ze*(Ts)*LPF2[(nf)].Wc-(Ts)*(Ts)*LPF2[(nf)].Wc*LPF2[(nf)].Wc)*a;}

#define	LPF2(x,y,nf)	{a=(x);\
					b=LPF2[(nf)].Kxn[0]*a+LPF2[(nf)].Kxn[1]*LPF2[(nf)].xn[0]+LPF2[(nf)].Kxn[2]*LPF2[(nf)].xn[1]\
					+LPF2[(nf)].Kyn[0]*LPF2[(nf)].yn[0]+LPF2[(nf)].Kyn[1]*LPF2[(nf)].yn[1];\
					LPF2[(nf)].yn[1]=LPF2[(nf)].yn[0];   LPF2[(nf)].yn[0]=b;\
					LPF2[(nf)].xn[1]=LPF2[(nf)].xn[0];   LPF2[(nf)].xn[0]=a; (y)=b;}

#define	INIT_HPF1(Wcut,Ts,nf)	{ HPF1[(nf)].Wc=(Wcut);\
					a=1./(2.+(Ts)*HPF1[(nf)].Wc);\
					HPF1[(nf)].Kxn[0]=2.*a;\
					HPF1[(nf)].Kxn[1]=-2.*a;\
					HPF1[(nf)].Kyn[0]=(2.-(Ts)*HPF1[(nf)].Wc)*a;\
					HPF1[(nf)].yn[0]=0.;HPF1[(nf)].xn[0]=0.;}

#define	UPDATE_HPF1(Wcut,Ts,nf)	{ HPF1[(nf)].Wc=(Wcut);\
					a=1./(2.+(Ts)*HPF1[(nf)].Wc);\
					HPF1[(nf)].Kxn[0]=2.*a;\
					HPF1[(nf)].Kxn[1]=-2.*a;\
					HPF1[(nf)].Kyn[0]=(2.-(Ts)*HPF1[(nf)].Wc)*a;}

#define	HPF1(x,y,nf)	{a=(x);\
					b=HPF1[(nf)].Kxn[0]*a+HPF1[(nf)].Kxn[1]*HPF1[(nf)].xn[0]+HPF1[(nf)].Kyn[0]*HPF1[(nf)].yn[0];\
					HPF1[(nf)].yn[0]=b;\
					HPF1[(nf)].xn[0]=a;\
					(y)=b;}

#define	INIT_HPF2(Wcut,Zeta,Ts,nf)	{ HPF2[(nf)].Wc=(Wcut); 	HPF2[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*HPF2[(nf)].Ze*(Ts)*HPF2[(nf)].Wc+(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc);\
					HPF2[(nf)].Kxn[0]=4*a;\
					HPF2[(nf)].Kxn[1]=-8*a;\
					HPF2[(nf)].Kxn[2]=HPF2[(nf)].Kxn[0];\
					HPF2[(nf)].Kyn[0]=(8-2*(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc)*a;\
					HPF2[(nf)].Kyn[1]=(-4+4*HPF2[(nf)].Ze*(Ts)*HPF2[(nf)].Wc-(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc)*a;\
					HPF2[(nf)].yn[1]=0.; HPF2[(nf)].yn[0]=0.;HPF2[(nf)].xn[1]=0.; HPF2[(nf)].xn[0]=0.;}

#define	UPDATE_HPF2(Wcut,Zeta,Ts,nf)	{ HPF2[(nf)].Wc=(Wcut); 	HPF2[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*HPF2[(nf)].Ze*(Ts)*HPF2[(nf)].Wc+(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc);\
					HPF2[(nf)].Kxn[0]=4*a;\
					HPF2[(nf)].Kxn[1]=-8*a;\
					HPF2[(nf)].Kxn[2]=HPF2[(nf)].Kxn[0];\
					HPF2[(nf)].Kyn[0]=(8-2*(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc)*a;\
					HPF2[(nf)].Kyn[1]=(-4+4*HPF2[(nf)].Ze*(Ts)*HPF2[(nf)].Wc-(Ts)*(Ts)*HPF2[(nf)].Wc*HPF2[(nf)].Wc)*a;}

#define	HPF2(x,y,nf)	{a=(x);\
					b=HPF2[(nf)].Kxn[0]*a+HPF2[(nf)].Kxn[1]*HPF2[(nf)].xn[0]+HPF2[(nf)].Kxn[2]*HPF2[(nf)].xn[1]\
					  +HPF2[(nf)].Kyn[0]*HPF2[(nf)].yn[0]+HPF2[(nf)].Kyn[1]*HPF2[(nf)].yn[1];\
					HPF2[(nf)].yn[1]=HPF2[(nf)].yn[0];   HPF2[(nf)].yn[0]=b;\
					HPF2[(nf)].xn[1]=HPF2[(nf)].xn[0];   HPF2[(nf)].xn[0]=a;\
					(y)=b;}

#define	INIT_BPF(Wcut,Zeta,Ts,nf)	{BPF[(nf)].Wc=(Wcut);BPF[(nf)].Ze=(Zeta);a=1./(4.+4.*BPF[(nf)].Ze*(Ts)*BPF[(nf)].Wc+(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc);\
					BPF[(nf)].Kxn[0]=4.*BPF[(nf)].Ze*BPF[(nf)].Wc*(Ts)*a; BPF[(nf)].Kxn[1]=0.;BPF[(nf)].Kxn[2]=-BPF[(nf)].Kxn[0];\
					BPF[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc)*a;BPF[(nf)].Kyn[1]=(-4.+4.*BPF[(nf)].Ze*(Ts)*BPF[(nf)].Wc-(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc)*a;\
					BPF[(nf)].yn[0]=0.;BPF[(nf)].xn[0]=0.;BPF[(nf)].yn[1]=0.;BPF[(nf)].xn[1]=0.;}

#define	UPDATE_BPF(Wcut,Zeta,Ts,nf)	{BPF[(nf)].Wc=(Wcut);BPF[(nf)].Ze=(Zeta);a=1./(4.+4.*BPF[(nf)].Ze*(Ts)*BPF[(nf)].Wc+(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc);\
					BPF[(nf)].Kxn[0]=4.*BPF[(nf)].Ze*BPF[(nf)].Wc*(Ts)*a; BPF[(nf)].Kxn[1]=0.;BPF[(nf)].Kxn[2]=-BPF[(nf)].Kxn[0];\
					BPF[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc)*a;	BPF[(nf)].Kyn[1]=(-4.+4.*BPF[(nf)].Ze*(Ts)*BPF[(nf)].Wc-(Ts)*(Ts)*BPF[(nf)].Wc*BPF[(nf)].Wc)*a;}

#define	BPF(x,y,nf)	{a=(x);\
					b=BPF[(nf)].Kxn[0]*a+BPF[(nf)].Kxn[1]*BPF[(nf)].xn[0]+BPF[(nf)].Kxn[2]*BPF[(nf)].xn[1]\
					+BPF[(nf)].Kyn[0]*BPF[(nf)].yn[0]+BPF[(nf)].Kyn[1]*BPF[(nf)].yn[1];\
					BPF[(nf)].yn[1]=BPF[(nf)].yn[0];   BPF[(nf)].yn[0]=b;\
					BPF[(nf)].xn[1]=BPF[(nf)].xn[0];   BPF[(nf)].xn[0]=a; (y)=b;}

#define	INIT_NF(Wcut,Zeta,Ts,nf) { NF[(nf)].Wc=(Wcut);	NF[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*NF[(nf)].Ze*(Ts)*NF[(nf)].Wc+(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc);\
					NF[(nf)].Kxn[0]=(4.+(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kxn[1]=(-8.+2.*(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kxn[2]=NF[(nf)].Kxn[0];\
					NF[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kyn[1]=(-4.+4.*NF[(nf)].Ze*(Ts)*NF[(nf)].Wc-(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].yn[0]=0.;NF[(nf)].xn[0]=0.;NF[(nf)].yn[1]=0.;NF[(nf)].xn[1]=0.;}

#define	UPDATE_NF(Wcut,Zeta,Ts,nf) { NF[(nf)].Wc=(Wcut);	NF[(nf)].Ze=(Zeta);\
					a=1./(4.+4.*NF[(nf)].Ze*(Ts)*NF[(nf)].Wc+(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc);\
					NF[(nf)].Kxn[0]=(4.+(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kxn[1]=(-8.+2.*(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kxn[2]=NF[(nf)].Kxn[0];\
					NF[(nf)].Kyn[0]=(8.-2.*(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;\
					NF[(nf)].Kyn[1]=(-4.+4.*NF[(nf)].Ze*(Ts)*NF[(nf)].Wc-(Ts)*(Ts)*NF[(nf)].Wc*NF[(nf)].Wc)*a;}

#define	NF(x,y,nf)	{a=(x);\
					b=NF[(nf)].Kxn[0]*a+NF[(nf)].Kxn[1]*NF[(nf)].xn[0]+NF[(nf)].Kxn[2]*NF[(nf)].xn[1]\
					  +NF[(nf)].Kyn[0]*NF[(nf)].yn[0]+NF[(nf)].Kyn[1]*NF[(nf)].yn[1];\
					NF[(nf)].yn[1]=NF[(nf)].yn[0];   NF[(nf)].yn[0]=b;\
					NF[(nf)].xn[1]=NF[(nf)].xn[0];   NF[(nf)].xn[0]=a;\
					(y)=b;}
					


// Inverse Factorial 
#define f2      	((float)(1./2.))
#define	f3			((float)(f2/3.))
#define	f4			((float)(f3/4.))
#define	f5			((float)(f4/5.))
#define	f6			((float)(f5/6.))
#define	f7			((float)(f6/7.))
#define	f8			((float)(f7/8.))
#define	f9			((float)(f8/9.))
#define	f10			((float)(f9/10.))
#define	f11			((float)(f10/11.))
#define	f12			((float)(f11/12.))
#define	f13			((float)(f12/13.))
#define	f14			((float)(f13/14.))
#define	f15			((float)(f14/15.))

				
#define	BOUND_PI(x)		((x) - 2.*PI*(int)((x + PI)/(2.*PI)))
#define SIN(x,x2)		((x)*((float)1.-(x2)*(f3-(x2)*(f5-(x2)*(f7-(x2)*(f9-(x2)*(f11-(x2)*(f13-(x2)*f15))))))))
//#define COS(x2)			((float)1.-(x2)*(f2-(x2)*(f4-(x2)*(f6-(x2)*(f8-(x2)*(f10-(x2)*(f12-(x2)*f14)))))))
#define COS(x2)			1-(x2*x2*f2)+(x2*x2*x2*x2*f4)
#define SIN_INV_X(x2)   (((float)1.-(x2)*(f3-(x2)*(f5-(x2)*(f7-(x2)*(f9-(x2)*(f11-(x2)*(f13-(x2)*f15))))))))
#define EXP(x)			((float)1.+(x)*((float)1.+(x)*(f2+(x)*(f3+(x)*(f4+(x)*(f5+(x)*(f6+(x)*f7)))))))
#define PI_h			PI/2
#define ATAN(x)		(PI_h)*(0.596227*x + x*x)/(1. + 1.192454*x + x*x)
#define ABS(x)			(((x) < 0) ? -(x) : (x))
#define ASIN(x)			(x)*(1 + x * x * (0.1666666 + x * x * (0.075 + x * x * 0.0446428)))


#endif	
