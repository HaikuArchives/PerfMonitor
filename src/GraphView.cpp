/*******************************************************************
 * GraphView.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "GraphView.h"

/******************************************************************
 * GraphView::GraphView()
 *****************************************************************/
GraphView::GraphView(BRect bound)	
	: BView(bound, "", B_FOLLOW_TOP, B_WILL_DRAW | B_PULSE_NEEDED)
{
	// store bounds to speed access
	bounds = bound;
	bounds.OffsetTo(B_ORIGIN);
	
	// set the timestep pointer
	pos = 0;
	
	// create an offscreen bmap and a view to draw into it
	offscreen = new BBitmap(bounds, B_COLOR_8_BIT, TRUE);
	drawer = new BView(bounds, "", B_FOLLOW_ALL, B_WILL_DRAW);
	offscreen->AddChild(drawer);
	
	// make the default color match the bg color
	SetViewColor(kBGCOLOR);
}

/******************************************************************
 * GraphView::GraphView()
 *****************************************************************/
GraphView::~GraphView()
{
	// deallocate bitmap
	delete offscreen;
}

/******************************************************************
 * GraphView::AttachedToWindow()
 *****************************************************************/
void GraphView::AttachedToWindow()	
{
	// store the window to speed access
	win = Window();
}

/******************************************************************
 * GraphView::Draw()
 *****************************************************************/
void GraphView::Draw(BRect /* update */)	
{
	float		height, width;
		
	width = bounds.Width();
	height = bounds.Height();
	
	// clear bmap
	offscreen->Lock();
	drawer->SetHighColor(kBGCOLOR);
	drawer->FillRect(bounds);
	
	// create hash marks 
	BPoint		left, right;
	
	for(float i = 0.25; i < 1.0; i+=.25) {
		left.Set(0, i * height);
		right.Set(width, i * height);
		drawer->SetHighColor(kSHADOW);
		drawer->StrokeLine(left, right);
	}
	
	// create 3D border
	BPoint		tl,tr,bl,br;
	
	tl.Set(0,0);
	tr.Set(width, 0);
	bl.Set(0,height);
	br.Set(width, height);
	
	drawer->SetHighColor(kSHADOW);
	drawer->StrokeLine(bl,br);
	drawer->StrokeLine(br,tr);
	drawer->SetHighColor(kHIGHLIGHT);
	drawer->StrokeLine(tl,tr);
	drawer->StrokeLine(tl,bl);
	
	// draw graph
	DrawGraph(width, height);
	
	// I hate it when I forget to Sync()
	drawer->Sync();
	offscreen->Unlock();
	
	// blit the bitmap to the screen
	win->Lock();
	DrawBitmapAsync(offscreen);
	win->Unlock();
}

/******************************************************************
 * GraphView::UpdateTimesteps()
 *****************************************************************/
void GraphView::UpdateTimesteps(double val, bool truth)
{
	// if it's greater than 1, reset it back to 1
	if(val > 1.0) {
		val = 1.0;
	}
	
	if(pos == kTIMESTEPS) {
		// reset pos
		pos--;
		
		// scoot values down
		for(int i = 0; i < pos; i++) {
			timestep[i] = timestep[i+1];
			operating[i] = operating[i+1];
		}
	}
	
	// fill in new value
	timestep[pos] = val;
	operating[pos] = truth;
	
	// increment position
	pos++;
	
	// show data to user
	Draw(bounds);
}

/******************************************************************
 * GraphView::DrawGraph()
 *****************************************************************/
void GraphView::DrawGraph(float, float)
{
}

