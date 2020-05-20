#include <16f877.h>  //derleyici k�t�phanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullan�lmayan baz� �zelliklerini kapat
#device ADC = 16 // 16 bit ��z�n�rl�kl� ADC kullanaca��z
#use delay(clock=4M) // osilat�r h�z� 4 Mhz olarak ayarla
#define LED pin_B0      //uyar� ledi RE2 portuna ba�l�
#define BUTON1 pin_A0
#include<lcd.c>         //LCD ekran k�t�phanesi
 

void main() {
   int16 analogVeri=0;
   
   //buton okumak i�in analog giri� ayarlamalar�  
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   //setup_timer_0(T0_DISABLED);
   //setup_timer_1(T1_DISABLED); 
   setup_timer_2(T2_DISABLED,0,1);
   // no comparator (voltage) NO voltage reference
   //setup_comparator(NC);
   
   set_tris_b(0b00000000); // B portunu komple OUTPUT yapar
   output_b(0);      // B portunu 0V yapar
   
   setup_adc_ports(AN0); //buton AN0 Portuna ba�l�
   
   lcd_init();
   
   lcd_putc("\f");
   delay_ms(300);      //300 ms bekle
   lcd_putc("Hello");
   delay_ms(300);      //300 ms bekle
   printf(lcd_putc, "World"); //    \f lcd ekran�n� temizle ve 1.1. koordinat�na al   
   delay_ms(300);      //300 ms bekle
      
      
   
   //loop blo�u s�rekli tekrar eder   
   //WHILE kodu sonsuz d�ng�ye sokar
   // ama TIMER1 kesmesini aktif etti�imiz i�in TIMER1 devreye girer
   while(true) {
      set_adc_channel(0); // AN0 numaral� kanaldan okuma yapaca��m�z� belirttik
      analogVeri = read_adc(); // AN0 numaral� kanaldan analog de�eri oku
      printf(lcd_putc, "\f");
      printf(lcd_putc, "deger= %Lu" ,analogVeri);
      delay_ms(50);    
      
   }         
}

