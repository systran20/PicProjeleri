#include <16f877.h>  //derleyici k�t�phanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullan�lmayan baz� �zelliklerini kapat
#use delay(clock=4M) // osilat�r h�z� 4 Mhz olarak ayarla

void main() {

   //�nce gerekli ayarlamalar� yap
   //char dizi[]={63,6,91,79,102,109,125,7,127,111};
   char dizi[]={63,6,91,79,102,109,125,7,127,111};
   
   set_tris_c(0x00); // 1ler basama�� C portuna ba�l�
   
   output_c(0);
   int x=0;
   //loop blo�u s�rekli tekrar eder
   while(1) {
      output_c(dizi[x]);
      delay_ms(100);
      x++;
      if (x>9) {
         x=0;
      }
   }   
}

