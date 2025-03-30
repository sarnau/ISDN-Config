#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern char *theConfBuf;
extern struct GesamtKonfig *all;

typedef struct MSNEntry{
	char msn0[9];
	char msn1[9];
	char msn2[9];
	char msn3[9];
	char msn4[9];
	char msn5[9];
	char msn6[9];
	char msn7[9];
	char msn8[9];
	char msn9[9];
} MSNEntryType;

XtResource msn_resources[] = {
{"msn0", "msn0", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn0), XtOffsetOf(MSNEntryType, msn0),
     XtRImmediate, NULL},
{"msn1", "msn1", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn1), XtOffsetOf(MSNEntryType, msn1),
     XtRImmediate, NULL},
{"msn2", "msn2", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn2), XtOffsetOf(MSNEntryType, msn2),
     XtRImmediate, NULL},
{"msn3", "msn3", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn3), XtOffsetOf(MSNEntryType, msn3),
     XtRImmediate, NULL},
{"msn4", "msn4", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn4), XtOffsetOf(MSNEntryType, msn4),
     XtRImmediate, NULL},
{"msn5", "msn5", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn5), XtOffsetOf(MSNEntryType, msn5),
     XtRImmediate, NULL},
{"msn6", "msn6", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn6), XtOffsetOf(MSNEntryType, msn6),
     XtRImmediate, NULL},
{"msn7", "msn7", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn7), XtOffsetOf(MSNEntryType, msn7),
     XtRImmediate, NULL},
{"msn8", "msn8", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn8), XtOffsetOf(MSNEntryType, msn8),
     XtRImmediate, NULL},
{"msn9", "msn9", XmtRBuffer,
     XmtSizeOf(MSNEntryType, msn9), XtOffsetOf(MSNEntryType, msn9),
     XtRImmediate, NULL},
};

void doShowConf(Widget mother);
void doSetupConf();
void doZuord();
void doMsns();

static Widget anltyp;
static Widget msns;
static Widget zuord;
MSNEntryType portmsns;
unsigned char AnzahlPorts=0;
int lastchosen=0;

/*
	Port1 immer extern , andere in-/extern...
*/
void checkPort()
{
	int port;

	getChooserState(anltyp,"*port",&port);
	disableChooserItem(anltyp,"*s0bus",0,(port>0));
	disableChooserItem(anltyp,"*s0bus",2,(port>0));
	disableChooserItem(anltyp,"*s0bus",3,(port>0));
}

void displayPortMode()
{
	unsigned char mode;
	unsigned char protokoll;
	unsigned char direction;
	int port;

	checkPort();
	getChooserState(anltyp,"*port",&port);

	mode=G_PORT_MODE(all->iconf[all->absel].Port[port].PortTyp);
	protokoll=G_PROTOKOLL(all->iconf[all->absel].Port[port].PortTyp);
	direction=G_S0(all->iconf[all->absel].Port[port].PortTyp);

	switch (direction) {
		case PORT_NICHT_KONFIGURIERT:
			direction=0;
			break;
		case PORT_DIRECTION_EXTERN:
			direction=1;
			break;
		case PORT_DIRECTION_INTERN:
			direction=2;
			break;
		case PORT_DIRECTION_QUER:
			direction=3;
			break;
	}
	setChooserState(anltyp,"*s0bus",direction);

	setChooserState(anltyp,"*anschl",(mode==PORT_MODE_PTMP) ? 0 : 1);
	set_active(anltyp,"*number",((direction==1) && 
									(mode==PORT_MODE_PTP)));
	disableChooser(anltyp,"*anschl",(direction==1));

	set_active(anltyp,"*msns_b",((mode==PORT_MODE_PTMP) &&
									(direction==1)));
	set_active(anltyp,"*zuord_b",((mode==PORT_MODE_PTMP) &&
									(direction==1)));

	disableChooser(zuord,"*app1",((all->iconf[all->absel].AnzPort>0) &&
					(G_S0(all->iconf[all->absel].Port[1].PortTyp)==PORT_DIRECTION_INTERN)));
	disableChooser(zuord,"*app2",((all->iconf[all->absel].AnzPort>0) &&
					(G_S0(all->iconf[all->absel].Port[2].PortTyp)==PORT_DIRECTION_INTERN)));
}

void setPortMode()
{
	int mode;
	int protokoll;
	int direction;
	int port;

	getChooserState(anltyp,"*port",&port);

	getChooserState(anltyp,"*anschl",&mode);
	getChooserState(anltyp,"*s0bus",&direction);
	
	switch (direction) {
		case 0:
			direction=PORT_NICHT_KONFIGURIERT;
			break;
		case 1:
			direction=PORT_DIRECTION_EXTERN;
			break;
		case 2:
			direction=PORT_DIRECTION_INTERN;
			break;
		case 3:
			direction=PORT_DIRECTION_QUER;
			break;
	}

	switch (mode) {
		case 0:
			mode=PORT_MODE_PTMP;
			break;
		case 1:
			mode=PORT_MODE_PTP;
			break;
	}
	protokoll=TLN_PROTOKOLL_EDSS1;

	if (direction!=PORT_NICHT_KONFIGURIERT)
		all->iconf[all->absel].Port[port].PortTyp=(unsigned char)(mode | protokoll | direction);
	else
		all->iconf[all->absel].Port[port].PortTyp=(unsigned char)0;

	displayPortMode();
}

unsigned long getTlnZuordnung(unsigned long *TlnZuordnung)
{
	char *chooserW[]={"*app0", "*app1","*app2",NULL};
	unsigned long dummy[3];
	
	getChooserState(zuord, chooserW[0],&dummy[0]);
	getChooserState(zuord, chooserW[1],&dummy[1]);
	getChooserState(zuord, chooserW[2],&dummy[2]);
	dummy[1]=dummy[1] << 10;
	dummy[2]=dummy[2] << 20;
	*TlnZuordnung=dummy[0]+dummy[1]+dummy[2];
	return(*TlnZuordnung);
}

void setTlnZuordnung(unsigned long TlnZuordnung)
{
	char *chooserW[]={"*app0", "*app1","*app2",NULL};
	int a,b,c;
	
	a=TlnZuordnung & 0x03FF;
	b=(int)((TlnZuordnung >> 10) & 0x03FF);
	c=(int)((TlnZuordnung >> 20) & 0x03FF);
	setChooserState(zuord, chooserW[0],a);
	if (getChooserSense(zuord,chooserW[1]))
		setChooserState(zuord, chooserW[1],b);
	else
		setChooserState(zuord, chooserW[1],0);
	if (getChooserSense(zuord,chooserW[2]))
		setChooserState(zuord, chooserW[2],c);
	else
		setChooserState(zuord, chooserW[2],0);
}

void displayAnlageTyp()
{
	unsigned long which=0;

	if (!zuord) {
		zuord=XmtBuildDialog(anltyp, "zuord_dlg",NULL, 0);
		if (!zuord) exit(1);
	}
	if (!msns) {
		msns=XmtBuildDialog(anltyp, "msns_dlg",
				msn_resources, XtNumber(msn_resources));
		if (!msns) exit(1);
	}
	getChooserState(anltyp,"*port",&which);
	if (which > all->iconf[all->absel].AnzPort)
		setChooserState(anltyp,"*port",0);
	disableChooserItem(anltyp,"*art",1,0);
	disableChooserItem(anltyp,"*art",2,0);
	disableChooser(zuord,"*app1",(all->iconf[all->absel].AnzPort>0));
	disableChooser(zuord,"*app2",(all->iconf[all->absel].AnzPort>1));
	disableChooserItem(anltyp,"*port",1,(all->iconf[all->absel].AnzPort>0));
	disableChooserItem(anltyp,"*port",2,(all->iconf[all->absel].AnzPort>1));

	displayPortMode();
}

void doZuord()
{
	unsigned long which=0;
	char wname[50];
	int i;

	getChooserState(anltyp,"*port",&which);
	lastchosen=which;
	for(i=0;i<10;i++) {
		disableChooserItem(zuord,"*msn",i,(all->iconf[all->absel].Port[which].MsnBlock[i][0]!='\0'));
		if (all->iconf[all->absel].Port[which].MsnBlock[i][0]!='\0') {
			sprintf(wname,"*msn.item%-d",i);
			set_labelname(zuord,wname,all->iconf[all->absel].Port[which].MsnBlock[i]);
		}
	}
	
    setItemLabels(zuord,"*app","Ruf #",0,0,7);
    if (hasPort(0,1))
        setItemLabels(zuord,"*app","Ruf #",1,0,9);
    if (hasPort(0,2))
        setItemLabels(zuord,"*app","Ruf #",2,0,9);

	getChooserState(zuord,"*msn",&lastchosen);
	setTlnZuordnung(all->iconf[all->absel].Port[which].Tln[lastchosen].ATlnZuordnung);
	XmtDialogDoSync(zuord,NULL);
	getTlnZuordnung(&all->iconf[all->absel].Port[which].Tln[lastchosen].ATlnZuordnung);
	lastchosen=0;
	setChooserState(zuord,"*msn",lastchosen);
	all->isdirty++;
}

void doMsns()
{
	unsigned long which=0;

	getChooserState(anltyp,"*port",&which);
	lastchosen=which;
	memcpy(&portmsns,&all->iconf[all->absel].Port[which].MsnBlock[0],sizeof(all->iconf[all->absel].Port[which].MsnBlock));
	if(XmtDialogDoSync(msns,&portmsns)) {
	    memcpy(&all->iconf[all->absel].Port[which].MsnBlock[0],&portmsns,sizeof(all->iconf[all->absel].Port[which].MsnBlock));
		all->isdirty++;
	}
}

void doMSNZuord (Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;
	unsigned long which=0;
	int dummy=ccb->item;
	
	getChooserState(anltyp,"*port",&which);
	getTlnZuordnung(&all->iconf[all->absel].Port[which].Tln[lastchosen].ATlnZuordnung);
	lastchosen=dummy;
	setTlnZuordnung(all->iconf[all->absel].Port[which].Tln[dummy].ATlnZuordnung);
}

void doS0Anschluss(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;

	all->iconf[all->absel].AnzPort=(unsigned char)ccb->item;
	
	displayAnlageTyp();
}

void doAnlTyp (Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;

	all->iconf[all->absel].AnzPort=(unsigned char)ccb->item;
	
	displayAnlageTyp();
}

void doShowConf(Widget mother)
{
		if (!anltyp) {
			anltyp=XmtBuildDialog(mother,
				"anltyp_dlg",NULL,
				0);
			if (!anltyp) exit(1);
		}
	lastchosen=0;
	setChooserState(anltyp,"*typ",(int)all->iconf[all->absel].AnzPort);
	displayAnlageTyp();
	XmtDialogDoSync(anltyp,NULL);
}

void doSetupConf()
{
	XmtVaRegisterCallbackProcedures(
		"doZuord", doZuord, XmtRCallbackUnused,
		"doMsns", doMsns, XmtRCallbackUnused,
		"doAnlTyp",doAnlTyp,XmtRCallbackUnused,
		"doMSNZuord",doMSNZuord,XmtRCallbackUnused,
		"checkPort",checkPort,XmtRCallbackUnused,
		"displayPortMode",displayPortMode,XmtRCallbackUnused,
		"setPortMode",setPortMode,XmtRCallbackUnused,
		NULL );
}
