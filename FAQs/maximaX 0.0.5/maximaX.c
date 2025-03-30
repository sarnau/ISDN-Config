#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>
#include <X11/Xmu/Editres.h>

#include "is1.h"
#include "maximaX.h"

static char *fallb_rsrcs[]= {
#include "maximaX.ad.h"
#include "kwzconf.ad.h"
#include "maxconf.ad.h"
#include "nahber.ad.h"
#include "sperrber.ad.h"
#include "rufzuord.ad.h"
#include "globdat.ad.h"
#include "tlnperm.ad.h"
#include "teltyp.ad.h"
#include "amtperm.ad.h"
#include "calldist.ad.h"
#include "gebdat.ad.h"
#include "gebuehr.ad.h"
#include "loccharge.ad.h"
#include "steuer.ad.h"
#include "teilndat.ad.h"
#include "gebkit.ad.h"
#include "max_help.ad.h"
    NULL
};

struct GesamtKonfig *all;

FILE *debugfile;
static Widget toplevel;
char *theConfBuf=NULL;
char cfgdirname[256]="";
char cfgpatbuf[256]="";
char anl_vers[256]="";
int debugflag=0;

void
maximax_vers()
{
	Widget w;

	if ((w=XmtNameToWidget(toplevel,"*theline"))) {
		XmtMsgLineClear(w,XmtMsgLineNow);
		XmtMsgLinePrintf(w,"maximaX v%s von Chris Benndorf",VERSION);
	}
}

void
show_anl_vers()
{
	Widget w;

	if ((w=XmtNameToWidget(toplevel,"*theline"))) {
		XmtMsgLineClear(w,XmtMsgLineNow);
		XmtMsgLinePrintf(w,"ROM-Version: %s",anl_vers);
	}
}

void
setFlags(int flag)
{
	all->havedata=flag;
	all->isdirty=flag;
}

void
activateMainButtons()
{
	set_active(toplevel,"*grund",all->havedata);
	set_active(toplevel,"*rufzuord",all->havedata);
	set_active(toplevel,"*appnum",all->havedata);
	set_active(toplevel,"*sichern",all->havedata);
	set_active(toplevel,"*global",all->havedata);
	set_active(toplevel,"*einricht",all->havedata);
	set_active(toplevel,"*schreiben",all->havedata);
	set_active(toplevel,"*endgeraet",all->havedata);
	set_active(toplevel,"*amtrecht",all->havedata);
	set_active(toplevel,"*avas",all->havedata);
	set_active(toplevel,"*gebuehr",all->havedata);
	set_active(toplevel,"*bsteuer",all->havedata);
	set_active(toplevel,"*user",all->havedata);
	disableChooser(toplevel,"*absel",all->havedata);
	disableChooserItem(toplevel,"*absel",1,0);

	if (all->havedata) {
	} else {
	}
}

void
dowrite(FILE *fp)
{
	char *cfgstring;
	int len;
	int state;
	
	getChooserState(toplevel,"*aqsel",&state);

	cfgstring= (state) ? CFG_STRING_A : CFG_STRING_Q ;
    len=fwrite(cfgstring,1,CFG_KENNUNG_LEN,fp);
	len=fwrite(&all->iconf[0],1,sizeof(IsdnKonfType)*2,fp);
	len=fwrite(&all->rufNrZuord[0],1,sizeof(RufNrZuordType)*2,fp);
	len=fwrite(&all->tlnBerechtigung[0],1,sizeof(TlnBerechtigungType)*2,fp);
	len=fwrite(&all->tlnRufvert[0],1,sizeof(TlnRufvertType)*2,fp);
	len=fwrite(&all->tlnGebKonf[0],1,sizeof(TlnGebKonfType)*2,fp);
	len=fwrite(&all->steuerKontakt[0],1,sizeof(SteuerKontaktType)*2,fp);
	len=fwrite(&all->globAnlDaten,1,sizeof(GlobAnlDatenType),fp);
	fclose(fp);
    all->isdirty=0;
    all->haveconffile++;
}
void
saveCFGFile()
{
	FILE *fp;
	int len;

	if(!all->havedata) return;

	if ((fp=fopen(all->cfgfilename,"w"))!=NULL) {
		dowrite(fp);
	}
}

void
saveCFGFileAs()
{
	FILE *fp;

    if (XmtAskForFile(toplevel,"*cfgsave_dlg","Datei sichern:",CFGPATH,
            "*.cfg",&fp,"w",all->cfgfilename,256,cfgdirname,256,
            cfgpatbuf,256,"No Help!")==True) {
		dowrite(fp);
	}
}

void
loadCFGFile()
{
    FILE *fp;
	char kenn[CFG_KENNUNG_LEN];
    int i;
    int len;

	setFlags(0);
    if (XmtAskForFile(toplevel,"*cfgload_dlg","Datei laden:",CFGPATH,
            "*.cfg",&fp,"r",all->cfgfilename,256,cfgdirname,256,
            cfgpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
            len=fread(kenn,1,CFG_KENNUNG_LEN,fp);
            if (isCorrectFile(toplevel,all->cfgfilename,"Konfigurationsdatei",
                    CFG_KENNUNG,kenn,strlen(CFG_KENNUNG))) {
            	len=fread(&all->iconf[0],1,sizeof(IsdnKonfType)*2,fp);
            	len=fread(&all->rufNrZuord[0],1,sizeof(RufNrZuordType)*2,fp);
            	len=fread(&all->tlnBerechtigung[0],1,sizeof(TlnBerechtigungType)*2,fp);
            	len=fread(&all->tlnRufvert[0],1,sizeof(TlnRufvertType)*2,fp);
            	len=fread(&all->tlnGebKonf[0],1,sizeof(TlnGebKonfType)*2,fp);
            	len=fread(&all->steuerKontakt[0],1,sizeof(SteuerKontaktType)*2,fp);
            	len=fread(&all->globAnlDaten,1,sizeof(GlobAnlDatenType),fp);
                all->isdirty=0;
                all->havedata++;
                all->haveconffile++;
            }
        }
        fclose(fp);
    }
	activateMainButtons();
}

void
showPercent(Widget wbox, int max, int num)
{
	float part=(100.00 / (float)max)*(float)num;	
   	XmtWorkingBoxSetScaleValue(wbox,(int)part);
}

int
sendAndCheck(int ser, Widget mother, Widget w, char cmd,
		char offs,void *buf, int size,int max,int *step, int dbg)
{
		int ret;

        if (!send_data(ser,cmd,offs,(char *)buf,size,dbg)) {
            stop_conn(mother,ser,ret,dbg);
            return(1);
		}
        if ((ret=isOK(ser,dbg))!=K_WR_OK){
            stop_conn(mother,ser,ret,dbg);
            return(1);
		}
		*step+=1;
		showPercent(w,max,*step);
		return(0);
}


void writeConfig()
{
	Widget w;
	FILE *fp;
	char buf[1024];
	int i,x,ret;
    int ser=0;
    int dbg=debugflag;
    int max=12;
	int port1_OK,port2_OK;

    if ((ser=openSer(all->thedevname))==-1) return;

	port1_OK=(all->iconf[0].AnzPort > 0);
	port2_OK=(all->iconf[0].AnzPort > 1);

	if (!port2_OK)
		memset(&all->iconf[0].Port[2],0,sizeof(IsdnPortType));
	if (!port1_OK)
		memset(&all->iconf[0].Port[1],0,sizeof(IsdnPortType));

    w=XmtDisplayWorkingDialog(toplevel,"cfgwritewbox",
            "Konfiguration schreiben", "Prozent:","Abbrechen",0,100,True,False);

    synchronize(ser);
	setFlags(0);
    sendBOTW(ser,dbg);
    if ((ret=isOK(ser,dbg))==K_WR_OK) {
		i=0;

		if(dbg) fprintf(debugfile,
			"********************* START WRITE ******************\n");
		if (sendAndCheck(ser,toplevel,w,K_WRITE_SO_PORT,0,&all->iconf[0].Port[0],
				186,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_SO_PORT,1,&all->iconf[0].Port[1],
				186,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_SO_PORT,2,&all->iconf[0].Port[2],
				186,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_ANL_DAT_LIST,-1,&all->globAnlDaten,
				112,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_TLN_RUFNR_LIST,-1,&all->rufNrZuord[0],
				100,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_S0_TLN,0,
				&all->iconf[0].Port[0].Tln[0], 79,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_S0_TLN,1,
				&all->iconf[0].Port[1].Tln[0],79,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_S0_TLN,2,
				&all->iconf[0].Port[2].Tln[0],79,max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_TLN_BER_LIST,-1,&all->tlnBerechtigung[0],
				sizeof(TlnBerechtigungType),max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_TLN_RUF_VERT_LIST,-1,&all->tlnRufvert[0],
				sizeof(TlnRufvertType),max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_TLN_GEB_KONF_LIST,-1,&all->tlnGebKonf[0],
				sizeof(TlnGebKonfType),max,&i,dbg)) return;

		if (sendAndCheck(ser,toplevel,w,K_WRITE_ST_KON_LIST,-1,&all->steuerKontakt[0],
				sizeof(SteuerKontaktType),max,&i,dbg)) return;

    	stop_conn(toplevel,ser,K_WR_OK,dbg);
		setFlags(1);

    } else stop_conn(toplevel,ser,ret,dbg);

	activateMainButtons();
}


void readConfig()
{
	Widget w;
	FILE *fp;
	char buf[1024];
	int i,x,ret;
    int ser=0;
    int dbg=debugflag;
    int max=13;

    if ((ser=openSer(all->thedevname))==-1) return;

    w=XmtDisplayWorkingDialog(toplevel,"cfgreadwbox",
            "Konfiguration lesen", "Prozent:","Abbrechen",0,100,True,False);

    synchronize(ser);
	setFlags(0);
    sendBOTR(ser,dbg);
    if ((ret=isOK(ser,dbg))==K_WR_OK) {
		i=0;
		if(dbg) fprintf(debugfile,
			"********************* START READ ******************\n");
    	write_cmd(ser,K_READ_SO_PORT,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		all->iconf[0].AnzPort=buf[4]-1; 
		memcpy(&all->iconf[0].Port[0],&buf[5],ret-5);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_SO_PORT #0",&buf[4],ret-4);

    	write_cmd(ser,K_READ_SO_PORT,K_READ_CMD_OFFSET,1,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->iconf[0].Port[1],&buf[5],ret-5);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_SO_PORT #1",&buf[4],ret-4);

    	write_cmd(ser,K_READ_SO_PORT,K_READ_CMD_OFFSET,2,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->iconf[0].Port[2],&buf[5],ret-5);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_SO_PORT #2",&buf[4],ret-4);

    	write_cmd(ser,K_READ_S0_TLN,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->iconf[0].Port[0].Tln,&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_S0_TLN #0",&buf[4],ret-4);

    	write_cmd(ser,K_READ_S0_TLN,K_READ_CMD_OFFSET,1,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->iconf[0].Port[1].Tln,&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_S0_TLN #1",&buf[4],ret-4);

    	write_cmd(ser,K_READ_S0_TLN,K_READ_CMD_OFFSET,2,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->iconf[0].Port[2].Tln,&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_S0_TLN #2",&buf[4],ret-4);

		write_cmd(ser,K_READ_TLN_RUFNR_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->rufNrZuord[0],&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_TLN_RUFNR_LIST",&buf[4],ret-4);

		write_cmd(ser,K_READ_TLN_BER_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->tlnBerechtigung[0],&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_TLN_BER_LIST",&buf[4],ret-4);

		write_cmd(ser,K_READ_TLN_RUF_VERT_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->tlnRufvert[0],&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_TLN_RUF_VERT_LIST",&buf[4],ret-4);

		write_cmd(ser,K_READ_ST_KON_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->steuerKontakt[0],&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_ST_KON_LIST",&buf[4],ret-4);

		write_cmd(ser,K_READ_TLN_GEB_KONF_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->tlnGebKonf[0],&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_TLN_GEB_KONF_LIST",&buf[4],ret-4);

		write_cmd(ser,K_READ_ANL_DAT_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		memcpy(&all->globAnlDaten,&buf[4],ret-4);
		showPercent(w,max,++i);
		if(dbg) dumppacket("K_READ_ANL_DAT_LIST",&buf[4],ret-4);

		write_cmd(ser,K_GET_EPROM_VER,0,0,dbg);
		if (!(ret=getNextPacket(toplevel,ser,buf,dbg))) return;
		showPercent(w,max,++i);
		strcpy(anl_vers,&buf[4]);
		if(dbg) dumppacket("K_GET_EPROM_VER",&buf[4],ret-4);

		setFlags(1);
		show_anl_vers();
    	stop_conn(toplevel,ser,K_WR_OK,dbg);
    } else stop_conn(toplevel,ser,ret,dbg);

	activateMainButtons();
}

void writeTlnConfig(Widget mother)
{
	Widget w;
	FILE *fp;
	char buf[1024];
	int i,x,ret;
    int ser=0;
    int dbg=debugflag;
    int max=30;
	int port1_OK,port2_OK;

    if ((ser=openSer(all->thedevname))==-1) return;

	port1_OK=(all->iconf[0].AnzPort > 0);
	port2_OK=(all->iconf[0].AnzPort > 1);

	if (!port2_OK)
		memset(&all->iconf[0].Port[2],0,sizeof(IsdnPortType));
	if (!port1_OK)
		memset(&all->iconf[0].Port[1],0,sizeof(IsdnPortType));

    w=XmtDisplayWorkingDialog(mother,"cfgwritewbox",
            "Konfiguration schreiben", "Prozent:","Abbrechen",0,100,True,False);

    synchronize(ser);
	setFlags(0);
    sendBOTW(ser,dbg);
    if ((ret=isOK(ser,dbg))==K_WR_OK) {
		i=0;

		if(dbg) fprintf(debugfile,
			"********************* START WRITE ******************\n");

		for(x=0;x<30;x++)
			if (sendAndCheck(ser,mother,w,K_WRITE_TLN_DATA_LIST,x,&all->tlnData[x].Geb,
				sizeof(TlnDataType)-1,max,&i,dbg)) return;

    	stop_conn(mother,ser,K_WR_OK,dbg);

    } else stop_conn(mother,ser,ret,dbg);
}

void readTlnConfig(Widget mother)
{
	Widget w;
	FILE *fp;
	char buf[1024];
	int i,x,ret;
    int ser=0;
    int dbg=debugflag;
    int max=30;

    if ((ser=openSer(all->thedevname))==-1) return;

    w=XmtDisplayWorkingDialog(mother,"tlnreadwbox",
            "Teilnehmerdaten lesen", "Prozent:","Abbrechen",0,100,True,False);

    synchronize(ser);
	setFlags(0);
    sendBOTR(ser,dbg);
    if ((ret=isOK(ser,dbg))==K_WR_OK) {
		i=0;
		if(dbg) fprintf(debugfile,
			"********************* START READ ******************\n");

		for(x=0;x < MAX_RUF_VAR_LEN;x++) {
			write_cmd(ser,K_READ_TLN_DATA_LIST,K_READ_CMD_OFFSET,x,dbg);
			if (!(ret=getNextPacket(mother,ser,buf,dbg))) return;
			memcpy(&all->tlnData[x],&buf[4],sizeof(TlnDataType));
			showPercent(w,max,++i);
			if(dbg) dumppacket("K_READ_TLN_DATA_LIST",&buf[4],ret-4);
		}

		all->havetlndata=1;
    	stop_conn(mother,ser,K_WR_OK,dbg);
    } else stop_conn(mother,ser,ret,dbg);
}

void
showConf()
{
	doShowConf(toplevel);
}

void
dokwz()
{
	doKurzwahlen(toplevel);
}

void
dornz()
{
	doDisplayRNZ(toplevel);
}

void
doglob()
{
	displayGlobDat(toplevel);
}

void
dotlnperm()
{
	displayTlnPerm(toplevel);
}

void
dotlntyp()
{
	displayTeleTyp(toplevel);
}

void
doamtperm()
{
	displayAmtsPerm(toplevel);
}

void
docalldist()
{
	displayCallDist(toplevel);
}

void
dogebdat()
{
	displayGebDat(toplevel);
}

void
dosteuerk()
{
	displaySteuer(toplevel);
}

void
douserfkt()
{
	displayUserFkt(toplevel);
}

void
doOutKit()
{
	doKitOut(toplevel);
}

void
doInKit()
{
	doKitIn(toplevel);
}

void
dotxtunfoc(Widget w, XtPointer p, XtPointer data)
{
	char *dummy;

	XtVaSetValues(w,XmNcursorPositionVisible,False,NULL);
}

void
dotextfocus(Widget w, XtPointer p, XtPointer data)
{
	char *dummy;

	XtVaGetValues(w,XmNvalue,&dummy,NULL);
	XtVaSetValues(w,XmNcursorPositionVisible,True,NULL);
	XmTextSetInsertionPosition (w, strlen(dummy));
}

void
quitting()
{
	if(all) free(all);
	if (debugflag) fclose(debugfile);
	exit(0);
}

void Check_11_97(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	int ok,val;

    val=atoi(data->input);
	ok=(val >=11 && val <=97);
    data->okay=ok;
}

void Check_0_2359(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	int ok,val;

	sscanf(data->input,"%04X",&val);
	ok=(val >=0 && val <=0x2359);
    data->okay=ok;
}

void Check_0_999(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	int ok,val;

    val=atoi(data->input);
	ok=(val >=0 && val <=999);
    data->okay=ok;
}

void Check_0_255(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	int ok,val;

    val=atoi(data->input);
	ok=(val >=0 && val <=255);
    data->okay=ok;
}

void CheckLeadZero(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	int ok;

    ok=!hasLeadingZero(data->input);
    data->okay=ok;
}

void 
SetUpVars()
{
	all->absel=0;

	strcpy(all->cfgfilename,CFGPATH);
	strcat(all->cfgfilename,"/");
	strcat(all->cfgfilename,DEFAULT_CFGFILE);

	strcpy(cfgdirname,CFGPATH);
}

void
usage(char *prog)
{
		fprintf(stderr,"usage: %s </dev/cuaX>\n",basename(prog));
		exit(1);
}

void AllocateStructs()
{
	if (!(theConfBuf=(char *)malloc(MAXCONFFILESIZE))) {
		perror("Fehler beim alloziieren des CFG-Seichers");
		exit(1);
	}
	if (!(all=(struct GesamtKonfig *)malloc(sizeof(struct GesamtKonfig)))) {
		perror("Fehler bei der Speicherallokation");
		exit(1);
	}
}

main(int argc, char **argv)
{
    XtAppContext app;
	extern char *optarg;
	extern int optind;
	int c;

	AllocateStructs();
		
    toplevel = XmtInitialize (&app, "MaximaX", NULL, 0,
                    &argc, argv, fallb_rsrcs, NULL, 0);

	while ((c=getopt(argc,argv,"D:")) != EOF)
		switch(c) {
			case 'D':
				if ((debugfile=fopen("/tmp/maximaX.debug","w"))!=NULL) {
					debugflag=atoi(optarg);
				}
				break;
			default:
				usage(argv[0]);
		}

	if (optind < argc) {
		all->thedevname=argv[optind++];
	}

	if (!all->thedevname) usage(argv[0]);

    XmtRegisterAll();
    XmtCreateChildren(toplevel);
    XtRealizeWidget (toplevel);

    XmtAddDeleteCallback(toplevel,XmDO_NOTHING,quitting,NULL);

    XmtVaRegisterCallbackProcedures(
          "quitting",     quitting,       XmtRCallbackUnused,
          "showConf",     showConf,       XmtRCallbackUnused,
          "dokwz",        dokwz,          XmtRCallbackUnused,
          "dornz",        dornz,          XmtRCallbackUnused,
          "doglob",       doglob,         XmtRCallbackUnused,
          "dotlnperm",    dotlnperm,      XmtRCallbackUnused,
          "dotlntyp",     dotlntyp,       XmtRCallbackUnused,
          "doamtperm",    doamtperm,      XmtRCallbackUnused,
          "docalldist",   docalldist,     XmtRCallbackUnused,
          "dogebdat",     dogebdat,       XmtRCallbackUnused,
          "dosteuerk",    dosteuerk,      XmtRCallbackUnused,
          "douserfkt",    douserfkt,      XmtRCallbackUnused,
          "doOutKit",     doOutKit,       XmtRCallbackUnused,
          "doInKit",      doInKit,        XmtRCallbackUnused,
          "dotextfocus",  dotextfocus,    XmtRCallbackUnused,
          "dotxtunfoc",   dotxtunfoc,     XmtRCallbackUnused,
          "loadCFGFile",  loadCFGFile,    XmtRCallbackUnused,
          "saveCFGFile",  saveCFGFile,    XmtRCallbackUnused,
          "saveCFGFileAs",saveCFGFileAs,  XmtRCallbackUnused,
          "readConfig",   readConfig,     XmtRCallbackUnused,
          "writeConfig",  writeConfig,    XmtRCallbackUnused,
          "readTlnConfig", readTlnConfig, XmtRCallbackUnused,
          "writeTlnConfig",writeTlnConfig,XmtRCallbackUnused,
          "CheckLeadZero", CheckLeadZero, XmtRCallbackUnused,
          "Check_11_97",  Check_11_97,    XmtRCallbackUnused,
          "Check_0_255",  Check_0_255,    XmtRCallbackUnused,
          "Check_0_999",  Check_0_999,    XmtRCallbackUnused,
          "Check_0_2359", Check_0_2359,   XmtRCallbackUnused,
          NULL );

#ifdef USE_EDITRES
	XtAddEventHandler(toplevel,0,True,_XEditResCheckMessages,NULL);
#endif

	doSetupKwz();
	doSetupConf();
	doSetupRNZ();
	doSetupPerm();
	doSetupCallDist();
	doSetupGebDat();
	doSetupZaehler();
	doSetupUser();
	doSetupKit();

	SetUpVars();
	maximax_vers();

    XtAppMainLoop(app);
}
