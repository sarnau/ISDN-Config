/*** *	Library f�r die serielle Schnittstelle ***/#define SERDEBUG		0#define CTB				1#include "SerialComm.h"#include "GlobalLib.h"#include "Utilities.h"#if SERDEBUG#include <stdio.h>#endif#include <Serial.h>#if CTB#include <CommResources.h>#include <CRMSerialDevices.h>#endifStr63		SDName;			// Name des aktiven PortsWORD		SDIn,SDOut;		// RefNum f�r Lesen und SchreibenWORD		SDPort = 0;		// aktueller AusgabeportUSTR		SDInBuf;		// Buffer f�r den EingangWORD		SDBreakChar;	// Break-Char (-1 = keins)UWORD		SDChecksum;		// Pr�fsumme f�r Lesen VOM OrganizerUWORD		SDTimeout;		// Timeoutl�nge beim Empfang in 60HzWORD		SDInitPara = baud9600+data8+stop10+noParity;char PortAUse : 0x290;		// sind in der Think C Library nicht definiertchar PortBUse : 0x291;char SpConfig : 0x1FB;/*** *	alle seriellen Ports ermitteln ***/VOID	SDLookup(Boolean (*callback)(WORD index,USTR portname,USTR indriver,USTR outdriver)){REG CRMRecPtr		theCRM;CRMRec				theCRMRec;REG CRMSerialPtr	theSerial;REG WORD			old,index;REG Boolean			flag;CHAR				index1,index2,index3;	if(gCTB) {								// CTB vorhanden?		old = 0;							// ID des Ports		index = 1;							// Index des Ports		while(1) {			theCRMRec.crmDeviceType = crmSerialDevice;			theCRMRec.crmDeviceID = old;			theCRM = &theCRMRec;			theCRM = (CRMRecPtr)CRMSearch((QElemPtr)theCRM); // Port suchen			if (!theCRM) 				break;						// keinen weiteren Port gefunden => raus			theSerial = (CRMSerialPtr)(theCRM->crmAttributes);			index1 = HGetState(theSerial->name);			index2 = HGetState(theSerial->inputDriverName);			index3 = HGetState(theSerial->outputDriverName);			HLock(theSerial->name);			HLock(theSerial->inputDriverName);			HLock(theSerial->outputDriverName);			flag = (*callback)(index,*(theSerial->name),*(theSerial->inputDriverName),							*(theSerial->outputDriverName));			HSetState(theSerial->name,index1);			HSetState(theSerial->inputDriverName,index2);			HSetState(theSerial->outputDriverName,index3);			if (flag) break;				// Treiber gefunden => raus			old = theCRM->crmDeviceID;		// der n�chste Port			index++;		}	} else {		Str255		sName,sIn,sOut;		for(index=1;;index++) {			GetIndString(&sName,1004,(index-1)*3+1);			if (!sName[0]) break;			GetIndString(&sIn,1004,(index-1)*3+2);			GetIndString(&sOut,1004,(index-1)*3+3);			if ((*callback)(index,sName,sIn,sOut)) break;		}	}}OSErr		SDCBOpenErr;Boolean	SDCBOpen(WORD index,USTR portname,USTR indriver,USTR outdriver);Boolean	SDCBOpen(WORD index,USTR portname,USTR indriver,USTR outdriver){REG OSErr	err;	if (!EqualString(portname,SDName,false,true)) return(false);	// Port gefunden?	err = OpenDriver(indriver,&SDIn);	if (err) {		SDIn = 0;						// Ports l�schen		SDCBOpenErr = err;		return(true);					// Fehler beim �ffnen => Abbruch	}	err = OpenDriver(outdriver,&SDOut);	if (err) {		CloseDriver(SDIn);				// Eingangstreiber schlie�en		SDIn = SDOut = 0;				// Ports l�schen		SDCBOpenErr = err;		return(true);					// Fehler beim �ffnen => Abbruch	}	SDPort = index;						// aktiver Port	SDCBOpenErr = noErr;	return(true);}/*** *	seriellen Port wieder abmelden ***/OSErr	SDClose(VOID){REG OSErr	err = noErr;	if (SDInBuf) {			// seriellen Buffer freigeben		SerSetBuf(SDIn,nil,(short)nil);		DisposPtr(SDInBuf);		SDInBuf = nil;	}	if (SDIn) {				// seriellen Port schlie�en		err = CloseDriver(SDIn);		SDIn = 0;	}	if (SDOut) {		if (err)			CloseDriver(SDOut);		else			err = CloseDriver(SDOut);		SDOut = 0;	}	SDPort = 0;	return(err);}/*** *	seriellen Port mit 9600 Baud 8N1 XON/XOFF anmelden ***/OSErr	SDOpen(VOID){STATIC SerShk	s = { false,false,XOn,XOff,parityErr + hwOverrunErr + framingErr,0,true,0 };REG OSErr		err;	if (SDPort>0) SDClose();	SDLookup(SDCBOpen);			// Treiber suchen und �ffnen	if (SDCBOpenErr) return(SDCBOpenErr);	// kein Treiber gefunden!	SDTimeout = 60;				// Timeout = 1s	SDBreakChar = -1;			// kein Break-Zeichen	err = SerReset(SDOut,SDInitPara);	if (err) {		SDClose();		return(err);	}	err = SerReset(SDIn,SDInitPara);	if (err) {		SDClose();		return(err);	}	err = SerHShake(SDOut,&s);	if (err) {		SDClose();		return(err);	}	SDInBuf = (USTR)NewPtr(SERIALBUF);	if (SDInBuf)			// gr��eren Buffer f�r den Eingang		SerSetBuf(SDIn,(Ptr)SDInBuf,SERIALBUF);	return(err);}/*** *	auf Bytes an der Seriellen warten ***/OSErr	SDWait(REG UWORD timeout,REG ULONG *count){REG OSErr	err;REG ULONG	time = TickCount() + timeout;REG Boolean	flag = false;	while(true) {		if (time < TickCount()) flag = true;// Timeout?		err = SerGetBuf(SDIn,(long*)count);	// Bytes im In-Buffer abfragen		if (err) return(err);				// OS-Error		if (*count > 0) return(noErr);		// es sind Bytes da		if (UserAbort()) return(userCanceledErr); // Anwender bricht ab		if (flag) return(1);				// Timeout	}}/*** *	String der L�nge �len� im Stdio-Window mit Steuerzeichen ausgeben ***/#if SERDEBUGvoid		DebugPrint(STR header,USTR s,UWORD len);void		DebugPrint(STR header,USTR s,UWORD len){REG WORD	i;REG UCHAR	cc;static UCHAR tab[] = {		"NULSOHSTXETXEOTENQACKBELBS TABLF VT FF CR SO SI "		"DLEDC1DC2DC3DC4NAKSYNETBCANEM SUBESCFS GS RS US " };static FILE	*f = nil;	if (!f) {		f = stdout;//		f = fopen("INPUT","w+");	}	fprintf(f,"%s",header);	for(i=0;i<len;i++) {		cc = s[i];		if (cc < 32) {			cc *= 3;			if (tab[cc+2] == ' ')				fprintf(f,"<%c%c:%d>",tab[cc],tab[cc+1],s[i]);			else				fprintf(f,"<%c%c%c:%d>",tab[cc],tab[cc+1],tab[cc+2],s[i]);		} else {			REG WORD j = i; REG BYTE temp;			while((s[i]>=32)&&(i<len)) i++;			temp = s[i]; s[i] = 0;			fprintf(f,"%s",&s[j]);			s[i--] = temp;		}	}	fprintf(f,"\n",SDChecksum);}#endif/*** *	einen String �ber die serielle Schnittstelle senden ***/OSErr	SDWrite(REG USTR s,REG UWORD count){REG OSErr	err;ULONG		c = count;#if SERDEBUG	DebugPrint("MAC:",s,count);#endif	err = FSWrite(SDOut,&c,s);			// String senden	if ((count != c)&&(err == noErr))		err = ioErr;					// Bytes nicht gesendet	if (!err) {		REG WORD	i;		for(i=0;i<count;i++)			// Pr�fsumme �ber den String			SDChecksum += (UWORD)s[i];	// Pr�fsumme updaten	}	if (UserAbort()) return(userCanceledErr); // Anwender bricht ab	return(err);}/*** *	zwei Bytes (Befehl) �ber die serielle Schnittstelle senden ***/OSErr	SDWriteCmd(UWORD cmd){	return(SDWrite((USTR)&cmd,2));}/*** *	count Bytes �ber die serielle Schnittstelle einlesen ***/OSErr	SDRead(REG USTR buf,REG UWORD maxbytes){REG OSErr	err;		// Fehlercode, der letzten OperationULONG		count;		// Anzahl der vorhandenen/gelesenen BytesREG UWORD	sum = 0;	// Anzahl der gelesenen Bytes#if SERDEBUGUSTR		bufstart = buf;	// Anfangsadresse vom Empfangsbuffer#endif	do {		err = SDWait(SDTimeout,&count);	// 1s Timeout		if ((err)||(!count)) break;		// Abbruch, wenn Fehler, oder alles gelesen		count = 1;		err = FSRead(SDIn,&count,buf);	// ein Byte aus dem Buffer einlesen		if (err) break;					// Abbruch, wenn Fehler		if (count != 1) return(ioErr);	// Es fehlen Bytes!		sum++;							// Zeichen z�hlen		SDChecksum += (unsigned short)*buf;	// Pr�fsumme updaten		if ((SDBreakChar>=0)&&(*buf == SDBreakChar)) {#if SERDEBUG			DebugPrint("IQ :",bufstart,sum);#endif			return(sum);				// Break-Char gefunden?		}		buf++;							// zum n�chsten Zeichen	} while(sum < maxbytes);			// alle Bytes gelesen?	if (err<0) return(err);				// Fehlercode zur�ckgeben	if (!sum) return(volOffLinErr);		// keine Antwort!#if SERDEBUG	DebugPrint("IQ :",bufstart,sum);#endif	return(sum);						// Anzahl der gelesenen Bytes}/*** *	Break-Char setzen ***/VOID	SDSetBrk(WORD brk){	SDBreakChar	= brk;}