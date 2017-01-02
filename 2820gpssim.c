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

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void uart_puts(const char *s);
void EEPROM_write(unsigned int ucAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int ucAddress);

int main (void) { 
   
   EEPROM_write(0x0A,0x11);

   char APRSstring[50];
   strcpy(APRSstring, "$$CRC843B,DF2ET>API282,DSTAR*:!5129.64N/00714.43Ey/Florian N18 Bochum\r");

   UBRRH = (uint8_t)(MYUBBR >> 8);
   UBRRL = (uint8_t)(MYUBBR);

   UCSRB = (1<<TXEN);

   UCSRC = (1 << USBS) | (3 << UCSZ0);

   DDRD |= _BV(PD5);

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
