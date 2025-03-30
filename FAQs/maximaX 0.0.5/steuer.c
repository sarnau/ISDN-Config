#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget steuerdlg;

XtResource steuer_resources[] = {
{"utou", "utou", XmtRBuffer, XmtSizeOf(SteuerKontaktType,UserToUser),
	XtOffsetOf(SteuerKontaktType, UserToUser), XtRImmediate, NULL},
{"rnr1", "rnr1", XmtRBuffer, XmtSizeOf(SteuerKontaktType,Rnr1),
	XtOffsetOf(SteuerKontaktType, Rnr1), XtRImmediate, NULL},
{"rnr2", "rnr2", XmtRBuffer, XmtSizeOf(SteuerKontaktType,Rnr2),
	XtOffsetOf(SteuerKontaktType, Rnr2), XtRImmediate, NULL},
{"rnr3", "rnr3", XmtRBuffer, XmtSizeOf(SteuerKontaktType,Rnr3),
	XtOffsetOf(SteuerKontaktType, Rnr3), XtRImmediate, NULL},
};

void
doSteuer()
{
}

void displaySteuer(Widget mother)
{
    Boolean port1isOK;
    Boolean port2isOK;
	char	s_start[10];
	char	s_end[10];
	int akt1,akt2,dummy;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);

	if (!steuerdlg) {
        steuerdlg=XmtBuildDialog(mother, "steuer_dlg",
                steuer_resources, XtNumber(steuer_resources));
        if (!steuerdlg) exit(1);
    }

	sprintf(s_start,"%04X",all->steuerKontakt[all->absel].StartZeit);
	sprintf(s_end  ,"%04X",all->steuerKontakt[all->absel].EndZeit);

	SetField(steuerdlg,"*start",s_start);
	SetField(steuerdlg,"*end",s_end);
	SetNumField(steuerdlg,"*ask",1,(int)all->steuerKontakt[all->absel].Ask1Zeit);
	SetNumField(steuerdlg,"*ask",2,(int)all->steuerKontakt[all->absel].Ask2Zeit);
	setChooserState(steuerdlg,"*akt1",(int)all->steuerKontakt[all->absel].Kontakt1);
	setChooserState(steuerdlg,"*akt2",(int)all->steuerKontakt[all->absel].Kontakt2);
	setChooserState(steuerdlg,"*auto",(int)all->steuerKontakt[all->absel].Autoanwahl);

	if (XmtDialogDoSync(steuerdlg,&all->steuerKontakt[all->absel])) {
		strcpy(s_start,(char *)GetField(steuerdlg,"*start"));
		strcpy(s_end,(char *)GetField(steuerdlg,"*end"));
		sscanf(s_start,"%04X",&all->steuerKontakt[all->absel].StartZeit);
		sscanf(s_end  ,"%04X",&all->steuerKontakt[all->absel].EndZeit);

		all->steuerKontakt[all->absel].Ask1Zeit=(short)GetNumField(steuerdlg,"*ask",1);
		all->steuerKontakt[all->absel].Ask2Zeit=(short)GetNumField(steuerdlg,"*ask",2);
		getChooserState(steuerdlg,"*akt1",&akt1);
		all->steuerKontakt[all->absel].Kontakt1=(short)akt1;
		getChooserState(steuerdlg,"*akt2",&akt2);
		all->steuerKontakt[all->absel].Kontakt2=(short)akt2;
		getChooserState(steuerdlg,"*auto",&dummy);
		all->steuerKontakt[all->absel].Autoanwahl=(short)dummy;
		all->isdirty++;
	}

}

void doSetupSteuer()
{
	XmtVaRegisterCallbackProcedures(
		"doSteuer",doSteuer,XmtRCallbackUnused,
		NULL );
}
