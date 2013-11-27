/*******************************************************************
 * PerfView.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "PerfView.h"

/******************************************************************
 * PerfView::PerfView()
 *****************************************************************/
PerfView::PerfView(BRect bound, int cpu) : GraphView(bound)
{
	// keep track of which CPU to watch
	this->cpu = cpu;
	
	// init calculating variables
	get_system_info(&sysInfo);
	oldActiveTime = sysInfo.cpu_infos[cpu].active_time; 
	oldSystemTime = system_time();
}

/*********************************************
 * PerfView::MouseDown()
 ********************************************/
void PerfView::MouseDown(BPoint /* point */)
{
	// if the CPU is currently enabled
	if(_kget_cpu_state_(cpu)) {
		int				numActive = 0;
		
		get_system_info(&sysInfo);
		
		// see how many cpus are currently active
		for(int i = 0; i < sysInfo.cpu_count; i++) {
			if(_kget_cpu_state_(i)) {
				numActive++;
			}
		}
			
		// if there's only one active CPU, it must be me... I want to live
		if(numActive == 1) {
			beep();
		}
		else { // ok, shut me down
			_kset_cpu_state_(cpu, FALSE);
		}
	}
	else {
		// enable CPU
		_kset_cpu_state_(cpu, TRUE);
	}
	
	// feedback to user
	Draw(bounds);
}

/******************************************************************
 * PerfView::DrawGraph()
 *****************************************************************/
void PerfView::DrawGraph(float width, float height)	
{	
	rgb_color	color;
	BPoint		start, end;
	
	drawer->BeginLineArray(kTIMESTEPS);
	// init starting point
	start.Set(1, height - 1 - (timestep[0] * (height-2)));
	for(float i = 0; i < pos-1; i++) {
		if(operating[(int) i + 1]) { // on: green light
			color = kGREEN;
		}
		else { // off: red light
			color = kRED;
		}
		// messy formulae to place line correctly in 3D bordered view
		end.Set((i+1) / kTIMESTEPS * width + 1, height - 1 - (timestep[(int)i+1] * (height-2)));
		
		drawer->AddLine(start, end, color);
		start = end; // update start
	}
	drawer->EndLineArray();
	
	// indicate current on/off status
	// if the processor is off, dim the view
	if(!_kget_cpu_state_(cpu)) {
		drawer->SetDrawingMode(B_OP_BLEND);
		drawer->SetHighColor(kDIMMED);
		drawer->FillRect(bounds);
		// restore drawing mode to default
		drawer->SetDrawingMode(B_OP_COPY);
	}
}

/******************************************************************
 * PerfView::Pulse()
 *****************************************************************/
void PerfView::Pulse()
{
	double			CPUTime, now;
	
	now  = system_time();
	get_system_info(&sysInfo);
		
	// calculate the CPU load fraction
	CPUTime = (sysInfo.cpu_infos[cpu].active_time - oldActiveTime)
			/ (now - oldSystemTime);
			
	// store values for next iteration
	oldActiveTime = sysInfo.cpu_infos[cpu].active_time;
	oldSystemTime = now;
	
	// determine processor's on/off status
	if(_kget_cpu_state_(cpu)) {
		UpdateTimesteps(CPUTime, TRUE);
	}
	else {
		UpdateTimesteps(CPUTime, FALSE);
	}
}
