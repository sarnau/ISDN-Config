#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget amtspermdlg;

void displayAmtsPerm(Widget mother)
{
	char which[10];
	int dummy=0;
	int i=0;
	Boolean port1isOK,port2isOK;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);

	if (!amtspermdlg) {
        amtspermdlg=XmtBuildDialog(mother, "amtsperm_dlg",
                NULL, 0);
        if (!amtspermdlg) exit(1);
    }


	for(i=0;i <= 9;i++){
		sprintf(which,"*tele%-d",i);
		setChooserState(amtspermdlg,which,(int)all->tlnBerechtigung[all->absel].Berechtigung[i]);
	}

	setItemCaptions(amtspermdlg,"*tele","Ruf #",0,7);
	if (port1isOK)
		setItemCaptions(amtspermdlg,"*tele","Ruf #",10,19);
	if (port2isOK)
		setItemCaptions(amtspermdlg,"*tele","Ruf #",20,29);
	disableChooser(amtspermdlg,"*tele8",port1isOK);
	disableChooser(amtspermdlg,"*tele9",port1isOK);

	for(i=10;i <= 19;i++){
		sprintf(which,"*tele%-d",i);
		setChooserState(amtspermdlg,which,(int)all->tlnBerechtigung[all->absel].Berechtigung[i]);
		disableChooser(amtspermdlg,which,port1isOK);
	}

	for(i=20;i <= 29;i++){
		sprintf(which,"*tele%-d",i);
		setChooserState(amtspermdlg,which,(int)all->tlnBerechtigung[all->absel].Berechtigung[i]);
		disableChooser(amtspermdlg,which,port1isOK);
	}

	if (XmtDialogDoSync(amtspermdlg,NULL)) {
		for(i=0;i < MAX_RUF_VAR_LEN;i++){
			sprintf(which,"*tele%-d",i);
			getChooserState(amtspermdlg,which,&dummy);
			all->tlnBerechtigung[all->absel].Berechtigung[i]=(unsigned char)dummy;
		}
		all->isdirty++;
	}
}
