/*******************************************************************
 * PerfWindow.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/

#ifndef _PERFWINDOW_H
#define _PERFWINDOW_H

class RAMView;

class PerfWindow : public BWindow {
	public:
						PerfWindow(BRect, const char *);
		virtual bool	QuitRequested();
		virtual void	MessageReceived(BMessage *);
				void	SetGraphStates(int32 *, int);
				void	RepositionViews();
};

#endif