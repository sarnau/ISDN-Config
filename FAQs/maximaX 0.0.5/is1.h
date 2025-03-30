#define KWZ_KENNUNG		"KwDaten"
#define KWZ_KENNUNG		"KwDaten"
#define CFG_KENNUNG		"Daten Konfigurator"
#define CFG_STRING_Q	"Daten Konfigurator MAXIMA IS-x V1.4"
#define CFG_STRING_A	"Daten Konfigurator EURACOM 18x V1.4"
#define CFG_KENNUNG_LEN 36

#define MSN_MAX_ANZ     10      /* Maximale Anzahl von MSN´s pro S0-Port */
#define MSN_MAX_LEN     9       /* Maximale Anzahl von Ziffern pro MSN */

#define RUF_NR_LEN      24      /* Maximale Länge einer Rufnummer */
#define KURZ_NAME_LEN   38
#define USER_TO_USER_LEN        32      /* Maximale Länge einer User to User Nachricht */
#define HAUPT_ANL_RNR_LEN       8       /* Maximale Länge der Hauptanlagenrufnummer */
#define MAX_RUF_VAR_LEN 30      /* Maximale Listenlänge */
#define NAH_BER_NR_LEN  5
#define K_CMD_OFFSET_MASK       (unsigned char)0xE0

#define K_READ_CMD_OFFSET       (unsigned char)0x00
#define K_WRITE_CMD_OFFSET      (unsigned char)0x20
#define K_SERVICE_CMD_OFFSET    (unsigned char)0x40
#define K_STATUS_CMD_OFFSET     (unsigned char)0x60

#define K_A_ANL_OFFSET  (unsigned char)0x00
#define K_B_ANL_OFFSET  (unsigned char)0x10

#define K_READ_SO_PORT  (unsigned char)( K_READ_CMD_OFFSET + 0x00 )
#define K_READ_S0_TLN   (unsigned char)( K_READ_CMD_OFFSET + 0x01 )
#define K_READ_TLN_RUFNR_LIST   (unsigned char)( K_READ_CMD_OFFSET + 0x02 )
#define K_READ_TLN_BER_LIST     (unsigned char)( K_READ_CMD_OFFSET + 0x03 )
#define K_READ_TLN_RUF_VERT_LIST        (unsigned char)( K_READ_CMD_OFFSET + 0x04 )
#define K_READ_TLN_GEB_KONF_LIST        (unsigned char)( K_READ_CMD_OFFSET + 0x05 )
#define K_READ_ST_KON_LIST      (unsigned char)( K_READ_CMD_OFFSET + 0x06 )
#define K_READ_NAH_BER_LIST     (unsigned char)( K_READ_CMD_OFFSET + 0x07 )
#define K_READ_SPERR_BER_LIST   (unsigned char)( K_READ_CMD_OFFSET + 0x08 )
#define K_READ_KURZ_ZENT_LIST   (unsigned char)( K_READ_CMD_OFFSET + 0x09 )
#define K_READ_ANL_DAT_LIST     (unsigned char)( K_READ_CMD_OFFSET + 0x0A )
#define K_READ_TLN_DATA_LIST    (unsigned char)( K_READ_CMD_OFFSET + 0x0B )
#define K_READ_TLN_GEB_LIST     (unsigned char)( K_READ_CMD_OFFSET + 0x0C )
#define K_READ_OUT_GEB_LIST     (unsigned char)( K_READ_CMD_OFFSET + 0x0D )
#define K_READ_IN_GEB_LIST      (unsigned char)( K_READ_CMD_OFFSET + 0x0E )

#define K_WRITE_SO_PORT (unsigned char)( K_WRITE_CMD_OFFSET + 0x00 )
#define K_WRITE_S0_TLN  (unsigned char)( K_WRITE_CMD_OFFSET + 0x01 )
#define K_WRITE_TLN_RUFNR_LIST  (unsigned char)( K_WRITE_CMD_OFFSET + 0x02 )
#define K_WRITE_TLN_BER_LIST    (unsigned char)( K_WRITE_CMD_OFFSET + 0x03 )
#define K_WRITE_TLN_RUF_VERT_LIST       (unsigned char)( K_WRITE_CMD_OFFSET + 0x04 )
#define K_WRITE_TLN_GEB_KONF_LIST       (unsigned char)( K_WRITE_CMD_OFFSET + 0x05 )
#define K_WRITE_ST_KON_LIST     (unsigned char)( K_WRITE_CMD_OFFSET + 0x06 )
#define K_WRITE_NAH_BER_LIST    (unsigned char)( K_WRITE_CMD_OFFSET + 0x07 )
#define K_WRITE_SPERR_BER_LIST  (unsigned char)( K_WRITE_CMD_OFFSET + 0x08 )
#define K_WRITE_KURZ_ZENT_LIST  (unsigned char)( K_WRITE_CMD_OFFSET + 0x09 )
#define K_WRITE_ANL_DAT_LIST    (unsigned char)( K_WRITE_CMD_OFFSET + 0x0A )
#define K_WRITE_TLN_DATA_LIST   (unsigned char)( K_WRITE_CMD_OFFSET + 0x0B )
#define K_WRITE_TLN_GEB_LIST    (unsigned char)( K_WRITE_CMD_OFFSET + 0x0C )
#define K_WRITE_OUT_GEB_LIST    (unsigned char)( K_WRITE_CMD_OFFSET + 0x0D )

#define K_GET_EPROM_VER (unsigned char)( K_SERVICE_CMD_OFFSET + 0x00 )
#define K_GET_EPROM_CRC (unsigned char)( K_SERVICE_CMD_OFFSET + 0x01 )
#define K_CHECK_EEP     (unsigned char)( K_SERVICE_CMD_OFFSET + 0x02 )
#define K_TEST_RAM      (unsigned char)( K_SERVICE_CMD_OFFSET + 0x03 )
#define K_CHK_RUFE      (unsigned char)( K_SERVICE_CMD_OFFSET + 0x04 )
#define K_CHK_HOERTOENE (unsigned char)( K_SERVICE_CMD_OFFSET + 0x05 )
#define K_CHK_KOPPELFELD        (unsigned char)( K_SERVICE_CMD_OFFSET + 0x06 )
#define K_CHK_S0_ANSCHLUSS      (unsigned char)( K_SERVICE_CMD_OFFSET + 0x07 )
#define K_GET_ERROR_SPEICHER    (unsigned char)( K_SERVICE_CMD_OFFSET + 0x08 )

#define K_WR_OK (unsigned char)( K_STATUS_CMD_OFFSET + 0x00 )
#define K_V24_WR_EINRICHT_START (unsigned char)( K_STATUS_CMD_OFFSET + 0x01 )
#define K_V24_RD_EINRICHT_START (unsigned char)( K_STATUS_CMD_OFFSET + 0x02 )
#define K_V24_EINRICHT_ENDE     (unsigned char)( K_STATUS_CMD_OFFSET + 0x03 )
#define K_V24_SERVICE_KZ        (unsigned char)( K_STATUS_CMD_OFFSET + 0x04 )

#define K_CRC_ERR       (unsigned char)( K_STATUS_CMD_OFFSET + 0x09 )
#define K_CMD_ERR       (unsigned char)( K_STATUS_CMD_OFFSET + 0x0A )
#define K_CMD_PRIO_CHANGED      (unsigned char)( K_STATUS_CMD_OFFSET + 0x0B )
#define K_V24_ERR       (unsigned char)( K_STATUS_CMD_OFFSET + 0x0C )
#define K_B_ANL_DATEN   (unsigned char)( K_STATUS_CMD_OFFSET + 0x0D )

#define K_V24_EINRICHT_START_REPEAT     (unsigned char)( K_STATUS_CMD_OFFSET + 0x0E )
#define K_V24_EINRICHT_ENDE_REPEAT      (unsigned char)( K_STATUS_CMD_OFFSET + 0x0F )

 typedef struct
  {
     unsigned char InfoLen __attribute__ ((packed));
			/* Länge des Buffers inclusive Längeneintrages.  */
     unsigned char  MsgType __attribute__ ((packed));
			/* Type der Message                              */
     unsigned short Checksumm __attribute__ ((packed));
			/* Checksumme der Daten                          */
     unsigned char Data[ 1 ] __attribute__ ((packed));
			/* Daten (1 ist Dummy für Struct)                */
   } Hdr;

typedef struct IsdnS0TlnZuord
 {
   unsigned long ATlnZuordnung __attribute__ ((packed));  /* Zuordnungsfeld der Eigenen-Anlage  */
   unsigned long BTlnZuordnung __attribute__ ((packed));  /* Zuordnungsfeld der Benachbarten-Anlage     */
 } IsdnS0TlnZuordType;

#define PORT_NICHT_KONFIGURIERT 0x00

#define PORT_DIRECTION_FILTER   0xc0
#define PORT_DIRECTION_INTERN   0x40
#define PORT_DIRECTION_EXTERN   0x80
#define PORT_DIRECTION_QUER     0xc0

#define PORT_MODE_FILTER        0x30
#define PORT_MODE_PTP   0x00
#define PORT_MODE_PTMP  0x10

#define TLN_PROTOKOLL_FILTER    0x0e
#define TLN_PROTOKOLL_EDSS1     0x02

#define G_PROTOKOLL(m)	(unsigned char)(m & TLN_PROTOKOLL_FILTER)
#define G_S0(m)			(unsigned char)(m & PORT_DIRECTION_FILTER)
#define G_PORT_MODE(m)	(unsigned char)(m & PORT_MODE_FILTER)

typedef struct IsdnPort
 {
   unsigned char                   PortTyp __attribute__ ((packed));
   unsigned char                   Anlagenrufnummer[ RUF_NR_LEN ] __attribute__ ((packed));
   unsigned char                   MsnBlock[ MSN_MAX_ANZ ][ MSN_MAX_LEN ] __attribute__ ((packed));
   IsdnS0TlnZuordType       Tln[ MSN_MAX_ANZ ] __attribute__ ((packed));
 } IsdnPortType;

typedef struct IsdnKonf
 {
   unsigned char   AnzPort __attribute__ ((packed));
   struct IsdnPort Port[ 3 ] __attribute__ ((packed)) ;
 } IsdnKonfType;

#define ANL_TYP_ANL             1       /* Standardanlage */
#define ANL_TYP_UNT_NAH         2       /* Unteranlage direkt an Hauptanlage */
#define ANL_TYP_UNT_FERN        3       /* Unteranlage über Amt an Hauptanlage */
#define ANL_TYP_A_VERN          4       /* Vernetzte Anlage "Master" */
#define ANL_TYP_B_VERN          5       /* Vernetzte Anlage "Slave" */

typedef struct RufNrZuord
 {
   char HauptAnl[ HAUPT_ANL_RNR_LEN + 1 ] __attribute__ ((packed));
   unsigned char Rufnummer[ MAX_RUF_VAR_LEN ][3] __attribute__ ((packed));
   unsigned char HauptAnlNahFern __attribute__ ((packed));
 } RufNrZuordType;

#define ANZ_AB_PORT		10
#define ANZ_TFE_PORT	2

typedef struct TlnBerechtigung
 {
   unsigned long Masterber __attribute__ ((packed));
   unsigned long Aufschaltsch __attribute__ ((packed));
   unsigned long Aufschalt __attribute__ ((packed));
   unsigned char Berechtigung[ MAX_RUF_VAR_LEN ] __attribute__ ((packed));
   unsigned char Geraetetyp  [ ANZ_AB_PORT - ANZ_TFE_PORT ] __attribute__ ((packed));

 }TlnBerechtigungType;

#define ANZ_NAH_BER_NR              20
#define ANZ_SPERR_NR                20
#define SPERR_NR_LEN                 5

typedef struct NahSperrBer
 {
   unsigned char  Nahber  [ ANZ_NAH_BER_NR ][ NAH_BER_NR_LEN + 1] __attribute__ ((packed));
   unsigned char  Sperrber[ ANZ_SPERR_NR   ][ SPERR_NR_LEN   + 1] __attribute__ ((packed));
 } NahSperrBerType;

typedef struct TlnRufvert
{
   unsigned long Ava1 __attribute__ ((packed));
   unsigned long BAva1 __attribute__ ((packed));
   unsigned long Ava2 __attribute__ ((packed));
   unsigned long BAva2 __attribute__ ((packed));
   unsigned long Tfe1 __attribute__ ((packed));
   unsigned long BTfe1 __attribute__ ((packed));
   unsigned long Tfe2 __attribute__ ((packed));
   unsigned long BTfe2 __attribute__ ((packed));
   unsigned long Rws __attribute__ ((packed));
   unsigned long BRws __attribute__ ((packed));
   unsigned long Zweitwecker1 __attribute__ ((packed));
   unsigned long Zweitwecker2 __attribute__ ((packed));
   unsigned char  RwsTime __attribute__ ((packed));
   unsigned char  AutoAmtGlob __attribute__ ((packed));
 } TlnRufvertType;

typedef struct SteuerKontakt
 {
   unsigned short StartZeit __attribute__ ((packed));
   unsigned short EndZeit __attribute__ ((packed));
   unsigned short Ask1Zeit __attribute__ ((packed));
   unsigned short Ask2Zeit __attribute__ ((packed));
   unsigned char Kontakt1 __attribute__ ((packed));
   unsigned char Kontakt2 __attribute__ ((packed));
   unsigned char Autoanwahl __attribute__ ((packed));
   unsigned char Rnr1[ RUF_NR_LEN + 1] __attribute__ ((packed));
   unsigned char Rnr2[ RUF_NR_LEN + 1] __attribute__ ((packed));
   unsigned char Rnr3[ RUF_NR_LEN + 1] __attribute__ ((packed));
   unsigned char UserToUser[ USER_TO_USER_LEN + 1 ] __attribute__ ((packed));
 } SteuerKontaktType;

#define CODE_NR_LEN           6

typedef struct GlobAnlDaten
 {
   char UserToUser[ USER_TO_USER_LEN + 1 ] __attribute__ ((packed));      /* für beide Anlagen gleich     */
   char CodeNummer[ CODE_NR_LEN + 1 ] __attribute__ ((packed));
   char Landeskennung[ CODE_NR_LEN + 1 ] __attribute__ ((packed));
   char OrtsKennZif[ NAH_BER_NR_LEN + 1 ] __attribute__ ((packed));
   char Hotline[ RUF_NR_LEN + 1 ] __attribute__ ((packed));
   unsigned char Moh __attribute__ ((packed));
   unsigned char AmtAmtWeitergabe __attribute__ ((packed));
   unsigned char Reserve[ 0x1f ] __attribute__ ((packed));
   unsigned char AktAva __attribute__ ((packed));
 } GlobAnlDatenType;

#define FW_GEB_PRN_MASK         (unsigned char)0xfc
#define FW_GEB_PRN_DRUCKER      (unsigned char)0x02

#define FW_GEB_PRN_DEST_MASK    (unsigned char)0xf3
#define FW_GEB_PRN_DEST_AUS     (unsigned char)0x00
#define FW_GEB_PRN_DEST_EIN     (unsigned char)0x04
#define FW_GEB_PRN_DEST_GEK     (unsigned char)0x08

#define FW_GEB_MSG_TO_TLN       (unsigned char)0x10

#define FW_GEB_IMPULS_MASK      (unsigned char)0x1f
#define FW_GEB_IMPULS_16K       (unsigned char)0x20
#define FW_GEB_IMPULS_12K       (unsigned char)0x40
#define FW_GEB_IMPULS_50HZ      (unsigned char)0x80

#define FW_PC_COM1              (unsigned char)0x01
#define FW_PC_COM2              (unsigned char)0x02
#define FW_PRN_CALLS_FROM_AMT   (unsigned char)0x04
#define FW_PRN_CONN_FROM_AMT    (unsigned char)0x08

typedef struct TlnGebKonf
  {
    unsigned long Pc __attribute__ ((packed));
    unsigned long Prn __attribute__ ((packed));
    unsigned long PrnEin __attribute__ ((packed));
    unsigned long PrnGek __attribute__ ((packed));
    unsigned long GebToTln __attribute__ ((packed));
    unsigned char Peak[ MAX_RUF_VAR_LEN + 1 ] __attribute__ ((packed));
    unsigned char GrundFaktor __attribute__ ((packed));
    unsigned char UserFaktor[ MAX_RUF_VAR_LEN + 1 ] __attribute__ ((packed));
    unsigned char Flags __attribute__ ((packed));
    unsigned char WaehrFaktor __attribute__ ((packed));
    unsigned char GebImpuls __attribute__ ((packed));      /* GebInfo zum Teilnehmer       */
    unsigned char GebInfo __attribute__ ((packed));
    unsigned char PrnZeilenAnz __attribute__ ((packed));   /* Anzahl der zu Druckenden Zeilen      */
  } TlnGebKonfType;

typedef struct TlnDataStruct
 {
   unsigned char         PortNr __attribute__ ((packed));	/* 0 */
   unsigned long         Geb __attribute__ ((packed));		/*1-4*/
   unsigned char         IntRufNr __attribute__ ((packed)); /* 5 */
   unsigned short        SchaltStatus __attribute__ ((packed)); /* 6-7 */
   unsigned short        WeckZeit __attribute__ ((packed)); 	/* 8-9*/
   unsigned short        Rechte __attribute__ ((packed));  		/*10-11*/
   unsigned short        Abteilung __attribute__ ((packed));	/*12-13*/
   unsigned short        Gruppe __attribute__ ((packed));		/*14-15*/
   unsigned short        KW_Tab[3] __attribute__ ((packed));	/*16-21*/
   unsigned short        CFB_Id[ RUF_NR_LEN + 1] __attribute__ ((packed)); /*22-46 47-71*/
   unsigned short        CFNR_Id[ RUF_NR_LEN + 1] __attribute__ ((packed));/*72-96 97-121*/
   unsigned char         CFNR_Zeit __attribute__ ((packed));	/* 122 */
   unsigned char         TlnTyp __attribute__ ((packed));	/* 123 */
 } TlnDataType;

#define SOS_ERROR_BUF_LEN    0x40

typedef struct SosFktErrorStruct
 {
   unsigned short Info __attribute__ ((packed));
   unsigned short LinNum __attribute__ ((packed));
   unsigned short ErrorC __attribute__ ((packed));
   unsigned char Task __attribute__ ((packed));
 } SosFktErrorType;

typedef struct ErrorStruct
 {
   unsigned short ErrorCnt __attribute__ ((packed));
   struct SosFktErrorStruct Err[ SOS_ERROR_BUF_LEN ] __attribute__ ((packed));
 }ErrorType;


typedef struct RdErrorStruct
 {
   unsigned short ErrorCnt __attribute__ ((packed));
   struct SosFktErrorStruct Err[ 0x10 ] __attribute__ ((packed));
 }RdErrorType;

typedef struct EinrichtKonfig
 {
   unsigned char          Kennung[CFG_KENNUNG_LEN] __attribute__ ((packed));
   struct IsdnKonf        S0[ 2 ] __attribute__ ((packed));
   struct RufNrZuord      RufNr[ 2 ] __attribute__ ((packed));
   struct TlnBerechtigung TlnBer[ 2 ] __attribute__ ((packed));
   struct TlnRufvert      TlnRufVert[ 2 ] __attribute__ ((packed));
   struct TlnGebKonf      TlnGebKonf[ 2 ] __attribute__ ((packed));
   struct SteuerKontakt   StKon[ 2 ] __attribute__ ((packed));
   struct GlobAnlDaten    AnlDat __attribute__ ((packed));
   unsigned char HauptUnteranlage __attribute__ ((packed));
 } EinrichtKonfigType;


#define KENN_LEN 19
#define ANZ_KW_NR  200

typedef struct KzNamen
   {
     unsigned char Name[ KURZ_NAME_LEN +1 ];
   } KzNamen;

typedef struct KurzwZiele
   {
     unsigned char RufNr[ RUF_NR_LEN + 1 ];
   } KurzwZieleType;

typedef struct KzWork
 {
   unsigned char Kennung[ KENN_LEN +1 ];
   struct KurzwZiele RNr[ ANZ_KW_NR ];
   struct KzNamen    Name[ ANZ_KW_NR ];
 } KzWorkType;

typedef struct split {
    unsigned long
        set0:10,
        set1:10,
        set2:10,
        res:2;
} splitType;

typedef struct GesamtKonfig {
    struct IsdnKonf iconf[2];
    struct RufNrZuord rufNrZuord[2];
    struct TlnBerechtigung tlnBerechtigung[2];
    struct TlnRufvert tlnRufvert[2];
    struct TlnGebKonf tlnGebKonf[2];
    struct SteuerKontakt steuerKontakt[2];
    struct GlobAnlDaten globAnlDaten;
    struct TlnDataStruct tlnData[30];
	struct KzWork kwz;
	struct NahSperrBer nahSperrBer;
	char *thedevname;
	char cfgfilename[256];
	char kwzfilename[256];
	char tlnfilename[256];
	int absel;
	int isdirty;
	int kwzisdirty;
	int tlnisdirty;
	int havedata;
	int havetlndata;
	int havekwzdata;
	int haveNuSdata;
	int havekwzname;
	int haveconffile;
	int havetlnfile;
} GesamtKonfigType;

