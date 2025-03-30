#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

XtResource nah_resources[] = {
{"num0", "num0", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[0]), XtRImmediate, NULL},
{"num1", "num1", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[1]), XtRImmediate, NULL},
{"num2", "num2", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[2]), XtRImmediate, NULL},
{"num3", "num3", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[3]), XtRImmediate, NULL},
{"num4", "num4", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[4]), XtRImmediate, NULL},
{"num5", "num5", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[5]), XtRImmediate, NULL},
{"num6", "num6", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[6]), XtRImmediate, NULL},
{"num7", "num7", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[7]), XtRImmediate, NULL},
{"num8", "num8", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[8]), XtRImmediate, NULL},
{"num9", "num9", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[9]), XtRImmediate, NULL},
{"num10", "num10", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[10]), XtRImmediate, NULL},
{"num11", "num11", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[11]), XtRImmediate, NULL},
{"num12", "num12", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[12]), XtRImmediate, NULL},
{"num13", "num13", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[13]), XtRImmediate, NULL},
{"num14", "num14", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[14]), XtRImmediate, NULL},
{"num15", "num15", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[15]), XtRImmediate, NULL},
{"num16", "num16", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[16]), XtRImmediate, NULL},
{"num17", "num17", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[17]), XtRImmediate, NULL},
{"num18", "num18", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[18]), XtRImmediate, NULL},
{"num19", "num19", XmtRBuffer, XmtSizeOf(NahSperrBerType,Nahber[0]),
	XtOffsetOf(NahSperrBerType, Nahber[19]), XtRImmediate, NULL},
};

Widget nahberdlg;

void doDisplayNahBer(Widget mother)
{
	    if (!nahberdlg) {
        nahberdlg=XmtBuildDialog(mother, "nahber_dlg",
                nah_resources, XtNumber(nah_resources));
        if (!nahberdlg) exit(1);
    }
	setInitial(nahberdlg,"*num0");
	if (XmtDialogDoSync(nahberdlg,&all->nahSperrBer)) {
		all->kwzisdirty++;
	}

}

