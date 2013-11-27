/*******************************************************************
 * GraphView.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#ifndef _GRAPHVIEW_H
#define _GRAPHVIEW_H

#include "PerfMonitor.h"
#include "ToggleView.h"

class GraphView : public BView {
	public:
						GraphView(BRect);
						~GraphView();
		virtual void	AttachedToWindow();
		virtual void	Draw(BRect);
		
		virtual	void	UpdateTimesteps(double, bool);
		virtual	void	DrawGraph(float, float);
	
	protected:
		BRect		bounds;
		BWindow		*win;
		BBitmap		*offscreen;
		BView		*drawer;
		int			pos;
		double		timestep[kTIMESTEPS];
		bool		operating[kTIMESTEPS];
};


#endif