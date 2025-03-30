/*** *	Library f�r die serielle Schnittstelle ***/#pragma once	// include this header				// only once per source file#include <Serial.h>#define	SERIALBUF	2048L		// Gr��e des Eingangsbuffers#define	XOn			0x11		// Software-Handshake-Codes#define	XOff		0x13extern	Str63		SDName;			// Name des aktiven Portsextern	WORD		SDIn,SDOut;		// RefNum f�r Lesen und Schreibenextern	WORD		SDPort;			// aktueller Ausgabeportextern	USTR		SDInBuf;		// Buffer f�r den Eingangextern	WORD		SDBreakChar;	// Break-Char (-1 = keins)extern	UWORD		SDChecksum;		// Pr�fsumme f�r Lesen VOM Organizerextern	UWORD		SDTimeout;		// Timeoutl�nge beim Empfang in 60Hzextern	WORD		SDInitPara;VOID	SDLookup(Boolean (*callback)(WORD index,USTR portname,USTR indriver,USTR outdriver));OSErr	SDClose(VOID);OSErr	SDOpen(VOID);OSErr	SDWait(UWORD timeout,ULONG *count);OSErr	SDWrite(USTR s,UWORD count);OSErr	SDWriteCmd(UWORD cmd);OSErr	SDRead(USTR buf,UWORD maxbytes);VOID	SDSetBrk(WORD brk);