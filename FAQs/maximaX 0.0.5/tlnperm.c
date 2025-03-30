#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget masadlg;
splitType set[3];
int lastset=0;

void getNsetPerm()
{
	int x,y,z;

	getChooserState(masadlg,"*set0",&x);
	getChooserState(masadlg,"*set1",&y);
	getChooserState(masadlg,"*set2",&z);

	set[lastset].set0=x;
	set[lastset].set1=y;
	set[lastset].set2=z;
}

void
doFunkPerm(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;
	int dummy=ccb->item;

	getNsetPerm();
	lastset=dummy;

	setChooserState(masadlg,"*set0",set[dummy].set0);
	setChooserState(masadlg,"*set1",set[dummy].set1);
	setChooserState(masadlg,"*set2",set[dummy].set2);
	
}

void displayTlnPerm(Widget mother)
{
	int i;
    Boolean port1isOK;
    Boolean port2isOK;
	splitType *dummy;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);


	if (!masadlg) {
        masadlg=XmtBuildDialog(mother, "tlnperm_dlg",
                NULL, 0);
        if (!masadlg) exit(1);
    }

	i=all->absel;
	lastset=0;

	dummy=(splitType *)&all->tlnBerechtigung[i].Masterber;
	set[0]=*dummy;
	dummy=(splitType *)&all->tlnBerechtigung[i].Aufschaltsch;
	set[1]=*dummy;
	dummy=(splitType *)&all->tlnBerechtigung[i].Aufschalt;
	set[2]=*dummy;

    setItemLabels(masadlg,"*set","Ruf #",0,0,7);
    if (hasPort(0,1))
        setItemLabels(masadlg,"*set","Ruf #",1,0,9);
    if (hasPort(0,2))
        setItemLabels(masadlg,"*set","Ruf #",2,0,9);

	setChooserState(masadlg,"*funk",0);
	setChooserState(masadlg,"*set0",set[0].set0);
	setChooserState(masadlg,"*set1",set[0].set1);
	setChooserState(masadlg,"*set2",set[0].set2);
	disableChooser(masadlg,"*set1",port1isOK);
	disableChooser(masadlg,"*set2",port2isOK);

	if (XmtDialogDoSync(masadlg,NULL)) {
		getNsetPerm();
		dummy=(splitType *)&all->tlnBerechtigung[i].Masterber;
		*dummy=set[0];
		dummy=(splitType *)&all->tlnBerechtigung[i].Aufschaltsch;
		*dummy=set[1];
		dummy=(splitType *)&all->tlnBerechtigung[i].Aufschalt;
		*dummy=set[2];
		all->isdirty++;
	}

}

void doSetupPerm()
{
	XmtVaRegisterCallbackProcedures(
		"doFunkPerm",doFunkPerm,XmtRCallbackUnused,
		NULL );
}
