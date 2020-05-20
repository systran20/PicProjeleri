#include <16f877.h>  //derleyici kütüphanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullanýlmayan bazý özelliklerini kapat
#use delay(clock=4M) // osilatör hýzý 4 Mhz olarak ayarla

void main() {

   //önce gerekli ayarlamalarý yap
   //char dizi[]={63,6,91,79,102,109,125,7,127,111};
   char dizi[]={63,6,91,79,102,109,125,7,127,111};
   
   set_tris_c(0x00); // 1ler basamaðý C portuna baðlý
   
   output_c(0);
   int x=0;
   //loop bloðu sürekli tekrar eder
   while(1) {
      output_c(dizi[x]);
      delay_ms(100);
      x++;
      if (x>9) {
         x=0;
      }
   }   
}

