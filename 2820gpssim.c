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

int main (void) {

   static const char APRSstring[] = "$$CRC07C9,DF2ET>API282,DSTAR*:!5129.64N/00714.43Ey000/000/A=000393Florian QRV on D-Star\r";

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
