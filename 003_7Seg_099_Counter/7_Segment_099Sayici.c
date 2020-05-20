#include <16f877.h>  //derleyici k�t�phanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullan�lmayan baz� �zelliklerini kapat
#use delay(clock=4M) // osilat�r h�z� 4 Mhz olarak ayarla

void main() {

   //�nce gerekli ayarlamalar� burada yap (SETUP BLO�U)
   
   //dizideki say�lar 0 1 2 3 4 5 6 7 8 9 rakamlar�n� olu�turmak i�in
   //hangi baca��n 5V, hangisinin 0V olmas�n� belirler 
   char dizi[]={63,6,91,79,102,109,125,7,127,111}; 
   
   char birler=0; // 1ler basama�� de�i�keni
   char onlar=0;  // 10ler basama�� de�i�keni
   set_tris_c(0x00); //C portunun t�m bacaklar�n� OUTPUT olarak ayarla
   set_tris_d(0x00); //D portunun t�m bacaklar�n� OUTPUT olarak ayarla 
   output_c(0);   // C portunun t�m bacaklar�n� LOW / 0V yap
   output_d(0);   // D portunun t�m bacaklar�n� LOW / 0V yap
      
   //LOOP blo�u s�rekli tekrar eder
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

