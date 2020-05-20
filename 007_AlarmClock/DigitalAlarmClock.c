#include <16F887.h>
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullanýlmayan bazý özelliklerini kapat
#use delay(clock = 4MHz)  //delay komutu için gerekli

//port giriþ çýkýþlarýný biz kendimiz belirleyeceðimiz için
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)

#define BUTON1 PIN_B0
#define BUTON2 PIN_B1
#define BUTON3 PIN_B2
#define BUTON4 PIN_B3
#define LED PIN_A5

const int secilen_deger=15536;     //kesme iþlemi için kullanýlacak DEÐER
//ayrýntýlý hesaplama EXCEL dosyasýnda mevcut

unsigned int8  i=0;
unsigned int8 hour=23, minute=57, second=50;
unsigned int8 alarmH=23, alarmM=58 ;
unsigned int8 state=0; 
//char time[] = "TIME:  :  :  ";   //zaman verisini saklamak için

int8 miliSaniye=0;

#int_timer1          //timer1 KESMESI (INTERRUPT)
void timer1_kesme() {   
   // https://simple-circuit.com/pic16f877a-timer1-module-interrupt/  
   // Timer1_freq = MCU_freq / {4 * Prescaler * (65536 – TMR1)}
   // Prescaler rate of the timer1 can be: 1, 2, 4 or 8.   
   
   //clear_interrupt(INT_TIMER1);
   //set_timer1(secilen_deger);   
   miliSaniye++;
   
   if (miliSaniye>9) {
      miliSaniye=0;
      second++;      
   }      
   
   if (second==60) {
      second=0;
      minute++;      
   }
   
   if (minute==60) {
      minute=0;
      hour++;
   }
   if (hour==24) {
      hour=0;
   }
   
   
   if (alarmH==hour && alarmM==minute) { 
      output_high(LED);
      return;
   }
   output_low(LED);
   
}

void display(){

   //state 0 ise ekranda SAATÝ göster
  if (state==0) {
     output_a(((miliSaniye / 10) << 4) + (miliSaniye % 10));  //milisaniye
     output_d(((second / 10) << 4) + (second % 10));   //saniye
     output_c(((minute / 10) << 4) + (minute % 10));  //dakika
     
     int8 saatTest=((hour % 10)>>0) ;
     //SAAT 1 LER BASAMAÐI
     output_bit(PIN_B4,saatTest & 0b00000001);  //bitwise 
     output_bit(PIN_B5,saatTest & 0b00000010);  
     output_bit(PIN_B6,saatTest & 0b00000100);  
     output_bit(PIN_B7,saatTest & 0b00001000);
     
     output_e((hour / 10));  //SAAT 10LAR basmaðý  
  }
  
  //state 1 ise ekranda ALARMI göster. (saniye ve milisaniye yok sayar)
  if (state==1) {
     output_a(0x00);  //milisaniye
     output_d(0x00);   //saniye
     output_c(((alarmM / 10) << 4) + (alarmM % 10));  //dakika
     
     int8 alarmTest=((alarmH % 10)>>0) ;
     //ALARM SAATi 1 LER BASAMAÐI
     output_bit(PIN_B4,alarmTest & 0b00000001);  //bitwise 
     output_bit(PIN_B5,alarmTest & 0b00000010);  
     output_bit(PIN_B6,alarmTest & 0b00000100);  
     output_bit(PIN_B7,alarmTest & 0b00001000);
     
     output_e((alarmH / 10));  //SAAT 10LAR basmaðý
      
  }   
  
}

void blink(){
  int8 j = 0;
  while(j < 10 && input(BUTON1) && input(BUTON2) && input(BUTON3)){
    j++;
    delay_ms(25);
  }
}

unsigned int16 edit(signed int8 parameter, tip){
  //while(!input(BUTON1));                         // Wait until button released  
  while(TRUE){
  
    //YANIP SÖNEN DEÐERÝ 1 ARTIRMAK ÝÇÝN
    while (!input(BUTON3)){                       // If button2 is pressed
      parameter++;
      if(i==0  && parameter > 23)               // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i==1 && parameter > 59)               // If minutes > 59 ==> minutes = 0
        parameter = 0;
      delay_ms(200);                             // Wait 200ms      
   }
   
    //YANIP SÖNEN DEÐERÝ 1 AZALTMAK ÝÇÝN
    while (!input(BUTON4)){                       // If button2 is pressed
      parameter--;
      if(i==0  && parameter < 0)               //saat  0 dan küçükse 23 ayarlar
        parameter = 23;
      if(i==1 && parameter < 0)               // dakika 0 dan küçükse 59 ayarlar
        parameter = 59;
      delay_ms(200);                             // Wait 200ms      
   }
  
   //EKRANDA YANIP SÖNME KISMI
   if (i==0 ) {
      //editlenmek istenen yer SAAT ise EKRANDA 55 GÖSTER, SS ÝLE BENZEDÝÐÝ ÝÇÝN
      //editlenmek istenen yer ALARM ise EKRANDA AA GÖSTER Alarm olduðunu belli etmek için      
      
      //SAAT EDÝTLENÝYOR
      if (tip==0) {
         output_e(0x05);      //SAAT 10 lar basamaðý için pic16f877 yeterli pin çýkýþý olmadýðýndan en sol bacaðý GND baðlý
         //bunun için 8 verisini buraya yazamayýz. zaten gerek te yok çünkü saat 00-23 olabilir. 10 lar basamaðýna da 0,1,2 
         //yazýlabilir. yanýp sönerken 8 yerine ona en benzeyen sayý olan 6 görünür.
         //saatin 1 ler basamaðý için
         output_bit(PIN_B4,1);
         output_bit(PIN_B5,0);
         output_bit(PIN_B6,1);
         output_bit(PIN_B7,0);         
      }
      
      //ALARM SAAT KISMI EDÝTLENÝYOR 
      if (tip==1) {
         output_e(0x06);      //SAAT 10 lar basamaðý için pic16f877 yeterli pin çýkýþý olmadýðýndan en sol bacaðý GND baðlý
         //bunun için 8 verisini buraya yazamayýz. zaten gerek te yok çünkü saat 00-23 olabilir. 10 lar basamaðýna da 0,1,2 
         //yazýlabilir. yanýp sönerken 8 yerine ona en benzeyen sayý olan 6 görünür.
         //saatin 1 ler basamaðý için
         output_bit(PIN_B4,0);
         output_bit(PIN_B5,1);
         output_bit(PIN_B6,0);
         output_bit(PIN_B7,1);     
      }
      
   }   
   else if (i==1) {
      //editlenmek istenen yer SAAT'in DAKÝKAsý ÝSE ekranda dd ile göster
      //EDÝTLENMEK ÝSTENEN YER ALARM DAKÝKASI ÝSE EKRANDA AA göster
      if (tip==0) {
         output_c(0xdd);
      }
      if (tip==1) {
         output_c(0xaa);
      }      
   }
   blink();    
   delay_ms(50);
   
   
   //O ANKÝ PARAMETRE VERÝSÝNÝ BCD OLARAK YAZ
   if (i==0) {     
      //MEVCUT SAAT DEÐERÝNÝN BÝRLER BASAMAÐI
      
      output_bit(PIN_B4,(parameter%10) & 0x1); 
      output_bit(PIN_B5,(parameter%10) & 0x2);  
      output_bit(PIN_B6,(parameter%10) & 0x4);
      output_bit(PIN_B7,(parameter%10) & 0x8);
      output_e(parameter /10);      

   }
   else if (i==1) {
      //MEVCUT DAKÝKA VERÝSÝ
      output_c(((parameter / 10) << 4) + (parameter % 10));
   }
   blink();
   delay_ms(50);   
   
    
   
   if(!input(BUTON2)){                          // If button RB0 is pressed
     While(!input(BUTON2));
     i++;
     return parameter;                          // Return parameter value and exit
   }  
 }
}

void main(){
  
  setup_adc_ports(NO_ANALOGS);
  setup_adc(ADC_OFF);
  port_b_pullups(0b00001111);       //pull up direnci ile düðme basýlý deðilse 5v, basýldýðýnda GND olacak
  
  //use fast_io sebebi
  set_tris_a(0b00000000);
  set_tris_b(0b00001111);           //b PORTUnun 0-3 kadar olanlarý giriþ olark ayarla ÇIKIÞ OLARAK ayarla
  set_tris_c(0b00000000);
  set_tris_d(0b00000000);
  set_tris_e(0b000);                //
  
  delay_ms(100);

  setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
  set_timer1(secilen_deger);
  //clear_interrupt(INT_TIMER1);        //timer1 taþma bayraðýný sýfýrla   
  enable_interrupts(INT_TIMER1);      //TÝMER 1 KESMESÝNÝ AKTÝF YAP   
  enable_interrupts(GLOBAL);          //KESMELERE ÝZÝN VER 
  delay_ms(100);
  
  
  
  //state=0 saati ekranda gösterme
  //state=1 alarmý ekranda gösterme durumu
  //state=2 editleme modu
  
  while(TRUE){  
  
    if(!input(BUTON1) && state==0){ // B1 basýldý ve STATE==0 ise ALARMI 10 sn göster tekrar SAATÝ GERÝ GÖSTER
      while (!input(BUTON1));    //buton1 býrakýlana kadar bekle
      state=1;    
    }
    
    else if(!input(BUTON1) && state==1){ // B1 basýldý ve STATE==1 ise EDÝT MODUNA GÝR
      while (!input(BUTON1));    //buton1 býrakýlana kadar bekle
      state=2;      
   }   
   else if (state==2) { //STATE 2 olduðunda EDÝT MODUNA GEÇ
      disable_interrupts(INT_TIMER1);      //TÝMER 1 KESMESÝNÝ PASÝF YAP      
      setup_timer_1(FALSE); // disable timer
      i=0;
      hour = edit(hour,0);            //0 parametresi saatin editlendiðini söylüyor
      minute = edit(minute,0);        //0 parametresi saatin editlendiðini söylüyor
      i=0;      
      alarmH=edit(alarmH,1);          //1 parametresi ALARM editlemeyi belirtiyor 
      alarmM=edit(alarmM,1);     //1 parametresi ALARM editlemeyi belirtiyor
      delay_ms(200);
      setup_timer_1(TRUE); // disable timer
      enable_interrupts(INT_TIMER1);      //TÝMER 1 KESMESÝNÝ AKTÝF YAP     
      state=0;
   }
   display();
   delay_ms(50);
   
  }
}
