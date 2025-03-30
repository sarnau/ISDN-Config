#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Xmt/All.h>
#include <Xm/List.h>

#include "is1.h"
#include "maximaX.h"

extern struct GesamtKonfig *all;

Widget userdlg;
char tlndirname[256];
char tlnpatbuf[256];
int	   lastTln;

void activateTndButtons()
{
		set_active(userdlg,"*whichone",all->havetlndata);
		set_active(userdlg,"*settings1",all->havetlndata);
		set_active(userdlg,"*settings2",all->havetlndata);
		set_active(userdlg,"*weckz",all->havetlndata);
		set_active(userdlg,"*nbb",all->havetlndata);
		set_active(userdlg,"*numl",all->havetlndata);
		set_active(userdlg,"*nnz",all->havetlndata);
		set_active(userdlg,"*rufz",all->havetlndata);
		set_active(userdlg,"*save",all->havetlndata);
		set_active(userdlg,"*load",all->havetlndata);
		set_active(userdlg,"*write",all->havetlndata);
		setChooserState(userdlg,"*whichone",0);
}

void
addS0(int port)
{
	Widget list;
	XmString item[10];
	char *numname;
	char line[20];
	int	i;

	if ((list  = XmtNameToWidget(userdlg, "*whichone.listSW.list"))==NULL)
		exit (1);

    for (i=0;i<10;i++) {
		numname=(char *)getAppNumStr(((port+1)*10)+i);
        sprintf(line,"%s (%s)", getAppName(((port+1)*10)+i),
								(numname)?numname : "\0");
        item[i] = XmStringCreateSimple(line);
    }

	XmListAddItems(list, item,10, 0);
	for (i=0;i<10;i++) XmStringFree(item[i]);
}

void
addAB()
{
	Widget list;
	XmString item[10];
	char *numname;
	char line[20];
	int	i;

	if ((list  = XmtNameToWidget(userdlg, "*whichone.listSW.list"))==NULL)
		exit (1);

	XmListDeleteAllItems(list);

    for (i=0;i<10;i++) {
		numname=(char *)getAppNumStr(i);
        sprintf(line,"%s (%s)", getAppName(i),
								(numname)?numname : "\0");
        item[i] = XmStringCreateSimple(line);
    }

	XmListAddItems(list, item,10, 0);
	for (i=0;i<10;i++) XmStringFree(item[i]);
}


void
doUser(int whichone)
{
	char    dummy[20];
	int 	status1;
	int 	status2;

	if (all->tlnData[whichone].WeckZeit != 0xFFFF) {
		sprintf(dummy,"%04X",all->tlnData[whichone].WeckZeit);
		SetField(userdlg,"*weckz",dummy);
	} else {
		SetField(userdlg,"*weckz","----");
	}

	SetField(userdlg,"*nbb",all->tlnData[whichone].CFB_Id);

	if (!all->tlnData[whichone].CFNR_Zeit) {
		SetField(userdlg,"*numl",all->tlnData[whichone].CFNR_Id);
		SetField(userdlg,"*nnz","");
		SetField(userdlg,"*rufz","");
	} else {
		SetField(userdlg,"*nnz",all->tlnData[whichone].CFNR_Id);
		SetField(userdlg,"*numl","");
		if (all->tlnData[whichone].CFNR_Id[0]!='\0') {
			sprintf(dummy,"%d",all->tlnData[whichone].CFNR_Zeit);
			SetField(userdlg,"*rufz",dummy);
		} else {
			SetField(userdlg,"*rufz","");
		}
	}
	status1=(int)all->tlnData[whichone].SchaltStatus & 0x00FF;
	setChooserState(userdlg,"*settings1",status1);
	status2=(int)((all->tlnData[whichone].SchaltStatus >> 8) & 0x00FF);
	setChooserState(userdlg,"*settings2",status2);
	lastTln = whichone;
}

void
getNsetUser(int whichone)
{
	char    dummy[20];
	int 	status1;
	int 	status2;
	int		rwtime;

	getChooserState(userdlg,"*settings1",&status1);
	getChooserState(userdlg,"*settings2",&status2);
	all->tlnData[lastTln].SchaltStatus=
				(unsigned short)((status2 << 8) + status1);

	if (all->tlnData[lastTln].WeckZeit != 0xFFFF)
		sscanf((char *)GetField(userdlg,"*weckz"),"%x",
			&all->tlnData[lastTln].WeckZeit);

	sscanf((char *)GetField(userdlg,"*rufz"),"%d",&rwtime);
	all->tlnData[lastTln].CFNR_Zeit=(unsigned char)rwtime;

	if (!all->tlnData[lastTln].CFNR_Zeit) {
		strcpy((char *)all->tlnData[lastTln].CFNR_Id,
			(char *)GetField(userdlg,"*numl"));
	} else {
		strcpy((char *)all->tlnData[lastTln].CFNR_Id,
			(char *)GetField(userdlg,"*nnz"));
	}
	strcpy((char *)all->tlnData[lastTln].CFB_Id,
			(char *)GetField(userdlg,"*nbb"));


	doUser(whichone);
}

void displayUserFkt(Widget mother)
{
    Boolean port1isOK;
    Boolean port2isOK;
	char	s_start[10];
	char	s_end[10];
	int akt1,akt2,dummy;

    port1isOK=hasPort(0,1);
    port2isOK=hasPort(0,2);

	if (!userdlg) {
        userdlg=XmtBuildDialog(mother, "tndat_dlg",
                NULL, 0);
        if (!userdlg) exit(1);
    }

	addAB();
	if (port1isOK) addS0(0);
	if (port2isOK) addS0(1);

	lastTln=0;
	doUser(lastTln);

	if (XmtDialogDoSync(userdlg,NULL)) {
		all->tlnisdirty++;
	}

}

void
selectTDat(Widget w, XtPointer p, XtPointer data)
{
	XmtChooserCallbackStruct *ccb=(XmtChooserCallbackStruct *)data;
	int dummy=ccb->item;

	getNsetUser(dummy);
}

void
loadTlnDat(Widget w, XtPointer p, XtPointer data)
{
    FILE *fp;
    int i;
    int len;

    if (XmtAskForFile(userdlg,"tlnload_dlg","Datei laden:",CFGPATH,
            "*.tnd",&fp,"r",all->tlnfilename,256,tlndirname,256,
            tlnpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
			for (i=0;i<30;i++) {
            	fread(&all->tlnData[i].SchaltStatus,1,2,fp);
            	fread(&all->tlnData[i].WeckZeit,1,2,fp);
            	fread(&all->tlnData[i].CFB_Id,1,RUF_NR_LEN+1,fp);
            	fread(&all->tlnData[i].CFNR_Id,1,RUF_NR_LEN+1,fp);
            	fread(&all->tlnData[i].CFNR_Zeit,1,1,fp);
			}
	        all->tlnisdirty=0;
            all->havetlndata++;
            all->havetlnfile++;
			activateTndButtons();
			lastTln=0;
			doUser(lastTln);
        }
        fclose(fp);
	}
}

void
saveTlnDat(Widget w, XtPointer p, XtPointer data)
{
    FILE *fp;
	int i;

	getNsetUser(lastTln);
    if (XmtAskForFile(userdlg,"tlnsave_dlg","Datei sichern:",CFGPATH,
            "*.tnd",&fp,"w",all->tlnfilename,256,tlndirname,256,
            tlnpatbuf,256,"No Help!")==True) {
        if (isNormalFile(fp)) {
			for (i=0;i<30;i++) {
            	fwrite(&all->tlnData[i].SchaltStatus,1,2,fp);
            	fwrite(&all->tlnData[i].WeckZeit,1,2,fp);
            	fwrite(&all->tlnData[i].CFB_Id,1,RUF_NR_LEN+1,fp);
            	fwrite(&all->tlnData[i].CFNR_Id,1,RUF_NR_LEN+1,fp);
            	fwrite(&all->tlnData[i].CFNR_Zeit,1,1,fp);
			}
            all->tlnisdirty=0;
            all->havetlnfile++;
        }
        fclose(fp);
    }
}

void
readTlnDat(Widget w, XtPointer p, XtPointer data)
{
	readTlnConfig(userdlg);
	if (all->havetlndata) {
		activateTndButtons();
		lastTln=0;
		doUser(lastTln);
	}
}

void
writeTlnDat(Widget w, XtPointer p, XtPointer data)
{
	getNsetUser(lastTln);
	if (all->havetlndata) {
		writeTlnConfig(userdlg);
	}
}

void Check_NNZ(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	Widget mother;
    int ok,val;

    if (data->input && strlen(data->input)) {
		mother=XtParent(w);
		if (mother) SetField(mother,"*numl","");
		if (mother && (!strlen((char *)GetField(mother,"*rufz")))) {
			SetNumField(mother,"*rufz",-1,all->tlnRufvert[0].RwsTime);
		}
	}
    data->okay=True;
}

void Check_NUML(Widget w, XtPointer client_data, XtPointer call_data)
{
    XmtInputFieldCallbackStruct *data=
        (XmtInputFieldCallbackStruct *)call_data;
	Widget mother;
    int ok,val;

    if (data->input && strlen(data->input)) {
		mother=XtParent(w);
		if (mother) SetField(mother,"*nnz","");
		if (mother) SetField(mother,"*rufz","");
	}
   	data->okay=True;
}

void doSetupUser()
{
	XmtVaRegisterCallbackProcedures(
		"selectTDat", selectTDat, XmtRCallbackUnused,
		"writeTlnDat",writeTlnDat,XmtRCallbackUnused,
		"readTlnDat", readTlnDat, XmtRCallbackUnused,
		"loadTlnDat", loadTlnDat, XmtRCallbackUnused,
		"saveTlnDat", saveTlnDat, XmtRCallbackUnused,
		"Check_NNZ",  Check_NNZ,  XmtRCallbackUnused,
		"Check_NUML", Check_NUML, XmtRCallbackUnused,
		NULL );
    strcpy(all->tlnfilename,CFGPATH);
    strcat(all->tlnfilename,"/");
    strcat(all->tlnfilename,DEFAULT_TNDFILE);
    strcpy(tlndirname,CFGPATH);
}
