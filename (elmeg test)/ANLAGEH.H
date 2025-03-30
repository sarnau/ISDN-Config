/* 
 * 
 *  Projekt Nr.: 3030 
 *
 *  Modul      : ANLAGEH
 *
 */

/* Typdefinitionen fÅr ANLAGEH */
/*  IFNDEF  */
#ifndef _ANLAGENTYPES_
#define _ANLAGENTYPES_

/*  TFE HEADER  */
#ifdef __TFE
#include <tfedath.h>
#endif

/*  SUBSC:CLASS SUBSCRIBER  */
class Subscriber 
{
    /*-------------- Private Daten --------------------*/
private:
    eBoolean    TagNachtBerechtigung;

    /*-------------- Subsc. gesperrt.()--*/
    eBoolean    Gesperrt;    //Muss abgefragt werden.

    /*-------------- Subscriber Type  ------------------------------*/
    eSubscriber Type;    

    /*-------------- Analog Device (fÅr ISDN immer Unknown) --------*/
    eAnalogDevice   AnalogDevice;
    
    /*-------------- Anklopfschutz (fÅr ISDN immer eFalse) ---------*/
    eBoolean    AnklopfSchutz;

    /*-------------- Gebuehren Erfassung --------------------*/
    eGebErf         GebErf;                  

    eGebRnrAusgabe  GebuehrenRnrAusgabe;

    /*-------------- Zugeordnete Interne Rufnummer-----------*/
    BYTE    IntRnr[MAX_INT_RNR_LEN +1];    

    /*-------------- AmtsBerechtigung. ----------------------*/
    eAmtsBerechtigung   Berechtigung;

    // öbermittlung der Rufnummer
    // ... des A-Teilnehmers
    eBoolean    APresentation;

    // ... des B-Teilnehmers
    eBoolean    BPresentation;

    // Automatischee Amtsbelegung(nicht bei KombiPort.)
    eBoolean    Amtsbelegung;

    // Default MSN Index
    BYTE        DefaultMSNIndex;

    // Rufumleitungsziel
    BYTE        DivRnr[MAX_EXT_RNR_LEN+1];

    // Rufumleitungsart
    eDiversion  Diversion;


#ifdef __TFE
    eBoolean    SchaltKontaktBerechtigung;
#endif

    /*-------------- Private Methods --------------------*/
private:

    /*-------------- Public Methods --------------------*/

public:

    /*-------------- Constructor --------------------*/
    Subscriber()
    {
        Init();
    };

public:
    /*-------------- Methods --------------------*/

    // Initialisierung 
    void Init( void );
    


    /*-------- Holen Setzen DayNightList Berechtigung. ----------*/

    void SetDayNight(eBoolean   TagNacht)
    {
        TagNachtBerechtigung = TagNacht;
    };

    eBoolean GetDayNight(void)
    {
        return TagNachtBerechtigung;
    };


    /*-------------- Holen - Setzen  des Subscribertyps --------------------*/
   eSubscriber GetSubscType(void)
    {
        return Type;
    };

    void SetSubscType(eSubscriber type)
    {
        Type = type;
    };

    /*--------- Holen - Setzen des Geraetetyps -----------*/
    eAnalogDevice  GetDeviceType(void)
    {
        return AnalogDevice;
    };

    void SetDeviceType(eAnalogDevice device)
    {
        AnalogDevice = device;
    };

    /*------- Anklopfschutz abfragen ----------*/
    eBoolean IsAnklopfSchutz(void)
    {
        return AnklopfSchutz;
    };

    // Setzen der Rufnummernanzeige
    eBoolean SetAPresentation( eBoolean pres );

    eBoolean SetBPresentation( eBoolean pres );

    // Lesen der Rufnummernanzeige
    eBoolean GetAPresentation( void )
    {
        return APresentation;
    }

    eBoolean GetBPresentation( void )
    {
        return BPresentation;
    }

    /*-------- Setzen des Anklopfschutzes (inline)---------*/
    void ActivateAnklopfSchutz(void)
    {
        if(Type == eSubscrAnalog)
        {
            AnklopfSchutz = eTrue;
        }
    };

    /*-------- Anklopfschutz deaktivieren (inline)---------*/
    void KillAnklopfSchutz(void)
    {
        AnklopfSchutz = eFalse;
    };

    /*-------------- --------------------*/
    /* 
        The MSN that is recognised internally and is sent as 
        Calling Party number for internal calls.
    */
    void   SetIntRnr(BYTE* InternRnr)
    {
        EKBSTRNCPY(IntRnr, InternRnr, MAX_INT_RNR_LEN ); 
    };

    /*-------------- Delete IntRnr. --------------------*/
    void   KillIntRnr(void)
    {
        IntRnr[0] = 0;
    };

    /*-------------- --------------------*/
    void GetIntRnr(BYTE* IRnr)
    {
        strcpy((char*)IRnr, (char*)IntRnr);
    };



    /*-------- Compares IntRnr and returns eTrue for Equal.------*/
    eBoolean CompareIntRnr(BYTE* IRnr);

    /*-------------- --------------------*/
    void SubscriberSperren(void)
    {
        Gesperrt = eTrue;
    };

    /*-------------- --------------------*/
    eBoolean IsGesperrt(void)
    {
        return Gesperrt;
    };

    /*-------------- --------------------*/
    void SubscriberFreiGeben(void)
    {
        Gesperrt = eFalse;
    };

    /*-------------- --------------------*/
    void SetGebErf(eGebErf GebErfassung)
    {
        GebErf = GebErfassung;
    };

    /*-------------- --------------------*/
    eGebErf IsGebErf(void)
    {
        return GebErf;
    }

    void SetGebRnrAusgabe( eGebRnrAusgabe RnrAusgabe)
    {
        GebuehrenRnrAusgabe = RnrAusgabe;
    }

    eGebRnrAusgabe GetGebRnrAusgabe( void )
    {
        return GebuehrenRnrAusgabe;
    }



    /*-------------- GetBerechtigung.--------------------*/
    eAmtsBerechtigung GetBerechtigung(void)
    {
        return Berechtigung;
    };

    /*-------------- SetBerechtigung.--------------------*/
    void SetBerechtigung(eAmtsBerechtigung berechtigung);

    // Ist eine externe Rufnummer eingetragen?
    eBoolean IsExtRnr(void);

    // Automatische Amtsbelegung g
    eBoolean SetAmtsbelegung(eBoolean Amtsbel);

    eBoolean GetAmtsbelegung(void)
    {
        return Amtsbelegung;
    };


    BYTE GetDefaultMSNIndex( void )
    {
        return DefaultMSNIndex;
    };

    eBoolean SetDefaultMSNIndex( BYTE Index );


    // ***************************************************************
    // Interne Rufumleitung
    eBoolean GetInternalDiversion(BYTE* DivRnr,                
                                  eDiversion& DivType);

    eBoolean DelInternalDiversion();

    eBoolean SetInternalDiversion(BYTE* DivRnr,                
                                  eDiversion DivType);





#ifdef __TFE

    void SetSchaltKontaktBerechtigung(eBoolean Switch)
    {
        SchaltKontaktBerechtigung = Switch;
    };

    eBoolean GetSchaltKontaktBerechtigung(void)
    {
        return SchaltKontaktBerechtigung;
    };

#endif

};

/*  ANLDATEN:CLASS SUBSCRIBERINFO  */
class SubscriberInfo
{
public:

    Subscriber  Subsc;
    ePortId     PortId;
    ePortType   Type;
    eBoolean    Belegt;

    /*- Constructor-*/

    SubscriberInfo()
    {
        PortId = ePortUnknown;
        Type =  UnknownPortType;
        Belegt = eFalse;
    };
    
};



/*  ANLDATEN:KLASSE: INTERNE RUFNUMMERN LISTE  */
class cIntRnrList
{
private:

    BYTE IntRnr[MAX_INT_RNR_ENTRIES][MAX_INT_RNR_LEN+1];

public:

// Constructor
    cIntRnrList()
    {
        init();   
    };
   
    void init();

    // Rufnummer eintragen
    eBoolean SetRnr( BYTE* Rnr );

    // Rufnummer lîschen
    eBoolean DelRnr( BYTE* Rnr );


    // Rufnummer Auslesen 
    eBoolean GetRnr( BYTE Index, BYTE* Rnr );

    // Is IntRnr in list.
    eBoolean IsRnrEntry( BYTE* Rnr);
    eBoolean IsRnrEntry(void);

    // Die Liste zusammenrucken
    void Compress(void);

    // EintrÑge zÑhlen
    BYTE CountEntry( void );

private:
    // Rufnummer lîschen
    void DelRnr( BYTE Index);
};

/*  ANLDATEN:KLASSE RUFVERTEILUNG  */
class cRufVerteilung
{
  private:

    cIntRnrList List[MAX_EXT_RNR_ENTRIES];

  public:


  // Constructor
    cRufVerteilung()
    {
        init(); 
    };
   
    void init();

    // Rufnummer eintragen
    eBoolean SetRnr( BYTE Index, BYTE* IntRnr );

    // Rufnummer lîschen
    eBoolean DelRnr( BYTE Index, BYTE* IntRnr );
    eBoolean DelRnr( BYTE* IntRnr );

    // Rufnummer Auslesen 
    eBoolean GetRnr( BYTE Index, cIntRnrList& NewList);

    // Ersten Eintrag von Rufnummer bestimmen
    eBoolean GetFirstRnrEntry( BYTE* IntRnr, BYTE& Index );

};

/*  ANLDATEN:CLASS: EXTERNE RUFUMLEITUNG  */
class ExtDiversionEntry
{

    /*---------- Private Daten ----------------*/
private:

    // Rufnummer zu der umgeleitet werden soll
    BYTE    DivExtRnr[MAX_EXT_RNR_LEN + 1];

    // Art der Rufumleitung
    eDiversion      DivType;
    
    /*---------- Public Daten ----------------*/

public:
    /*------- Methods. -------*/

    eBoolean Get( eDiversion &Type, BYTE *ExtRnr )
    {
        strcpy( (char *)ExtRnr, (char *)DivExtRnr );

        Type = DivType;

        if( Type == eDiversionUnknown )
            return eFalse;
        else
            return eTrue;

    }


    void     Set( eDiversion Type, BYTE *ExtRnr )
    {

        strcpy( (char *)DivExtRnr, (char *)ExtRnr );

        DivType = Type;

    }

    void     Delete( void )
    {
        DivExtRnr[0] = 0x00;
        DivType = eDiversionUnknown;
    }

};


/*  ANLDATEN:CLASS ANLAGENDATEN  */
class AnlagenDaten
{
    /*---------- Private Daten ----------------*/
private:      

    // Hardware Version
    eTkVariante Variante;

    // Sprachen Index fÅr Texte Feld  
    eSprachIndex SprachIndex;

    /*--------- Set day or night list active. --*/
    eAVLTagNacht    CurrentAVList;

    /*---------- Music On Hold ----------------*/
    eBoolean        MusicOnHold;

    // Konfiguration des Seriellen Port (Drucker/PC)
    eSerialPortConf SerialPortConf;

    /*---------- External PortConfiguration PP / PMP ----------*/
    ePortConfiguration   ExtPortConfiguration;

    // Externe Rufumleitung der Anlagenrufnummer / Default-MSN (???)
    ExtDiversionEntry    ExtDiversion[ANZ_DIV_SERVICE];

    // GebÅhrenFormat ein- oder vierzeilig
    eGebFormat      GebuehrenFormat;

    // WÑhrungsMultiplikator (Hundertstel)
    WORD    WaehrungsMultiplikator;

    /*---------- Teilnehmer Daten ------------------------------*/
    SubscriberInfo   Subscribers[TK_ANZ_SUBSC];

    /*---------- Durchwahlnummer 0 ----------------*/
    BYTE            DuwaNr[MAX_DUWA_LEN +1];
    
    /*---------- Einricht Code Nummer ----------------*/
    BYTE            PIN[MAX_PIN_LEN +1];

    // Rufnummer des MOH Ports
    BYTE RnrMOH[MAX_INT_RNR_LEN+1];

    // Rufnummer zur Amtsholung 
    BYTE RnrExtBelegung[MAX_INT_RNR_LEN+1];

    // MSN 
    BYTE MSN[MAX_EXT_RNR_ENTRIES][MAX_MSN_LEN+1];

    // Anrufverteilungslisten
    cRufVerteilung       Tag;
    cRufVerteilung       Nacht;


    // NUR FUER TFE
#ifdef __TFE

    Tfedat     TfeData;

#endif

  // ****************************************************************
  // Methoden   


  // Konstruktor
  public:

    AnlagenDaten( eTkVariante Version, eBoolean Tfe );
    AnlagenDaten( eTkVariante Version );
    AnlagenDaten( void );

  //***************************************************************  
  // Allgemeine Methoden  
  public:

    // Variante auslesen
    eTkVariante GetVariante(void) {
        return Variante;
        };    

    // Auslesen aller internen Rufnummern 
    void GetAllIntNumbers(BYTE IntRnrList[TK_ANZ_SUBSC][MAX_INT_RNR_LEN + 1]);

    // TFE vorhanden ????
    eBoolean IsTFE( void );

    // Daten des Teilnehmers
    #ifndef __WINDOS__
    void GetSubscriber( BYTE nr, SubscriberInfo &info )
    {
        info = Subscribers[nr]; 
    }
    #endif 

    // Rufnummer belegt
    eBoolean IsTlnRnrValid(BYTE* IntRnr);

    // Amtsholungsnummer?
    eBoolean IsRnrForExternalCall(BYTE* IntRnr);

    // Hole Teilnehmer Typ ISDN / Analog
    eSubscriber GetSubscType(BYTE* IntRnr);

    // Returns eTrue if Pid initialised.
    eBoolean IsPortInit(ePortId Pid);

    // TePort For IntRnr.
    ePortId GetExtPort(BYTE* IntRnr);

    //Abfrage fÅr PortId
    eBoolean IsExternalPort( ePortId Port );

    // Gibt es einen interne S0-Bus
    eBoolean IsInternISDN( void );

  private:  

    // Initialisierung der Anlagendaten
    void Init( eTkVariante Variante, eBoolean Tfe );

    // Amtsberechtigung bewerten
    eBoolean CompareBerechtigung(BYTE *Rnr, eAmtsBerechtigung Berechtigung, eBoolean IsExtern);

    // TRUE wenn noch weitere Tln gesperrt werden kînnen
    eBoolean IsSperrenAllowed(void);

    // Weitere BeschrÑnkungen der Amtsberechtigung zulÑssig?
    eBoolean IsOutgoingOk(void);

    // DienstVergleich
    eBoolean CompareDevice(BYTE InfTC, BYTE Cap, eAnalogDevice Device);

    // Interne RNR in Index der Rufverteilungen umwandeln
    BYTE IntRnrToIndex( BYTE *IntRnr )
    {
        return IntRnr[0] - '0';
    }

    eBoolean IsFaxCall(BYTE HlCap);
    eBoolean IsTelephoneCall(BYTE BCap, BYTE HlCap);


  //***************************************************************
  // Konfiguration des Externen Ports PP/PMP   
  public:
    ePortConfiguration   GetExtPortConfig(void)
    {
        return   ExtPortConfiguration;
    };

  protected:
    void SetExtPortConfig(ePortConfiguration PortConfig)
    {
        ExtPortConfiguration = PortConfig;
    };

  //***************************************************************
  // Durchwahl   (NUR BEI PUNKT ZU PUNKT
  protected:
    void SetDuwaNr(BYTE* DurchwahlNmr)
    {
        EKBSTRNCPY( DuwaNr, DurchwahlNmr, MAX_DUWA_LEN );
    };

  //***************************************************************
  // Einrichtberechtigungscode
  public:
    void GetPin(BYTE* Pin)
    {
        EKBSTRNCPY(Pin, PIN, MAX_PIN_LEN);
    };

  protected:
    void SetPin(BYTE* Pin)
    {
        EKBSTRNCPY(PIN, Pin, MAX_PIN_LEN);
    };

    eBoolean ComparePin(BYTE *Str)
    {
        if( EKBSTRNCMP(PIN, Str, MAX_PIN_LEN) == 0 )
            return eTrue;
        else
            return eFalse;
    };

  //***************************************************************
  // Sprachen Index
  public:

    eSprachIndex GetSprachIndex( void )
    {
        return SprachIndex;
    };   

  //***************************************************************
  // Music On Hold
  public:
    eBoolean   GetMOH( void )
    {
        return MusicOnHold;
    }

  protected:
    eBoolean SetMOH( eBoolean moh );

  //***************************************************************
  // Externe Rufnummer (MSN)
  public:

    eBoolean GetExtRnr( BYTE *IntRnr, BYTE *DialedRnr, BYTE *Calling );
    eBoolean GetExtRnr( BYTE *IntRnr, BYTE *ExtRnr, eBoolean PMPOnly );

    eBoolean GetExtRnr( BYTE *IntRnr, BYTE *ExtRnr, BYTE &Index );

    eBoolean GetExtRnr(BYTE* IntRnr, BYTE* ExtRnr, eBoolean PMPOnly, BYTE &Index);

/*  War bis zum 11/10/96 "private". Wurde von mir zum Erzeugen der 
    externen Adressliste gebraucht. Carsten...
*/
    eBoolean GetExtRnr(BYTE Index, BYTE* ExtRnr);
    
    BYTE     GetMSNIndex( BYTE *ExtRnr );

    eBoolean GetAnlagenRnr( BYTE *RNR );
    void     GetAmtsNr(BYTE* AmtNr)
    {
        EKBSTRNCPY(AmtNr,RnrExtBelegung,MAX_INT_RNR_LEN);
    }

    //**************************************************
    // Rufnummer Verwaltung fÅr Externe Rufumleitung
    //

    #ifndef __WINDOS__    // Unter WINDOS steht SuppData nicht zur VerfÅgung
    #ifndef __DOS_EINRPC__
        
    eBoolean GetAllIntRnrForDiversion(SuppData Data,
                                      cIntRnrList& List);
    #endif
    #endif

    eBoolean GetAllIntRnrForDiversion(BYTE *ExtRnr,
                                      cIntRnrList& List, eBoolean Append = eFalse);

    eBoolean GetDefaultMSN(BYTE *IntRnr, BYTE *ExtRnr );
    
/* Calle war's    
  protected:
*/
  public: 
    eBoolean SetExtRnr( BYTE Index, BYTE* ExtRnr );
  protected:
    eBoolean CheckIndex( BYTE Index, eBoolean EmptyAllowed );

  private:
    eBoolean CheckForExtRnr(void);

  //***************************************************************
  // Default MSN Index        
  public:
    BYTE GetDefaultMSNIndex( BYTE* IntRnr );

/*  protected: */
    eBoolean SetDefaultMSNIndex( BYTE* IntRnr, BYTE Index );




  //***************************************************************
  // Rufverteilungslisten
  public:

    // Suche Zielport mit gewÑhlter Rufnummer
    eRufNumber GetDestPort(BYTE* Rnr, ePortId &ToPortId);

    // Bestimme interne RNR fÅr externe RNR (CalledPartyNum)
    eRufNumber GetIntWithExtRnr(BYTE* ExtRnr, cIntRnrList& List );
    eRufNumber GetIntWithExtRnr(BYTE* ExtRnr,
                                cIntRnrList& List, 
                                BYTE InfTC, BYTE HlCap);


    // Erste RNR des Ports
    eBoolean   GetIntRnrWithPortId(ePortId PortId, BYTE* IntRnr);

    // Tln. Rnr. in Rufverteilung des Index eizutragen
    // (Nicht mehr als zwei ISDN Tln. in Liste)
    eBoolean   CheckTlnForIndex(eAVLTagNacht ListAuswahl,
                                BYTE Index, 
                                BYTE *IntRnr);


/* Calle war's...
protected:
*/
  public:
    // Interne RNR in Rufverteilungsliste schreiben
    eBoolean   SetRnrRufVerteilung( eAVLTagNacht List,
                                    BYTE Index,
                                    BYTE *IntRnr);

    // Interne RNR in Rufverteilungsliste lîschen
    eBoolean   DelRnrRufVerteilung( eAVLTagNacht List,
                                    BYTE Index,
                                    BYTE *IntRnr);
    eBoolean   DelRnrRufVerteilung( eAVLTagNacht List,
                                    BYTE *IntRnr);


    // Alle Rufnummern einer Liste
    eBoolean GetRnrRufVerteilung( eAVLTagNacht List,
                              BYTE Index,cIntRnrList& RnrList);


  private:

    eRufNumber P_P_GetIntWithExtRnr(BYTE* ExtRnr, 
                                    cIntRnrList& List,
                                    BYTE InfTC, BYTE HlCap,
                                    eBoolean WithDienst = eFalse);

    eRufNumber PMP_GetIntWithExtRnr(BYTE* ExtRnr, 
                                    cIntRnrList& List,
                                    BYTE InfTC, BYTE HlCap,
                                    eBoolean WithDienst = eFalse);

    // Check wether numbers barred.
    eBoolean CheckSperrung(cIntRnrList& RnrList,
                        BYTE InfTC, BYTE HlCap,
                        eBoolean WithDienst);

    eBoolean SearchExtRnr(BYTE* ExtRnr, BYTE& Index);

    void GetAllFaxNumbers(cIntRnrList& List);

    void GetFirstTelephones(cIntRnrList& List);


  //***************************************************************
  // Aktuelle Rufverteilungsliste Tag/Nacht
  protected:
    void SetAktiveRufVerteilung( eAVLTagNacht ActList )
    {
        CurrentAVList = ActList;      
    }  

  //***************************************************************
  // Teilnehmer Sperre 
  public:
    eBoolean IsGesperrt( BYTE *IntRnr );

  protected:
    eBoolean SubscriberSperren(BYTE* IntRnr);
    eBoolean SubscriberFreiGeben(BYTE* IntRnr);

  //***************************************************************
  // Amtsberechtigung
  public:  
    eBoolean TestBerechtigung(BYTE* IntRnr, BYTE *ExtRnr, eBoolean IsExtern);
    eAmtsBerechtigung GetBerechtigung(BYTE* IntRnr);

  protected:
    eBoolean SetBerechtigung(BYTE* IntRnr, eAmtsBerechtigung Berechtigung);

  //****************************************************************
  // Berechtigung Anrufliste Umschalten / Rufumleitung im Amt
  public:  
    eBoolean TestAVLBerechtigung(BYTE* MyIntRnr);

  protected:     
    eBoolean SetAVLBerechtigung(BYTE* MyIntRnr, eBoolean Berechtigung);

  //****************************************************************
  // Anklopfschutz
  public:
    eBoolean IsAnklopfSchutz(BYTE* IntRnr);

  protected:
    eBoolean SubscriberAnklopfSchutz_On(BYTE* IntRnr);
    eBoolean SubscriberAnklopfSchutz_Off(BYTE* IntRnr);

  //****************************************************************
  // GerÑtetyp fÅr analoge Teilnehmer
  public:
    eAnalogDevice GetAnalogDeviceType(BYTE* IntRnr);

  protected:
    eBoolean SetAnalogDeviceType(BYTE* IntRnr, eAnalogDevice device);

  //****************************************************************
  // Rufumleitung der Anlagenrufnummer / Default MSN
  public:
    eBoolean GetExtDiversion( eDivService Service, eDiversion &Type, BYTE *ExtRnr );

  protected:
    void SetExtDiversion( eDivService Service, eDiversion Type, BYTE *ExtRnr );
    void DeleteExtDiversion( eDivService Service );

  //****************************************************************
  // öbermittlung der Rufnummer
  public:
    eBoolean GetAPresentation( BYTE *IntRnr );
    eBoolean GetBPresentation( BYTE *IntRnr );

  protected:  
    eBoolean SetAPresentation( BYTE *IntRnr, eBoolean pres );
    eBoolean SetBPresentation( BYTE *IntRnr, eBoolean pres );

  //***************************************************************
  // GebÅhren

  public:
    // GebÅhrenerfassung                                        
    eGebErf GetGebErf(BYTE* IntRnr);

    eGebFormat GetGebuehrenFormat( void )
    {
        return GebuehrenFormat;
    }
  
    // Multiplikator
    WORD GetWaehrungsMultiplikator(void)
    {
        return WaehrungsMultiplikator;
    }

    eGebRnrAusgabe GetGebuehrenRnrAusgabe(BYTE *IntRnr);

  protected:

    // GebÅhrenerfassung setzen
    eBoolean SetGebErf(BYTE* IntRnr, eGebErf GebErfassung);

    // GebÅhrenerfassung fÅr alle Teilnehmer setzen
    void SetAllGebErf(eGebErf GebErfassung);

    // GebÅhrenFormat Functions
    void SetGebuehrenFormat(eGebFormat GebFormat)
    {
        GebuehrenFormat = GebFormat;
    };

    // Multiplikator
    eBoolean SetWaehrungsMultiplikator(WORD Multi);

    void SetGebuehrenRnrAusgabe(BYTE *IntRnr, eGebRnrAusgabe RnrAusgabe);


  //**************************************************************
  // Automatische Amtsbelegung
  public:
    eBoolean GetAmtsbelegung(BYTE* IntRnr);

  protected:
    eBoolean SetAmtsbelegung(BYTE* IntRnr, eBoolean Amtsbel);


  //**************************************************************
  // Interne Rufumleitung
  public:  
    eBoolean GetInternalDiversion(BYTE* IntRnr,
                                  BYTE* DivRnr,                
                                  eDiversion& DivType);

    eBoolean TestInternalDiversion(BYTE* IntRnr,
                                   BYTE* DivRnr,                
                                   eDiversion DivType);

    eBoolean SetInternalDiversion(BYTE* IntRnr,
                                  BYTE* DivRnr,                
                                  eDiversion DivType);

    eBoolean DelInternalDiversion(BYTE* IntRnr);

  protected:  
    void DelAllInternalDiversion(void);



#ifdef __TFE

  // **************************************************************
  // TFE Methoden 
  // **************************************************************
  public:  
    // TFE SchaltKontaktBerechtigung.
    eBoolean SubscriberGetKontaktBerechtigung(BYTE* IntRnr);

    eBoolean SubscriberGetDoorOpenBerechtigung(BYTE* );


    // Methoden der Klingellisten
    eBoolean KlingelIsExtern(eTfeKlingelKnopf KnopfNummer)
    {
        return TfeData.KlingelIsExtern(KnopfNummer);
    }

    eBoolean SetKlingelExtSwitch(eTfeKlingelKnopf KnopfNummer, eBoolean Mode)
    {
        return TfeData.KlingelSetExtSwitch(KnopfNummer, Mode);
    }

    BYTE GetKlingelIntNumbers(eTfeKlingelKnopf KnopfNummer, BYTE RnrList[MAX_INT_RING_ENTRIES][MAX_INT_RNR_LEN + 1])
    {
        return TfeData.KlingelGetIntNumbers(KnopfNummer, RnrList);
    }

    eBoolean GetKlingelExtRnr(eTfeKlingelKnopf KnopfNummer, BYTE* Rnr)
    {
        return TfeData.KlingelGetExtRnr(KnopfNummer, Rnr);
    }

    // Alarmverhalten umschalten
    eBoolean SetAlarmAction( eAlarmAction act );

    eAlarmAction GetAlarmAction( void )
    {
        return TfeData.AlarmGetAlarmAction();
    }

    BYTE GetAlarmIntNumbers( BYTE RnrList[MAX_INT_ALARM_ENTRIES][MAX_INT_RNR_LEN + 1] )
    {
        return TfeData.AlarmGetIntNumbers(RnrList);
    }
    
    eBoolean GetAlarmExtNumbers( BYTE ExtRnrList[MAX_EXT_ALARM_ENTRIES][MAX_EXT_RNR_LEN+1])
    {
        return TfeData.AlarmGetExtNumbers( ExtRnrList);
    }

    BYTE GetDoorOpenerTime( void )
    {
        return TfeData.GetDoorOpenerTime();
    }

    WORD GetSchaltkontaktTime( void )
    {
        return TfeData.GetSchaltkontaktTime();
    }

    eZusatzLaeutwerk GetZusatzLWVerhalten(void)
    {
        return TfeData.GetZusatzLWVerhalten();
    }

    eBoolean SwitchZusatzLW( BYTE *ExtRnr );


    eBoolean GetExtAlarmIntDest(BYTE* IntRnr);


  // **************************************************************
  // TFE Methoden NUR fuer Einrichter
  // **************************************************************

  protected:
    // Subscriber
    eBoolean SubscriberSetKontaktBerechtigung(BYTE* IntRnr, eBoolean Switch);

    // Klingel
    eBoolean SetKlingelIntRnr(eTfeKlingelKnopf KnopfNummer, BYTE* Rnr)
    {
        return TfeData.KlingelSetIntRnr(KnopfNummer, Rnr);
    }

    eBoolean DelKlingelIntRnr(eTfeKlingelKnopf KnopfNummer, BYTE* Rnr)
    {
        return TfeData.KlingelDelIntRnr(KnopfNummer, Rnr);
    }


    void SetKlingelExtRnr(eTfeKlingelKnopf KnopfNummer, BYTE* Rnr)
    {
        TfeData.KlingelSetExtRnr(KnopfNummer, Rnr);
    }

    // Alarm
    // Externe Alarmnummern
    void SetAlarmExtRnr( WORD nr, BYTE *Rnr )
    {
        TfeData.AlarmSetExtRnr(nr, Rnr);
    }

    // Interne Alarmnummern
    eBoolean SetAlarmIntRnr( BYTE *Rnr )
    {
       return TfeData.AlarmSetIntRnr(Rnr);
    }

    eBoolean DelAlarmIntRnr( BYTE *Rnr )
    {
       return TfeData.AlarmDelIntRnr(Rnr);
    }

    // Schaltkontakt / TÅrîffner / ZusatzlÑutwerk
    eBoolean SetZusatzLWVerhalten( eZusatzLaeutwerk ZLW );

    eBoolean SetSchaltkontaktTime( WORD t )
    {
        return TfeData.SetSchaltkontaktTime( t );
    }

    eBoolean SetDoorOpenerTime( BYTE t )
    {
        return TfeData.SetDoorOpenerTime( t );
    }

#endif // __TFE

friend class Einricht;           // Einrichter darf auch geschÅtzte 
                                // Daten setzen


#ifdef __WINDOS__               
friend class CElmegcfgDoc;      // Maybe these classes too...:-)
friend class CTKView;           // Probably not as they're Windows classes.
friend class CMainFrame;
#endif

#ifdef __DOS_EINRPC__
friend class TeView;
friend class TElmegApp;
#endif

};


/*  PORTDATA:CLASS BCHANNELVW  */
// lÑnge der Callid ist max 8 plus einer abschlie·enden Null
#define   MAX_CALLID_LEN   9

#define   NO_CEI      0xFF

typedef enum
{
    PBChannFree = FALSE,
    PBChannUsed = TRUE
}ePBChann;


typedef enum
{
    PBCallIdUsed    = 0,
    PBCallIdUnused  = 1
}ePBCallId;


typedef enum
{
    PBChannUnReserved,
    PBChannReserved
}ePBChannReservation;


class PBchannels {
 
 private:

    ePBChann   State;
    ePBChannReservation  Reservation;
    BYTE       CEI;
//  strCallId  CallId;
    ePortId    Port;       // Zielport; benutzt B-Kanal

    // zÑhlt, wieviele Instanzen den Bkanal reseviert haben
    BYTE      ReserveCount;  
    
 public:
    PBchannels();
    ~PBchannels() {};

    friend class BchannVerWaltung;
};


class BchannVerWaltung {
 
 private:

   PBchannels*     BChannels;
   BYTE          MaxChannels;

   eBoolean IsChannFree(eBCh Channel);
   void init(BYTE  MaxBChannels);
   void set_all_free(eBCh cha);

 protected:

   eBoolean FreeBch(eBCh Channel, BYTE Cei, eBoolean CallPresent);
   eBoolean FreeBch(eBCh Channel, BYTE Cei);

   // Methoden fÅr ces
   void SetCesForBch(eBCh Channel, BYTE ces);
   eBCh GetBchForCes(BYTE ces); 
   eBCh FurtherBchForCes(eBCh Channel, BYTE ces);

   eBoolean SetVorgegebeneBch( eBCh Channel, BYTE Cei = NO_CEI, ePortId Port = ePortUnknown);

   eBoolean BchForPortUsed(ePortId Port);

   eBCh     GetAnyFreeBCh(BYTE Cei);   
   eBCh     GetBevorzugtBCh( eBCh Channel, BYTE Cei);
   eBoolean GetVorgeschriebBCh( eBCh Channel, BYTE Cei);

   // Methoden fÅr Reservierung des Bkanals
   eBoolean ReserveBch(eBCh Channel, BYTE Cei, ePBChannReservation ReserveType);
   eBCh     GetReservedBCh(BYTE CEI);
   void     FreeReservedBCh(eBCh Channel, BYTE CEI);
   eBoolean GetReserveStatus ( eBCh Channel );
   BYTE     GetReserveCount(eBCh cha); 

public:

   BchannVerWaltung(){};

friend class PortDataBase;
};


/*  PORTDATA:CLASS PORTDATABASE  */
class PortDataBase 
{
protected:
    /*---------- Protected Data ----------------*/
   
    AnlagenDaten* AnlagenData;

    BchannVerWaltung  BChannVW;
       

public:

    // Leistungsmerkmalanforderung auswerten
    eLeistungsMerkmal EvalLMRequest( BYTE *LMStr, eConnectionState State, BYTE *Data )
    {
        return eval_lm_request( LMStr, State, Data );
    }

    //------------------------------------------------------------
    // Zugriff auf Anlagendaten

    // Hole AmtsNummer
    void     GetAmtsNr(BYTE* AmtNr)
    {
        AnlagenData->GetAmtsNr(AmtNr);
    }

    // Hole Rufnummer fÅr Port
    eBoolean GetIntRnr(BYTE* IntRnr, ePortId ToPortId)
    {
        return AnlagenData->GetIntRnrWithPortId(ToPortId, IntRnr);
    };

    // Zielport fÅr Rufnummer suchen
    eRufNumber GetDestPort(BYTE* IntRnr, ePortId &ToPortId)
    {
        return AnlagenData->GetDestPort(IntRnr, ToPortId);
        
    };

    // Suche interne Rufnummer(n) mit externe RNR
    eRufNumber GetDestPort(BYTE *ExtRnr, cIntRnrList& IntRnrList )
    {
       return AnlagenData->GetIntWithExtRnr(ExtRnr, IntRnrList);
    };

    // Suche interne Rufnummer(n) mit externe RNR und Dienst
    eRufNumber     GetDestPort(BYTE *ExtRnr, BYTE BC, BYTE HLC, cIntRnrList& IntRnrList)
    {
       return AnlagenData->GetIntWithExtRnr(ExtRnr,IntRnrList,BC,HLC);
    };

    // PrÅfe Amtsberechtigung des Teilnehmers Rnr zu wÑhlen
    eBoolean TestBerechtigung( BYTE *IntRnr, BYTE *Rnr, eBoolean IsExtern = eFalse )
    {
        return AnlagenData->TestBerechtigung( IntRnr, Rnr, IsExtern );
    };


    // Hole Umschalte Berechtigung Tag/Nacht/eingerichtete Rufumleitung
    eBoolean TestUmschalteBerechtigung(BYTE* IntRnr)
    {
        return AnlagenData->TestAVLBerechtigung(IntRnr);
    };

    // Hole eingerichtete Rufumleitung
    eBoolean GetExtDiversion( eDivService Service,
                              eDiversion &Type, BYTE *ExtRnr )
    {
        return AnlagenData->GetExtDiversion( Service, Type, ExtRnr );
    };


    // Suche externe RNR fÅr gehende Belegung
    void    GetExternalCallingPN(BYTE* IntRnr, 
                                 BYTE* CalledPN, 
                                 BYTE* ExtCallingPN)
    {
        AnlagenData->GetExtRnr(IntRnr, CalledPN, ExtCallingPN);
    };

    eBoolean GetExtRnr( BYTE *IntRnr, BYTE *ExtRnr, BYTE &Index )
    {
      return AnlagenData->GetExtRnr(IntRnr,ExtRnr,Index );
    }


    // MSN Index fÅr Rufnummer auslesen
    BYTE GetMSNIndex( BYTE *ExtRnr )
    {
        return AnlagenData->GetMSNIndex(ExtRnr); 
    };

    /*- Returns Current Berechtigung for this IntRnr. -*/
    eAmtsBerechtigung GetBerechtigung(BYTE* IntRnr)
    {
        return AnlagenData->GetBerechtigung(IntRnr); 
    };


    /*------------ Needed by TE for the configuration. -------*/
    ePortConfiguration GetPortConfig(void)
    {
        return AnlagenData->GetExtPortConfig();
    };

    /*------------  -------*/
    eAnalogDevice GetDeviceType(BYTE* IntRnr)
    {
        return AnlagenData->GetAnalogDeviceType(IntRnr);
    };

    /*------------  -------*/
    eBoolean IsAnklopfSchutz(BYTE* IntRnr)
    {
        return AnlagenData->IsAnklopfSchutz(IntRnr);
    };

    // Teilnehmer gesperrt ?
    eBoolean IsGesperrt( BYTE *IntRnr )
    {
        return AnlagenData->IsGesperrt( IntRnr );
    }

    // Presentation abfragen
    eBoolean GetAPresentation( BYTE *IntRnr )
    {
        return AnlagenData->GetAPresentation( IntRnr );
    }

    eBoolean GetBPresentation( BYTE *IntRnr )
    {
        return AnlagenData->GetBPresentation( IntRnr );
    }

    eBoolean IsExternalPort( ePortId Port )
    {
        return AnlagenData->IsExternalPort( Port );
    }


    // automatische Amtsbelegung
    eBoolean GetAmtsbelegung(BYTE* IntRnr)
    {
        return AnlagenData->GetAmtsbelegung(IntRnr);
    }


  //**************************************************************
  // Interne Rufumleitung
  //************************************************************** 
    eBoolean GetInternalDiversion(BYTE* IntRnr,
                                  BYTE* DivRnr,                
                                  eDiversion& DivType)
    {
      return AnlagenData->GetInternalDiversion(IntRnr,DivRnr,DivType);
    }

    eBoolean TestInternalDiversion(BYTE* IntRnr,
                                   BYTE* DivRnr,                
                                   eDiversion DivType)
    {
      return AnlagenData->TestInternalDiversion(IntRnr,DivRnr,DivType);
    }

    eBoolean SetInternalDiversion(BYTE* IntRnr,
                                   BYTE* DivRnr,                
                                   eDiversion DivType)
    {
      return AnlagenData->SetInternalDiversion(IntRnr,DivRnr,DivType);
    }

    eBoolean DelInternalDiversion(BYTE* IntRnr)
    {
      return AnlagenData->DelInternalDiversion(IntRnr);
    }


    // --------------------------------------------------------------
    // Daten der Anlage abfragen 

    eBoolean GetMOH( void )
    {
        return AnlagenData->GetMOH();
    };

    // Get TE Port For My IntRnr.
    ePortId GetMyExtPort(BYTE* IntRnr)
    {
        return AnlagenData->GetExtPort(IntRnr);
    };
    
    // Get Anlagen Rnr.
    eBoolean GetAnlagenRnr(BYTE* AnlRnr)
    {
        return AnlagenData->GetAnlagenRnr(AnlRnr);
    }

  //******************************************************************
  // Rufnummer Verwaltung fÅr Externe Rufumleitung
  //******************************************************************
    #ifndef __WINDOS__
    #ifndef __DOS_EINRPC__
    eBoolean GetAllIntRnrForDiversion(SuppData Data,
                                      cIntRnrList& List);
    #endif
    #endif

    eBoolean GetAllIntRnrForDiversion(BYTE *ExtRnr,
                                      cIntRnrList& List);

    eBoolean GetDefaultMSN(BYTE *IntRnr, BYTE *ExtRnr )
    {
        return AnlagenData->GetDefaultMSN(IntRnr, ExtRnr);
    };
    




//*********************************************************************
// TÅrfreisprecheinrichtung
//*********************************************************************

#ifdef __TFE

    eBoolean SubscriberGetKontaktBerechtigung(BYTE* IntRnr)
    {
        return AnlagenData->SubscriberGetKontaktBerechtigung(IntRnr);
    }

    eBoolean SubscriberGetDoorOpenBerechtigung(BYTE* IntRnr)
    {
        return AnlagenData->SubscriberGetDoorOpenBerechtigung(IntRnr);
    }

    BYTE GetDoorOpenerTime( void )
    {
        return AnlagenData->GetDoorOpenerTime();
    }

    WORD GetSchaltkontaktTime( void )
    {
        return AnlagenData->GetSchaltkontaktTime();
    }

    // ZusatzlÑutwerk
    eZusatzLaeutwerk GetZusatzLWVerhalten(void)
    {
        return AnlagenData->GetZusatzLWVerhalten();
    }

    eBoolean SwitchZusatzLW( BYTE *ExtRnr )
    {
        return AnlagenData->SwitchZusatzLW( ExtRnr );
    }

    // Klingel Liste Methoden.
    
    // Get internal number for a (one of three) buzzer.
    BYTE GetKlingelIntNumbers(eTfeKlingelKnopf KnopfNummer, BYTE RnrList[MAX_INT_RING_ENTRIES][MAX_INT_RNR_LEN + 1])
    {
        return AnlagenData->GetKlingelIntNumbers(KnopfNummer, RnrList);
    }


    // Get external number for a (one of three) buzzer.

    eBoolean GetKlingelExtRnr(eTfeKlingelKnopf KnopfNummer, BYTE* Rnr)
    {
        return AnlagenData->GetKlingelExtRnr(KnopfNummer, Rnr);
    }


    // Readout of the external switch.
    eBoolean KlingelIsExtern(eTfeKlingelKnopf KnopfNummer)
    {
        return AnlagenData->KlingelIsExtern(KnopfNummer);
    }

    // Alarm Methods.

    // Get internal numberlist for the alarm.
    BYTE GetAlarmIntNumbers(BYTE RnrList[MAX_INT_ALARM_ENTRIES][MAX_INT_RNR_LEN + 1] )
    {
        return AnlagenData->GetAlarmIntNumbers(RnrList);
    }


    // Get external numberlist(2 numbers) for the alarm.
    eBoolean GetAlarmExtNumbers( BYTE ExtRnrList[MAX_EXT_ALARM_ENTRIES][MAX_EXT_RNR_LEN+1])
    {
        return AnlagenData->GetAlarmExtNumbers(ExtRnrList);
    }


    // Get Alarm action (internal/external number or no action)
    eAlarmAction GetAlarmAction(void)
    {
        return AnlagenData->GetAlarmAction();
    }


    // Klingel- und Alarmverhalten umschalten
    eBoolean SetKlingelExtSwitch(eBoolean Mode)
    {
        return AnlagenData->SetKlingelExtSwitch(eTfeKlingel1, Mode);
    }

    eBoolean SetAlarmAction( eAlarmAction act )
    {
        return AnlagenData->SetAlarmAction( act );
    }


    eBoolean GetExtAlarmIntDest(BYTE* IntRnr)
    {
        return AnlagenData->GetExtAlarmIntDest(IntRnr);
    }  
#endif  // __TFE


//**********************************************************************
// B-Kanal Verwaltung
//**********************************************************************
    void FreeBChann( eBCh Channel, BYTE Cei)
    {
        BChannVW.FreeBch( Channel, Cei);
    }


   void FreeBChann( eBCh Channel, BYTE Cei, BYTE index)
    {
        eBoolean    CallsPresent = eFalse;

        BChannVW.FreeBch(  Channel,  Cei, CallsPresent);

    };

    BYTE SetBChann(eBCh Channel, BYTE Cei, ePortId Port)
    {
      return BChannVW.SetVorgegebeneBch(Channel, Cei, Port);
    };


    eBoolean BchForPortUsed(ePortId Port)
    {
      return BChannVW.BchForPortUsed(Port);
    };

    eBCh FurtherBchForCes(eBCh channel, BYTE ces) {
      return BChannVW.FurtherBchForCes(channel, ces);
    }; 

    void SetCesForBch(eBCh BChannel,BYTE ces) {
     BChannVW.SetCesForBch(BChannel, ces);
    };

    eBCh GetBchForCes(BYTE ces){
     return BChannVW.GetBchForCes(ces);
    }; 

    BYTE ReserveBChann(eBCh Channel, BYTE Cei, ePBChannReservation Reservation)
    {
        return BChannVW.ReserveBch(Channel, Cei, Reservation );
    };

    BYTE GetReserveCount(eBCh cha){
        return BChannVW.GetReserveCount(cha);
    }; 

    eBCh GetFreeBChann(BYTE Cei)   
    {
         return BChannVW.GetAnyFreeBCh(Cei);   
    };

    eBCh GetBevorzugtenBChann(eBCh Channel, BYTE Cei)   
    {
         return BChannVW.GetBevorzugtBCh(Channel, Cei);   
    };

    eBoolean GetVorgeschriebenBChann(eBCh Channel, BYTE Cei)   
    {
         return BChannVW.GetVorgeschriebBCh(Channel,Cei);   
    };
    
    eBCh GetReservedBChann(BYTE CEI)
    {
         return BChannVW.GetReservedBCh(CEI);
    };

    void FreeReservedBChann( eBCh Channel, BYTE CEI)
    {
         BChannVW.FreeReservedBCh( Channel, CEI);
    }

    eBoolean GetReservationStatus( eBCh Channel)
    {
         return BChannVW.GetReserveStatus( Channel);
    }
 
private:

    eLeistungsMerkmal eval_lm_request( BYTE *LMStr, eConnectionState State, BYTE *Data );

    eLeistungsMerkmal lm_avl( BYTE *LMStr, eConnectionState State );

    eLeistungsMerkmal lm_anklopfschutz( BYTE *LMStr, eConnectionState State );

    eLeistungsMerkmal lm_tfe( BYTE *LMStr );

    eLeistungsMerkmal lm_ext_div( BYTE *LMStr, eConnectionState State, BYTE *Data );

    eLeistungsMerkmal lm_int_div( BYTE *LMStr, eConnectionState State, BYTE *Data );

    eLeistungsMerkmal lm_three_party( eConnectionState State );

    eLeistungsMerkmal lm_parken( BYTE *LMStr, eConnectionState State, BYTE *Data );


    eLeistungsMerkmal get_rnr(BYTE offset, eLeistungsMerkmal LM,
                                BYTE *LMStr, BYTE *Data);




    /*---------- Constructor ----------------*/
public:

    PortDataBase( int maxPortTln, AnlagenDaten*  AnlagenDaten, 
                  BYTE MaxBChannels)
    {
        AnlagenData = AnlagenDaten; 
        BChannVW.init(MaxBChannels);
    };


};



// Um im CASE auch Funktionen definieren zu kînnen :
#define Portdata PortDataBase

/*  ENDIF  */
#endif


