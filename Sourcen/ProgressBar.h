// ProgressBar.h#pragma once#include "SharewareFlags.h"#if NEED_PROGRESS_BAR// ProgressBarclass ProgressBar {	DialogPtr	dp;	long		max;	long		value;	long		aktval;	// Progressbar an "value" anpassen	void		Update();public:	ProgressBar(short dlogID, short index, long size);	~ProgressBar() { if(dp) DisposeDialog(dp); }	void		Delta(long delta) { value += delta; Update(); };	void		Set(long val) { value = val; Update(); };	void		SetMax(long iMax) { max = iMax; };	void		SetInfo(ConstStr255Param inStr);};#endif