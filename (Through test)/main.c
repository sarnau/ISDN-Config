#include "SerialComm.h"#include <stdio.h>#include <ctype.h>#include <string.h>#include <SIOUX.h>/*** *	Seriellen Datenstrom als ASCII ausgeben ***/static void		DebugPrint(char* header,StringPtr s,unsigned short len){static FILE	*f = nil;short	i;UInt16	cc;	if (!f) {		f = stdout;//		f = fopen("INPUT","w+");	}	fprintf(f,"%s<%d>:",header, len);	bool	wasDigit = false;	for(i=0; i<len; i++) {		cc = s[i];		if(!isprint(cc)) {			if(wasDigit == true)			{				fprintf(f, "\"");				wasDigit = false;			}			fprintf(f, "<0x%2.2x>", UInt16(cc));		} else {			if(wasDigit == false)			{				fprintf(f, "\"");				wasDigit = true;			}			fprintf(f, "%c", cc);		}	}	if(wasDigit == true)	{		fprintf(f, "\"");		wasDigit = false;	}	fprintf(f, "\n");}/*** *	main() ***/void		main(){	SIOUXSettings.asktosaveonclose = false;	SIOUXSettings.wasteusetempmemory = true;	SerialDriver	sdElmeg("\pModemanschlu§");	SerialDriver	sdPC("\pDruckeranschlu§");	sdPC.SetTimeout(0);	sdElmeg.SetTimeout(0);	puts("SD Debugger");	puts("===========");	again:	try {		UInt8	eBuf[512];		UInt8	pcBuf[512];		int		eBufPos = 0;		int		pcBufPos = 0;		do {			UInt8	theElmegChar = 0;			if(sdElmeg.Read(&theElmegChar, sizeof(UInt8)) > 0)			{				sdPC.Write(&theElmegChar, sizeof(UInt8));				eBuf[eBufPos++] = theElmegChar;			}			UInt8	thePCChar = 0;			if(sdPC.Read(&thePCChar, sizeof(UInt8)) > 0)			{				sdElmeg.Write(&thePCChar, sizeof(UInt8));				pcBuf[pcBufPos++] = thePCChar;			}			eBuf[eBufPos] = 0;			pcBuf[pcBufPos] = 0;			if(theElmegChar == 13)			{				DebugPrint("Elmeg", eBuf, eBufPos);				eBufPos = 0;			}			if(thePCChar == 13)			{				DebugPrint("PC", pcBuf, pcBufPos);				pcBufPos = 0;			}		} while(!Button());	} 	catch(...) {		puts("[Timeout]");		if(!Button())			goto again;	}}