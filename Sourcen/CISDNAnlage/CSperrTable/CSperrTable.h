// ===========================================================================//	CSperrTable.cp					� 1996, Markus Fritze. All rights reserved.// ===========================================================================#pragma once#include <LWindow.h>#include <LTableView.h>#include <LAttachment.h>#include "CISDNAnlage.h"#include "PPFactory.h"// ---------------------------------------------------------------------------//		� Class CSperrTable// ---------------------------------------------------------------------------class	CSperrTable :	 public LWindow, public LAttachment {public:	static bool	sRegisterClass;public:	enum { class_ID = 'SpTa' };							CSperrTable() : LAttachment(msg_AnlageDirty, nil) {};						CSperrTable(SWindowInfo &inWindowInfo) : LWindow(inWindowInfo), LAttachment(msg_AnlageDirty, nil) {};						CSperrTable(ResIDT inWINDid, UInt32 inAttributes,								LCommander *inSuperCommander) : LWindow(inWINDid, inAttributes, inSuperCommander), LAttachment(msg_AnlageDirty, nil) {};						CSperrTable(LStream *inStream) : LWindow(inStream), LAttachment(msg_AnlageDirty, nil) {};	virtual void		FinishCreateSelf();protected:	CISDNAnlage			*mAnlage;	virtual void	ExecuteSelf(							MessageT		inMessage,							void			*ioParam);};// ---------------------------------------------------------------------------//		� Class CSperrTableView// ---------------------------------------------------------------------------class	CSperrTableView :	 public LTableView, public LCommander {public:	static bool	sRegisterClass;public:	enum { class_ID = 'SpTV' };							CSperrTableView() : LTableView() {};						CSperrTableView(const SPaneInfo &inPaneInfo, const SViewInfo &inViewInfo) : LTableView(inPaneInfo, inViewInfo) {};						CSperrTableView(LStream *inStream);	virtual Boolean		ClickSelect(								const STableCell		&inCell,								const SMouseDownEvent	&inMouseDown);	virtual void		ClickCell(								const STableCell		&inCell,								const SMouseDownEvent	&inMouseDown);protected:	CISDNAnlage			*mAnlage;	virtual void		DrawCell(								const STableCell		&inCell,								const Rect				&inLocalRect);};