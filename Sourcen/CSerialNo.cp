#if SHAREWARE_VERSION#include "CSerialNo.h"CCRCTable::CCRCTable(){	mCRCTab = new unsigned short[65536L];	Uint16		chksum = 0xfded;	for(Int32 count=0; count<65536; count++) {		mCRCTab[count] = chksum;		if(chksum & 1)			chksum = (chksum >> 1) ^ 0xb400;		else			chksum >>= 1;	}}static Uint8			mBitNo[80] = {				0x22,0x4A,0x34,0x0F,0x43,0x04,0x2B,0x30,				0x19,0x2F,0x0A,0x49,0x4D,0x0E,0x12,0x2A,				0x21,0x4E,0x40,0x28,0x3C,0x1A,0x07,0x46,				0x13,0x2C,0x14,0x1E,0x15,0x00,0x23,0x38,				0x41,0x3B,0x27,0x4F,0x4C,0x03,0x2E,0x17,				0x39,0x48,0x35,0x1F,0x33,0x05,0x4B,0x0C,				0x1C,0x44,0x24,0x29,0x3F,0x31,0x1D,0x42,				0x3D,0x0B,0x45,0x26,0x01,0x47,0x36,0x11,				0x3E,0x0D,0x16,0x20,0x06,0x37,0x1B,0x10,				0x08,0x02,0x25,0x32,0x3A,0x09,0x2D,0x18 };// Bit "inBit" ab Adresse "inAddr" testenBoolean	CSerialNo::TestBit(const Uint8 *inAddr, Uint16 inBit) const{	return (inAddr[inBit >> 3] & (1 << (inBit & 7))) != 0;}// ASCII-Byte in 5-Bit wandeln und in den Datenstrom wegschreibenvoid		CSerialNo::ASC210Byte(Uchar serChar, tSerBuf &serBuf, Uint16 &count){	// ASCII-Zeichen in 5-Bit-Index wandeln	Uint8	index;	for(index=0; index<32 && mSerCodes[index] != serChar; index++) {}	// 5 Bit verarbeiten	for(int i=0; i<5; i++) {		Uint8	bno = mBitNo[count++];		if(TestBit(&index, i))			serBuf[bno >> 3] |= 1 << (bno & 7);		else			serBuf[bno >> 3] &= ~(1 << (bno & 7));	}}// 5 Bit aus dem Datenstrom in ASCII-Byte wandelnUchar		CSerialNo::Byte2ASCII(const tSerBuf serBuf, Uint16 &count){Uint8	c = 0;	// 5 Bit verarbeiten	for(int i=0; i<5; i++) {		if(TestBit(serBuf, mBitNo[count++]))			c |= 1 << i;		else			c &= ~(1 << i);	}	return mSerCodes[c];}// ASCII-Seriennummer in Buffer umrechnenvoid		CSerialNo::CalcSerial2Buf(const tSerialNo serNum, tSerBuf &buf){	tSerialNo	theSerialNum = serNum;	::LowercaseText((Ptr)&theSerialNum[1], theSerialNum.Length(), false);	Uint16		theCount = 0;	for(int i=1; i<=theSerialNum.Length(); i++) {		Uchar	theChar = theSerialNum[i];		if(theChar >= '0')			ASC210Byte(theChar, buf, theCount);	}	for(theCount = 0; theCount<(cBufSize * 8); theCount += 2) {		short	c = theCount + 1;		if(TestBit(buf, theCount) ^ TestBit(buf, c))			buf[c >> 3] |= 1 << (c & 7);		else			buf[c >> 3] &= ~(1 << (c & 7));	}}// Buffer in ASCII-Seriennummer umrechnenvoid		CSerialNo::CalcBuf2Serial(tSerialNo &oSerNumStr, const tSerBuf inBuf){tSerBuf		theBuf;	BlockMoveData(inBuf, theBuf, sizeof(tSerBuf));	for(Uint16 count = 0; count<(cBufSize * 8); count += 2) {		Uint16	c = count + 1;		if(TestBit(theBuf, count) ^ TestBit(theBuf, c))			theBuf[c >> 3] |= 1 << (c & 7);		else			theBuf[c >> 3] &= ~(1 << (c & 7));	}	oSerNumStr = "\p";	Uint16 count = 0;	for(int i=0; i<16; i++) {		oSerNumStr += Byte2ASCII(theBuf, count);		if(i == 3 || i == 7 || i == 11)			oSerNumStr += "\p-";	}}// 10 Byte in 2 x 5 Byte zerlegenvoid		CSerialNo::Extract2Pars(const tSerBuf buf, tSerHalfBuf &serNum, tSerHalfBuf &chkCode){	for(int index=0; index<(cBufSize / 2 * 8); index++) {		int	i = index << 1;		if(TestBit(buf, i))			serNum[index >> 3] |= 1 << (index & 7);		else			serNum[index >> 3] &= ~(1 << (index & 7));		i++;		if(TestBit(buf, i))			chkCode[index >> 3] |= 1 << (index & 7);		else			chkCode[index >> 3] &= ~(1 << (index & 7));	}}// 2 x 5 Byte zu 10 Byte zusammenf�genvoid		CSerialNo::Merge2Pars(tSerBuf &buf, const tSerHalfBuf serNum, const tSerHalfBuf chkCode){	int		i = 0;	for(int index=0; index<(cBufSize / 2 * 8); index++) {		if(TestBit(serNum, index))			buf[i >> 3] |= 1 << (i & 7);		else			buf[i >> 3] &= ~(1 << (i & 7));		i++;		if(TestBit(chkCode, index))			buf[i >> 3] |= 1 << (i & 7);		else			buf[i >> 3] &= ~(1 << (i & 7));		i++;	}}Uint16		CSerialNo::CalcChecksumCode(Uint16 count){Uint16		chksum = 0xfded;	while(count-- > 0) {		if(chksum & 1)			chksum = (chksum >> 1) ^ 0xb400;		else			chksum >>= 1;	}	return chksum;}Uint16		CSerialNo::Check(const tSerialNo serialNumStr, Uint8 &oProductCode){tSerBuf		buf;tSerHalfBuf	serNum;tSerHalfBuf	chkCode;	CalcSerial2Buf(serialNumStr, buf);			// Seriennummer in 10 Byte Zahl umrechnen lassen	Extract2Pars(buf, serNum, chkCode);			// in Seriennummer und Pr�fzahl zerlegen	LStr255	theCodeCodeStr((void*)chkCode, 5);	if(theCodeCodeStr != "\p�iGMA")		return 0;	Uint16		theSerialNum = *(Uint16*)serNum;	if(CalcChecksumCode(theSerialNum) != *(Uint16*)(serNum + 3)) return 0;	oProductCode = serNum[2];	return theSerialNum;}Boolean		CSerialNo::Calc(tSerialNo &oSerNumStr, Uint16 iSerialNum, Uint8 iProductCode){tSerHalfBuf	serNum;tSerHalfBuf	chkCode = "�iGMA";	*(Uint16*)serNum = iSerialNum;			// Seriennummer	*(Uint8*)(serNum + 2) = iProductCode;			// Produktcode	*(Uint16*)(serNum + 3) = mCRCTable.GetTable()[iSerialNum];	tSerBuf		buf;	Merge2Pars(buf, serNum, chkCode);	CalcBuf2Serial(oSerNumStr, buf);	Uint8		theProductCode;	if(Check(oSerNumStr, theProductCode) == false || theProductCode != iProductCode) {		oSerNumStr = "\p";		return false;	}	return true;}#endif