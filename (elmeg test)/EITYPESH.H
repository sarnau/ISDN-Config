/* 
 *  
 *  Modul : EITYPESH
 *
 */

/* Typdefinitionen fÅr EITYPESH */
/*  IFDEF  */


/*  EITYPES:MESSAGE TYPEN  */
typedef enum 
{
    eEvEinrConfReq,           // Vt(Com) -> Subsc  belegen
    eEvEinrConfEnd,           // Vt(Com) -> Subsc  verlassen
    eEvEinrConfInfo,          // Vt -> Subsc   Ziffern-Info
    eEvEinrConfNAck,          // Subsc -> Vt   neg. RÅckmeldung
    eEvEinrConfAck,           // Subsc -> Vt   pos. RÅckmeldung
    eEvEinrConfData,          // Com  <>  Vt   Kompletter Datensatz down / up
    eEvEinrConfDataReq,       // Com  ->  Vt   Kompletter Datensatz up request
    eEvEinrConfVersion,
    eEvEinrConfVersionReq,
    eEvEinrConfDelTlnDatAll,  // Alle LM loeschen (Proz.901)
    eEvEinrConfDelTlnDat,     // LM fÅr Tln. n lîschen (Proz.9n) 

       // direkte setzbefehle an den einrichter, 
       // ausgeloest durch prozedur-aufrufe
    eEvEinrSetAlarmOff,       // *840: Alarmfunktion ausschalten 
    eEvEinrSetAlarmInt,       // *841: Alarm intern signalisieren
    eEvEinrSetAlarmExt,       // *842: Alarm extern signalisieren
    eEvEinrSetKlingel1Int,    // *830: Klingel1 intern signalisieren
    eEvEinrSetKlingel1Ext,    // *831: Klingel1 extern signalisieren
    eEvEinrSetTagVariante,    // *80: AVA Tagvariante aktivieren
    eEvEinrSetNachtVariante,  // *81: AVA Nachtvariante aktivieren
    
    eEvEinrSetInternalDiversion, //Interne Rufumleitung  

   eEvEinrUnknown
} eEvEinr;

/*  EITYPES:CAUSE WERTE  */
typedef enum
{
    eEinrCauseUnknown,
    eEinrCauseBusy,
    eEinrCauseDataNotValid
} eEinrCause;

/*  ENDIF  */



