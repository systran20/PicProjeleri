#include <16f877.h>  //derleyici k�t�phanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullan�lmayan baz� �zelliklerini kapat
#use delay(clock=4M) // osilat�r h�z� 4 Mhz olarak ayarla

int8 i=0;
int8 iBCD=0;
void main() {
   set_tris_c(0b00000000); // C portunu OUTPUT yapar
   delay_ms(50);
   output_c(0x00);      // C portunu 0V yapar
   delay_ms(50);
   
   //loop blo�u s�rekli tekrar eder
   while(1) {
      iBCD = (i%10);
      output_c(iBCD);
      delay_ms(100);
      i++;
      if (i>9) {i=0;}
      
   }   
}

