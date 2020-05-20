#include <16f877.h>  //derleyici kütüphanesini ekle
#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullanýlmayan bazý özelliklerini kapat
#device ADC = 16 // 16 bit çözünürlüklü ADC kullanacaðýz
#use delay(clock=4M) // osilatör hýzý 4 Mhz olarak ayarla
#define LED pin_B0      //uyarý ledi RE2 portuna baðlý
#define BUTON1 pin_A0
#include<lcd.c>         //LCD ekran kütüphanesi
 

void main() {
   int16 analogVeri=0;
   
   //buton okumak için analog giriþ ayarlamalarý  
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
   
   setup_adc_ports(AN0); //buton AN0 Portuna baðlý
   
   lcd_init();
   
   lcd_putc("\f");
   delay_ms(300);      //300 ms bekle
   lcd_putc("Hello");
   delay_ms(300);      //300 ms bekle
   printf(lcd_putc, "World"); //    \f lcd ekranýný temizle ve 1.1. koordinatýna al   
   delay_ms(300);      //300 ms bekle
      
      
   
   //loop bloðu sürekli tekrar eder   
   //WHILE kodu sonsuz döngüye sokar
   // ama TIMER1 kesmesini aktif ettiðimiz için TIMER1 devreye girer
   while(true) {
      set_adc_channel(0); // AN0 numaralý kanaldan okuma yapacaðýmýzý belirttik
      analogVeri = read_adc(); // AN0 numaralý kanaldan analog deðeri oku
      printf(lcd_putc, "\f");
      printf(lcd_putc, "deger= %Lu" ,analogVeri);
      delay_ms(50);    
      
   }         
}

