#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../is1.h"


void showTransferErr(char *msg);
void stop_conn(int ser, int err, int dbg);
int getNextPacket(int ser, char *buf, int dbg);
void dumppacket(char *titel,char *buf,int len);

void showTransferErr(char *msg)
{
	fprintf(stderr,"Fehler bei der Uebertragung: %s\n",msg);
}

void stop_conn(int ser, int err, int dbg)
{
	char msg[100];

	if(dbg && err!=K_WR_OK) fprintf(stderr,"ERROR: #%04X !!!!!!!!!!!!!!\n",err);
	if (err) synchronize(ser);
	sendEOT(ser,dbg);
	isOK(ser,dbg);
	closeSer(ser);
	switch (err) {
		case K_WR_OK:
		case 0:
			break;
		case K_CRC_ERR:
			showTransferErr("CRC-Fehler");
			break;
		case -1:
		case K_V24_ERR:
			showTransferErr("Uebertragungs-Fehler");
			break;
		case K_CMD_ERR:
			showTransferErr("CMD-Fehler");
			break;
		case K_CMD_PRIO_CHANGED:
			showTransferErr("Prio-Fehler");
			break;
		default:
			sprintf(msg,"Unbekannter Fehler %04X",err);
			showTransferErr(msg);
			break;
	}
	return;
}

int getNextPacket(int ser, char *buf, int dbg)
{
	int ret;

	memset(buf,0,1024);
	ret=readAnswer(ser,buf,dbg);
	if (ret==-1){
    	stop_conn(ser,K_V24_ERR,dbg);
    	return(0);
   	}
   	if (!isCheckSumOK(buf,ret,dbg)){
   		stop_conn(ser,K_CRC_ERR,dbg);
   		return(0);
   	}
	return(ret);		
}

void dumppacket(char *titel,char *buf,int len)
{
    int i,j;

    fprintf(stderr,"\n\n%s\n",titel);
    for(i=0;;i+=8) {
        for(j=0;j<8;j++) {
            if (i+j>len) break;
            fprintf(stderr,"%02.2X ",(buf[i+j] & 0x0FF));
        }
        for(j=0;j<8;j++) {
            if (i+j>len) return;
            fprintf(stderr,"%c ",
                (buf[i+j]>=32 && buf[i+j]<=126)?buf[i+j]:'.');
        }
        fprintf(stderr,"\n");
    }
}

