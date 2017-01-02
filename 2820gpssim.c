/*
 * 2820gpssim
 * Fixed GPS Position Simulator for IC-2820
 *
 * Created: 22.12.2016
 *
 */

#define F_CPU 8000000UL
#define BAUDRATE 9600
#define MYUBBR ((F_CPU / (BAUDRATE * 16L)) - 1)
#define VERSION 0.1

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void uart_puts(const char *s);
void EEPROM_write(unsigned int ucAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int ucAddress);
uint16_t calc_crc_ccitt(uint16_t crc, const char *buffer, int len);

int main (void) {

   UBRRH = (uint8_t)(MYUBBR >> 8);
   UBRRL = (uint8_t)(MYUBBR);

   UCSRB = (1<<TXEN);
   UCSRC = (1 << USBS) | (3 << UCSZ0);

   DDRD |= _BV(PD5);

   /*
    * write callsign to 0x00
    */
   EEPROM_write(0x00,0x44);
   EEPROM_write(0x01,0x46);
   EEPROM_write(0x02,0x32);
   EEPROM_write(0x03,0x45);
   EEPROM_write(0x04,0x54);
   EEPROM_write(0x05,0x20);
   EEPROM_write(0x06,0x20);
   EEPROM_write(0x07,0x20);
   /*
    * write APRS destination to 0x08
    */
   EEPROM_write(0x08,0x41);
   EEPROM_write(0x09,0x50);
   EEPROM_write(0x0A,0x49);
   EEPROM_write(0x0B,0x32);
   EEPROM_write(0x0C,0x38);
   EEPROM_write(0x0D,0x32);

   char APRSstring[50];

   int i = 0;
   for (i = 0; i<8; i++) {
      char c;
      c = EEPROM_read(i);
      if (c != ' ') { 
         strcat(APRSstring, &c);
      }
   }
   strcat(APRSstring, ">");
   for (i = 8; i<14; i++) {
      char c;
      c = EEPROM_read(i);
      strcat(APRSstring, &c);
   }
   /*
    * Add via
    */
   strcat(APRSstring, ",DSTAR*:!");
   /*
    * Add latitude
    */
   strcat(APRSstring, "5129.64N");
   /*
    * Add symbol table selector
    */
   strcat(APRSstring, "/");
   /*
    * Add longitude
    */
   strcat(APRSstring, "00714.43E");
   /*
    * Add symbol
    */
   strcat(APRSstring, "y");
   /*
    * Add separator
    */
   strcat(APRSstring, "/");
   /*
    * Add comment
    */
   strcat(APRSstring, "Florian N18 Bochum");
   /*
    * Add carriage return at the end of the line
    */
   strcat(APRSstring, "\r");

   // uint16_t crc;
   // uint16_t crc16;
   // crc = calc_crc_ccitt(0xFFFF, APRSstring, 60);
   // crc16 = (crc ^ 0xFFFF);
   // char lo = crc16 & 0xFF;
   // char hi = crc16 >> 8;
   // strcat(APRSstring, &lo);

   //strcpy(APRSstring, "$$CRC843B,DF2ET>API282,DSTAR*:!5129.64N/00714.43Ey/Florian N18 Bochum\r");

   for (;;) {
      PORTD |= _BV(PD5);
      uart_puts(APRSstring);
      PORTD &=~ _BV(PD5);
      _delay_ms(3000);
   }
   return 0;

}

void uart_puts(const char *s) {
   while (*s) {
      while (!(UCSRA & (1<<UDRE)));
      UDR = *s;
      s++;
   }
}

void EEPROM_write(unsigned int ucAddress, unsigned char ucData) {
   /* 
    * Wait for completion of previous write
    *  */
   while(EECR & (1<<EEPE));
   /* 
    * Set Programming mode */
   EECR = (0<<EEPM1)|(0<<EEPM0);
   /* Set up address and data registers */
   EEAR = ucAddress;
   EEDR = ucData;
   /* 
    * Write logical one to EEMPE */
   EECR |= (1<<EEMPE);
   /* Start eeprom write by setting EEPE */
   EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int ucAddress) {
   /* Wait for completion of previous write */
   while(EECR & (1<<EEPE));
   /* Set up address register */
   EEAR = ucAddress;
   /* 
    * Start eeprom read by writing EERE */
   EECR |= (1<<EERE);
   /* Return data from data register */
   return EEDR;
}

// Polynome 0x8408
const uint16_t __flash crc_ccitt_table[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

uint16_t calc_crc_ccitt(uint16_t crc, const char *buffer, int len) {
   while (len--) {
      char c = *buffer++;
      crc = (crc >> 8) ^ crc_ccitt_table[(crc ^ c) & 0xff];
   }
   return crc;
}
