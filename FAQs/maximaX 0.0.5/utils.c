/****************************************************************
 *
 * Kommentieren Sie Ihren Kot!
 *
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>
#include "is1.h"

extern struct GesamtKonfig *all;
extern FILE *debugfile;

char *getAppName(int which);
char *getAppNumStr(int which);
int hasLeadingZero(char *what);
int hasPort(int anl,int port);
void setItemCaptions(Widget mother,char *grpname,char *newstring,
					int from,int to);
void setItemLabels(Widget mother,char *grpname,char *newstring,
					int group,int from,int to);
void set_captionname(Widget mother, char *wname, char *newstring);
void set_labelname(Widget mother, char *wname, char *newstring);
int GetNumField(Widget mother, char *grpname,int item);
void SetNumField(Widget mother, char *grpname,int item,int val);
void SetField(Widget mother, char *wname,char *msg);
char *GetField(Widget mother, char *wname);
void set_activegrp(Widget mother, char *grpname,int item, int sense);
void set_active(Widget mother, char *wname, int sense);
void setInitial(Widget mother, char *wname);
void setChooserState(Widget mother, char *choosername,int state);
void getChooserState(Widget mother, char *choosername,int *state);
Boolean getChooserSense(Widget mother, char *choosername);
void disableChooserItem(Widget mother, char *choosername,int item,int onoff);
void disableChooser(Widget mother, char *choosername,int onoff);
char *get_Phonenumber(char *buf);
char *get_CallName(char *buf);
void showTransferErr(Widget mother,char *msg);
int isCorrectFile(Widget mother,char *fname,char *filetyp,char *kennung,
					char *found,int cmplen);
int isNormalFile(FILE *fp);
void stop_conn(Widget mother, int ser, int err, int dbg);
int getNextPacket(Widget mother,int ser, char *buf, int dbg);
void dumppacket(char *titel,char *buf,int len);
void printBin(char *title,int bin,int len);

#ifdef __FreeBSD__
char * basename(char * str);
#endif

char *appnames[]= {
	"AB  #1", "AB  #2", "AB  #3", "AB  #4", "AB  #5",
	"AB  #6", "AB  #7", "AB  #8", "TFE #1", "TFE #2",
	"SO  #0", "SO  #1", "SO  #2", "SO  #3", "SO  #4",
	"SO  #5", "SO  #6", "SO  #7", "SO  #8", "SO  #9",
	"S1  #0", "S1  #1", "S1  #2", "S1  #3", "S1  #4",
	"S1  #5", "S1  #6", "S1  #7", "S1  #8", "S1  #9",
	NULL };

char *getAppName(int which)
{
	return(appnames[which]);
}

char *getAppNumStr(int which)
{
	if (all->rufNrZuord[all->absel].Rufnummer[which][0]!='\0')
		return(all->rufNrZuord[all->absel].Rufnummer[which]);
	else return(NULL);
}

int hasLeadingZero(char *what)
{
	if (what && strlen(what))
		return(what[0]=='0');
	else
		return(0);
}

int hasPort(int anl,int port)
{
	return((all->iconf[all->absel].AnzPort >= port) && 
		(G_S0(all->iconf[all->absel].Port[port].PortTyp)==PORT_DIRECTION_INTERN));
}

void setItemCaptions(Widget mother,char *grpname,char *newstring,int from,int to)
{
    char wname[50];
    char nname[50];
	int j;

    for(j=from;j<=to;j++) {
        if (all->rufNrZuord[all->absel].Rufnummer[j][0]!='\0') {
            sprintf(wname,"%s%-d",grpname,j);
            sprintf(nname,"%s%s: ",newstring,
				all->rufNrZuord[all->absel].Rufnummer[j]);
            set_captionname(mother,wname,nname);
        }
    }
}

void setItemLabels(Widget mother,char *grpname,char *newstring,int group,int from,int to)
{
    char wname[50];
    char nname[50];
	int j;

    for(j=from;j<=to;j++) {
        if (all->rufNrZuord[all->absel].Rufnummer[j+group*10][0]!='\0') {
            sprintf(wname,"%s%-d.item%-d",grpname,group,j);
            sprintf(nname,"%s%s",newstring,
				all->rufNrZuord[all->absel].Rufnummer[j+group*10]);
            set_labelname(mother,wname,nname);
        }
    }
}

void set_labelname(Widget mother, char *wname, char *newstring)
{
    Widget w;
	XmString item;

    if ((w=XmtNameToWidget(mother,wname))) {
		item=XmStringCreateSimple(newstring);
		XtVaSetValues(w,XmNlabelString,item,NULL);
		XmStringFree(item);
    }
}

void set_captionname(Widget mother, char *wname, char *newstring)
{
    Widget w;
	XmString item;

    if ((w=XmtNameToWidget(mother,wname))) {
		item=XmStringCreateSimple(newstring);
		XtVaSetValues(w,XmtNlayoutCaption,item,NULL);
		XmStringFree(item);
    }
}

void SetNumField(Widget mother, char *grpname,int item,int val)
{
	char wname[30];
	char dummy[20];

	if (item >= 0)
		sprintf(wname,"%s%-d",grpname,item);
	else
		strcpy(wname,grpname);
	sprintf(dummy,"%d",val);
	SetField(mother,wname,dummy);
}

void SetField(Widget mother, char *wname,char *msg)
{
	Widget w;
    char *rvalue;

	if ((w=XmtNameToWidget(mother,wname))) {
        XmtInputFieldSetString(w,msg);
	} 
}

int GetNumField(Widget mother, char *grpname,int item)
{
	char wname[30];

	if (item >= 0)
		sprintf(wname,"%s%-d",grpname,item);
	else
		strcpy(wname,grpname);
	return(strtol(GetField(mother,wname),NULL,10));
}

char *GetField(Widget mother, char *wname)
{
	Widget w;
    char *rvalue;

	if ((w=XmtNameToWidget(mother,wname))) {
        return(XmtInputFieldGetString(w));
	} 
	return(NULL);
}

void set_activegrp(Widget mother, char *grpname,int item, int sense)
{
    Widget w;
	char wname[30];

	sprintf(wname,"%s%-d",grpname,item);
    if ((w=XmtNameToWidget(mother,wname))) {
		XtSetSensitive(w,sense);
    }
}

void set_active(Widget mother, char *wname, int sense)
{
    Widget w;

    if ((w=XmtNameToWidget(mother,wname))) {
		XtSetSensitive(w,sense);
    }
}

void setInitial(Widget mother, char *wname)
{
    Widget w;

    if ((w=XmtNameToWidget(mother,wname))) {
		XmtSetInitialFocus(mother,w);
	}
}

void setChooserState(Widget mother, char *choosername,int state)
{
    Widget w;

    if ((w=XmtNameToWidget(mother,choosername))) {
		XmtChooserSetState(w,state,False);
	}
}

void getChooserState(Widget mother, char *choosername,int *state)
{
    Widget w;

    if ((w=XmtNameToWidget(mother,choosername))) {
		*state=XmtChooserGetState(w);
	}
}

Boolean getChooserSense(Widget mother, char *choosername)
{
    Widget w;

    if ((w=XmtNameToWidget(mother,choosername))) {
		return(XtIsSensitive(w));
	}
	return(False);
}

void disableChooserItem(Widget mother, char *choosername,int item,int onoff)
{
    Widget w;
	int	numitems,i;

    if ((w=XmtNameToWidget(mother,choosername))) {
		XmtChooserSetSensitive(w,item,onoff);
	}
}

void disableChooser(Widget mother, char *choosername,int onoff)
{
    Widget w;
	int	numitems,i;

    if ((w=XmtNameToWidget(mother,choosername))) {
		XtSetSensitive(w,onoff);
	}
}

char *get_Phonenumber(char *buf)
{
    static char ret[25];
    char *p;
    int i;
    unsigned char a,b;

    p=ret;

    if (buf[0]==-1) return("-----");
    for (i=0;i<12;i++)  {
        a=(buf[i] >> 4) & 0x0F;
        b=buf[i] & 0x0F;
    	if (a==0x0B && b==0x0B) return("#9");
        if (a == 0x0F) {
            p[0]='\0';
            return(ret);
        }
        p[0]=(char)(a+48);
        p++;
        if (b == 0x0F) {
            p[0]='\0';
            return(ret);
        }
        p[0]=(char)(b+48);
        p++;
    }
    p[0]='\0';
    return(ret);
}

char *
get_CallName(char *phone)
{
	int i;

	if (!all->havekwzdata) return("");
	if (phone[0]=='-') return("Analoger Teilnehmer");
	if (phone[0]=='#') return("Privatgespräch");
	for (i=0;i<200;i++) {
		if (!strcmp(phone,all->kwz.RNr[i].RufNr)) return(all->kwz.Name[i].Name);
	}
	return("");
}

void showTransferErr(Widget mother,char *msg)
{
	XmtDisplayErrorMsg(mother,"tranfer_err",
		"Fehler bei der Uebertragung: \n%s",
		"Uebertragungs-Fehler",
		"No Help!",
		msg);
}

int isCorrectFile(Widget mother,char *fname,char *filetyp,char *kennung,
					char *found,int cmplen)
{
	int ret;

	ret=strncmp(kennung,found,cmplen)==0;
	if (!ret) {
		XmtDisplayErrorMsg(mother,"wrongFile",
			"Die Datei \"%s\" \nist kein %s!",
			"Datei-Fehler",
			"No Help!",
			fname,filetyp);
		return(0);
	} else {
		return(1);
	}
}

int isNormalFile(FILE *fp)
{
	struct stat thestat;
	int ret;

	if (!(ret=fstat(fileno(fp),&thestat))) {
		if (S_ISREG(thestat.st_mode)) return(1);
		return(0);
	} else return(0);
}

void stop_conn(Widget mother, int ser, int err, int dbg)
{
	char msg[100];

	if(dbg && err!=K_WR_OK) fprintf(debugfile,"ERROR: #%04X !!!!!!!!!!!!!!\n",err);
	if (err) synchronize(ser);
	sendEOT(ser,dbg);
	isOK(ser,dbg);
	closeSer(ser);
	XmtHideWorkingDialog(mother);
	XSetInputFocus(XtDisplay(mother),XtWindow(mother),RevertToParent,CurrentTime);
	switch (err) {
		case K_WR_OK:
		case 0:
			break;
		case K_CRC_ERR:
			showTransferErr(mother,"CRC-Fehler");
			break;
		case -1:
		case K_V24_ERR:
			showTransferErr(mother,"Uebertragungs-Fehler");
			break;
		case K_CMD_ERR:
			showTransferErr(mother,"CMD-Fehler");
			break;
		case K_CMD_PRIO_CHANGED:
			showTransferErr(mother,"Prio-Fehler");
			break;
		default:
			sprintf(msg,"Unbekannter Fehler %04X",err);
			showTransferErr(mother,msg);
			break;
	}
	return;
}

int getNextPacket(Widget mother,int ser, char *buf, int dbg)
{
	int ret;

	memset(buf,0,1024);
	ret=readAnswer(ser,buf,dbg);
	if (ret==-1){
    	stop_conn(mother,ser,K_V24_ERR,dbg);
    	return(0);
   	}
   	if (!isCheckSumOK(buf,ret,dbg)){
   		stop_conn(mother,ser,K_CRC_ERR,dbg);
   		return(0);
   	}
	return(ret);		
}

void dumppacket(char *titel,char *buf,int len)
{
	int i,j;

	fprintf(debugfile,"\n\n%s\n",titel);
	for(i=0;;i+=8) {
		for(j=0;j<8;j++) {
			if (i+j>len) break;
			fprintf(debugfile,"%02.2X ",(buf[i+j] & 0x0FF));
		}
		for(j=0;j<8;j++) {
			if (i+j>len) return;
			fprintf(debugfile,"%c ",
				(buf[i+j]>=32 && buf[i+j]<=126)?buf[i+j]:'.');
		}
		fprintf(debugfile,"\n");
	}
	fflush(debugfile);
}

void printBin(char *title,int bin,int len)
{
	int i;

	printf("%s",title);
	i=len;
	do {
		printf("%c",(bin & (0x01 << i-1)) ? '1' : '0');
		i--;
	} while (i);
	printf("\n");
}

#ifdef __FreeBSD__
char * basename(char * str)
{
    if(strrchr(str,'/') != NULL) {
        return((strrchr(str,'/')+1));
    }else{
        return(str);
    }
}
#endif
