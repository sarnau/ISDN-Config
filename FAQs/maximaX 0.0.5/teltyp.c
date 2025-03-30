#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget teltypdlg;

void displayTeleTyp(Widget mother)
{
	char which[10];
	int dummy=0;
	int port;
	int i=0;

	if (!teltypdlg) {
        teltypdlg=XmtBuildDialog(mother, "teltyp_dlg",
                NULL, 0);
        if (!teltypdlg) exit(1);
    }

	port=all->absel;

	setItemCaptions(teltypdlg,"*tln","Ruf #",0,7);

	setChooserState(teltypdlg,"*tln0",(int)all->tlnBerechtigung[port].Geraetetyp[0]);
	setChooserState(teltypdlg,"*tln1",(int)all->tlnBerechtigung[port].Geraetetyp[1]);
	setChooserState(teltypdlg,"*tln2",(int)all->tlnBerechtigung[port].Geraetetyp[2]);
	setChooserState(teltypdlg,"*tln3",(int)all->tlnBerechtigung[port].Geraetetyp[3]);
	setChooserState(teltypdlg,"*tln4",(int)all->tlnBerechtigung[port].Geraetetyp[4]);
	setChooserState(teltypdlg,"*tln5",(int)all->tlnBerechtigung[port].Geraetetyp[5]);
	setChooserState(teltypdlg,"*tln6",(int)all->tlnBerechtigung[port].Geraetetyp[6]);
	setChooserState(teltypdlg,"*tln7",(int)all->tlnBerechtigung[port].Geraetetyp[7]);

	if (XmtDialogDoSync(teltypdlg,NULL)) {
		for(i=0;i<9;i++){
			sprintf(which,"*tln%-d",i);
			getChooserState(teltypdlg,which,&dummy);
			all->tlnBerechtigung[port].Geraetetyp[i]=(unsigned char)dummy;
		}
		all->isdirty++;
	}

}
