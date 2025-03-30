#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget lcfdlg;

void
disableLCFnums()
{
	Boolean port1isOK;
	Boolean port2isOK;
	int i;

	port1isOK=hasPort(0,1);
	port2isOK=hasPort(0,2);

	for (i=0;i<10;i++) {
		SetNumField(lcfdlg,"*lcf",i,
				(int)all->tlnGebKonf[all->absel].UserFaktor[i]);

		set_activegrp(lcfdlg,"*lcf",i+10,port1isOK);
		if (port1isOK) {
			SetNumField(lcfdlg,"*lcf",i+10,
				(int)all->tlnGebKonf[all->absel].UserFaktor[i+10]);
		}

		set_activegrp(lcfdlg,"*lcf",i+20,port2isOK);
		if (port2isOK) {
			SetNumField(lcfdlg,"*lcf",i+20,
				(int)all->tlnGebKonf[all->absel].UserFaktor[i+20]);
		}
	}
	setItemCaptions(lcfdlg,"*lcf","Ruf #",0,29); 
}

void
readLCFnums()
{
	Boolean port1isOK;
	Boolean port2isOK;
	int i;

	port1isOK=hasPort(0,1);
	port2isOK=hasPort(0,2);

	for (i=0;i<10;i++) {
		all->tlnGebKonf[all->absel].UserFaktor[i]=(char)GetNumField(lcfdlg,"*lcf",i);
		if (port1isOK)
			all->tlnGebKonf[all->absel].UserFaktor[i+10]=
				(char)GetNumField(lcfdlg,"*lcf",i+10);
		if (port2isOK) 
			all->tlnGebKonf[all->absel].UserFaktor[i+20]=
				(char)GetNumField(lcfdlg,"*lcf",i+20);
	}
}

void doDisplayLCF(Widget mother)
{
	if (!lcfdlg) {
        lcfdlg=XmtBuildDialog(mother, "loccharge_dlg",
                NULL, 0);
        if (!lcfdlg) exit(1);
    }
	disableLCFnums();
	if (XmtDialogDoSync(lcfdlg,NULL)) {
		readLCFnums();
		all->isdirty++;
	}

}
