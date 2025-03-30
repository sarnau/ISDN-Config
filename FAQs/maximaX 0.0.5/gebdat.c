#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"

extern struct GesamtKonfig *all;

Widget gebdatdlg;
splitType geb[5];
int geblastsel=0;

void setShortIfPrnOn(int chooser,int which,int onoff)
{
	char item[30];

	sprintf(item,"*geb%-d",chooser);
	disableChooserItem(gebdatdlg,item,which,onoff);
}

void checkShortPrnOn()
{
	int i;

	for(i=0;i<10;i++){
		setShortIfPrnOn(0,i,(geb[2].set0 & (0x01 << i)));
	}

	if (hasPort(0,1)) 
		for(i=0;i<10;i++){
			setShortIfPrnOn(1,i,(geb[2].set1 & (0x01 << i)));
	}

	if (hasPort(0,2)) 
		for(i=0;i<10;i++){
			setShortIfPrnOn(2,i,(geb[2].set2 & (0x01 << i)));
	}

}

void setAllOn()
{
	int i;

	for(i=0;i<10;i++){
		setShortIfPrnOn(0,i,1);
	}

	if (hasPort(0,1)) 
		for(i=0;i<10;i++){
			setShortIfPrnOn(1,i,1);
	}

	if (hasPort(0,2)) 
		for(i=0;i<10;i++){
			setShortIfPrnOn(2,i,1);
	}

}

void getNsetGeb()
{
	int x,y,z;

	getChooserState(gebdatdlg,"*geb0",&x);
	getChooserState(gebdatdlg,"*geb1",&y);
	getChooserState(gebdatdlg,"*geb2",&z);

	geb[geblastsel].set0=x;
	geb[geblastsel].set1=y;
	geb[geblastsel].set2=z;
}

void
doGebDat(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;
	int dummy=ccb->item;

	getNsetGeb();
	geblastsel=dummy;

	if (dummy==3) checkShortPrnOn();
	else         setAllOn();

	setChooserState(gebdatdlg,"*geb0",geb[dummy].set0);
	setChooserState(gebdatdlg,"*geb1",geb[dummy].set1);
	setChooserState(gebdatdlg,"*geb2",geb[dummy].set2);
	
}

void displayGebDat(Widget mother)
{
	int i,j;
	int base,zlanz,com,impuls,calls;
	char wname[50];
	char nname[50];
	char s_dummy[20];
    Boolean port1isOK;
    Boolean port2isOK;
	splitType *dummy;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);

	if (!gebdatdlg) {
        gebdatdlg=XmtBuildDialog(mother, "gebdat_dlg",
                NULL, 0);
        if (!gebdatdlg) exit(1);
    }

	i=all->absel;
	geblastsel=2;
	base=(int)all->tlnGebKonf[i].GrundFaktor;
	zlanz=(int)all->tlnGebKonf[i].PrnZeilenAnz;
	com=(int)(all->tlnGebKonf[i].Flags & 0x03)-1;
	calls=(int)((all->tlnGebKonf[i].Flags >> 2) & 0x03);
	impuls=(int)((all->tlnGebKonf[i].GebImpuls >> 5) & 0x07)-1;

	SetNumField(gebdatdlg,"*base",-1,base);
	SetNumField(gebdatdlg,"*zlanz",-1,zlanz);

	dummy=(splitType *)&all->tlnGebKonf[i].Pc;
	geb[0]=*dummy;
	dummy=(splitType *)&all->tlnGebKonf[i].Prn;
	geb[1]=*dummy;
	dummy=(splitType *)&all->tlnGebKonf[i].PrnEin;
	geb[2]=*dummy;
	dummy=(splitType *)&all->tlnGebKonf[i].PrnGek;
	geb[3]=*dummy;
	dummy=(splitType *)&all->tlnGebKonf[i].GebToTln;
	geb[4]=*dummy;

	for(j=0;j<8;j++) {
		if (all->rufNrZuord[all->absel].Rufnummer[j][0]!='\0') {
			sprintf(wname,"*geb0.item%-d",j);
			sprintf(nname,"Ruf #%s",all->rufNrZuord[all->absel].Rufnummer[j]);
			set_labelname(gebdatdlg,wname,nname);
		}
	}
    
	setChooserState(gebdatdlg,"*slct",geblastsel);
	setChooserState(gebdatdlg,"*com",com);
	setChooserState(gebdatdlg,"*calls",calls);
	setChooserState(gebdatdlg,"*impuls",impuls);
	setChooserState(gebdatdlg,"*geb0",geb[geblastsel].set0);
	setChooserState(gebdatdlg,"*geb1",geb[geblastsel].set1);
	setChooserState(gebdatdlg,"*geb2",geb[geblastsel].set2);
	disableChooser(gebdatdlg,"*geb1",port1isOK);
	disableChooser(gebdatdlg,"*geb2",port2isOK);
	disableChooserItem(gebdatdlg,"*slct",0,0);
	disableChooserItem(gebdatdlg,"*slct",1,0);


	if (XmtDialogDoSync(gebdatdlg,NULL)) {
		getNsetGeb();
		dummy=(splitType *)&all->tlnGebKonf[i].Pc;
		*dummy=geb[0];
		dummy=(splitType *)&all->tlnGebKonf[i].Prn;
		*dummy=geb[1];
		dummy=(splitType *)&all->tlnGebKonf[i].PrnEin;
		*dummy=geb[2];
		dummy=(splitType *)&all->tlnGebKonf[i].PrnGek;
		*dummy=geb[3];
		dummy=(splitType *)&all->tlnGebKonf[i].GebToTln;
		*dummy=geb[4];
		base=GetNumField(gebdatdlg,"*base",-1);
		all->tlnGebKonf[i].GrundFaktor=(char)base;
		zlanz=GetNumField(gebdatdlg,"*zlanz",-1);
		all->tlnGebKonf[i].PrnZeilenAnz=(char)zlanz;
		getChooserState(gebdatdlg,"*com",&com);
		getChooserState(gebdatdlg,"*calls",&calls);
		getChooserState(gebdatdlg,"*impuls",&impuls);

		all->tlnGebKonf[i].Flags=((calls << 2) | (com +1));
		all->tlnGebKonf[i].GebImpuls=((impuls+1) << 5);
		all->isdirty++;
	}

}

void doGebLCF(Widget w, XtPointer p, XtPointer data)
{
	doDisplayLCF(gebdatdlg);
}

void doGebStand(Widget w, XtPointer p, XtPointer data)
{
	doGebZaehler(gebdatdlg);
}

void doSetupGebDat()
{
	XmtVaRegisterCallbackProcedures(
		"doGebDat",doGebDat,XmtRCallbackUnused,
		"doGebLCF",doGebLCF,XmtRCallbackUnused,
		"doGebStand",doGebStand,XmtRCallbackUnused,
		NULL );
}
