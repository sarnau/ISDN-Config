/*** *	SharewareFlags.h : About-Dialogbox, etc anpassen ***/#define BETA_VERSION		0#define DEMO_VERSION		0#define DEBUG_VERSION		0// Version f�r exakt eine Anlage erzeugen#define ISTEC_ONLY			1#define EURACOM_ONLY		0#define ELMEG_ONLY			0#define MORE_VERSIONS		0// Programmcode f�r folgende Anlagen �bersetzen#if MORE_VERSIONS#define INCLUDE_ISTEC		1#define INCLUDE_EURACOM		1#define INCLUDE_ELMEG		1#endif#if ISTEC_ONLY#define INCLUDE_ISTEC		1#define INCLUDE_EURACOM		0#define INCLUDE_ELMEG		0#endif#if EURACOM_ONLY#define INCLUDE_ISTEC		0#define INCLUDE_EURACOM		1#define INCLUDE_ELMEG		0#endif#if ELMEG_ONLY#define INCLUDE_ISTEC		0#define INCLUDE_EURACOM		0#define INCLUDE_ELMEG		1#endif#if INCLUDE_ISTEC | INCLUDE_EURACOM | INCLUDE_ELMEG#define NEED_PROGRESS_BAR	1#else#define NEED_PROGRESS_BAR	0#endif