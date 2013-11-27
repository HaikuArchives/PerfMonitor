/*******************************************************************
 * PerfView.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#ifndef _PERFVIEW_H
#define _PERFVIEW_H

#include "PerfMonitor.h"
#include "GraphView.h"

class PerfView : public GraphView {
	public:
						PerfView(BRect, int);
		virtual void	MouseDown(BPoint);
		virtual	void	Pulse();
				void	Update(double, bool);
				void	DrawGraph(float, float);
	
	private:
		double			oldActiveTime, oldSystemTime;
		system_info		sysInfo;
		int 			cpu;
};


#endif