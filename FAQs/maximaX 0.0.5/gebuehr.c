#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>
#include <X11/Xmu/Editres.h>

#include "is1.h"

extern struct GesamtKonfig *all;
extern int debugflag;
extern FILE *debugfile;


Widget zaehlerdlg;

void showGebuehren(char *buf)
{
	Widget list;
	long *gebuehren;
	XmString item[30];
	int i,j;
	char line[100];
	long summe;
	
	gebuehren=(long *)buf;
	
	if ((list  = XmtNameToWidget(zaehlerdlg, "*zaehler"))==NULL) return;
	summe=0;
	j=0;

	XmListDeleteAllItems(list);

	for (i=0;i<30;i++) {
		if ((gebuehren[i]!=0x5a5a5a5a) &&
		(all->rufNrZuord[all->absel].Rufnummer[i][0]!='\0')) {
			sprintf(line," %-10s (#%-s)    %04.2f DM",
				getAppName(i),
				all->rufNrZuord[all->absel].Rufnummer[i],
				(float)(((float)gebuehren[i])/100.00));
			summe+=gebuehren[i];
			item[j++]=XmStringCreateSimple(line);
		}
	}
	XmListAddItems(list, item,j, 0);
	for (i=0;i<j;i++) XmStringFree(item[i]);

	sprintf(line,"%04.2f DM",(float)(((float)summe)/100.00));
	set_labelname(zaehlerdlg,"summe",line);
	
}

void
doGebGet()
{
	Widget w;
	char buf[1024];
	int ser=0;
	int dbg=debugflag;
	int ret;

	if ((ser=openSer(all->thedevname))==-1) return;

	w=XmtDisplayWorkingDialog(zaehlerdlg,"gebreaddlg",
		"Gebührenzähler lesen...", "","",0,100,False,False);

	synchronize(ser);
	sendBOTR(ser,dbg);
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		write_cmd(ser,K_READ_TLN_GEB_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(zaehlerdlg,ser,buf,dbg))) return;
		if(dbg) dumppacket("K_READ_TLN_GEB_LIST",&buf[4],ret-4);
		stop_conn(zaehlerdlg,ser,K_WR_OK,dbg);
		showGebuehren(&buf[4]);
	} else stop_conn(zaehlerdlg,ser,ret,dbg);
}

void
doGebDel()
{
	Widget w;
	long *gebuehren;
	char buf[1024];
	int ser=0;
	int dbg=debugflag;
	int ret;

	gebuehren=(long *)buf;

	if (XmtDisplayWarningAndAsk(zaehlerdlg, "gebuehrdel",
		"Wollen Sie wirklich alle Gebührendaten\nauf 0.00 DM setzten?",
		"Ja","Nein",XmtNoButton,"No Help!")) {

		if ((ser=openSer(all->thedevname))==-1) return;

		w=XmtDisplayWorkingDialog(zaehlerdlg,"gebwritedlg",
			"Gebührenzähler lõschen...", "","",0,100,False,False);

		synchronize(ser);
		sendBOTW(ser,dbg);
		if ((ret=isOK(ser,dbg))==K_WR_OK) {
			memset(buf,0,1023);

        	if (!send_data(ser,K_WRITE_TLN_GEB_LIST,-1,buf,124,dbg)) {
            	stop_conn(zaehlerdlg,ser,ret,dbg);
            	return;
        	}
        	if ((ret=isOK(ser,dbg))!=K_WR_OK){
            	stop_conn(zaehlerdlg,ser,ret,dbg);
            	return;
        	}
	
			stop_conn(zaehlerdlg,ser,K_WR_OK,dbg);
			showGebuehren(buf);
		} else stop_conn(zaehlerdlg,ser,ret,dbg);
	}
}

void
doGebZaehler(Widget mother)
{
    if (!zaehlerdlg) {
        zaehlerdlg=XmtBuildDialog(mother, "gebzahl_dlg",
                NULL, 0);
        if (!zaehlerdlg) exit(1);
    }
    if (XmtDialogDoSync(zaehlerdlg,NULL)) {
    }
}

void doSetupZaehler()
{
	XmtVaRegisterCallbackProcedures(
		"doGebGet",doGebGet,XmtRCallbackUnused,
		"doGebDel",doGebDel,XmtRCallbackUnused,
		NULL );
}
