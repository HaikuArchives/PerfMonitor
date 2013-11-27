/*******************************************************************
 * RAMView.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#ifndef _RAMVIEW_H
#define _RAMVIEW_H

#include "PerfMonitor.h"
#include "GraphView.h"

class RAMView : public GraphView {
	public:
						RAMView(BRect);
		virtual void	Pulse();
		virtual void	DrawGraph(float, float);
};


#endif