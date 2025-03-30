/* 
 * 
 *  Projekt Nr.: 3030 
 *
 *  Modul      : TKTYPESH.H
 *
 */

/* Typdefinitionen fÅr TKTYPESH.H */
/*  IFNDEF  */
#ifndef _TK_TYPES_
#define _TK_TYPES_

/*  STRING FUNKTIONEN  */
inline BYTE *EKBSTRNCPY( BYTE *d, BYTE *s, int len )
{
    strncpy((char*)d, (char *)s, len );
    d[len] = 0x00;
    return d;
}

inline int EKBSTRNCMP( BYTE *s1, BYTE *s2, int len )
{
    return strncmp((char*)s1, (char *)s2, len );
}

inline int EKBSTRCMP( BYTE *s1, BYTE *s2 )
{
    return strcmp((char*)s1, (char *)s2 );
}

inline int EKBSTRLEN( BYTE *s )
{
    return strlen((char*)s);
}

/*  DEFINES  */
#define NO_KOPPELPUNKT      0xff

#define MAX_EXT_RNR_LEN 30      // max. LÑnge Externe Rufnummer
#define MAX_INT_RNR_LEN  1      // max. LÑnge Interne Rufnummer
#define MAX_MSN_LEN     14

#define MAX_INT_RNR_ENTRIES  8  // max. Entries in Interne Rufnummer Liste.
#define MAX_EXT_RNR_ENTRIES 10  // max. Entries in Externe Rufnummer Liste.

#define MAX_AVL_ENTRIES     MAX_INT_RNR_ENTRIES

#define MAX_DUWA_LEN     3       // max. LÑnge der Durwahlnummer
#define MAX_PIN_LEN      4       // max. LÑnge des PIN
#define TK_ANZ_SUBSC     9       // max. Anzahl Teilnehmer/Apparate
#define TK_MAX_PORT_TLN  8       // max. IntRnr per Port


#define MAX_NT_SUBSCR_RNR 10
#define MAX_TE_SUBSCR_RNR 10


#define PMP_DEFAULT_MSN   1
#define P_P_DEFAULT_MSN   0



#define ANZAHL_GEBUEHRENDATEN       100

/*  SUBSCRIBER TYPE  */
typedef enum
{
    eSubscrUnknown, 
    eSubscrAnalog,
    eSubscrISDN,
    eSubscrTFE
}eSubscriber;



/*  CONFIGURATION TYPES  */
typedef enum
{
    ePortConfigurationUnknown,
    ePortConfigurationP_P,
    ePortConfigurationPMP
}ePortConfiguration;


/*  PORTPROTOCOL TYPES  */
typedef enum
{
    ePortProtocolUnknown,
    ePortProtocolEuro
}ePortProtocol;

/*  ANALOG DEVICE TYPE  */
typedef enum
{
    eAnalogDeviceUnknown,
    eAnalogDeviceTelephone,
    eAnalogDeviceFax,
    eAnalogDeviceModem,
    eAnalogDeviceAnsweringMachine,
    eAnalogDeviceKombiPort,
    eAnalogDeviceNstAnl
}eAnalogDevice;



/*  ANRUFLISTE TAG/NACHT  */

typedef enum
{
    eAVLTag, 
    eAVLNacht
}eAVLTagNacht;


/*  KONFIGURATION DES SERIELLEN PORTS  */
typedef enum
{
    eSerialPortConfPC,             // PC: 19200 Bit/s 8/N/1
    eSerialPortConfPrinter         // Drucker: 9600 Bit/s 8/N/1 
} eSerialPortConf;

/*  RUF NUMBER STATE  */
typedef enum
{
    eRufNumberUnknown,
    eRufNumberInComplete,
    eRufNumberComplete
}eRufNumber;

/*  GEBÅHREN ERFASSUNGS TYPES  */
typedef enum
{
    eGebErfOff,
    eGebErfOn
}eGebErf;

typedef enum
{
    eGebRnrAusgabeAus,
    eGebRnrAusgabeTeil,
    eGebRnrAusgabeVoll
} eGebRnrAusgabe;

typedef enum
{
    eGebFormatEinZeilig,
    eGebFormatVierZeilig
} eGebFormat;


/*  AMTSBERECHTIGUNG  */
typedef enum
{
    eAmtsBerechtigtKeinAmt,
    eAmtsBerechtigtHalbAmt,
    eAmtsBerechtigtOrt,
    eAmtsBerechtigtNational,
    eAmtsBerechtigtInterNational
}eAmtsBerechtigung;

typedef enum
{
    eAmtsBelegungOff = eFalse,
    eAmtsBelegungOn  = eTrue
}eAmtsBelegung;



/*  HARDWARE VARIANTE  */
typedef enum
{
    eTkVarianteL0,      // TE + 4 Analog        Eumex 306
    eTkVarianteL1,      // TE + NT + 2 Analog
    eTkVarianteL2,      // TE + NT + 4 Analog   Eumex 308
    eTkVarianteC23,
    eTkVarianteC43
} eTkVariante;

/*  AB-RUFARTEN  */
typedef enum {
    eAbRufExtern,
    eAbRufIntern,
    eAbRufRueck,
    eAbRufWiederAnruf,
    eAbRufTFE1,
    eAbRufTFE2,
    eAbRufTFE3,
    eAbRufAlarm,
    eAbRufKein = 0xff
} eAbRuf;


/*  TYPEN UND KONSTANTEN LEISTUNGSMERKMALE  */

// ZustÑnde der Verbindung, die Leistungsmermal aufruft
typedef enum
{
    eConnStWahl,
    eConnStRuf,
    eConnStBesetzt,
    eConnStGespr,
    eConnStGesprAnklopf,
    eConnStRueckWahl,
    eConnStRueckRuf,
    eConnStRueckBesetzt,
    eConnStRueckGespr,
    eConnSt3Pty    
} eConnectionState;


// Leistungsmermale
typedef enum
{
    eLMNotComplete,             // Information nicht vollstÑndig

    // Wahlzustand
    eLMPickUp,                  // *0
    eLMPickUpAnsMachine,        // *4

    eLMCallBackDel,             // *5

    eLMDivIntDel,         // *60
    eLMDivIntUncond,      // *61XXX...XXX*
    eLMDivIntBusy,        // *62XXX...XXX*
    eLMDivIntNoReply,     // *63XXX...XXX*
    eLMDivExtDel,         // *70
    eLMDivExtUncond,      // *71XXX...XXX*
    eLMDivExtBusy,        // *72XXX...XXX*
    eLMDivExtNoReply,     // *73XXX...XXX*

    eLMDivEinrOnAll,      // *750 
    eLMDivEinrOnTel,      // *751
    eLMDivEinrOnFax,      // *752
    eLMDivEinrOnData,     // *753

    eLMDivEinrOffAll,     // *760 
    eLMDivEinrOffTel,     // *761
    eLMDivEinrOffFax,     // *762
    eLMDivEinrOffData,    // *763


    eLMAVLTag,              // *80
    eLMAVLNacht,            // *81

    eLMAnklopfSchutzAus,    // *90
    eLMAnklopfSchutzEin,    // *91

    // Ruf/Besetz
    eLMCallBackReq,         // *4

    // GesprÑch/Anklopfen
    eLMCallReject,          // *0

    // RÅckfrage/GesprÑch
    eLMDisconnectActiv,     // *0
    eLM3Pty,                // *3

    // TÅrîffner / TFE nur Wahlzustand
    eLMDoorOpener,          // *1   & Rueckfrage-Wahlzustand
    eLMSwitchOff,           // *820
    eLMSwitchOn,            // *821
    eLMSwitchTimed,         // *822
    eLMKlingelIntern,       // *830
    eLMKlingelExtern,       // *831
    eLMAlarmActionAus,      // *840
    eLMAlarmActionIntern,   // *841
    eLMAlarmActionExtern,   // *842

    eLMParkResume,          // Nur fÅr TEST

    eLMError = 0xFF             // Eingabe fehlerhaft

} eLeistungsMerkmal;



static const BYTE   InfoTasteLM          = '*';       
static const BYTE   InfoTasteLMEnd       = '*';       
static const BYTE   InfoTasteEinricht    = '#';       
static const BYTE   InfoTasteEinrichtEnd = '#';       

/*  SPRACH INDEX  */
typedef enum
{
    eSprachIndexDeutsch,
    eSprachIndexEnglisch,
    eSprachIndexFranzoesisch,
    eSprachIndexItalienisch,



    eMaxSprachIndex // MUSS IMMER DER LETZTE EINTRAG BLEIBEN
} eSprachIndex;


// Extern Referenzen zu den Sprach Feldern


/*  EDIVERSION TYPES  */

// Rufumleitungsarten

typedef enum
{
    eDiversionUnknown,          // Keine Umleitung
    eDiversionUncond,           // Sofort    
    eDiversionBusy,             // Bei Besetzt
    eDiversionNoReply           // Nach Zeit
}eDiversion;


// Dienste

typedef enum
{
    eDivServiceAll,             // Alle Dienste

    eDivServiceTelephony,       // Umfa·t alle folgenden Sprachdienste
    eDivServiceSpeech,
    eDivServiceAudio3_1kHz,
    eDivServiceTelephony3_1kHz,

    eDivServiceFax,             // Umfa·t alle folgenden Faxdienste 
    eDivServiceFaxGr2_3,
    eDivServiceFaxGr4,

    eDivServiceData

}eDivService;

static const BYTE ANZ_DIV_SERVICE = 9;

// Aktion der Rufumleitung
typedef enum
{
    eDivActionActivate,
    eDivActionDeActivate,
    eDivActionInterrogate
} eDivAction;

/*  TFE DATEN  */
#ifdef __TFE

// EintrÑge in die Klingelliste
#define MAX_INT_RING_ENTRIES    9   
#define MAX_EXT_RING_ENTRIES    1

// EintrÑge in die Alarmliste
#define MAX_INT_ALARM_ENTRIES   9
#define MAX_EXT_ALARM_ENTRIES   2

// Dauer TÅrîffner 
#define MIN_DOOROPEN_PERIOD         1   // Sekunden   
#define MAX_DOOROPEN_PERIOD         9
#define DEFAULT_DOOROPEN_PERIOD     3



// ZusatzlÑutwerk Ansprechverhalten
typedef enum
{
    eLaeutwerkMSN_0,
    eLaeutwerkMSN_1,
    eLaeutwerkMSN_2,
    eLaeutwerkIntern_S0,
    eLaeutwerkAlle_Anrufer,
    eLaeutwerkAus = 0xFF
}eZusatzLaeutwerk;


// Bezeichnungen der Klingelknîpfe
typedef enum
{
    // TFE KlingelKnîpfe
    eTfeKlingel1,               // TFE Klingel1
    eTfeKlingel2,               // TFE Klingel2
    eTfeKlingel3,               // TFE Klingel3
    eTfeKlingelMax              // TFE anzahl klingelknoepfe
} eTfeKlingelKnopf;


typedef enum
{
    eAlarmAus,
    eAlarmIntern,
    eAlarmExtern
}eAlarmAction;

#endif

/*  EXTERN REFERENZEN ZU DEN SPRACHFELDE  */


// Texte fÅr GebÅhrenausgabe
#define MAX_TEXT_ANZ_GEBUEHREN          8
#define MAX_TEXT_LEN_GEBUEHREN         15

extern const char TexteGebuehren[eMaxSprachIndex][MAX_TEXT_ANZ_GEBUEHREN][MAX_TEXT_LEN_GEBUEHREN];


/*  ENDIF  */
#endif


