 /****************************************************************************/
 /* Funktionsname   : C h e c k s u m                                        */
 /*--------------------------------------------------------------------------*/
 /* Aufgabe         : Berechnet die CRC-Summe f�r ein Feld vorgegebener L�nge*/
 /*                                                                          */
 /* Eingabe-Param.  : Zeiger auf den Feldanfang                              */
 /*                   L�nge des Feldes                                       */
 /*                                                                          */
 /* Return-Wert     : 16 Bit-CRC-Summe                                       */
 /****************************************************************************/

 unsigned short  MkChecksum (unsigned char *Buffer, unsigned short Len )
 {
   register unsigned short Checks = 0;

   for ( ; Len ; Len-- )
   {
     if ( Checks & 01 )
       Checks = (unsigned short)(( Checks >> 1 ) + 0x8000 );
     else
       Checks >>= 1;

     Checks = (unsigned short)( Checks + (unsigned short)*Buffer );
     Buffer++;
   }
   return Checks;
 }

