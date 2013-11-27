/*******************************************************************
 * main.cpp
 * written by Steve Sprang
 * Copyright © 1997 Steve Sprang
 * <sprang@andrew.cmu.edu>
 ******************************************************************/
 
#include "PerfMonitor.h"
#include "PerfWindow.h"
#include <fcntl.h>
#include <unistd.h>

const char *app_signature = "application/x-be-executable";

// an Application class definition
class PApplication : public BApplication {
	public:
						PApplication();
		virtual void	AboutRequested();
};

/******************************************************************
 * main()
 *****************************************************************/
main()
{
	PApplication	*app;
	
	app = new PApplication();
	
	app->Run();
	delete(app);
	
	return 0;
}

/******************************************************************
 * PApplication::PApplication()
 *****************************************************************/
PApplication::PApplication()
	: BApplication(app_signature)
{	
	PerfWindow		*win;
	BPath			path;
	BRect			bound;
	BPoint			winLoc;
	system_info		sysInfo;
	int				numViews, ref;
	int32			numGraphs, *state;
	
	// create window bound based on number of CPUs
	get_system_info(&sysInfo);
	numViews = sysInfo.cpu_count + 1; // add one for RAMView
	bound.Set(0,0, kWIDTH, numViews * kHEIGHT + (numViews-1));
	win = new PerfWindow(bound, "PerfMonitor v1.2");
	
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		path.Append("perfmonitor_settings");
		ref = open(path.Path(), O_RDONLY);
		if (ref >= 0) {
			read(ref, (char *)&winLoc, sizeof(winLoc));
			win->MoveTo(winLoc);

			BRect frame = win->Frame();
			frame.InsetBy(-4, -4);
			if (!frame.Intersects(BScreen(win).Frame())) {
				// it's not visible so reposition.
				win->MoveTo(200, 100);
			}
			
			read(ref, (char *)&numGraphs, sizeof(int32));
			
			// should always be true (unless disk or settings file is moved to a new machine)
			// if, by chance, they differ, do nothing special
			if(numGraphs == numViews) { 
				state = new int32[numGraphs];
				for(int i = 0; i < numGraphs; i++) {
					read(ref, (char *)&(state[i]), sizeof(int32));
				}
				win->SetGraphStates(state, numGraphs);
				delete state;
			}
			close(ref);
		}
		else {	
			win->MoveTo(200, 100);
		}
	}
	win->Show();
}

/******************************************************************
 * PerfWindow::AboutRequested() : no way to get here currently
 *****************************************************************/
void PApplication::AboutRequested()
{
	BAlert	*about;
	
	char	string[240];
	
	strcpy(string, "PerfMonitor v1.2\nby Steve Sprang\nCopyright © 1997\n");
	strcat(string, "All rights reserved.\n\ne-mail: sprang@andrew.cmu.edu");
	
	about = new BAlert("", string, "OK");
	about->Go();
}

