#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../is1.h"
#include "../gebkit.h"

IsdnKonfType iconf[2];
char *debugfile="/tmp/maximaX.debug";
int portnum;
int debugflag=0;

char *
get_Phonenumber(char *buf)
{
	static char ret[25];
	char *p;
	int i;
	unsigned char a,b;

	p=ret;

	if (buf[0]==-1) return("Analog");
	for (i=0;i<12;i++)	{
		a=(buf[i] >> 4) & 0x0F;
		b=buf[i] & 0x0F;
		if (a == 0x0F) {
			p[0]='\0';
			return(ret);
		}
		p[0]=(char)(a+48);
		p++;
		if (b == 0x0F) {
			p[0]='\0';
			return(ret);
		}
		p[0]=(char)(b+48);
		p++;
	}
	p[0]='\0';
	return(ret);
}

void readConfig(char *thedevname)
{
	char buf[1024];
	char dummy[10];
	int j,i,x,ret;
	unsigned int r;
	char a,b;
	int minuten,sekunden;
	int ser=0;
	int dbg=debugflag;
	Outgoing_CallsType *test;

	if ((ser=openSer(thedevname))==-1) return;
	synchronize(ser);
	sendBOTR(ser,dbg);
	if ((ret=isOK(ser,dbg))==K_WR_OK) {
		j=0;
		puts("App. Datum Uhrz. Gebuehr Dauer Nummer");
		for (i=0;;i++) {
			write6_cmd(ser,0x0D,0x00,(unsigned short)j,dbg);
			if (!(ret=getNextPacket(ser,buf,dbg))) return;
			if (buf[0]==0x04) {
				stop_conn(ser,K_WR_OK,dbg);
				return;
			}
			r=(unsigned int)buf[0] & 0x00FF;
			r=r / sizeof(Outgoing_CallsType);

			for (x=0;x<r;x++){
				test=(Outgoing_CallsType *)&buf[4+(sizeof(Outgoing_CallsType)*x)];
				j++;
				minuten=(test->Dauer / 60);
				sekunden=test->Dauer - (minuten*60);
				printf("%-4x %02d.%02d %02d:%02d ",
					test->Appnum,test->Tag,
					test->Monat,
					test->Stunde,
					test->Minute);

				sprintf(dummy,"%2.2f",(float)((float)test->Gebuehr / 100));

				printf("%7s %02d:%02d ",
					dummy,
					minuten,
					sekunden);

				puts(get_Phonenumber(&test->Nummer[0]));
			}
		}

		stop_conn(ser,K_WR_OK,dbg);
	} else stop_conn(ser,ret,dbg);
}

main(int argc, char **argv)
{
	char *thedevname;
	int i,j;

	if (!argv[1]) {
		fprintf(stderr,"Usage: %s /dev/cuaX\n",argv[0]);
		exit(1);
	}
	thedevname=argv[1];
	readConfig(thedevname);
}
