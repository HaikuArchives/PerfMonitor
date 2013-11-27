/*******************************************************************
 * PerfWindow.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "PerfView.h"
#include "RAMView.h"
#include "PerfWindow.h"
#include "ToggleView.h"
#include <fcntl.h>
#include <unistd.h>

/******************************************************************
 * PerfWindow::PerfWindow()
 *****************************************************************/
PerfWindow::PerfWindow(BRect bound, const char *title)
	: BWindow(bound, title, B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	ToggleView		*perf;
	system_info		sysInfo;
	BRect			rect;
	int				offset, numViews;
	char			label[128];
		
	Lock();
	
	bound.OffsetTo(B_ORIGIN);
	
	get_system_info(&sysInfo);
	numViews = sysInfo.cpu_count + 1; // add one for RAMView
	
	// determine rect size and offset based on number of CPUs
	rect = bound;
	rect.bottom = rect.bottom - (numViews - 1);
	rect.bottom /= numViews;
	offset = rect.bottom + 1;
	
	// create a ToggleView for each processor
	for(int cpu = 0; cpu < sysInfo.cpu_count; cpu++) {
		sprintf(label, "CPU %d", cpu);
		perf = new ToggleView(rect, label, cpu);
		AddChild(perf);
		rect.OffsetBy(0, offset);
	}
	
	// create a ToggleView for RAM
	strcpy(label, "RAM utilization");
	
	perf = new ToggleView(rect, label, -1);
	AddChild(perf);
	
	// pulse once a second
	SetPulseRate(kPULSERATE);
	
	Unlock();
}

/******************************************************************
 * PerfWindow::SetGraphStates()
 *****************************************************************/
void PerfWindow::SetGraphStates(int32 *states, int num)
{
	for(int i = 0; i < num; i++) {
		if(states[i] == HIDDEN) {
			BMessage	*msg = new BMessage(mRESIZE);
			msg->AddInt32("state", HIDDEN);
			PostMessage(msg, (ToggleView *) ChildAt(i));
		}
	}
}

/******************************************************************
 * PerfWindow::MessageReceived()
 *****************************************************************/
void PerfWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
		case mRESIZE:
			long	state;
			state = msg->FindInt32("state");
			
			// see if we're hiding or showing
			if(state == HIDDEN) {
				ResizeBy(0, -kGRAPHSIZE);
			}
			else {
				ResizeBy(0, kGRAPHSIZE);
			}
			
			// adjust the view positions in the window
			RepositionViews();
	}
}

/******************************************************************
 * PerfWindow::RepositionViews()
 *****************************************************************/
void PerfWindow::RepositionViews()
{
	BView	*child;
	BPoint	tl;
	
	tl.Set(0,0);
	
	// move each child view so that it is immediately below the view above it
	for(long i = 0; i < CountChildren(); i++) {
		child = ChildAt(i);
		child->MoveTo(tl);
		tl.y += (child->Bounds().bottom + 1);
	}
}

/******************************************************************
 * PerfWindow::QuitRequested()
 *****************************************************************/
bool PerfWindow::QuitRequested()
{
	BPath		path;
	BPoint		lefttop;
	int	     	ref;
	int32		numGraphs, state;
	
	// remember window location
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
		path.Append("perfmonitor_settings");
		ref = creat(path.Path(), 0777);
		if (ref >= 0) {
			lefttop = Frame().LeftTop();
			write(ref, (char *)&lefttop, sizeof(BPoint));
			
			numGraphs = CountChildren();
			write(ref, (char *)&numGraphs, sizeof(int32));
			
			for(int i = 0; i < numGraphs; i++) {
				state = ((ToggleView *) ChildAt(i))->GetState();
				write(ref, (char *)&state, sizeof(int32));
			}
			
			close(ref);
		}
	}
	
	// tell the app to quit
	be_app->PostMessage(B_QUIT_REQUESTED);
	return TRUE;
}