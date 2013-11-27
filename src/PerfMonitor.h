/*******************************************************************
 * PerfMonitor.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#ifndef _PERFMONITOR_H
#define _PERFMONITOR_H

/* undocumented kernel routines */

extern "C" int  _kget_cpu_state_(int cpu);
extern "C" int  _kset_cpu_state_(int cpu, int enabled);

/* BMessages */
enum {
	mRESIZE		= 'resz'
};

/* enums */
enum {
	VISIBLE,
	HIDDEN
};

/* constants */

const int	kTIMESTEPS = 90;
// pulserate = 1 minute's worth of microseconds div by timesteps
const int	kPULSERATE = 60000000 / kTIMESTEPS;
// window dims
const int	kTOGGLESIZE = 14;
const int	kGRAPHSIZE = 40;
const int	kWIDTH = 180;
const int	kHEIGHT = kGRAPHSIZE + kTOGGLESIZE;

// some custom colors
const rgb_color kRED = {128,0,0};
const rgb_color kGREEN = {0,128,0};
const rgb_color kBLUE = {0,0,128};
const rgb_color kBGCOLOR = {210,210,210};
const rgb_color kHIGHLIGHT = {250,250,250};
const rgb_color kSHADOW = {160,160,160};
const rgb_color kDIMMED = {100,100,100};
const rgb_color kTOGGLEBGCOLOR = {192,192,192};
const rgb_color kTOGGLEBLUE = {0,192,255};

#endif