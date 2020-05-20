#include <16f877.h>  //derleyici kütüphanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullanýlmayan bazý özelliklerini kapat
#use delay(clock=4M) // osilatör hýzý 4 Mhz olarak ayarla

void main() {

   //önce gerekli ayarlamalarý burada yap (SETUP BLOÐU)
   
   //dizideki sayýlar 0 1 2 3 4 5 6 7 8 9 rakamlarýný oluþturmak için
   //hangi bacaðýn 5V, hangisinin 0V olmasýný belirler 
   char dizi[]={63,6,91,79,102,109,125,7,127,111}; 
   
   char birler=0; // 1ler basamaðý deðiþkeni
   char onlar=0;  // 10ler basamaðý deðiþkeni
   set_tris_c(0x00); //C portunun tüm bacaklarýný OUTPUT olarak ayarla
   set_tris_d(0x00); //D portunun tüm bacaklarýný OUTPUT olarak ayarla 
   output_c(0);   // C portunun tüm bacaklarýný LOW / 0V yap
   output_d(0);   // D portunun tüm bacaklarýný LOW / 0V yap
      
   //LOOP bloðu sürekli tekrar eder
   while(1) {
   
      output_c(dizi[birler]);
      output_d(dizi[onlar]);
      
      delay_ms(100); //100 ms bekler
      birler++;
      if (birler>9) {
         birler=0;
         onlar++;
      }
      if (onlar>9) {
         onlar=0;
      }
   }   
}

