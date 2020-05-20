#include <16F887.h>
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD //kullan�lmayan baz� �zelliklerini kapat
#use delay(clock = 4MHz)  //delay komutu i�in gerekli

//port giri� ��k��lar�n� biz kendimiz belirleyece�imiz i�in
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

const int secilen_deger=15536;     //kesme i�lemi i�in kullan�lacak DE�ER
//ayr�nt�l� hesaplama EXCEL dosyas�nda mevcut

unsigned int8  i=0;
unsigned int8 hour=23, minute=57, second=50;
unsigned int8 alarmH=23, alarmM=58 ;
unsigned int8 state=0; 
//char time[] = "TIME:  :  :  ";   //zaman verisini saklamak i�in

int8 miliSaniye=0;

#int_timer1          //timer1 KESMESI (INTERRUPT)
void timer1_kesme() {   
   // https://simple-circuit.com/pic16f877a-timer1-module-interrupt/  
   // Timer1_freq = MCU_freq / {4 * Prescaler * (65536 � TMR1)}
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

   //state 0 ise ekranda SAAT� g�ster
  if (state==0) {
     output_a(((miliSaniye / 10) << 4) + (miliSaniye % 10));  //milisaniye
     output_d(((second / 10) << 4) + (second % 10));   //saniye
     output_c(((minute / 10) << 4) + (minute % 10));  //dakika
     
     int8 saatTest=((hour % 10)>>0) ;
     //SAAT 1 LER BASAMA�I
     output_bit(PIN_B4,saatTest & 0b00000001);  //bitwise 
     output_bit(PIN_B5,saatTest & 0b00000010);  
     output_bit(PIN_B6,saatTest & 0b00000100);  
     output_bit(PIN_B7,saatTest & 0b00001000);
     
     output_e((hour / 10));  //SAAT 10LAR basma��  
  }
  
  //state 1 ise ekranda ALARMI g�ster. (saniye ve milisaniye yok sayar)
  if (state==1) {
     output_a(0x00);  //milisaniye
     output_d(0x00);   //saniye
     output_c(((alarmM / 10) << 4) + (alarmM % 10));  //dakika
     
     int8 alarmTest=((alarmH % 10)>>0) ;
     //ALARM SAATi 1 LER BASAMA�I
     output_bit(PIN_B4,alarmTest & 0b00000001);  //bitwise 
     output_bit(PIN_B5,alarmTest & 0b00000010);  
     output_bit(PIN_B6,alarmTest & 0b00000100);  
     output_bit(PIN_B7,alarmTest & 0b00001000);
     
     output_e((alarmH / 10));  //SAAT 10LAR basma��
      
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
  
    //YANIP S�NEN DE�ER� 1 ARTIRMAK ���N
    while (!input(BUTON3)){                       // If button2 is pressed
      parameter++;
      if(i==0  && parameter > 23)               // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i==1 && parameter > 59)               // If minutes > 59 ==> minutes = 0
        parameter = 0;
      delay_ms(200);                             // Wait 200ms      
   }
   
    //YANIP S�NEN DE�ER� 1 AZALTMAK ���N
    while (!input(BUTON4)){                       // If button2 is pressed
      parameter--;
      if(i==0  && parameter < 0)               //saat  0 dan k���kse 23 ayarlar
        parameter = 23;
      if(i==1 && parameter < 0)               // dakika 0 dan k���kse 59 ayarlar
        parameter = 59;
      delay_ms(200);                             // Wait 200ms      
   }
  
   //EKRANDA YANIP S�NME KISMI
   if (i==0 ) {
      //editlenmek istenen yer SAAT ise EKRANDA 55 G�STER, SS �LE BENZED��� ���N
      //editlenmek istenen yer ALARM ise EKRANDA AA G�STER Alarm oldu�unu belli etmek i�in      
      
      //SAAT ED�TLEN�YOR
      if (tip==0) {
         output_e(0x05);      //SAAT 10 lar basama�� i�in pic16f877 yeterli pin ��k��� olmad���ndan en sol baca�� GND ba�l�
         //bunun i�in 8 verisini buraya yazamay�z. zaten gerek te yok ��nk� saat 00-23 olabilir. 10 lar basama��na da 0,1,2 
         //yaz�labilir. yan�p s�nerken 8 yerine ona en benzeyen say� olan 6 g�r�n�r.
         //saatin 1 ler basama�� i�in
         output_bit(PIN_B4,1);
         output_bit(PIN_B5,0);
         output_bit(PIN_B6,1);
         output_bit(PIN_B7,0);         
      }
      
      //ALARM SAAT KISMI ED�TLEN�YOR 
      if (tip==1) {
         output_e(0x06);      //SAAT 10 lar basama�� i�in pic16f877 yeterli pin ��k��� olmad���ndan en sol baca�� GND ba�l�
         //bunun i�in 8 verisini buraya yazamay�z. zaten gerek te yok ��nk� saat 00-23 olabilir. 10 lar basama��na da 0,1,2 
         //yaz�labilir. yan�p s�nerken 8 yerine ona en benzeyen say� olan 6 g�r�n�r.
         //saatin 1 ler basama�� i�in
         output_bit(PIN_B4,0);
         output_bit(PIN_B5,1);
         output_bit(PIN_B6,0);
         output_bit(PIN_B7,1);     
      }
      
   }   
   else if (i==1) {
      //editlenmek istenen yer SAAT'in DAK�KAs� �SE ekranda dd ile g�ster
      //ED�TLENMEK �STENEN YER ALARM DAK�KASI �SE EKRANDA AA g�ster
      if (tip==0) {
         output_c(0xdd);
      }
      if (tip==1) {
         output_c(0xaa);
      }      
   }
   blink();    
   delay_ms(50);
   
   
   //O ANK� PARAMETRE VER�S�N� BCD OLARAK YAZ
   if (i==0) {     
      //MEVCUT SAAT DE�ER�N�N B�RLER BASAMA�I
      
      output_bit(PIN_B4,(parameter%10) & 0x1); 
      output_bit(PIN_B5,(parameter%10) & 0x2);  
      output_bit(PIN_B6,(parameter%10) & 0x4);
      output_bit(PIN_B7,(parameter%10) & 0x8);
      output_e(parameter /10);      

   }
   else if (i==1) {
      //MEVCUT DAK�KA VER�S�
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
  port_b_pullups(0b00001111);       //pull up direnci ile d��me bas�l� de�ilse 5v, bas�ld���nda GND olacak
  
  //use fast_io sebebi
  set_tris_a(0b00000000);
  set_tris_b(0b00001111);           //b PORTUnun 0-3 kadar olanlar� giri� olark ayarla �IKI� OLARAK ayarla
  set_tris_c(0b00000000);
  set_tris_d(0b00000000);
  set_tris_e(0b000);                //
  
  delay_ms(100);

  setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
  set_timer1(secilen_deger);
  //clear_interrupt(INT_TIMER1);        //timer1 ta�ma bayra��n� s�f�rla   
  enable_interrupts(INT_TIMER1);      //T�MER 1 KESMES�N� AKT�F YAP   
  enable_interrupts(GLOBAL);          //KESMELERE �Z�N VER 
  delay_ms(100);
  
  
  
  //state=0 saati ekranda g�sterme
  //state=1 alarm� ekranda g�sterme durumu
  //state=2 editleme modu
  
  while(TRUE){  
  
    if(!input(BUTON1) && state==0){ // B1 bas�ld� ve STATE==0 ise ALARMI 10 sn g�ster tekrar SAAT� GER� G�STER
      while (!input(BUTON1));    //buton1 b�rak�lana kadar bekle
      state=1;    
    }
    
    else if(!input(BUTON1) && state==1){ // B1 bas�ld� ve STATE==1 ise ED�T MODUNA G�R
      while (!input(BUTON1));    //buton1 b�rak�lana kadar bekle
      state=2;      
   }   
   else if (state==2) { //STATE 2 oldu�unda ED�T MODUNA GE�
      disable_interrupts(INT_TIMER1);      //T�MER 1 KESMES�N� PAS�F YAP      
      setup_timer_1(FALSE); // disable timer
      i=0;
      hour = edit(hour,0);            //0 parametresi saatin editlendi�ini s�yl�yor
      minute = edit(minute,0);        //0 parametresi saatin editlendi�ini s�yl�yor
      i=0;      
      alarmH=edit(alarmH,1);          //1 parametresi ALARM editlemeyi belirtiyor 
      alarmM=edit(alarmM,1);     //1 parametresi ALARM editlemeyi belirtiyor
      delay_ms(200);
      setup_timer_1(TRUE); // disable timer
      enable_interrupts(INT_TIMER1);      //T�MER 1 KESMES�N� AKT�F YAP     
      state=0;
   }
   display();
   delay_ms(50);
   
  }
}
