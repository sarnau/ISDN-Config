/* 
 * 
 *  Projekt Nr.: 3030 
 *
 *  Modul      : GLTYPESH
 *
 */

/* Typdefinitionen fÅr GLTYPESH */
/*  #IFNDEF GLTYPES #DEFINE GLTYPES  */
#ifndef GLTYPES
#define GLTYPES


/*  PROZESSTYPEN  */
typedef enum 
{
    D3TProc,
    D3Management,
    D3VT,
    E3TProc,    // Intern EDSS1 L3
    E3TETProc,  // Extern EDSS1 L3
    VT_Proc,
    Euro_Proc,
    D2BroadCast,
    D2PtpEntity,
    D2Entity,
    D2Management,
    L1Llc,
    L2Llc,
    L1Management,
    D2Mux,
    D2Sapi,
    TimerTask,
    V24Task,
    V24RxTask,
    V24TxTask,
    MainTask,

// ab hier neu fuer tk-anlage
    VerbTonTask,
    RufTask,        // Hardware AB Ruf Erzeugung
    AbTask,         // Hardware AB Schleife/MFV Wahl auswerten
    VttestTask,     // NUR FUER TEST !!!???!!!
    ComTask,        // Kommunikation mit dem PC/Drucker via V.24
    EinrTask,       // Einrichter
    ChargingTask,   // GebÅhren Task
    TfeTask,        // TFE-Verwaltungstask
    TfePollingTask, // TFE-Pollingtask
    CTIL2RxTask,      // CTI-Layer2 Task
    CTIL2TxTask,
    TSPITask,       // Telephony Service Provider Task

#ifdef __TEST386
    VtDummy,
    CtrlLlc,
    IdecHandler,
    EpicHandler,
#endif

    HwTask,

    DebugTask

} ProzessTyp;

#define LowPrio 2

typedef enum
{
    PrioD3TProc          = LowPrio,
    PrioD3Management     = LowPrio,
    PrioD3VT             = LowPrio,
    PrioE3TProc          = LowPrio,
    PrioE3TETProc        = LowPrio,
    PrioVtProc           = LowPrio,
    PrioEuro_Proc        = LowPrio,
    PrioD2BroadCast      = LowPrio,
    PrioD2PtpEntity      = LowPrio,
    PrioD2Management     = LowPrio,
    PrioL1Llc            = LowPrio,
    PrioL2Llc            = LowPrio,
// FÅr TK ???    PrioCtrlLlc          = LowPrio,
    PrioL1Management     = LowPrio,
    PrioD2MuxRx          = LowPrio,
    PrioD2MuxTx          = LowPrio,
    PrioD2Sapi           = LowPrio,
    PrioTimerTask        = LowPrio + 1,
// FÅr TK ???    PrioVtDummy          = LowPrio,
// FÅr TK ???    PrioD3SduInd         = LowPrio,
// FÅr TK ???    PrioD3SduReq         = LowPrio,
// FÅr TK ???    PrioD4SduInd         = LowPrio,
// FÅr TK ???    PrioD4SduReq         = LowPrio,
// FÅr TK ???    PrioV24PcService     = LowPrio,
    PrioV24Task          = LowPrio,
    PrioV24RxTask        = LowPrio+1,
    PrioV24TxTask        = LowPrio,

// ab hier tk anlage
    PrioVerbTonTask      = LowPrio,
    PrioRufTask          = LowPrio,
    PrioAbTask           = LowPrio,
    PrioTfeTask          = LowPrio,
    PrioTfePollingTask   = LowPrio,
    PrioVttestTask       = LowPrio,   // dummy vttask fuer ab-test 
    PrioComTask          = LowPrio,   
    PrioEinrTask         = LowPrio,
    PrioChargingTask     = LowPrio,
    PrioCTILayer2Task    = LowPrio,
    PrioTSPITask         = LowPrio,
// Sonstiges
    PrioDbgCtrl          = LowPrio,


    // NUR FUER TEST MIT 386
    PrioIDECRxTask       = LowPrio,
    PrioIDECTxTask       = LowPrio,

    PrioHSCXRxTask       = LowPrio,
    PrioHSCXTxTask       = LowPrio,

    PrioDummyTask        = LowPrio,
    PrioHwTask           = LowPrio

} TaskPrio;




  #define StandardStack 2048


typedef enum
{
    StackD3TProc          = StandardStack,
    StackD3Management     = StandardStack,
    StackD3VT             = StandardStack,
    StackE3TProc          = StandardStack,
    StackE3TETProc        = StandardStack,
    StackVT_Proc          = StandardStack,
    StackEuro_Proc        = StandardStack,
    StackD2BroadCast      = StandardStack,
    StackD2PtpEntity      = StandardStack,
    StackD2Management     = StandardStack,
    StackL1Llc            = StandardStack,
    StackL2Llc            = StandardStack,
    StackCtrlLlc          = StandardStack,
    StackL1Management     = StandardStack,
    StackD2MuxRx          = StandardStack,
    StackD2MuxTx          = StandardStack,
    StackD2Sapi           = StandardStack,
    StackTimerTask        = StandardStack,
   
// FÅr TK ???    StackD3SduInd         = StandardStack,
// FÅr TK ???    StackD3SduReq         = StandardStack,
// FÅr TK ???    StackD4SduInd         = StandardStack,
// FÅr TK ???    StackD4SduReq         = StandardStack,
// FÅr TK ???    StackV24PcService     = StandardStack,
    StackV24Task          = StandardStack,
    StackV24RxTask        = StandardStack,
    StackV24TxTask        = StandardStack,

// ab hier tk anlage
    StackVerbTonTask      = StandardStack,
    StackRufTask          = StandardStack,
    StackAbTask           = StandardStack, 
    StackTfeTask          = StandardStack,
    StackTfePollingTask   = StandardStack,
    StackVttestTask       = StandardStack, 
    StackComTask          = StandardStack, 
    StackEinrTask         = StandardStack,
    StackChargingTask     = StandardStack, 
    StackCTILayer2Task    = StandardStack,
    StackTSPITask         = StandardStack,

    StackDbgCtrl          = StandardStack,

    // NUR FUER TEST MIT 386
    StackIDECRxTask       = StandardStack,
    StackIDECTxTask       = StandardStack,

    StackHSCXRxTask       = StandardStack,
    StackHSCXTxTask       = StandardStack,

    StackDummyTask        = StandardStack,

    StackHwTask           = StandardStack

} TaskStacks;



/*  MAILBOX SLOT -GRî·EN  */
#define StandardSlotSize  5

// die max. Anzahl der MailboxeintrÑge fÅr die Schicht-2 Prozesse
// D2PtpEntity, D2Entity und D2BroadCast mu· immer grî·er sein als
// die max. Anzahl der IQueue-EintrÑge

#define D2ProcSlotSize    20

typedef enum 
{
    MBSlotD3TProc       = StandardSlotSize,
    MBSlotD3Management  = StandardSlotSize,
    MBSlotD3VT          = StandardSlotSize,
    MBSlotD2BroadCast   = D2ProcSlotSize,
    MBSlotD2PtpEntity   = D2ProcSlotSize,
    MBSlotD2Entity      = D2ProcSlotSize,
    MBSlotD2Management  = StandardSlotSize,
    MBSlotL1Llc         = StandardSlotSize,
    MBSlotL2Llc         = StandardSlotSize,
    MBSlotL1Management  = StandardSlotSize,
    MBSlotD2Mux         = StandardSlotSize,
    MBSlotD2Sapi        = StandardSlotSize,
    MBSlotTimerTask     = StandardSlotSize
} MboxSlotSize;



/*  PROTOCOL DISCRIMINATOR  */
typedef enum
{
//    ProtDiscN0 = 0x40,
//    ProtDiscN1 = 0x41,
    ProtDiscEuro = 0x08

} ProtDisc; 

/*  MESSAGE TYPES  */
typedef enum
{

// Typedefinition fÅr MessageType fÅr Nachrichten mit Protocol
// Discriminator EURO

//  Konstante   Code      Beschreibung               

    ESCape       = 0x00, // Escape - Code
    Alerting     = 0x01,   
    CallProc     = 0x02,
    Connect      = 0x07,
    ConnectAck   = 0x0f,
    Progress     = 0x03,
    SETup        = 0x05,
    SetAck       = 0x0d,
    Resume       = 0x26,
    ResumeAck    = 0x2e,
    ResumeRej    = 0x22,
    Suspend      = 0x25,
    SuspendAck   = 0x2d,
    SuspendRej   = 0x21,
    Disconnect   = 0x45,
    Release      = 0x4d,
    ReleaseCompl = 0x5a,
    Restart      = 0x46,
    RestartAck   = 0x4e,
    Information  = 0x7b,
    Notify       = 0x6e,
    StatusX      = 0x7d,
    StatusEnq    = 0x75,

       // fÅr Dienste einrichten:

    Hold         = 0x24,
    HoldAck      = 0x28,
    HoldRej      = 0x30,
    Retrieve     = 0x31,
    RetrieveAck  = 0x33,
    RetrieveRej  = 0x37,
    Facility     = 0x62,
    Register     = 0x64

} MessageType;


/*  PORTTYPES  */
typedef enum
{
    UnknownPortType,        // Nicht belegt
    NTPortType,             // Interner S0
    TEPortType,             // Externer S0
    ABPortType,             // a/b
    TFEPortType             // TÅrfreisprecheinrichtung
}ePortType;


/*  GERÑTEKONFIGURATION  */
typedef enum
{
    MGK      = 0x00,
    NStAnl   = 0x01,
    EuroP_P  = 0x02,
    EuroP_MP = 0x03

} EndgerKonfig; 

/*  MSGTYPES ZUM VERBINDUNGEN EIN / AUSSCHALTEN  */
#define     VERBINDUNG_CONNECT      1
#define     VERBINDUNG_DISCONNECT   2

/*  PROTOCOL TYPES  */
#define PROTOCOL_1TR6   0
#define PROTOCOL_DSS1   1



/*  EREIGNISSE ZUR VT (EDSS1)  */
typedef enum
{
    EvIndRestartConfirm = 20,
    EvIndNoEvent,
    EvIndAlerting,
    EvIndConnect,
    EvIndDisconnect,
    EvIndInfo,
    EvIndMoreInfo,
    EvIndNotify,
    EvIndProceeding,
    EvIndProgress,
    EvIndRelease,
    EvIndReleaseComplete,
    EvIndReleaseConfirm,
    EvIndResume,
    EvIndSetupConfirm,
    EvIndSetup,
    EvIndStatus,
    EvIndSuspend,
    EvIndTimeout,
      
    // neue fuer die user-side
    EvIndReqResume,
    EvIndReqSuspend,
    EvIndReject,
    EvIndError,
    EvIndSetupConfirmError,
    EvIndSetupCompleteError,
    EvIndSetupComplete,
    EvIndSuspendConfirmTimeError,
    EvIndSuspendConfirm,    
    EvIndSuspendConfirmError,
    EvIndResumeConfirm,
    EvIndResumeConfirmTimeError,
    EvIndReleaseConfirmError,
    EvIndReleaseError,
    EvIndStatusError,

    // Leistungsmerkmale

    EvIndFacility,

    EvIndHold,
    EvIndHoldAck,
    EvIndHoldRej,
    
    EvIndRetrieve,
    EvIndRetrieveAck,
    EvIndRetrieveRej

} VTIndMessageType;
 

/*  EREIGNISSE VON DER VT (EDSS1)  */
typedef enum
{
    EvReqNoEvent = 20,
    EvReqAlerting,
    EvReqDisconnect,
    EvReqInfo,
    EvReqMoreInfo,
    EvReqNotify,
    EvReqProceeding,
    EvReqProgress,
    EvReqReject,
    EvReqRelease,
    EvReqResumeRej,
    EvReqResumeResp,
    EvReqSetup,
    EvReqSetupResp,
    EvReqSetCompl,
    EvReqStatEnq,
    EvReqSuspendRej,
    EvReqSuspendResp,
    EvReqRestart,

// Erweiterungen 
    EvReqResume,
    EvReqSuspend,

    // Leistungsmerkmale
    EvReqFacility,

    EvReqHold,
    EvReqHoldAck,
    EvReqHoldRej,
    
    EvReqRetrieve,
    EvReqRetrieveAck,
    EvReqRetrieveRej


} VTReqMessageType;
 

/*  #ENDIF  */
#endif


