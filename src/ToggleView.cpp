/*******************************************************************
 * ToggleView.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "ToggleView.h"
#include "GraphView.h"
#include "PerfView.h"
#include "RAMView.h"

/******************************************************************
 * IndicatorView::IndicatorView()
 *****************************************************************/
IndicatorView::IndicatorView(BRect bound, int state, BView *owner)
	: BView(bound, "", B_FOLLOW_TOP, B_WILL_DRAW)
{
	this->state = state; // state: hidden or visible
	this->owner = owner;
	
	bounds = bound; // store bounds to speed access
	SetViewColor(kTOGGLEBGCOLOR);
}

/******************************************************************
 * IndicatorView::GetState()
 *****************************************************************/
int32 IndicatorView::GetState()
{
	return state;
}

/******************************************************************
 * IndicatorView::ToggleState()
 *****************************************************************/
void IndicatorView::ToggleState()
{
	state = !state;
	Invalidate(); // redraw
}

/******************************************************************
 * IndicatorView::MouseDown()
 *****************************************************************/
void IndicatorView::MouseDown(BPoint)
{
	ToggleState();
	
	// resize view and window accordingly
	BMessage 	*msg = new BMessage(mRESIZE);
	msg->AddInt32("state", state);
	Window()->PostMessage(msg, owner);
}

/******************************************************************
 * IndicatorView::Draw()
 *****************************************************************/
void IndicatorView::Draw(BRect)
{
	// draw a triangle to show toggle state
	BPoint	A,B,C;
	
	// hardwired triangle coordinates...
	// I thought about loading in two little bmaps, but that'd be sort of silly
	if(state == HIDDEN) {
		A.Set(3,0);
		B.Set(3,10);
		C.Set(8,5);
	}
	else { // must be VISIBLE
		A.Set(0,3);
		B.Set(10,3);
		C.Set(5,8);
	}
	SetHighColor(kTOGGLEBLUE);
	FillTriangle(A,B,C);
	SetHighColor(0,0,0);
	StrokeTriangle(A,B,C);
}

/******************************************************************
 * ToggleView::ToggleView()
 *****************************************************************/
ToggleView::ToggleView(BRect bound, const char *label, int which)
	: BView(bound, "", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect		rect;
	
	bounds = bound;
	bounds.OffsetTo(B_ORIGIN);
	
	// create a label
	rect.Set(0,0,bounds.Width(),kTOGGLESIZE);
	rect.OffsetBy(kTOGGLESIZE + 2, 0);
	labelv = new BStringView(rect, "", label);
	labelv->SetViewColor(kTOGGLEBGCOLOR);
	AddChild(labelv);
	
	// hardcoded rectangle size for indicator
	rect.Set(0,0,11,11);
	// position it so that everything looks dandy
	rect.OffsetBy(3,2);
	indicator = new IndicatorView(rect, VISIBLE, this);
	AddChild(indicator);
	
	// adjust bounds for adding GraphView
	rect = bounds;
	rect.top += kTOGGLESIZE;
	
	// decide whether to create a CPU graph or a RAM graph
	if(which >= 0) {
		graph = new PerfView(rect, which);
	}
	else {
		graph = new RAMView(rect);
	}
	AddChild(graph);
		
	SetViewColor(kTOGGLEBGCOLOR);
}

/******************************************************************
 * ToggleView::MessageReceived()
 *****************************************************************/
void ToggleView::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
		case mRESIZE:
			int32	state;
			state = msg->FindInt32("state");
			
			// see if we're hiding or showing the graph
			if(state == HIDDEN) {
				graph->Hide();
				ResizeBy(0, -(bounds.Height() - kTOGGLESIZE));
			}
			else {
				graph->Show();
				ResizeBy(0, (bounds.Height() - kTOGGLESIZE));
			}
			
			// make sure indicator matches actual state
			if(state != indicator->GetState()) {
				indicator->ToggleState();
			}
			
			// tell the window it better resize
			Window()->PostMessage(msg);
	}
}

/******************************************************************
 * ToggleView::GetState()
 *****************************************************************/
int32 ToggleView::GetState()
{
	return indicator->GetState();
}

/******************************************************************
 * ToggleView::ToggleState()
 *****************************************************************/
void ToggleView::ToggleState()
{
	indicator->ToggleState();
}

