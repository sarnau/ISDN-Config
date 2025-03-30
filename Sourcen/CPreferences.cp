#include "CPreferences.h"#include "CISDNAnlage.h"#include <UMemoryMgr.h>CPreferences	*gPrefs;CPreferences::CPreferences(){	// (a) Datei im Preferences Ordner lokalisieren	SInt16	theVRef;	SInt32	theDirID;	OSErr	theErr;	// Namen des Preferences File holen	LStr255	thePrefName;	thePrefName.Assign(ResIDT(1000), 9);	// Prefernces-Ordner suchen	theErr = ::FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &theVRef, &theDirID);	if (theErr == noErr) {		::FSMakeFSSpec(theVRef, theDirID, thePrefName, &mMacFileSpec);	} else {		LString::CopyPStr(thePrefName, mMacFileSpec.name, sizeof (Str63));	}	// (b) Datei �ffnen	try {		OpenDataFork(fsCurPerm);	}	catch(LException inErr) {		// (c) ggf. anlegen		CreateNewDataFile('EPAK', 'pref', smSystemScript);		OpenDataFork(fsCurPerm);	} 	CloseDataFork();	// (d) Preferences einlesen	ReadPrefs();}void	CPreferences::ReadPrefs(){	OpenDataFork(fsCurPerm);	try {		*this >> P.Version;	}	catch(LException inErr) {		P.Version = 0x0000;	} 	if(P.Version != 0x0110) {		P.Version = 0x0110;		P.Port = "\p";		P.DefHersteller = Anlage_Emmerich;		P.DefAnlage = 2;		P.DefSoftware = 200;		P.FirstLaunch = true;		BackupP.Version = 0x0000;	} else {		Str255	tempPort;		*this >> tempPort;		P.Port = tempPort;		*this >> P.DefHersteller;		*this >> P.DefAnlage;		*this >> P.DefSoftware;		*this >> P.FirstLaunch;		BackupP = P;	}	CloseDataFork();}void	CPreferences::WritePrefs(){	if(BlockCompare(&BackupP, &P, sizeof(P)) == 0) return;	OpenDataFork(fsCurPerm);	*this << P.Version;	*this << ConstStringPtr(P.Port);	*this << P.DefHersteller;	*this << P.DefAnlage;	*this << P.DefSoftware;	P.FirstLaunch = false;	*this << P.FirstLaunch;	CloseDataFork();}