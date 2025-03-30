#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

XtResource sperr_resources[] = {
{"num0", "num0", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[0]), XtRImmediate, NULL},
{"num1", "num1", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[1]), XtRImmediate, NULL},
{"num2", "num2", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[2]), XtRImmediate, NULL},
{"num3", "num3", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[3]), XtRImmediate, NULL},
{"num4", "num4", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[4]), XtRImmediate, NULL},
{"num5", "num5", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[5]), XtRImmediate, NULL},
{"num6", "num6", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[6]), XtRImmediate, NULL},
{"num7", "num7", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[7]), XtRImmediate, NULL},
{"num8", "num8", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[8]), XtRImmediate, NULL},
{"num9", "num9", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[9]), XtRImmediate, NULL},
{"num10", "num10", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[10]), XtRImmediate, NULL},
{"num11", "num11", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[11]), XtRImmediate, NULL},
{"num12", "num12", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[12]), XtRImmediate, NULL},
{"num13", "num13", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[13]), XtRImmediate, NULL},
{"num14", "num14", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[14]), XtRImmediate, NULL},
{"num15", "num15", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[15]), XtRImmediate, NULL},
{"num16", "num16", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[16]), XtRImmediate, NULL},
{"num17", "num17", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[17]), XtRImmediate, NULL},
{"num18", "num18", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[18]), XtRImmediate, NULL},
{"num19", "num19", XmtRBuffer, XmtSizeOf(NahSperrBerType,Sperrber[0]),
	XtOffsetOf(NahSperrBerType, Sperrber[19]), XtRImmediate, NULL},
};

Widget sperrdlg;

void doDisplaySperrBer(Widget mother)
{
	    if (!sperrdlg) {
        sperrdlg=XmtBuildDialog(mother, "sperrber_dlg",
                sperr_resources, XtNumber(sperr_resources));
        if (!sperrdlg) exit(1);
    }
	if (XmtDialogDoSync(sperrdlg,&all->nahSperrBer)) {
		all->kwzisdirty++;
	}

}

