#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

XtResource rnz_resources[] = {
{"hptnum", "hptnum", XmtRBuffer, XmtSizeOf(RufNrZuordType,HauptAnl),
	XtOffsetOf(RufNrZuordType, HauptAnl), XtRImmediate, NULL},
{"num0", "num0", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[0]), XtRImmediate, NULL},
{"num1", "num1", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[1]), XtRImmediate, NULL},
{"num2", "num2", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[2]), XtRImmediate, NULL},
{"num3", "num3", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[3]), XtRImmediate, NULL},
{"num4", "num4", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[4]), XtRImmediate, NULL},
{"num5", "num5", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[5]), XtRImmediate, NULL},
{"num6", "num6", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[6]), XtRImmediate, NULL},
{"num7", "num7", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[7]), XtRImmediate, NULL},
{"num8", "num8", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[8]), XtRImmediate, NULL},
{"num9", "num9", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[9]), XtRImmediate, NULL},
{"num10", "num10", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[10]), XtRImmediate, NULL},
{"num11", "num11", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[11]), XtRImmediate, NULL},
{"num12", "num12", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[12]), XtRImmediate, NULL},
{"num13", "num13", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[13]), XtRImmediate, NULL},
{"num14", "num14", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[14]), XtRImmediate, NULL},
{"num15", "num15", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[15]), XtRImmediate, NULL},
{"num16", "num16", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[16]), XtRImmediate, NULL},
{"num17", "num17", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[17]), XtRImmediate, NULL},
{"num18", "num18", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[18]), XtRImmediate, NULL},
{"num19", "num19", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[19]), XtRImmediate, NULL},
{"num20", "num20", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[20]), XtRImmediate, NULL},
{"num21", "num21", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[21]), XtRImmediate, NULL},
{"num22", "num22", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[22]), XtRImmediate, NULL},
{"num23", "num23", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[23]), XtRImmediate, NULL},
{"num24", "num24", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[24]), XtRImmediate, NULL},
{"num25", "num25", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[25]), XtRImmediate, NULL},
{"num26", "num26", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[26]), XtRImmediate, NULL},
{"num27", "num27", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[27]), XtRImmediate, NULL},
{"num28", "num28", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[28]), XtRImmediate, NULL},
{"num29", "num29", XmtRBuffer, XmtSizeOf(RufNrZuordType,Rufnummer[0]),
	XtOffsetOf(RufNrZuordType, Rufnummer[19]), XtRImmediate, NULL},
};

Widget rnzdlg;

void
disableRNZnums()
{
	Boolean port1isOK;
	Boolean port2isOK;

	port1isOK=hasPort(0,1);
	port2isOK=hasPort(0,2);

	set_active(rnzdlg,"*num10",port1isOK);
	set_active(rnzdlg,"*num11",port1isOK);
	set_active(rnzdlg,"*num12",port1isOK);
	set_active(rnzdlg,"*num13",port1isOK);
	set_active(rnzdlg,"*num14",port1isOK);
	set_active(rnzdlg,"*num15",port1isOK);
	set_active(rnzdlg,"*num16",port1isOK);
	set_active(rnzdlg,"*num17",port1isOK);
	set_active(rnzdlg,"*num18",port1isOK);
	set_active(rnzdlg,"*num19",port1isOK);

	set_active(rnzdlg,"*num20",port2isOK);
	set_active(rnzdlg,"*num21",port2isOK);
	set_active(rnzdlg,"*num22",port2isOK);
	set_active(rnzdlg,"*num23",port2isOK);
	set_active(rnzdlg,"*num24",port2isOK);
	set_active(rnzdlg,"*num25",port2isOK);
	set_active(rnzdlg,"*num26",port2isOK);
	set_active(rnzdlg,"*num27",port2isOK);
	set_active(rnzdlg,"*num28",port2isOK);
	set_active(rnzdlg,"*num29",port2isOK);
}
void
setHptMode(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;

	int dummy=ccb->item;

	all->rufNrZuord[all->absel].HauptAnlNahFern=(unsigned char)dummy+1;
	set_active(rnzdlg,"*hptnum",(all->rufNrZuord[all->absel].HauptAnlNahFern==3));
}

void doDisplayRNZ(Widget mother)
{
	if (!rnzdlg) {
        rnzdlg=XmtBuildDialog(mother, "rufzuord_dlg",
                rnz_resources, XtNumber(rnz_resources));
        if (!rnzdlg) exit(1);
    }
	disableRNZnums();
	setChooserState(rnzdlg,"*hpttyp",(int)all->rufNrZuord[all->absel].HauptAnlNahFern-1);
	set_active(rnzdlg,"*hptnum",(all->rufNrZuord[all->absel].HauptAnlNahFern==3));
	if (XmtDialogDoSync(rnzdlg,&all->rufNrZuord[all->absel])) {
		all->isdirty++;
	}

}

void
doSetupRNZ()
{
    XmtVaRegisterCallbackProcedures(
          "setHptMode",   setHptMode,   XmtRCallbackUnused,
          NULL );
}
