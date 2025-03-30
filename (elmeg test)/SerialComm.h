/*** *	Library f�r die serielle Schnittstelle ***/#pragma once	// include this header				// only once per source file#include <Serial.h>//#define	SERIALBUF	8192L		// Gr��e des Eingangsbuffers#define	XOn			0x11		// Software-Handshake-Codes#define	XOff		0x13#define	SDInitPara	baud9600+data8+stop10+noParityclass SerialDriver {	Str63			SDName;			// Name des aktiven Ports	short			SDIn,SDOut;		// RefNum f�r Lesen und Schreiben	short			SDPort;			// aktueller Ausgabeport	StringPtr		SDInBuf;		// Buffer f�r den Eingang	unsigned short	SDTimeout;		// Timeoutl�nge beim Empfang in 60Hz	OSErr			SDCBOpenErr;	void	Lookup();	Boolean	CBOpen(short index,StringPtr portname,StringPtr indriver,StringPtr outdriver);	void	DebugPrint(char* header,StringPtr s,unsigned short len);	void	Open(void);	void	Close(void);public:	SerialDriver(StringPtr drvName) { BlockMoveData(drvName, SDName, drvName[0]+1); SDPort = 0; Open(); };	~SerialDriver() { Close(); };	OSErr	Wait(unsigned short timeout,long *count);	void	Write(StringPtr s,unsigned short count);	short	Read(StringPtr buf,unsigned short maxbytes);	void	DTR(Boolean flag);	void	SetTimeout(const unsigned short timeout) { SDTimeout = timeout; };};