#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget avarwsdlg;
splitType set[12];
int lastdist=0;

void getNsetAVARWS()
{
	int x,y,z;

	getChooserState(avarwsdlg,"*grp0",&x);
	getChooserState(avarwsdlg,"*grp1",&y);
	getChooserState(avarwsdlg,"*grp2",&z);

	set[lastdist].set0=x;
	set[lastdist].set1=y;
	set[lastdist].set2=z;
}

void
doCallDist(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;
	int dummy=ccb->item;

	getNsetAVARWS();
	lastdist=dummy;

	setChooserState(avarwsdlg,"*grp0",set[dummy].set0);
	setChooserState(avarwsdlg,"*grp1",set[dummy].set1);
	setChooserState(avarwsdlg,"*grp2",set[dummy].set2);
	
}

void displayCallDist(Widget mother)
{
	int i,j;
    Boolean port1isOK;
    Boolean port2isOK;
	splitType *dummy;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);

	if (!avarwsdlg) {
        avarwsdlg=XmtBuildDialog(mother, "calldist_dlg",
                NULL, 0);
        if (!avarwsdlg) exit(1);
    }

	i=all->absel;
	lastdist=0;

	dummy=(splitType *)&all->tlnRufvert[i].Ava1;
	set[0]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Ava2;
	set[1]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Tfe1;
	set[2]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Tfe2;
	set[3]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Rws;
	set[4]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Zweitwecker1;
	set[5]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].Zweitwecker2;
	set[6]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].BAva1;
	set[7]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].BAva2;
	set[8]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].BTfe1;
	set[9]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].BTfe2;
	set[10]=*dummy;
	dummy=(splitType *)&all->tlnRufvert[i].BRws;
	set[11]=*dummy;

	setItemLabels(avarwsdlg,"*grp","Ruf #",0,0,7);
	if (port1isOK)
		setItemLabels(avarwsdlg,"*grp","Ruf #",1,0,9);
	if (port2isOK)
		setItemLabels(avarwsdlg,"*grp","Ruf #",2,0,9);
    
	setChooserState(avarwsdlg,"*todo",0);
	setChooserState(avarwsdlg,"*grp0",set[0].set0);
	setChooserState(avarwsdlg,"*grp1",set[0].set1);
	setChooserState(avarwsdlg,"*grp2",set[0].set2);
	disableChooser(avarwsdlg,"*grp1",port1isOK);
	disableChooser(avarwsdlg,"*grp2",port2isOK);

	if (XmtDialogDoSync(avarwsdlg,NULL)) {
		getNsetAVARWS();
		dummy=(splitType *)&all->tlnRufvert[i].Ava1;
		*dummy=set[0];
		dummy=(splitType *)&all->tlnRufvert[i].Ava2;
		*dummy=set[1];
		dummy=(splitType *)&all->tlnRufvert[i].Tfe1;
		*dummy=set[2];
		dummy=(splitType *)&all->tlnRufvert[i].Tfe2;
		*dummy=set[3];
		dummy=(splitType *)&all->tlnRufvert[i].Rws;
		*dummy=set[4];
		dummy=(splitType *)&all->tlnRufvert[i].Zweitwecker1;
		*dummy=set[5];
		dummy=(splitType *)&all->tlnRufvert[i].Zweitwecker2;
		*dummy=set[6];
		dummy=(splitType *)&all->tlnRufvert[i].BAva1;
		*dummy=set[7];
		dummy=(splitType *)&all->tlnRufvert[i].BAva2;
		*dummy=set[8];
		dummy=(splitType *)&all->tlnRufvert[i].BTfe1;
		*dummy=set[9];
		dummy=(splitType *)&all->tlnRufvert[i].BTfe2;
		*dummy=set[10];
		dummy=(splitType *)&all->tlnRufvert[i].BRws;
		*dummy=set[11];
		all->isdirty++;
	}

}

void doSetupCallDist()
{
	XmtVaRegisterCallbackProcedures(
		"doCallDist",doCallDist,XmtRCallbackUnused,
		NULL );
}
