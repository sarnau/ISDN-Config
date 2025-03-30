#define MAXINCALLS	75
#define MAXOUTCALLS	1000

typedef struct Outgoing_Calls
{
	unsigned long  Gebuehr     __attribute__ ((packed));
	unsigned short Dauer       __attribute__ ((packed));
	unsigned char  Appnum      __attribute__ ((packed)); /* BCD */
	unsigned char  Monat       __attribute__ ((packed));
	unsigned char  Tag         __attribute__ ((packed));
	unsigned char  Stunde      __attribute__ ((packed));
	unsigned char  Minute      __attribute__ ((packed));
	unsigned char  Nummer[12]  __attribute__ ((packed)); /* BCD; 0x?F ENDE*/
} Outgoing_CallsType;

/* Die Nummer ist im BCD-Format, sollte die Anzahl der Stellen ungerade
sein, so wird das letzte Nibble mit 0xF gefuellt */

#define NoConnect		0xAA
#define AnalogCaller	0xFF
typedef struct Incomming_Calls
{
	unsigned char  Appnum      __attribute__ ((packed)); /* BCD; 0xAA n.a.*/
	unsigned char  Monat       __attribute__ ((packed));
	unsigned char  Tag         __attribute__ ((packed));
	unsigned char  Stunde      __attribute__ ((packed));
	unsigned char  Minute      __attribute__ ((packed));
	unsigned char  Nummer[12]  __attribute__ ((packed)); /* BCD; 0xFF Analog*/
} Incomming_CallsType;
