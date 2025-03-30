#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"
#include "maximaX.h"

typedef struct KWZEntry{
    char name[38];
    char number[25];
} KWZEntryType;

XtResource edit_resources[] = {
{"name", "name", XmtRBuffer,
     XmtSizeOf(KWZEntryType, name), XtOffsetOf(KWZEntryType, name),
     XtRImmediate, NULL},
{"number", "number", XmtRBuffer,
     XmtSizeOf(KWZEntryType, number), XtOffsetOf(KWZEntryType, number),
     XtRImmediate, NULL},
};

extern struct GesamtKonfig *all;
extern int debugflag;
extern FILE *debugfile;

KWZEntryType	edit_data;
static Widget kwzdlg;
static Widget editor;
static Widget mover;
char thedirname[256]="";
char patbuf[256]="";

void
checkButtons()
{
	set_active(kwzdlg,"*save",(all->kwzisdirty && all->havekwzdata && all->havekwzname));
	set_active(kwzdlg,"*saveas",all->havekwzdata);
	set_active(kwzdlg,"*savetxt",all->havekwzdata);
	set_active(kwzdlg,"*print",all->havekwzdata);
	set_active(kwzdlg,"*progcfg",all->havekwzdata);
	set_active(kwzdlg,"*saveall",all->havekwzdata);
	set_active(kwzdlg,"*delcfg",all->havekwzdata);
	set_active(kwzdlg,"*editcfg",all->havekwzdata);
	set_active(kwzdlg,"*movecfg",all->havekwzdata);
	set_active(kwzdlg,"*mergenames",all->havekwzdata);
	set_active(kwzdlg,"*nahber_b",all->haveNuSdata);
	set_active(kwzdlg,"*sperrber_b",all->haveNuSdata);
    XmUpdateDisplay(kwzdlg);
}

int
getSelPos()
{
	Widget w;
	int *vals;
	int count;
	int ret;

	if (w=XmtNameToWidget(kwzdlg,"*ziele")) {
		XmListGetSelectedPos(w,&vals,&count);
		if (vals) {
			ret=vals[0];
			XtFree((char *)vals);
			return(ret);
		}
	}
	return(0);
}

void
replaceLine(int which,char *name, char *num)
{
	Widget w;
	XmString item;
	char line[100];

	if (w=XmtNameToWidget(kwzdlg,"*ziele")) {
		strcpy(all->kwz.Name[which].Name,name);
		memset(all->kwz.RNr[which].RufNr,0,RUF_NR_LEN);
		strcpy(all->kwz.RNr[which].RufNr,num);
		sprintf(line,"%4d %-38.38s %-25.25s", which+100,name, num);
		item = XmStringCreateSimple(line);
		XmListReplaceItemsPos(w, &item,1, which+1);
		XmStringFree(item);
		all->kwzisdirty++;
	}
}

void moveLine(int i)
{
	Widget w;
	XmString item;
	Boolean	weissnich;
	char line[100];
	int oldpos,newpos;

	oldpos=i;
	newpos=i+100;
	
	if (w=XmtNameToWidget(kwzdlg,"*ziele")) {
		strcpy(edit_data.name,all->kwz.Name[i].Name);
		strcpy(edit_data.number,all->kwz.RNr[i].RufNr);
		if (XmtAskForInteger(kwzdlg,"mover",
			"Neue Kurzwahl: (100-299)",&newpos,100,299,"No Help!")) {
			newpos-=100;
			if (newpos==oldpos) return;
			if (all->kwz.Name[newpos].Name[0]!='\0') {
				if (XmtDisplayWarningAndAsk(kwzdlg, "overwrite",
							"Alten Eintrag ueberschreiben?",
							"Ja","Nein",XmtNoButton,"No Help!")) {
					replaceLine(newpos,all->kwz.Name[i].Name,all->kwz.RNr[i].RufNr);
					replaceLine(oldpos,"","");
				}
			} else {
				replaceLine(newpos,all->kwz.Name[i].Name,all->kwz.RNr[i].RufNr);
				replaceLine(oldpos,"","");
			}
		}
		checkButtons();
	}
}

void editLine(int i)
{
	Widget w;
	XmString item;
	char line[100];
	
	if (w=XmtNameToWidget(kwzdlg,"*ziele")) {
		strcpy(edit_data.name,all->kwz.Name[i].Name);
		strcpy(edit_data.number,all->kwz.RNr[i].RufNr);
		if (!editor) {
			editor=XmtBuildDialog(kwzdlg,
				"editentry_dlg",edit_resources,
				XtNumber(edit_resources));
			if (!editor) exit(1);
		}
		if (XmtDialogDoSync(editor,&edit_data)) {
				replaceLine(i,edit_data.name, edit_data.number);
		}
		checkButtons();
	}
}

void
writeTextFile(FILE *fp)
{
	int i;
	int j=99;
	fprintf(fp,"%4s %-25.25s %12.12s\n%s\n",
		" Nr.","Name","Telefon",
		" ------------------------------------------");
	for (i=0;i<ANZ_KW_NR;i++) {
		if (i==100) {
			fprintf(fp,"%c",12);
			fprintf(fp,"%4s %-25.25s %12.12s\n%s\n",
				" Nr.","Name","Telefon",
				" ------------------------------------------");
		}
		if (all->kwz.RNr[i].RufNr[0]) {
			if ((i!=j+1) && (i!=100) && (i)) fprintf(fp," ...\n");
			fprintf(fp,"%4d %-25.25s %12.12s\n",
				i+100,all->kwz.Name[i].Name,all->kwz.RNr[i].RufNr);
			j=i;
		}
	}
}

void
showkwz()
{
	Widget list;
	XmString item[200];
	int i;
	char line[100];

	if (!all->havekwzdata) return;
	if ((list  = XmtNameToWidget(kwzdlg, "*ziele"))==NULL) exit (1);
	XmListDeleteAllItems(list);

	for (i=0;i<ANZ_KW_NR;i++) {
		sprintf(line,"%4d %-38.38s %-25.25s",
		i+100,all->kwz.Name[i].Name,all->kwz.RNr[i].RufNr);
		item[i] = XmStringCreateSimple(line);
	}
	XmListAddItems(list, item,200, 0);
	for (i=0;i<ANZ_KW_NR;i++) XmStringFree(item[i]);
	checkButtons();
}


static void editselct(Widget w, XtPointer tag, XtPointer data)
{
	XmListCallbackStruct *list_data = (XmListCallbackStruct *)data;
	int i=list_data->item_position - 1;

	editLine(i);
	
}

void
get4delete(Widget w, XtPointer tag, XtPointer data)
{
	int i;

	i=getSelPos();
	if (!i) return;
	i--;
	replaceLine(i,"","");
	checkButtons();
}

void
get4edit(Widget w, XtPointer tag, XtPointer data)
{
	int i;

	i=getSelPos();
	if (!i) return;
	i--;
	
	editLine(i);
}

void
get4move(Widget w, XtPointer tag, XtPointer data)
{
	int i;

	i=getSelPos();
	if (!i) return;
	i--;
	
	moveLine(i);
}

void
mergeFile()
{
	FILE *fp;
	char lthefilename[256]="";
	char lthedirname[256]="";
	char lpatbuf[256]="";

	if (!all->havekwzdata) {
		return;
	}

	strcpy(lthefilename,all->kwzfilename);
	strcpy(lthedirname,CFGPATH);
	if (XmtAskForFile(kwzdlg,"merge_dlg","Namen nachladen aus:",CFGPATH,
			"*.kwz",&fp,"r",lthefilename,256,lthedirname,256,
			lpatbuf,256,"No Help!")==True) {
		if (isNormalFile(fp)) {
			fseek(fp,(long)XtOffsetOf(KzWorkType,Name),SEEK_SET);
			fread(all->kwz.Name,1,sizeof(KzNamen)*ANZ_KW_NR,fp);
			all->kwzisdirty++;
			if (isCorrectFile(kwzdlg,lthefilename,"Kurzwahl-Datei",
					KWZ_KENNUNG,all->kwz.Kennung),strlen(KWZ_KENNUNG)) showkwz();
		}
		fclose(fp);
	}
}

void
printText()
{
	FILE *fp;

	if ((fp=popen("lpr","w"))!=NULL) {
		writeTextFile(fp);
		checkButtons();
		pclose(fp);
	}
}

void
saveFile()
{
	FILE *fp;

	if ((fp=fopen(all->kwzfilename,"w"))!=NULL) {
		if (isNormalFile(fp)) {
			fwrite(&all->kwz,1,sizeof(KzWorkType),fp);
			fwrite(&all->nahSperrBer,1,sizeof(NahSperrBerType),fp);
			all->kwzisdirty=0;
			checkButtons();
		}
		fclose(fp);
	}
}

void
saveFileas()
{
	FILE *fp;

	if (XmtAskForFile(kwzdlg,"save_dlg","Datei sichern:",CFGPATH,
			"*.kwz",&fp,"w",all->kwzfilename,256,thedirname,256,
			patbuf,256,"No Help!")==True) {
		if (isNormalFile(fp)) {
			fwrite(&all->kwz,1,sizeof(KzWorkType),fp);
			fwrite(&all->nahSperrBer,1,sizeof(NahSperrBerType),fp);
			all->kwzisdirty=0;
			checkButtons();
		}
		fclose(fp);
	}
}

void
saveText()
{
	FILE *fp;
	char lthefilename[256]="";
	char lthedirname[256]="";
	char lpatbuf[256]="";

	if (XmtAskForFile(kwzdlg,"txt_dlg","Text sichern:",CFGPATH,
			"*.txt",&fp,"w",lthefilename,256,lthedirname,256,
			lpatbuf,256,"No Help!")==True) {
		if (isNormalFile(fp)) {
			writeTextFile(fp);
			checkButtons();
		}
		fclose(fp);
	}
}

void
loadNshowFile(Widget mother,int showflg)
{
	FILE *fp;
	int i;
	int len;

	if (XmtAskForFile(mother,"load_dlg","Datei laden:",CFGPATH,
			"*.kwz",&fp,"r",all->kwzfilename,256,thedirname,256,
			patbuf,256,"No Help!")==True) {
		if (isNormalFile(fp)) {
			fread(&all->kwz,1,sizeof(KzWorkType),fp);
			fread(&all->nahSperrBer,1,sizeof(NahSperrBerType),fp);
			if (isCorrectFile(mother,all->kwzfilename,"Kurzwahl-Datei",
					KWZ_KENNUNG,all->kwz.Kennung,strlen(KWZ_KENNUNG))) {
				for(i=0;i<ANZ_KW_NR;i++) {
					len=strlen(all->kwz.RNr[i].RufNr);
					memset(&all->kwz.RNr[i].RufNr[len],0,RUF_NR_LEN-len);
				}
				all->kwzisdirty=0;
				all->havekwzdata++;
				all->haveNuSdata++;
				all->havekwzname++;
				if (showflg) showkwz();
			}
		}
		fclose(fp);
	}
}

void
loadFile()
{
	loadNshowFile(kwzdlg,TRUE);
}

void
readISkwz()
{
	Widget w;
	FILE *fp;
	char buf[1024];
	int i,ret;
	int ser=0;
	int dbg=debugflag;

	if ((ser=openSer(all->thedevname))==-1) return;

	w=XmtDisplayWorkingDialog(kwzdlg,"readwbox",
			"KWZ lesen", "Kurzwahl","Abbrechen",0,202,True,False);

	synchronize(ser);
	sendBOTR(ser,dbg);
	i=0;
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		do {
			memset(buf,0,1023);
			write_cmd(ser,K_READ_KURZ_ZENT_LIST,K_READ_CMD_OFFSET,i,dbg);
			if (!(ret=getNextPacket(kwzdlg,ser,buf,dbg))) return;
			memcpy(all->kwz.RNr[i].RufNr,buf+4,ret-4);
			i+=8;
			XmtWorkingBoxSetScaleValue(w,i);
		} while (i<200);

		write_cmd(ser,K_READ_NAH_BER_LIST ,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(kwzdlg,ser,buf,dbg))) return;
		memcpy(&all->nahSperrBer.Nahber,&buf[4],ret-4);
		XmtWorkingBoxSetScaleValue(w,i);
		i++;
		if(dbg) fprintf(debugfile,"K_READ_NAH_BER_LIST: %d\n",ret-4);

		write_cmd(ser,K_READ_SPERR_BER_LIST,K_READ_CMD_OFFSET,0,dbg);
		if (!(ret=getNextPacket(kwzdlg,ser,buf,dbg))) return;
		memcpy(&all->nahSperrBer.Sperrber,&buf[4],ret-4);
		XmtWorkingBoxSetScaleValue(w,i);
		if(dbg) fprintf(debugfile,"K_READ_SPERR_BER_LIST: %d %d\n",
					ret-4,sizeof(NahSperrBerType));

		stop_conn(kwzdlg,ser,K_WR_OK,dbg);
		strcpy(all->kwz.Kennung,KWZ_KENNUNG);
		all->havekwzdata++;
		all->kwzisdirty++;
		all->haveNuSdata++;
		showkwz();
	} else stop_conn(w,ser,ret,dbg);
}


void
writeISkwz()
{
	Widget w;
	FILE *fp;
	unsigned char buf[1024];
	int i,ret;
	int ser=0;
	int dbg=debugflag;

	if (!all->havekwzdata) return;
	if ((ser=openSer(all->thedevname))==-1) return;

	w=XmtDisplayWorkingDialog(kwzdlg,"writewbox",
			"KWZ schreiben", "Kurzwahl","Abbrechen",0,202,True,False);

	synchronize(ser);
	sendBOTW(ser,dbg);
	i=0;
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		do {
			if (!send_data(ser, K_WRITE_KURZ_ZENT_LIST,
					(unsigned char)i, all->kwz.RNr[i].RufNr,200,dbg))
			{
				stop_conn(kwzdlg,ser,ret,dbg);
				return;
			}

			if ((ret=isOK(ser,dbg))!=K_WR_OK){
				stop_conn(kwzdlg,ser,ret,dbg);
				return;
			}
			i+=8;
			XmtWorkingBoxSetScaleValue(w,i);
		
		} while (i<200);

		if (!send_data(ser, K_WRITE_NAH_BER_LIST,
				-1, &all->nahSperrBer.Nahber[0][0],sizeof(all->nahSperrBer.Nahber),dbg))
		{
			stop_conn(kwzdlg,ser,ret,dbg);
			return;
		}

		if ((ret=isOK(ser,dbg))!=K_WR_OK){
			stop_conn(kwzdlg,ser,ret,dbg);
			return;
		}
		XmtWorkingBoxSetScaleValue(w,++i);

		if (!send_data(ser, K_WRITE_SPERR_BER_LIST,
				-1, &all->nahSperrBer.Sperrber,(ANZ_SPERR_NR * (SPERR_NR_LEN + 1)),
				dbg))
		{
			stop_conn(kwzdlg,ser,ret,dbg);
			return;
		}

		if ((ret=isOK(ser,dbg))!=K_WR_OK){
			stop_conn(kwzdlg,ser,ret,dbg);
			return;
		}
		XmtWorkingBoxSetScaleValue(w,++i);

		stop_conn(kwzdlg,ser,K_WR_OK,dbg);
		showkwz();
	} else stop_conn(w,ser,ret,dbg);
}

void
doKurzwahlen(Widget mother)
{
	if (!kwzdlg) {
		kwzdlg=XmtBuildDialog(mother,"kwz_dlg",NULL,0);
		if (!kwzdlg) exit(1);
	}
	checkButtons();
	XmtDialogDoSync(kwzdlg,NULL);
}

void
closekwz()
{
	Widget list;

	if (all->kwzisdirty)
		if (XmtDisplayWarningAndAsk(kwzdlg, "dosave",
			"Wollen Sie die Aenderungen sichern?",
			"Ja","Nein",XmtYesButton,"No Help!")) {
				saveFile();
				all->kwzisdirty=0;
		}
}

void
showSperrBer()
{
	doDisplaySperrBer(kwzdlg);
}

void
showNahBer()
{
	doDisplayNahBer(kwzdlg);
}

void
doSetupKwz()
{
    XmtVaRegisterCallbackProcedures(
          "loadFile",   loadFile,   XmtRCallbackUnused,
          "saveFile",   saveFile,   XmtRCallbackUnused,
          "saveFileas", saveFileas, XmtRCallbackUnused,
          "saveText",   saveText,   XmtRCallbackUnused,
          "printText",  printText,  XmtRCallbackUnused,
          "mergeFile",  mergeFile,  XmtRCallbackUnused,
          "editselct",  editselct,  XmtRCallbackUnused,
          "get4delete", get4delete, XmtRCallbackUnused,
          "get4edit",   get4edit,   XmtRCallbackUnused,
          "get4move",   get4move,   XmtRCallbackUnused,
          "readISkwz",  readISkwz,  XmtRCallbackUnused,
          "writeISkwz", writeISkwz, XmtRCallbackUnused,
          "closekwz",   closekwz,   XmtRCallbackUnused,
          "showNahBer", showNahBer, XmtRCallbackUnused,
          "showSperrBer", showSperrBer, XmtRCallbackUnused,
          NULL );

	strcpy(all->kwzfilename,CFGPATH);
	strcat(all->kwzfilename,"/");
	strcat(all->kwzfilename,DEFAULT_KWZFILE);
	strcpy(thedirname,CFGPATH);
}
