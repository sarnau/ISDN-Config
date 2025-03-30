/*
	partly but shamelessly stolen from:
	mdial 9/1996 by G. Zander <hifi@scorpio.in-berlin.de>
*/
#include <stdio.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/termios.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "is1.h"

extern FILE *debugfile;

int readAnswer(int ser,char *buf,int dbg);
int isOK(int ser,int dbg);
int isCheckSumOK(char *buf,int len,int dbg);
int writebuf(int ser, char *buf, int len,int do_csm, int dbg);
int write_cmd(int ser, char cmd, char cmd_off,char cmd_ext,int dbg);
int write6_cmd(int ser, char cmd, char cmd_off,unsigned short cmd_ext,int dbg);
int send_data(int ser, char cmd, char cmd_off,char *buf,int len,int dbg);
void synchronize(int ser);
void emptySer(int ser);
void sendBOTR(int ser,int dbg);
void sendBOTT(int ser,int dbg);
void sendBOTW(int ser,int dbg);
void sendEOT(int ser,int dbg);
void sendBOTRR(int ser,int dbg);
void sendEOTR(int ser,int dbg);
int lockSer(char *ser_dev);



static struct itimerval mzeit1,mzeit2;
static struct termios tser,tsero;
static int serori;
char lockfile[255]="";
char tmpbuff[1023];
char outbuff[1023];


static void dummy()
{
}
void minisleep(duration)
unsigned long duration;
{

        mzeit1.it_interval.tv_sec =
        mzeit1.it_interval.tv_usec =
        mzeit1.it_value.tv_sec = 0;
        mzeit1.it_value.tv_usec = duration << 10;
    /* OK, set the alarm. */
    signal(SIGALRM, dummy);
        setitimer(ITIMER_REAL,&mzeit1,&mzeit2);
        pause();
}

/* read one char from fd */
char readch(int fd,int *err,int dbg)
{
	static fd_set readseta;
	struct timeval timeout;
	unsigned char k;

	*err=0;

	timeout.tv_sec=1; /* used for checking the echo */
	timeout.tv_usec=0;
    FD_ZERO(&readseta);
    FD_SET(fd,&readseta);
    select(FD_SETSIZE,&readseta,NULL,NULL,&timeout);
    if (FD_ISSET(fd,&readseta)) {
          if (read(fd,&k,1)!=-1) {
              if (dbg>2) fprintf(debugfile," %X",k);
              return (k);
          }
	      perror("reading data");
		  *err=-1;
		  return(-1);
    }
	if (dbg) fprintf(debugfile,"\nTimeout while reading data!\n");
	*err=-1;
	return(-1);
}

int readAnswer(int ser,char *buf, int dbg)
{
	char *bp=buf;
	int i=0;
	int len=0;
	int err=0;
	
	if (dbg>2) fprintf(debugfile,"\n");
	while(!(len=(int)(readch(ser,&err,dbg) & 0x00FF)));
	if (err!=0) return(-1);
	buf[0]=(unsigned char)len;
	if (dbg>2) fprintf(debugfile,"\nWaiting...%d\n",len);
	if (dbg>2) fprintf(debugfile,"IN: ");
	for(i=0;i<len-1;i++)
		buf[i+1]=readch(ser,&err,dbg);
		if (err!=0) {
			if (dbg>2) fprintf(debugfile,"\nPremature...\n");
			return(-1);
		}
	if (dbg>2) fprintf(debugfile,"\nLen...%d\n",len);
	return(len);
}

int isOK(int ser,int dbg)
{
	int ret;
	if ((ret=readAnswer(ser,tmpbuff,dbg))==-1) {
		return(-1);
	} 

	return(tmpbuff[1] & 0x00FF);
}

int isCheckSumOK(char *buf,int len,int dbg)
{
	unsigned short *sp;
	unsigned short csm;

	sp=(unsigned short *)buf;
	csm=MkChecksum(&buf[4],len-4);
	if (dbg>2) fprintf(debugfile,"\nCheckSum: %08X %08X\n",sp[1],csm);
	return(csm==sp[1]);
}

int writech(int ser, char ch, int dbg)
{
	static fd_set writeseta;
	struct timeval timeout;

	if (dbg>2) fprintf(debugfile," %X",ch);
	timeout.tv_sec=1; /* used for checking the echo */
	timeout.tv_usec=0;
	FD_ZERO(&writeseta);
	FD_SET(ser,&writeseta);
	select(FD_SETSIZE,NULL,&writeseta,NULL,&timeout);
	if (FD_ISSET(ser,&writeseta)) {
		write(ser,&ch,1);
		return(1);
	}
	if (dbg>2)
		fprintf(debugfile,"\n!!!!!!!Error writing data to serial port!");
	return(0);
}

int writebuf(int ser, char *buf, int len,int do_csm, int dbg)
{
	unsigned short *sp;
	int i;
	unsigned short csm=0;

	if (do_csm) {
		sp=(unsigned short *)buf;
		csm=MkChecksum(&buf[4],len-4);
		sp[1]=csm;
		if (dbg>2 ) fprintf(debugfile,"\nCheckSum: %08X\n",csm);
	}
	if (dbg>2) fprintf(debugfile,"\nOUT: ");
	for(i=0;i<len;i++) if (!writech(ser,buf[i],dbg)) return(0);
	if (len>5)
		for(i=0;i<=9;i++) if (!writech(ser,'\0',dbg)) return(0);
	return(1);
}

int write_cmd(int ser, char cmd, char cmd_off,char cmd_ext,int dbg)
{
	outbuff[0]=5;
	outbuff[1]=cmd;
	outbuff[2]=0;
	outbuff[3]=cmd_off;
	outbuff[4]=cmd_ext;

	if (!writebuf(ser,outbuff,5,0,dbg)) return(0);
	minisleep(20);
	return(1);
}

int write6_cmd(int ser, char cmd, char cmd_off,unsigned short cmd_ext,int dbg)
{
	unsigned short *dummy;

	outbuff[0]=6;
	outbuff[1]=cmd;
	outbuff[2]=0;
	outbuff[3]=cmd_off;

	dummy=(unsigned short *)&outbuff[4];
	*dummy=cmd_ext;

	if (!writebuf(ser,outbuff,6,0,dbg)) return(0);
	minisleep(20);
	return(1);
}

int send_data(int ser, char cmd, char cmd_off,char *buf,int len,int dbg)
{
	int addlen=5;
	int ret;

	memset(outbuff,0,1023);

	if (cmd_off==-1) addlen=4;

	outbuff[0]=(unsigned char)len+addlen;
	outbuff[1]=cmd;
	outbuff[2]=0;
	outbuff[3]=0;
	if (cmd_off!=-1) outbuff[4]=cmd_off;
	memcpy(outbuff+addlen,buf,len);
	ret=writebuf(ser,outbuff,len+addlen,1,dbg);
	minisleep(20);
	return(ret);
}

void sendBOTR(int ser,int dbg)
{
	if (!write_cmd(ser,K_V24_RD_EINRICHT_START,'\0',0,dbg)) {
		tcflush(ser,TCIOFLUSH);
	}
}

void sendBOTT(int ser,int dbg)
{
	write_cmd(ser,0x0D,'\0',0,dbg);
}

void sendBOTW(int ser,int dbg)
{
	write_cmd(ser,K_V24_WR_EINRICHT_START,'\0',0,dbg);
}

void sendEOT(int ser,int dbg)
{
	write_cmd(ser,K_V24_EINRICHT_ENDE,'\0',0,dbg);
}

void sendBOTRR(int ser,int dbg)
{
	write_cmd(ser,K_V24_EINRICHT_START_REPEAT,'\0',0,dbg);
}

void sendEOTR(int ser,int dbg)
{
	write_cmd(ser,K_V24_EINRICHT_ENDE_REPEAT,'\0',0,dbg);
}

void synchronize(int ser)
{
	char buf[256];
	memset(buf,0,255);
	write(ser,buf,255);
}

void emptySer(int ser)
{
	char ch;

	while (read(ser,&ch,1)>0);
}


int lockSer(char *ser_dev)
{
	static int i,n,lock,pid;
	char buf[256];
	struct stat stt;

	if (LOCKDIR[0] && stat(LOCKDIR, &stt) == 0)
		sprintf(lockfile, "%s/LCK..%s", LOCKDIR, basename(ser_dev));
	else
		lockfile[0] = 0;

	if (lockfile[0] && (lock = open(lockfile, O_RDONLY)) >= 0) {
		n = read(lock, buf, 127);
		if (n > 0) {
			pid = -1;
			if (n == 4)
				/* Kermit-style lockfile. */
				pid = *(int *)buf;
			else {
				/* Ascii lockfile. */
				buf[n] = 0;
				sscanf(buf, "%d", &pid);
			}
			if (pid > 0 && kill((pid_t)pid, 0) < 0 && errno == ESRCH) {
				fprintf(stderr, "Lockfile is stale. Overriding it..\n");
				sleep(1);
				unlink(lockfile);
			} else
				n = 0;
		}
		if (n == 0) {
			fprintf(stderr, "Device %s is locked.\n", ser_dev);
			return(-1);
		}
	}

	if ( lockfile[0]) {
		/* Create lockfile compatible with UUCP-1.2 */
		if ((lock = open(lockfile, O_WRONLY | O_CREAT | O_EXCL, 0666)) < 0) {
			perror(lockfile);
			fprintf(stderr, "Cannot create lockfile. Sorry.\n");
			return(-1);
		}
		(void) umask(022);
		sprintf(buf, "%05d maximaX\n", getpid());
		write(lock, buf, strlen(buf));
		close(lock);
	}
	return(0);
}

int openSer(char *ser_dev)
{
	int ser;

	if (lockSer(ser_dev)) return(-1);
	ser=open(ser_dev,O_RDWR|O_NDELAY|O_EXCL);	
	if (ser==-1) {
		fprintf(stderr,"Cannot open %s, errno %d\n",ser_dev,errno);
		if (lockfile[0]) unlink(lockfile);
		return(-1);
	}

	tcgetattr(ser,&tsero);
	tser=tsero;
	tser.c_iflag &= ~(IGNBRK | IGNCR | INLCR | ICRNL | 
#ifndef __FreeBSD__
		IUCLC |
#endif
		IXANY | IXON | IXOFF | INPCK | ISTRIP);
	tser.c_iflag |= (BRKINT | IGNPAR | IGNBRK);
	tser.c_oflag &= ~OPOST;
	tser.c_lflag &= ~(ICANON | ISIG | ECHO | ECHONL | ECHOE | ECHOK);
	tser.c_cflag &= ~(HUPCL | CRTSCTS | CSIZE) ;
	tser.c_cflag |= CREAD | CRTSCTS | CS8 | CSTOPB | CLOCAL;
	tser.c_cc[VMIN] = 1;
	tser.c_cc[VTIME] = 5;
	cfsetospeed(&tser,B4800);
	cfsetispeed(&tser,B4800);
	tcsetattr(ser, TCSANOW, &tser);

	return(ser);
}

int
closeSer(int ser)
{
	fcntl(ser,F_SETFL,serori);
	tser=tsero;
	tcflush(ser,TCIOFLUSH);
	tcsetattr(ser,TCSANOW,&tser);
	
	close(ser);
	if (lockfile[0]) unlink(lockfile);
	return(0);
}
