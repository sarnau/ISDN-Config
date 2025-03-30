#pragma onceclass CCRCTable {	Uint16		*mCRCTab;public:	CCRCTable();	~CCRCTable() { delete mCRCTab; };	const Uint16	*GetTable() const { return mCRCTab; };};class CSerialNo {private:	enum { cBufSize = 10 };	typedef Uint8			tSerHalfBuf[cBufSize / 2];	typedef Uint8			tSerBuf[cBufSize];	typedef LStr255			tSerialNo;	ConstStringPtr			mSerCodes;	// Bit "inBit" ab Adresse "inAddr" testen	Boolean			TestBit(const Uint8 *inAddr, Uint16 inBit) const;	// ASCII-Byte in 5-Bit wandeln und in den Datenstrom wegschreiben	void			ASC210Byte(Uchar serChar, tSerBuf &serBuf, Uint16 &count);	// 5 Bit aus dem Datenstrom in ASCII-Byte wandeln	Uchar			Byte2ASCII(const tSerBuf serBuf, Uint16 &count);	// ASCII-Seriennummer in Buffer umrechnen	void			CalcSerial2Buf(const tSerialNo serNum, tSerBuf &buf);	// Buffer in ASCII-Seriennummer umrechnen	void			CalcBuf2Serial(tSerialNo &oSerNumStr, const tSerBuf buf);	// 10 Byte in 2 x 5 Byte zerlegen	void			Extract2Pars(const tSerBuf buf, tSerHalfBuf &serNum, tSerHalfBuf &chkCode);	// 2 x 5 Byte zu 10 Byte zusammenf�gen	void			Merge2Pars(tSerBuf &buf, const tSerHalfBuf serNum, const tSerHalfBuf chkCode);	CCRCTable		mCRCTable;public:	CSerialNo() { mSerCodes = (ConstStringPtr)"hx6qzj2pt9l1rbmgdny3es7uak8v54cf"; };	// Seriennummer errechnen und testen	Boolean			Calc(tSerialNo &oSerNumStr, Uint16 iSerialNum, Uint8 iProductCode);	// Seriennummern-String auf G�ltigkeit testen	Uint16			Check(const tSerialNo serialNumStr, Uint8 &oProductCode);	// Pr�fsummcode errechnen	Uint16			CalcChecksumCode(Uint16 count);};