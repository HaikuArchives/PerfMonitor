/*******************************************************************
 * RAMView.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "RAMView.h"

/******************************************************************
 * RAMView::RAMView()
 *****************************************************************/
RAMView::RAMView(BRect bound) : GraphView(bound)
{
}

/******************************************************************
 * RAMView::Pulse()
 *****************************************************************/
void RAMView::Pulse()
{
	double	ratio;
	
	system_info		sysInfo;
	get_system_info(&sysInfo);
		
	// calculate the percentage of used pages
	ratio = (double) sysInfo.used_pages / sysInfo.max_pages; 
	
	// the TRUE below doesn't mean anything for this class
	UpdateTimesteps(ratio, TRUE);
}

/******************************************************************
 * RAMView::DrawGraph()
 *****************************************************************/
void RAMView::DrawGraph(float width, float height)
{
	BPoint		start,end;
	
	drawer->BeginLineArray(kTIMESTEPS);
	
	// init starting point
	start.Set(1, height - 1 - (timestep[0] * (height-2)));
	for(float i = 0; i < pos-1; i++) {
		// messy formulae to place line correctly in 3D bordered view
		end.Set((i+1) / kTIMESTEPS * width + 1, height - 1 - (timestep[(int)i+1] * (height-2)));
		drawer->AddLine(start, end, kBLUE);
		start = end; // update start
	}
	drawer->EndLineArray();
}