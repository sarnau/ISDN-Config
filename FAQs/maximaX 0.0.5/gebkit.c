#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>
#include <X11/Xmu/Editres.h>

#include "is1.h"
#include "gebkit.h"

#define LIST_OUT "%.4d %2s %02d.%02d %02d:%02d %6s %02d:%02d:%02d %19s %-38s"
#define CALC_OUT "%s,%02d.%02d,%02d:%02d,%d,%2.2f,#%s,%s"

extern struct GesamtKonfig *all;
extern int debugflag;
extern FILE *debugfile;


Widget outkitdlg;
Widget inkitdlg;
Incomming_CallsType *incallbuf;
Outgoing_CallsType *outcallbuf;



char *
getKitAppNum(unsigned char app)
{
	static char ret[20];

	if (app == 0xAA) {
		return("--");
	} else {
		sprintf(ret,"%2X",app);
		return(ret);
	}
}

Widget
clearList(Widget mother,char *name)
{
	static Widget list;
	if ((list  = XmtNameToWidget(mother,name))==NULL) return(NULL);
	XmListDeleteAllItems(list);
	return(list);
}

/****************** Ausgehende Gesraeche **********************/
int createOutbuf()
{
	return(((outcallbuf =
		(Outgoing_CallsType *)calloc(MAXOUTCALLS,sizeof(Outgoing_CallsType)))!=NULL));
}

void
copy2outcallbuf(char *buf,int which,int num)
{
	memcpy(&outcallbuf[which],buf,sizeof(Outgoing_CallsType)*num);
}

char *
printOutcallbuf(int num,int spreadsheet)
{
	static char line[256];
	char val[10];
	char *phone;
	char *callname;
	int stunden,minuten,sekunden;

	if (outcallbuf[num].Appnum == 0 ) return(NULL);
	if (outcallbuf[num].Tag==255)    outcallbuf[num].Tag=0;
	if (outcallbuf[num].Monat==255)  outcallbuf[num].Monat=0;
	if (outcallbuf[num].Stunde==255) outcallbuf[num].Stunde=0;
	phone=(char *)get_Phonenumber(&outcallbuf[num].Nummer[0]),
	callname=(char *)get_CallName(phone);
	if (spreadsheet)
		sprintf(line,CALC_OUT,
			getKitAppNum(outcallbuf[num].Appnum),
			outcallbuf[num].Tag,
			outcallbuf[num].Monat,
			outcallbuf[num].Stunde,
			outcallbuf[num].Minute,
			outcallbuf[num].Dauer,
			(float)((float)outcallbuf[num].Gebuehr / 100),
			phone,
			callname
		);
	else {
		stunden=(outcallbuf[num].Dauer / 3600);
		minuten=((outcallbuf[num].Dauer - (stunden * 3600)) / 60);
		sekunden=outcallbuf[num].Dauer - (minuten*60) - (stunden * 3600);
		sprintf(val,"%2.2f",(float)((float)outcallbuf[num].Gebuehr / 100));
		sprintf(line,LIST_OUT,
			num,
			getKitAppNum(outcallbuf[num].Appnum),
			outcallbuf[num].Tag,
			outcallbuf[num].Monat,
			outcallbuf[num].Stunde,
			outcallbuf[num].Minute,
			val,
			stunden,
			minuten,
			sekunden,
			phone,
			callname
		);
	}
	return(line);
}

void
ShowOutList()
{
	Widget list;
	XmString *item;
	char *line;
	int i,j;

	list = clearList(outkitdlg,"*gebOtext");
	if (!list) return;

	if (!all->havekwzdata) loadNshowFile(outkitdlg,FALSE);

	if ((item=(XmString *)calloc(MAXOUTCALLS,sizeof(XmString)))==NULL) {
		perror("");
		return;
	}

	for (i=0;i<MAXOUTCALLS;i++) {
		line=printOutcallbuf(i,FALSE);
		if (line)
			item[i] = XmStringCreateSimple(line);
		else break;
	}

	j=i;
	XmListAddItemsUnselected(list, item,j, 0);

	for (i=0;i<j;i++) {
		if (item[i]) XmStringFree(item[i]);
	}

	free(item);
}

void
writeOutCalls(FILE *fp,int cut,int spreadsheet)
{
	char *line;
	int i;

	for (i=0;i<MAXOUTCALLS;i++) {
		line=printOutcallbuf(i,spreadsheet);
		if (line)  {
			if (cut & !spreadsheet)
				fprintf(fp,"%80.80s\n",line);
			else
				fprintf(fp,"%s\n",line);
		}
	}
}

void
printOutCalls()
{
	FILE *fp;
	
	if (!outcallbuf) return;

	if ((fp=popen("lpr","w"))!=NULL) {
		writeOutCalls(fp,TRUE,FALSE);
		pclose(fp);
	}
}

void
saveCvsOutCalls()
{
    FILE *fp;
    char lthefilename[256]="";
    char lthedirname[256]="";
    char lpatbuf[256]="";

    if (XmtAskForFile(outkitdlg,"cvs_dlg","CVS-Tabelle sichern:",CFGPATH,
            "*.cvs",&fp,"w",lthefilename,256,lthedirname,256,
            lpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
            writeOutCalls(fp,FALSE,TRUE);
        }
        fclose(fp);
    }
}

void
saveOutCalls()
{
    FILE *fp;
    char lthefilename[256]="";
    char lthedirname[256]="";
    char lpatbuf[256]="";

    if (XmtAskForFile(outkitdlg,"txt_dlg","Text sichern:",CFGPATH,
            "*.tab",&fp,"w",lthefilename,256,lthedirname,256,
            lpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
            writeOutCalls(fp,FALSE,FALSE);
        }
        fclose(fp);
    }
}

int
outDaten(char *buf,int counter)
{
	char *dummy;
	int i;
	unsigned int num;
	
	num=(unsigned int)(buf[0] & 0x0FF) / sizeof(Outgoing_CallsType);
	if (num == 0) return(-1);
	copy2outcallbuf(&buf[4],counter,num);
	return(num);
}

void
doKitGetOut()
{
	Widget w;
	Widget list;
	char buf[1024];
	int ser=0;
	int dbg=debugflag;
	int ret;
	int i,j;

	if (!outcallbuf) 
		if (!createOutbuf())
			return;

	if ((ser=openSer(all->thedevname))==-1) return;

	w=XmtDisplayWorkingDialog(outkitdlg,"gebreaddlg",
		"Ausgehende Gespräche lesen...", "Prozent:","",0,100,True,False);

	synchronize(ser);
	sendBOTR(ser,dbg);
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		for(i=0;i<MAXOUTCALLS;) {
			write6_cmd(ser,K_READ_OUT_GEB_LIST,K_READ_CMD_OFFSET,(unsigned short)i,dbg);
			if (!(ret=getNextPacket(outkitdlg,ser,buf,dbg))) return;
			if(dbg) dumppacket("K_READ_TLN_GEB_LIST",buf,ret);
			if (buf[0]==0x04) break;
			i+=outDaten(buf,i);
			showPercent(w,MAXOUTCALLS,i);
		}
		stop_conn(outkitdlg,ser,K_WR_OK,dbg);
		ShowOutList(); 
	} else stop_conn(outkitdlg,ser,ret,dbg);
}

void
doKitOut(Widget mother)
{
    if (!outkitdlg) {
        outkitdlg=XmtBuildDialog(mother, "gebOlist_dlg",
                NULL, 0);
        if (!outkitdlg) exit(1);
    }
    if (XmtDialogDoSync(outkitdlg,NULL)) {
    }
}

/****************** Eingehende Gesraeche **********************/
int
createInbuf()
{
	return(((incallbuf =
		(Incomming_CallsType *)calloc(MAXINCALLS,sizeof(Incomming_CallsType)))!=NULL));
}

void
copy2incallbuf(char *buf,int which,int num)
{
	memcpy(&incallbuf[which],buf,sizeof(Incomming_CallsType)*num);
}

char *
printIncallbuf(int num)
{
	static char line[256];
	char *phone;
	char *callname;

	if (incallbuf[num].Appnum == 0 ) return(NULL);
	if (incallbuf[num].Tag==255)    incallbuf[num].Tag=0;
	if (incallbuf[num].Monat==255)  incallbuf[num].Monat=0;
	if (incallbuf[num].Stunde==255) incallbuf[num].Stunde=0;
	phone=(char *)get_Phonenumber(&incallbuf[num].Nummer[0]),
	callname=(char *)get_CallName(phone);
	sprintf(line,"%.4d %2s %02d.%02d %02d:%02d %19s %-38s",
		num,
		getKitAppNum(incallbuf[num].Appnum),
		incallbuf[num].Tag,
		incallbuf[num].Monat,
		incallbuf[num].Stunde,
		incallbuf[num].Minute,
		phone,
		callname
	);
	return(line);
}

void
ShowInList()
{
	Widget list;
	XmString item[MAXINCALLS];
	char *line;
	int i,j;

	list = clearList(inkitdlg,"*gebItext");
	if (!list) return;

	if (!all->havekwzdata) loadNshowFile(inkitdlg,FALSE);


	for (i=0;i<MAXINCALLS;i++) {
		line=printIncallbuf(i);
		if (line)
			item[i] = XmStringCreateSimple(line);
		else break;
	}

	j=i;
	XmListAddItemsUnselected(list, item,j, 0);

	for (i=0;i<j;i++) {
		if (item[i]) XmStringFree(item[i]);
	}
}

void
writeInCalls(FILE *fp,int cut)
{
	char *line;
	int i;

	for (i=0;i<MAXINCALLS;i++) {
		line=printIncallbuf(i);
		if (line)  {
			if (cut) fprintf(fp,"%80.80s\n",line);
			else     fprintf(fp,"%s\n",line);
		}
	}
}

void
printInCalls()
{
	FILE *fp;
	
	if (!incallbuf) return;

	if ((fp=popen("lpr","w"))!=NULL) {
		writeInCalls(fp,TRUE);
		pclose(fp);
	}
}

void
saveInCalls()
{
    FILE *fp;
    char lthefilename[256]="";
    char lthedirname[256]="";
    char lpatbuf[256]="";

    if (XmtAskForFile(inkitdlg,"txt_dlg","Text sichern:",CFGPATH,
            "*.txt",&fp,"w",lthefilename,256,lthedirname,256,
            lpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
            writeInCalls(fp,FALSE);
        }
        fclose(fp);
    }
}

int
inDaten(char *buf,int counter)
{
	char *dummy;
	int i;
	unsigned int num;
	
	num=(unsigned int)(buf[0] & 0x0FF) / sizeof(Incomming_CallsType);
	if (num == 0) return(-1);
	copy2incallbuf(&buf[4],counter,num);
	return(num);
}


void
doKitGetIn()
{
	Widget w;
	Widget list;
	char buf[1024];
	int ser=0;
	int dbg=debugflag;
	int ret;
	int i,j;

	if (!incallbuf) 
		if (!createInbuf())
			return;

	if ((ser=openSer(all->thedevname))==-1) return;

	w=XmtDisplayWorkingDialog(inkitdlg,"gebreaddlg",
		"Eingegangene Gespräche auslesen...", "Prozent:","",0,100,True,False);

	synchronize(ser);
	sendBOTR(ser,dbg);
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		for(i=0;i<MAXINCALLS;) {
			write_cmd(ser,K_READ_IN_GEB_LIST,K_READ_CMD_OFFSET,i,dbg);
			if (!(ret=getNextPacket(inkitdlg,ser,buf,dbg))) return;
			if(dbg) dumppacket("K_READ_IN_GEB_LIST",buf,ret);
			if (buf[0] == 0x04) break;
			j=inDaten(buf,i);
			if (j == -1) break;
			i+=j;
			showPercent(w,MAXINCALLS,i);
		}
		stop_conn(inkitdlg,ser,K_WR_OK,dbg);
		ShowInList();
	} else stop_conn(inkitdlg,ser,ret,dbg);
}


void
doKitDelOut()
{
	Widget w;
	long *gebuehren;
	char buf[1024];
	int ser=0;
	int dbg=debugflag;
	int ret;

	gebuehren=(long *)buf;

	if (XmtDisplayWarningAndAsk(outkitdlg, "gebkitdel",
		"Wollen Sie wirklich den Gebührenspeicher lõschen?",
		"Ja","Nein",XmtNoButton,"No Help!")) {

		if ((ser=openSer(all->thedevname))==-1) return;

		w=XmtDisplayWorkingDialog(outkitdlg,"gebkitdlg",
			"Gebührenspeicher lõschen...", "","",0,100,False,False);

		synchronize(ser);
		sendBOTW(ser,dbg);
		if ((ret=isOK(ser,dbg))==K_WR_OK) {
			memset(buf,0,1023);

        	if (!send_data(ser,K_WRITE_OUT_GEB_LIST,-1,buf,1,dbg)) {
            	stop_conn(outkitdlg,ser,ret,dbg);
            	return;
        	}
        	if ((ret=isOK(ser,dbg))!=K_WR_OK){
            	stop_conn(outkitdlg,ser,ret,dbg);
            	return;
        	}
	
			stop_conn(outkitdlg,ser,K_WR_OK,dbg);
		} else stop_conn(outkitdlg,ser,ret,dbg);
	}
}

void
doKitDelIn()
{
}

void
doKitIn(Widget mother)
{
    if (!inkitdlg) {
        inkitdlg=XmtBuildDialog(mother, "gebIlist_dlg",
                NULL, 0);
        if (!inkitdlg) exit(1);
    }
    if (XmtDialogDoSync(inkitdlg,NULL)) {
    }
}

void doSetupKit()
{
	XmtVaRegisterCallbackProcedures(
		"doKitGetOut",doKitGetOut,XmtRCallbackUnused,
		"doKitGetIn",doKitGetIn,XmtRCallbackUnused,
		"printInCalls",printInCalls,XmtRCallbackUnused,
		"saveInCalls",saveInCalls,XmtRCallbackUnused,
		"printOutCalls",printOutCalls,XmtRCallbackUnused,
		"saveOutCalls",saveOutCalls,XmtRCallbackUnused,
		"saveCvsOutCalls",saveCvsOutCalls,XmtRCallbackUnused,
		"doKitDelOut",doKitDelOut,XmtRCallbackUnused,
		NULL );
}
