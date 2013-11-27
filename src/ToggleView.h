/*******************************************************************
 * ToggleView.h
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#ifndef _TOGGLEVIEW_H
#define _TOGGLEVIEW_H

class GraphView;

class IndicatorView : public BView {
	public:
						IndicatorView(BRect, int, BView *);
		virtual void	Draw(BRect);
		virtual void	MouseDown(BPoint);
				int32	GetState();
				void	ToggleState();
				
		private:
			int32		state;
			BRect		bounds;
			BView		*owner;
};

/***/

class ToggleView : public BView {
	public:
						ToggleView(BRect, const char *, int);
		virtual void	MessageReceived(BMessage *msg);
				int32	GetState();
				void	ToggleState();	
	private:
			BRect			bounds;
			BStringView		*labelv;
			IndicatorView	*indicator;
			GraphView		*graph;
};

#endif