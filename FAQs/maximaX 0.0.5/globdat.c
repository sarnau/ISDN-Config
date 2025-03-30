#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

XtResource glob_resources[] = {
{"uus", "uus", XmtRBuffer, XmtSizeOf(GlobAnlDatenType,UserToUser),
	XtOffsetOf(GlobAnlDatenType, UserToUser), XtRImmediate, NULL},
{"code", "code", XmtRBuffer, XmtSizeOf(GlobAnlDatenType,CodeNummer),
	XtOffsetOf(GlobAnlDatenType, CodeNummer), XtRImmediate, NULL},
{"lkz", "lkz", XmtRBuffer, XmtSizeOf(GlobAnlDatenType,Landeskennung),
	XtOffsetOf(GlobAnlDatenType, Landeskennung), XtRImmediate, NULL},
{"okz", "okz", XmtRBuffer, XmtSizeOf(GlobAnlDatenType,OrtsKennZif),
	XtOffsetOf(GlobAnlDatenType, OrtsKennZif), XtRImmediate, NULL},
{"hot", "hot", XmtRBuffer, XmtSizeOf(GlobAnlDatenType,Hotline),
	XtOffsetOf(GlobAnlDatenType, Hotline), XtRImmediate, NULL},
};

Widget globdatdlg;

void displayGlobDat(Widget mother)
{
	int dummy;
	char s[100];

	if (!globdatdlg) {
        globdatdlg=XmtBuildDialog(mother, "globdat_dlg",
                glob_resources, XtNumber(glob_resources));
        if (!globdatdlg) exit(1);
    }

	sprintf(s,"%d",(int)all->tlnRufvert[0].RwsTime);
	SetField(globdatdlg,"*rwst",s);
	setChooserState(globdatdlg,"*moh",(int)all->globAnlDaten.Moh);
	setChooserState(globdatdlg,"*amtamt",(int)all->globAnlDaten.AmtAmtWeitergabe);
	setChooserState(globdatdlg,"*autamt",(int)all->tlnRufvert[0].AutoAmtGlob);
	setChooserState(globdatdlg,"*ava",(int)all->globAnlDaten.AktAva);
	

	if (XmtDialogDoSync(globdatdlg,&all->globAnlDaten)) {
		getChooserState(globdatdlg,"*amtamt",&dummy);
    	all->globAnlDaten.AmtAmtWeitergabe=(unsigned char)dummy;
		getChooserState(globdatdlg,"*ava",&dummy);
    	all->globAnlDaten.AktAva=(unsigned char)dummy;
		getChooserState(globdatdlg,"*moh",&dummy);
    	all->globAnlDaten.Moh=(unsigned char)dummy;
		getChooserState(globdatdlg,"*autamt",&dummy);
    	all->tlnRufvert[0].AutoAmtGlob=(unsigned char)dummy;
		all->tlnRufvert[0].RwsTime=(char)atoi(GetField(globdatdlg,"*rwst"));
		all->isdirty++;
	}

}

