/*-------------------------------------------------------------------------
   ser_ir.h - source file for serial routines

   Copyright (C) 1999, Josef Wolf <jw AT raven.inka.de>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#include "ser_ir.h"

/* This file implements a serial interrupt handler and its supporting
* routines. Compared with the existing serial.c and _ser.c it has
* following advantages:
* - You can specify arbitrary buffer sizes (umm, up to 255 bytes),
*   so it can run on devices with _little_ memory like at89cx051.
* - It won't overwrite characters which already are stored in the
*   receive-/transmit-buffer.
*/

/* BUG: those definitions (and the #include) should be set dynamically
* (while linking or at runtime) to make this file a _real_ library.
*/
#include <stc12.h>
#define XBUFLEN 4
#define RBUFLEN 8

/* You might want to specify idata, pdata or xdata for the buffers */
static unsigned char __pdata rbuf[RBUFLEN], xbuf[XBUFLEN];
static unsigned char rcnt, xcnt, rpos, xpos;
static __bit busy;
#if 0
void
ser_init (void)
{
   ES = 0;
   rcnt = xcnt = rpos = xpos = 0;  /* init buffers */
   busy = 0;
   AUXR |= 0x40; //��ʱ��1ʱ��ΪFosc,��1T
   AUXR &= 0xFE; //����1ѡ��ʱ��1Ϊ�����ʷ�����
   SCON = 0x50;  //8λ����,�ɱ䲨����
   PCON |= 0x80; /* SMOD = 1; �����ʱ���*/
   TMOD &= 0x0f; /* use timer 1 */
   TMOD |= 0x20; //�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ

   // Buad rate = (2^SOMD / 32) * (T1�����)
   // Buad rate = (2^SOMD / 32) * ((12000000Hz / 1��Ƶ) / (256 - 217))
   TL1 = 178; TH1 = 178; ET1 = 0; TR1 = 1;    /* 19200bps with 12MHz crystal */
   ES = 1;
}
#else

void ser_init (void)		//9600bps@11.0592MHz
{
    ES = 0;
    rcnt = xcnt = rpos = xpos = 0;  /* init buffers */
    busy = 0;

	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��1ʱ��ΪFosc/12,��12T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ

	TL1 = 0xFA;		//�趨��ʱ��ֵ
	TH1 = 0xFA;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

   PS = 1;
	ES = 1;
}

#endif

void
ser_handler (void) __interrupt 4
{
   if (RI) {
	   RI = 0;
	   /* don't overwrite chars already in buffer */
	   if (rcnt < RBUFLEN)
		   rbuf [(unsigned char)(rpos+rcnt++) % RBUFLEN] = SBUF;
   }
   if (TI) {
	   TI = 0;
	   if (busy = xcnt) {   /* Assignment, _not_ comparison! */
		   xcnt--;
		   SBUF = xbuf [xpos++];
		   if (xpos >= XBUFLEN)
			   xpos = 0;
	   }
   }
}

void
ser_putc (unsigned char c)
{
   while (xcnt >= XBUFLEN) /* wait for room in buffer */
	   ;
   ES = 0;
   if (busy) {
	   xbuf[(unsigned char)(xpos+xcnt++) % XBUFLEN] = c;
   } else {
	   SBUF = c;
	   busy = 1;
   }
   ES = 1;
}

unsigned char
ser_getc (void)
{
   unsigned char c;
   while (!rcnt)   /* wait for character */
	   ;
   ES = 0;
   rcnt--;
   c = rbuf [rpos++];
   if (rpos >= RBUFLEN)
	   rpos = 0;
   ES = 1;
   return (c);
}

#pragma save
#pragma noinduction
void
ser_puts (unsigned char *s)
{
   unsigned char c;
   while (c=*s++) {
	   if (c == '\n') ser_putc ('\r');
	   ser_putc (c);
   }
}
#pragma restore

void
ser_gets (unsigned char *s, unsigned char len)
{
   unsigned char pos, c;

   pos = 0;
   while (pos <= len) {
	   c = ser_getc ();
	   if (c == '\r') continue;        /* discard CR's */
	   s[pos++] = c;
	   if (c == '\n') break;           /* NL terminates */
   }
   s[pos] = '\0';
}

unsigned char
ser_can_xmt (void)
{
   return XBUFLEN - xcnt;
}

unsigned char
ser_can_rcv (void)
{
   return rcnt;
}
