/*** *	WindowsGlobal.c * *	einige zus�tzliche Windowfunktionen, ohne Bezug zur Window-Library * *	�1991 C-Lab, Markus Fritze ***/#include "WindowsGlobal.h"#include "Utilities.h"#include "GlobalLib.h"#include "DialogLib.h"/*** *	H�he der Windowtitelzeile ermitteln ***/short		WindowTHeight(WindowPtr w){	return((**(((WindowPeek)w)->contRgn)).rgnBBox.top-(**(((WindowPeek)w)->strucRgn)).rgnBBox.top);}/*** *	neue DrawWindow() Routine, verschiebt das gesamte Window ***/void	NewDragWindow(WindowPtr w,Point pt,Rect *bounds){Point		OldPoint,NewPoint;Point		OffsetPoint;short	h,v;GrafPtr		savePort;		GetPort(&savePort);								// save the current port	SetPort(w);										// set coordinate system to window	OldPoint = NewPoint = pt;						// make local copies of current mouse	OffsetPoint.h = w->portRect.left;				// stuff top-left of window in point	OffsetPoint.v = w->portRect.top;	LocalToGlobal(&OffsetPoint);					// convert too global coordinates	OffsetPoint.h = pt.h - OffsetPoint.h;			// this offset is from window to mouse	OffsetPoint.v = pt.v - OffsetPoint.v;	while (StillDown()) {							// is mouse still down ?		GetMouse(&NewPoint);		LocalToGlobal(&NewPoint);					// whe now ?		if (PtInRect(NewPoint, bounds)) {			// respect application boundaries			if ((NewPoint.h != OldPoint.h) || (NewPoint.v != OldPoint.v)) {				h = NewPoint.h - OffsetPoint.h;		// calculate new position of window				v = NewPoint.v - OffsetPoint.v;		// using the offset of mouse-window				MoveWindow(w,h,v,FALSE);			// just do it				OldPoint = NewPoint;				// save new 'old' location			}		}	}	SetPort(savePort);								// restore coordinate system}/*** *	BringBehind * *	Move a window from far back to right behind another window ***/void	BringBehind(WindowPtr w,WindowPtr behindWindow){GrafPtr		savePort;	// Current portPoint		corner;		// Top left of visible region	if (!behindWindow) {			// hinter �kein� Window?		BringToFront((WindowPtr)w);	// dann Window ganz nach vorne		return;	}	GetPort(&savePort);				// Save current port	SetPort(w);				// Use this window's port	// Save portion of window which is originally visible	CopyRgn(w->visRgn,gUtilRgn);	// Adjust the window's plane	SendBehind(w,behindWindow);	//We must draw the newly exposed portion of the window. Find the	//difference between the present structure region and what was	//originally visible. Before doing this, we must convert the	//originally visible region to global coords.	corner = topLeft((**gUtilRgn).rgnBBox);	LocalToGlobal(&corner);	OffsetRgn(gUtilRgn, (corner.h - (**gUtilRgn).rgnBBox.left),						(corner.v - (**gUtilRgn).rgnBBox.top));	// Now we can difference the regions.  Save space by putting the	// result back in theRgn.  Before calling DiffRgn, theRgn is the	// originally visible region.  Afterwards, theRgn is the newly	// exposed region of the window.	DiffRgn(((WindowPeek)w)->strucRgn, gUtilRgn, gUtilRgn);	PaintOne((WindowPeek)w, gUtilRgn); // Draw newly exposed region	// 	Since window has moved forward, we must adjust the visible	// regions of this window and those behind it.	CalcVisBehind((WindowPeek)w, ((WindowPeek)w)->strucRgn);	SetPort(savePort);					// Restore the original port}/*** *	obere linke Ecke des Windows in globalen Koordinaten zur�ckgeben. Es wird *	dabei NUR die Content-Region beachtet! (nicht die DragRegion) ***/Point	GetGlobalTopLeft(WindowPtr window){GrafPtr		oldPort;Point		globalPt;		GetPort(&oldPort);						// alten Port retten	SetPort(window);						// Windowport setzen	globalPt = TopLeft(window->portRect);	LocalToGlobal(&globalPt);				// in globale Koordinaten umrechnen	SetPort(oldPort);						// Port zur�cksetzen	return(globalPt);}// This procedure is used to get the rectangle that surrounds the entire// structure of a window.  This is true whether or not the window is visible.// If the window is visible, then it is a simple matter of using the bounding// rectangle of the structure region.  If the window is invisible, then the// strucRgn is not correct.  To make it correct, then window has to be moved// way off the screen and then made visible.  This generates a valid strucRgn,// although it is valid for the position that is way off the screen.  It still// needs to be offset back into the original position.  Once the bounding// rectangle for the strucRgn is obtained, the window can then be hidden again// and moved back to its correct location.  Note that ShowHide is used,// instead of ShowWindow and HideWindow.  HideWindow can change the plane of// the window.  Also, ShowHide does not affect the hiliting of windows.Rect	GetWindowStructureRect(WindowPtr window){#define kOffscreenLoc 0x4000GrafPtr		oldPort;Rect		structureRect;Point	windowLoc;		if (((WindowPeek)window)->visible)	// Window sichtbar?		structureRect = (*(((WindowPeek)window)->strucRgn))->rgnBBox;	// ja!	else {		GetPort(&oldPort);		SetPort(window);		windowLoc = GetGlobalTopLeft(window);		MoveWindow(window,windowLoc.h,kOffscreenLoc,FALSE);		ShowHide(window,TRUE);		structureRect = (*(((WindowPeek)window)->strucRgn))->rgnBBox;		ShowHide(window,FALSE);		MoveWindow(window,windowLoc.h,windowLoc.v,FALSE);		SetPort(oldPort);		OffsetRect(&structureRect,0,windowLoc.v-kOffscreenLoc);	}	return(structureRect);}/*** *	Content-Rechteck eines Windows ermitteln ***/Rect	GetWindowContentRect(WindowPtr window){WindowPtr	oldPort;Rect		contentRect;	GetPort(&oldPort);	SetPort(window);	contentRect = window->portRect;	LocalToGlobalRect(&contentRect);	SetPort(oldPort);	return(contentRect);}/*** *	Test, ob das Window zur Applikation geh�rt. �windowKind� ist bei System- *	Windows z.B. DAs negativ und �windowKind� kleiner �userKind� sind f�r *	Apple reserviert (mit Ausnahme von �dialogKind�) ***/Boolean	IsAppWindow(WindowPtr window){short		i;	if (window) {		i = ((WindowPeek)window)->windowKind;		if ((i >= userKind)||(i == dialogKind))			return(TRUE);	}	return(FALSE);}/*** *	Test, ob das Window zu einem DA geh�rt. ***/Boolean	IsDAWindow(WindowPtr window){	if (window)				// DAs haben ein negatives �windowKind�		if (((WindowPeek)window)->windowKind < 0)			return(TRUE);	return(FALSE);}/*** *	Ist das Window eine Dialogbox? ***/Boolean	IsDialogWindow(WindowPeek window){	if (window) {		if	(window->windowKind == dialogKind)			return(TRUE);	}	return(FALSE);}/*** *	Rechteck des Hauptbildschirms ermitteln ***/Rect	GetMainScreenRect(void){GDHandle	mainDevice;GrafPtr			mainPort;	if (gQDVersion > kQDOriginal) {		mainDevice = GetMainDevice();		return((*mainDevice)->gdRect);	} else {		GetWMgrPort(&mainPort);		return(mainPort->portRect);	}}/*** *	Device ermitteln, auf dem der gr��te Teil des Rechtecks liegt. ***/GDHandle	GetRectDevice(Rect globalRect){long		area;long		maxArea;GDHandle	device;GDHandle	deviceToReturn;Rect			intersection;	deviceToReturn = GetMainDevice();			// Use as default choice.	maxArea = 0;	for (device = GetDeviceList(); device; device = GetNextDevice(device)) {		if (TestDeviceAttribute(device, screenDevice)		  && TestDeviceAttribute(device, screenActive)		  && SectRect(&globalRect, &((*device)->gdRect), &intersection)) {			area = (intersection.right - intersection.left) *				   (intersection.bottom - intersection.top);			if (area > maxArea) {				deviceToReturn = device;				maxArea = area;			}		}	}	return(deviceToReturn);}/*** *	Rechteck vom Device ermitteln, auf dem der gr��te Teil des Rechtecks liegt. ***/Rect	GetRectDeviceRect(Rect globalRect){	if (gQDVersion > kQDOriginal)		return((*GetRectDevice(globalRect))->gdRect);	else		return(GetMainScreenRect());}/*** *	Rechteck vom Device ermitteln, auf dem der gr��te Teil des Windows liegt. ***/GDHandle	GetWindowDevice(WindowPtr window){	return(GetRectDevice(GetWindowStructureRect(window)));}/*** *	Device ermitteln, auf dem der gr��te Teil des Windows liegt. ***/Rect	GetWindowDeviceRect(WindowPtr window){	if (gQDVersion > kQDOriginal)		return((*GetWindowDevice(window))->gdRect);	else		return(GetMainScreenRect());}/*** *	Das Rechteck des Device zur�ckgeben, auf dem der gr��te Teil des Windows *	liegt. Wenn auf dem Device die Men�zeile dargestellt wird, so wird diese *	vorher abgezogen. ***/Rect	GetWindowDeviceRectNMB(WindowPtr window){	Rect		deviceRect, tempRect;	deviceRect = GetWindowDeviceRect(window);	// Rechteck vom Window-Device	tempRect = GetMainScreenRect();				// Rechteck vom Main-Screen-Device	if (EqualRect(&deviceRect, &tempRect))		// der gleiche Screen?		deviceRect.top += GetMBarHeight();		// dann die Men�leiste abziehen	return(deviceRect);							// Rechteck zur�ckgeben}/* Center a window within a particular device.  The device to center the window** within is determined by passing a related window.  This allows related** windows to be kept on the same device.  This is useful if an alert related** to a specific window, for example. */Rect	CenterWindow(WindowPtr window,WindowPtr relatedWindow){WindowPtr	whichDevice;Rect			deviceRect, oldWindowRect, newWindowRect, contentRect;	if (!(whichDevice = relatedWindow)) whichDevice = window;		/* If we have a window to center against, use the device for that window,		** else use the device for the window that is getting centered. */	deviceRect = GetWindowDeviceRectNMB(whichDevice);		// We now have the rectangle of the device we want to center within.	oldWindowRect = newWindowRect = GetWindowStructureRect(window);	PositionRectInRect(deviceRect, &newWindowRect, FixRatio (1,2),					   FixRatio (1,3));		/* Figure out the new window strucRect so we can compare it against		** the old strucRect.  This will tell us how much to move the window. */	contentRect = GetWindowContentRect(window);		// Get where the window is now.	OffsetRect(&contentRect, newWindowRect.left - oldWindowRect.left,							newWindowRect.top  - oldWindowRect.top);		// Calculate the new content rect.	MoveWindow(window, contentRect.left, contentRect.top, false);		// Move the window to the new location.	return(contentRect);}// This algorithm for staggering windows does quite a good job.  It also is// quite gnarly.  Here's the deal:// There are pre-designated positions that we will try when positioning a// window.  These slots will be tried from the upper-left corner towards the// lower-right corner.  If there are other windows in that slot, then we will// consider that slot taken, and proceed to the next slot.  A slot is// determined to be taken by checking a point with a slop area.  This slop// area is diamond-shaped, not simply rectangular.  If there is no other// visible window with an upper-left corner within the slopt diamond, then// we are allowed to position our window there.// The above rule holds true unless this forces the window to be partly// off the screen.  If the window ends up partly off the screen, then we try// a new diagonal just below the first diagonal we tried.  We keep trying// lower and lower diagonals until we find a spot for the window, or the// diagonal doesn't fit on the screen at all.  If the diagonal doesn't fit,// then we try diagonals to the right of the first diagonal.  If even this// doesn't work, then we give up and put the window in the original spot// we tried.Rect	StaggerWindow(WindowPtr window,WindowPtr relatedWindow){WindowPtr	whichDevice, staggerFromWindow;Rect		deviceRect, oldWindowRect, newWindowRect, slot1;Rect		testRect, contentRect, staggerFromRect;Point		delta, absdelta;Boolean		contained, vertTry;short		diamondSize, diagNum, tryNum;	if (!(whichDevice = relatedWindow)) whichDevice = window;		// If we have a window to stagger from, use the device for that window,		// else use the device for the window that is getting staggered.	deviceRect = GetWindowDeviceRectNMB(whichDevice);		// We now have the rect of the device we want to stagger within.	oldWindowRect = GetWindowStructureRect(window);	newWindowRect.top    = deviceRect.top  + kStartPtV;	newWindowRect.left   = deviceRect.left + kStartPtH;	newWindowRect.bottom = newWindowRect.top  + oldWindowRect.bottom - oldWindowRect.top;	newWindowRect.right  = newWindowRect.left + oldWindowRect.right  - oldWindowRect.left;		// We now have a new rect for the first window position slot.	slot1 = newWindowRect;		// We keep this slot in case we find no acceptable slots.  If we		// don't find an acceptable one, we will use this one anyway.	diamondSize = (kStaggerH < kStaggerV) ? kStaggerH : kStaggerV;	for (diagNum = 0, vertTry = true;;) {		for (tryNum = 0;; ++tryNum) {			SectRect(&newWindowRect, &deviceRect, &testRect);			if (!(contained = EqualRect(&newWindowRect, &testRect))) break;				// Break if the slot we are testing went off the device.			for (staggerFromWindow = FrontWindow();				 staggerFromWindow;				 staggerFromWindow =					(WindowPtr)((WindowPeek)staggerFromWindow)->nextWindow			) {				if (!((WindowPeek)staggerFromWindow)->visible) continue;					// This window is invisible.  Staggering from an invisible					// window is going to confuse the user, so don't do it.				testRect = GetWindowDeviceRectNMB(staggerFromWindow);				if (!EqualRect(&testRect, &deviceRect)) continue;					// This window doesn't belong to the device we are trying to					// stagger on, so skip it and go to the next window.				staggerFromRect = GetWindowStructureRect(staggerFromWindow);				delta.v = staggerFromRect.top  - newWindowRect.top;				delta.h = staggerFromRect.left - newWindowRect.left;				if ((absdelta.v = delta.v) < 0) absdelta.v = -delta.v;				if ((absdelta.h = delta.h) < 0) absdelta.h = -delta.h;				if ((absdelta.h + absdelta.v) < diamondSize) {					if ((delta.h + delta.v) > 0)						OffsetRect(&newWindowRect, delta.h, delta.v);							// If the window that took our slot is closer to							// the lower-right corner than we are, then use							// this window's location as the basis for the							// slots from now on.  This will align new windows							// with previous windows that are not gridded to							// the default slot positions.  The check for > 0							// is necessary to prevent bouncing between two							// existing windows.  This check guarantees that							// we are progressing with the evaluation.					break;						// Break because this slot is already used.					}			}			if (!staggerFromWindow) break;				// If the window pointer is nil, then we tried all the windows				// and none of them occupied this slot.  This means that the				// slot is available for the new window.				OffsetRect(&newWindowRect, kStaggerH, kStaggerV);				// Since this slot was taken, try the next slot and go through				// the window list again.		}		if (contained) break;		newWindowRect = slot1;		if (!tryNum) {			if (!vertTry) break;		// Nothing works.  No spots at all.			vertTry = FALSE;			// Try across for the next pass.			diagNum = 0;		}		++diagNum;		if (vertTry) OffsetRect(&newWindowRect, 0, diagNum * kStaggerV);		else         OffsetRect(&newWindowRect, diagNum * kStaggerH, 0);	}	contentRect = GetWindowContentRect(window);		// Get where the window is now.	OffsetRect(&contentRect,newWindowRect.left-oldWindowRect.left,							newWindowRect.top-oldWindowRect.top);		// Calculate the new content rect.	MoveWindow(window,contentRect.left,contentRect.top,FALSE);		// Move the window to the new location.	return(contentRect);}// Zoom the window to the size appropriate for the device that contains the// most of the window.  An additional feature is that you can state the// maximum that a window should be zoomed, either horizontally or vertically.// If you pass in a maximum of 0 for the zoom for either direction, then that// direction will be zoomed to fit the device.void	ZoomToWindowDevice(WindowPtr window,short maxWidth,short maxHeight,short zoomDir,Boolean front){GrafPtr		oldPort;Rect		contentRect, structureRect, deviceRect, newRect;short		width, height, dx, dy;	GetPort(&oldPort);	SetPort(window);	EraseRect(&window->portRect); 		// Recommended for cosmetic reasons.	// If there is the possibility of multiple gDevices, then we must check them to	// make sure we are zooming onto the right display device when zooming out.	if ((zoomDir == inZoomOut) && (gQDVersion > kQDOriginal)) {		contentRect	  = GetWindowContentRect(window);		structureRect = GetWindowStructureRect(window);		deviceRect	  = GetWindowDeviceRectNMB(window);		deviceRect.left   += (contentRect.left - structureRect.left + 2);		deviceRect.top    += (contentRect.top - structureRect.top + 2);		deviceRect.right  -= (structureRect.right - contentRect.right + 2);		deviceRect.bottom -= (structureRect.bottom - contentRect.bottom + 2);		newRect = deviceRect;		if (maxWidth)			if ((width = deviceRect.right - deviceRect.left) > maxWidth)				newRect.right = (newRect.left = contentRect.left) + maxWidth;		if (maxHeight)			if ((height = deviceRect.bottom - deviceRect.top) > maxHeight)				newRect.bottom = (newRect.top = contentRect.top) + maxHeight;		if ((dx = deviceRect.left - newRect.left) < 0)			if ((dx = deviceRect.right - newRect.right) > 0)				dx = 0;		if ((dy = deviceRect.top - newRect.top) < 0)			if ((dy = deviceRect.bottom - newRect.bottom) > 0)				dy = 0;		OffsetRect(&newRect, dx, dy);		(*(WStateDataHandle)(((WindowPeek)window)->dataHandle))->stdState = newRect;			// Set up the WStateData record for this window.	}	ZoomWindow(window,zoomDir,front);	SetPort(oldPort);}