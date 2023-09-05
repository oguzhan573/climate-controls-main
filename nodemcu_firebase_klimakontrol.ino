
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "" // http:// veya https:// olmadan yazın
#define FIREBASE_AUTH "f"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

FirebaseData veritabanim;
//burada tanımladığımız değişkenler ile komut vermediğimiz durumlarda kodun işlem yapmaması için durum değişkenleri tanımladık.
int butonstatus = 0;
int butondurumu = 0;
int sicaklik = 0;
int sicaklikdurumu = 0;
int fanhizi = 0;
int fandurumu = 0;
int isitmadurumu = 0;
int isitma = 0;

const uint16_t kIrLed = D2; 
IRsend irsend(kIrLed);  .

////////////////////////////////////////////////////////////////////////////////////////////

// IRrecvDumpV2.ino dosyası kullanarak seri monitörden okuduğumuz kumanda komutları
uint16_t klimaAC[59]  = {8878, 4182,  510, 1590,  484, 556,  490, 546,  482, 556,  488, 1600,  486, 556,  486, 550,  486, 550,  506, 536,  484, 564,  486, 558,  484, 564,  486, 558,  484, 564,  486, 558,  484, 540,  504, 
                        548,  488, 550,  484, 1598,  482, 1588,  482, 550,  486, 550,  484, 540,  502, 564,  486, 558,  486, 556,  486, 1600,  486, 1602,  484}; // Klima ON 18 derece cool fan 1

uint16_t klimaKAPAT[59] = {8852, 4178,  482, 1616,  486, 554,  482, 564,  484, 560,  484, 1602,  484, 552,  482, 560,  482, 548,  486, 1604,  484, 1608,  486, 564,  486, 560,  484, 546,  504, 558,  482, 560,  484, 556,  486, 
                          558,  486, 550,  484, 558,  484, 552,  482, 550,  486, 1586,  484, 550,  484, 1610,  484, 552,  484, 538,  504, 550,  484, 1610,  486}; // Klima off 18 derece cool fan 1
                             
uint16_t derece18_fan1_on[59] = {8848, 4208,  510, 1586,  484, 552,  484, 576,  460, 538,  502, 1584,  486, 548,  502, 556,  486, 556,  486, 548,  486, 558,  486, 552,  482, 546,  
                                504, 1602,  484, 550,  484, 548,  486, 560,  484, 538,  502, 550,  484, 1596,  482, 1592,  486, 550,  484, 558,  510, 524,  486, 550,  484, 1594,  486, 550,  484, 1610,  484, 1594,  484};  // 18 derece fan2

uint16_t derece18_fan2_on[59] = {8878, 4176,  480, 1608,  488, 558,  484, 554,  488, 550,  486, 1610,  462, 558,  486, 554,  488, 548,  486, 556,  486, 558,  484, 550,  508, 528, 
                                 484, 1588,  482, 558,  484, 550,  484, 560,  484, 562,  488, 558,  484, 1598,  488, 1592,  486, 1586,  484, 556,  486, 550,  484, 1606,  486, 558, 
                                 484, 1598,  488, 552,  484, 558,  484};  // 18 derece fan2

 uint16_t derece18_fan3_on[59] = {8868, 4160,  482, 1608,  486, 538,  504, 552,  484, 544,  506, 1574,  504, 540,  502, 560,  482, 560,  484, 538,  504, 550,  484, 558,  484, 558,  484, 1586,  486, 558,  484, 552,  484, 550,  486, 538,
                                   502, 552,  506, 1596,  460, 1586,  484, 558,  484, 562,  482, 1602,  484, 550,  486, 1584,  486, 1594,  484, 552,  484, 1594,  486}; // 18 derece fan3

 uint16_t derece19_fan1_on[59] = {8876, 4180,  510, 1600,  464, 558,  486, 556,  508, 536,  484, 1592,  486, 550,  484, 566,  484, 566,  486, 556,  486, 558,  484, 550,  484, 550,  484, 1616,  464, 550,  484, 558,  484, 558,  488, 556, 
                                  484, 1592,  486, 558,  494, 548,  484, 550,  506, 528,  484, 560,  484, 558,  484, 1606,  462, 1600,  488, 558,  484, 550,  490}; // 19 derece fan1

  uint16_t derece19_fan2_on[59] ={8878, 4176,  480, 1598,  488, 558,  482, 568,  500, 544,  486, 1598,  494, 544,  490, 544,  486, 556,  502, 540,  508, 528,  482, 568,  508, 534,  508, 1592,  504, 534,  486, 546,  494, 550,  504, 530, 
                                   494, 1602,  482, 536,  486, 556,  490, 1584,  506, 526,  486, 558,  506, 1568,  508, 530,  486, 1598,  510, 536,  488, 1588,  488};                                                                  

uint16_t derece19_fan3_on[59] ={8876, 4166,  480, 1598,  488, 560,  484, 546,  488, 550,  484, 1588,  482, 548,  486, 558,  484, 556,  488, 550,  484, 536,  506, 548,  486, 548,  486, 1596,  482, 550,  484, 558,  484, 558,  486, 562, 
                               488, 1602,  484, 550,  484, 556,  486, 564,  486, 556,  488, 1590,  488, 550,  506, 1572,  484, 1610,  482, 1572,  484, 554,  488};

uint16_t derece20_fan1_on[59]= {8876, 4196,  484, 1598,  510, 536,  484, 556,  510, 526,  508, 1564,  508, 534,  486, 556,  486, 556,  508, 528,  508, 526,  486, 558,  508, 534,  488, 1602,  506, 534,  490, 546,  482, 568,  508, 534, 
                                484, 1608,  502, 544,  508, 1574,  488, 550,  486, 548,  486, 550,  486, 550,  486, 1606,  488, 1602,  506, 534,  504, 1572,  504};
  
uint16_t derece20_fan2_on[59]= {8848, 4178,  480, 1606,  502, 544,  482, 568,  486, 556,  484, 1600,  510, 528,  502, 542,  508, 534,  508, 534,  484, 568,  508, 534,  482, 568,  508, 1566,  488, 550,  508, 526,  486, 556,  504, 530, 
                               486, 1598,  510, 536,  508, 1566,  504, 1588,  484, 526,  506, 536,  492, 1592,  488, 556,  492, 1598,  506, 1566,  512, 528,  496};

uint16_t derece20_fan3_on[59]={8872, 4154,  480, 1608,  488, 556,  508, 528,  484, 566,  496, 1580,  488, 556,  510, 534,  486, 558,  484, 556,  508, 528,  486, 556,  510, 532,  486, 1588,  506, 536,  508, 526,  486, 550,  486, 556,  
                               488, 1594,  508, 532,  506, 1578,  494, 538,  484, 558,  484, 1600,  486, 564,  510, 1570,  508, 1564,  506, 1564,  484, 1616,  484};

uint16_t derece21_fan1_on[59]={8872, 4154,  482, 1608,  488, 556,  486, 554,  482, 564,  486, 1592,  486, 556,  486, 558,  484, 560,  484, 556,  488, 550,  506, 536,  484, 554,  488, 1588,  484, 558,  484, 550,  486, 550,  486, 554, 
                               486, 1620,  460, 1592,  486, 552,  484, 550,  484, 560,  484, 564,  488, 558,  486, 1608,  486, 1586,  484, 1612,  482, 552,  482};

uint16_t derece21_fan2_on[59]={8850, 4206,  512, 1586,  486, 550,  486, 550,  484, 538,  504, 1586,  484, 548,  504, 538,  504, 538,  502, 550,  486, 558,  484, 552,  486, 546,  504, 1602,  484, 550,  486, 548,  484, 558,  486, 538, 
                               504, 1586,  484, 1600,  486, 558,  484, 1590,  504, 548,  488, 548,  486, 1592,  484, 560,  486, 1572,  506, 1584,  502, 1588,  504};

uint16_t derece21_fan3_on[59]={8848, 4204,  512, 1612,  460, 550,  486, 550,  486, 556,  486, 1592,  480, 564,  488, 556,  486, 556,  486, 550,  506, 536,  508, 528,  484, 564,  486, 1604,  506, 524,  488, 548,  486, 558,  486, 556, 
                               486, 1588,  506, 1580,  506, 534,  486, 554,  488, 556,  486, 1610,  484, 528,  508, 538,  488, 556,  488, 564,  488, 554,  488};

uint16_t derece22_fan1_on[59]={8876, 4182,  512, 1576,  486, 558,  486, 536,  506, 558,  486, 1582,  506, 548,  484, 560,  486, 546,  504, 546,  504, 558,  486, 546,  506, 548,  486, 1580,  504, 558,  484, 540,  528, 514,  504, 550,  
                               486, 1600,  486, 1600,  486, 1574,  504, 538,  504, 556,  486, 546,  506, 550,  486, 1584,  486, 1586,  484, 1608,  486, 1594,  484}; 

uint16_t derece22_fan2_on[59]={8902, 4154,  482, 1600,  488, 558,  484, 546,  504, 556,  486, 1582,  504, 548,  486, 548,  486, 558,  486, 536,  506, 550,  484, 546,  506, 538,  504, 1610,  486, 538,  504, 548,  488, 556,  484, 550,  
                               486, 1584,  486, 1608,  486, 1594,  484, 1592,  486, 538,  504, 558,  486, 1574,  504, 1592,  484, 550,  486, 538,  506, 548,  486};

uint16_t derece22_fan3_on[59]={8878, 4174,  482, 1606,  488, 558,  484, 556,  486, 548,  486, 1588,  482, 556,  488, 554,  486, 550,  484, 556,  488, 558,  484, 552,  506, 526,  486, 1588,  482, 558,  486, 550,  486, 558,  484, 564,  
                               486, 1606,  504, 1576,  488, 1596,  506, 528,  484, 550,  486, 1594,  484, 556,  486, 564,  488, 556,  488, 562,  488, 1598,  488};

uint16_t derece23_fan1_on[59]={8896, 4164,  482, 1584,  504, 558,  486, 548,  486, 548,  484, 1586,  484, 550,  484, 558,  484, 538,  504, 550,  484, 558,  486, 550,  486, 550,  484, 1594,  484, 552,  484, 558,  486, 558,  486, 1582, 
                               504, 552,  484, 566,  486, 556,  486, 548,  504, 558,  484, 540,  502, 550,  484, 558,  486, 556,  486, 558,  484, 548,  502};

uint16_t derece23_fan2_on[59]={8846, 4192,  482, 1582,  504, 550,  484, 550,  486, 538,  528, 1560,  486, 548,  486, 558,  486, 538,  506, 538,  504, 548,  488, 556,  486, 548,  488, 1592,  486, 556,  486, 558,  486, 544,  504, 1602,  
                               488, 538,  504, 540,  504, 546,  506, 1582,  504, 548,  486, 558,  484, 1586,  486, 1606,  486, 558,  486, 538,  504, 1590,  504};

uint16_t derece23_fan3_on[59]={8852, 4176,  482, 1608,  482, 562,  484, 566,  462, 582,  458, 1628,  486, 550,  484, 560,  482, 544,  502, 558,  482, 568,  486, 558,  486, 548,  500, 1594,  486, 550,  484, 552,  484, 560,  482, 1596,  
                               484, 558,  484, 558,  482, 554,  482, 552,  482, 554,  482, 1594,  484, 558,  458, 578,  482, 554,  480, 1614,  482, 554,  482};

uint16_t derece24_fan1_on[59]={8870, 4220,  484, 1600,  486, 558,  484, 538,  504, 550,  486, 1574,  498, 560,  482, 558,  484, 540,  502, 550,  484, 550,  484, 558,  486, 538,  506, 1602,  484, 540,  502, 552,  482, 548,  504, 1600, 
                               486, 550,  484, 540,  502, 1608,  486, 540,  504, 550,  484, 552,  482, 550,  486, 540,  504, 546,  504, 558,  484, 1582,  504};

uint16_t derece24_fan2_on[59]={8876, 4180,  480, 1582,  504, 558,  486, 546,  504, 556,  486, 1582,  504, 548,  486, 548,  488, 556,  486, 538,  504, 550,  486, 546,  506, 536,  504, 1608,  486, 538,  506, 548,  486, 556,  486, 1592,  
                              486, 550,  486, 550,  484, 1574,  504, 1586,  484, 546,  504, 558,  486, 1580,  504, 1584,  488, 556,  486, 1574,  504, 550,  486};

uint16_t derece24_fan3_on[59]={8872, 4188,  482, 1582,  504, 558,  486, 548,  486, 558,  484, 1602,  484, 538,  504, 550,  486, 550,  486, 548,  486, 550,  486, 548,  486, 550,  484, 1588,  482, 550,  486, 548,  484, 550,  486, 1610,  
                              482, 558,  486, 564,  484, 1606,  484, 564,  486, 558,  486, 1600,  484, 550,  486, 550,  486, 558,  484, 1594,  486, 1602,  484};

uint16_t derece25_fan1_on[59]={8900, 4140,  482, 1584,  528, 534,  486, 548,  486, 550,  486, 1584,  488, 548,  508, 534,  484, 540,  502, 550,  486, 556,  484, 550,  486, 548,  486, 1594,  484, 550,  484, 560,  484, 558,  484, 1584,  
                              502, 550,  486, 1590,  504, 556,  486, 548,  486, 548,  484, 550,  486, 548,  486, 558,  486, 558,  486, 1592,  486, 558,  486};

uint16_t derece25_fan2_on[59]={8878, 4166,  480, 1600,  510, 532,  486, 548,  488, 548,  490, 1580,  484, 550,  484, 558,  486, 538,  502, 550,  486, 556,  486, 548,  488, 548,  486, 1594,  484, 550,  484, 558,  484, 560,  486, 1582,  
                               502, 552,  486, 1590,  502, 558,  508, 1562,  482, 540,  504, 558,  486, 1610,  486, 1574,  502, 558,  484, 1602,  486, 1610,  484};

uint16_t derece25_fan3_on[59]={8850, 4178,  480, 1590,  504, 540,  528, 524,  502, 538,  506, 1580,  504, 550,  486, 558,  486, 540,  504, 538,  504, 546,  504, 538,  504, 546,  504, 1574,  504, 548,  486, 548,  486, 558,  486, 1592,  
                               486, 538,  504, 1574,  504, 548,  486, 548,  486, 558,  486, 1574,  504, 550,  486, 538,  504, 1584,  486, 550,  486, 550,  484};

uint16_t derece26_fan1_on[59]={8854, 4180,  480, 1608,  486, 558,  484, 564,  486, 558,  486, 1600,  486, 552,  484, 562,  482, 558,  508, 516,  504, 566,  484, 558,  486, 566,  508, 1570,  504, 532,  484, 550,  486, 558,  484, 1596,  
                               482, 556,  486, 1574,  504, 1588,  482, 564,  486, 558,  486, 546,  526, 526,  488, 550,  484, 550,  486, 1592,  486, 1588,  482}; 

uint16_t derece26_fan2_on[59]={8876, 4182,  512, 1588,  484, 556,  486, 548,  486, 558,  488, 1582,  504, 538,  502, 550,  486, 550,  482, 562,  482, 546,  506, 558,  484, 548,  502, 1604,  484, 548,  504, 538,  504, 540,  502, 1586,  
                               484, 552,  484, 1582,  504, 1594,  486, 1608,  486, 550,  486, 552,  484, 1582,  486, 1586,  484, 1588,  484, 550,  484, 556,  486};

uint16_t derece26_fan3_on[59]={8876, 4166,  480, 1582,  504, 558,  484, 550,  486, 550,  486, 1586,  482, 552,  486, 556,  488, 556,  484, 550,  484, 558,  484, 550,  486, 550,  484, 1594,  484, 550,  484, 558,  486, 558,  486, 1600,  
                              486, 548,  486, 1610,  484, 1592,  486, 550,  484, 558,  484, 1596,  486, 548,  484, 558,  486, 1582,  504, 558,  486, 1592,  486};

uint16_t derece27_fan1_on[59]={8858, 4174,  480, 1606,  488, 554,  488, 550,  484, 564,  488, 1590,  488, 556,  486, 558,  484, 558,  484, 556,  488, 548,  488, 558,  486, 554,  488, 1610,  480, 538,  486, 550,  486, 548,  488, 1590,  
                              488, 1586,  484, 550,  486, 554,  510, 534,  486, 558,  486, 564,  488, 558,  486, 554,  488, 1590,  488, 548,  486, 558,  486};

uint16_t derece27_fan2_on[59]={8878, 4172,  480, 1582,  502, 558,  486, 550,  482, 552,  486, 1590,  486, 558,  484, 550,  484, 560,  484, 538,  504, 560,  482, 558,  484, 550,  484, 1610,  484, 560,  484, 546,  504, 556,  486, 1588,  
                               482, 1594,  486, 550,  484, 552,  484, 1594,  486, 536,  502, 532,  506, 1574,  504, 1586,  484, 1584,  486, 556,  486, 1586,  484};

uint16_t derece27_fan3_on[59]= {8850, 4208,  508, 1590,  482, 552,  484, 550,  484, 540,  502, 1588,  482, 548,  504, 538,  504, 540,  502, 550,  484, 560,  482, 550,  486, 548,  502, 1604,  484, 550,  486, 548,  484, 560,  484, 1594,  
                                484, 1610,  486, 564,  486, 558,  484, 538,  502, 560,  484, 1596,  482, 550,  486, 548,  486, 1594,  484, 1610,  482, 560,  486};

uint16_t derece28_fan1_on[59]={8852, 4178,  480, 1590,  506, 538,  504, 546,  506, 536,  506, 1580,  506, 548,  486, 558,  486, 538,  504, 540,  502, 548,  504, 540,  504, 546,  504, 1576,  504, 548,  486, 550,  486, 558,  486, 1592,  
                               486, 1590,  506, 548,  484, 1594,  488, 538,  504, 546,  504, 538,  506, 538,  504, 538,  504, 1584,  486, 548,  486, 1600,  486};

uint16_t derece28_fan2_on[59]={8874, 4174,  478, 1584,  502, 558,  484, 550,  484, 550,  488, 1590,  488, 556,  484, 550,  486, 560,  482, 558,  484, 560,  486, 538,  504, 550,  484, 1612,  482, 558,  484, 566,  486, 556,  484, 1590,  
                              480, 1594,  484, 552,  486, 1586,  484, 1600,  464, 580,  484, 552,  484, 1594,  482, 1590,  484, 1602,  484, 1584,  502, 552,  482};

uint16_t derece28_fan3_on[59]={8870, 4156,  480, 1606,  512, 534,  484, 566,  492, 552,  482, 1600,  488, 550,  490, 552,  488, 554,  508, 536,  484, 568,  506, 536,  484, 566,  508, 1574,  506, 526,  486, 548,  486, 550,  486, 1604,  
                               506, 1576,  498, 546,  508, 1588,  484, 526,  490, 552,  508, 1566,  488, 550,  500, 540,  486, 1588,  506, 1566,  504, 1566,  504};

uint16_t derece29_fan1_on[59]={8846, 4170,  502, 1580,  504, 558,  486, 556,  484, 550,  484, 1584,  486, 558,  486, 548,  488, 538,  526, 542,  486, 558,  486, 546,  504, 556,  486, 1582,  506, 556,  486, 548,  486, 548,  486, 1586,  
                               484, 1594,  484, 1594,  510, 532,  486, 558,  486, 540,  504, 550,  484, 550,  486, 548,  484, 1586,  484, 1602,  508, 534,  486};

uint16_t derece29_fan2_on[59]={8850, 4176,  482, 1592,  504, 556,  486, 548,  502, 558,  486, 1582,  504, 550,  484, 558,  486, 558,  484, 558,  484, 566,  486, 540,  504, 566,  486, 1572,  504, 550,  486, 550,  484, 558,  486, 1594,  
                              486, 1606,  486, 1586,  484, 550,  484, 1594,  486, 558,  486, 556,  486, 1586,  484, 1582,  504, 1594,  484, 1576,  504, 1610,  484};

uint16_t derece29_fan3_on[59]={8854, 4178,  480, 1590,  504, 538,  504, 546,  504, 540,  504, 1600,  486, 550,  510, 532,  484, 538,  504, 538,  504, 546,  504, 538,  504, 546,  504, 1574,  504, 550,  484, 548,  488, 556,  484, 1596,  
                              484, 1590,  506, 1584,  484, 550,  486, 556,  486, 556,  486, 1590,  504, 558,  486, 1592,  486, 548,  486, 558,  484, 538,  504};

uint16_t derece30_fan1_on[59]={8852, 4206,  510, 1612,  460, 548,  486, 550,  486, 556,  486, 1610,  484, 540,  488, 554,  488, 554,  488, 550,  486, 556,  486, 550,  484, 564,  488, 1604,  506, 526,  486, 550,  484, 558,  484, 1590,  
                               488, 1606,  488, 1600,  488, 1598,  488, 556,  488, 558,  484, 556,  486, 556,  486, 550,  486, 1626,  460, 1598,  488, 1604,  482}; 

uint16_t derece30_fan2_on[59]={8900, 4178,  536, 1562,  510, 514,  504, 548,  484, 558,  510, 1558,  506, 538,  502, 550,  486, 548,  486, 556,  486, 546,  504, 558,  486, 546,  504, 1602,  486, 546,  528, 516,  504, 538,  504, 1584,  
                              486, 1592,  486, 1574,  504, 1586,  486, 1584,  484, 550,  486, 550,  486, 1584,  486, 556,  486, 538,  504, 550,  486, 548,  510};

uint16_t derece30_fan3_on[59]={8822, 4180,  510, 1586,  486, 550,  486, 538,  504, 548,  486, 1586,  486, 556,  486, 550,  484, 560,  486, 556,  486, 556,  486, 550,  486, 550,  486, 1594,  484, 550,  486, 558,  492, 530,  504, 1610,  
                               484, 1610,  486, 1602,  484, 1610,  486, 548,  486, 556,  484, 1612,  484, 540,  500, 1604,  484, 566,  484, 558,  484, 1610,  484};

uint16_t derece27_heat_fan3_on[59]={8876, 4182,  512, 1588,  484, 554,  488, 550,  486, 558,  484, 1598,  488, 554,  488, 550,  484, 548,  486, 558,  486, 564,  486, 558,  508, 540,  502, 1590,  482, 1606,  486, 532,  504, 550,  484, 1598, 
                               488, 1598,  480, 554,  488, 548,  486, 1610,  484, 550,  484, 1590,  480, 550,  486, 548,  510, 534,  486, 1590,  488, 558,  484};
void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Ağ Bağlantısı Oluşturuluyor");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP adresine bağlanıldı: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Ağ bağlantısı kesilirse tekrar bağlanmasına izin veriyoruz
  Firebase.reconnectWiFi(true);
  
  pinMode(D2, OUTPUT);

  //bu bölümde aşağıda tanımladığımız fonksiyonlar ile veritabanından veri çekiyoruz ve veritabanındaki son değerleri aşağıdaki durum değerlerine atamak için kullanacağız
  fangetir();
  sicaklikgetir(); 
  onoffgetir();
  isitmagetir();
  

  //veritabanından aldığımız ilk açılış değerlerini durum değişkenlerimize atıyoruz.
  butondurumu = butonstatus;
  sicaklikdurumu = sicaklik;
  fandurumu = fanhizi;
  sicaklikdurumu=sicaklik;

}

void loop()
{
  while (butonstatus == 0) 
  { //Klima durumu kapalı ise hiçbir işlem yapmayacak
    onoffgetir(); //whiledan çıkış için klima onoff durumunu sürekli kontrol ediyor.
    Serial.println("Klima Kapalı");
  }

 //her döngüde verileri tekrar veritabanından çekip güncelliyoruz.
  fangetir();
  sicaklikgetir();
  onoffgetir();
  isitmagetir();
  

  if (butondurumu == 0) 
  { //klimanın kapalı olma durumu

    if (butonstatus == 1) 
    { //açma komutu geliyor ve işleniyor
      irsend.sendRaw(klimaAC, 59, 38);
      Serial.println("Klima Açma Komutu gönderildi");
      butondurumu = 1;
      fandurumu = 1;
      sicaklikdurumu = 18;//fan durumu ve sicaklik durumu bilgisini de olması gereken şekle atıyoruz.

    }


  }
  else {//klimanın açık olduğu durum

    if (butonstatus == 0) 
    { //kapatma komutu gelirse kapat çık
      irsend.sendRaw(klimaKAPAT, 59, 38);
      Serial.println("Klima Kapatma Komutu gönderildi");
      butondurumu = 0;
    }
    else 
    { 

      if (sicaklik != sicaklikdurumu) 
      {
        
        switch (sicaklik)
        {
          case 18: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece18_fan1_on, 59, 38);
                Serial.println("18 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 18;
                break;

              case 2:
                irsend.sendRaw(derece18_fan2_on, 59, 38);
                Serial.println("18 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 18;
                break;

              case 3: 
                irsend.sendRaw(derece18_fan3_on, 59, 38);
                Serial.println("18 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 18;
                break;

              default:
                break;
            }

            break;

          case 19:
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece19_fan1_on, 59, 38);
                Serial.println("19 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 19;
                break;

              case 2: 
                irsend.sendRaw(derece19_fan2_on, 59, 38);
                Serial.println("19 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 19;
                break;

              case 3: 
                irsend.sendRaw(derece19_fan3_on, 59, 38);
                Serial.println("19 derece Fan 3 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 19;
                break;

              default:
                break;
            }

            break;
          case 20: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece20_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 20;
                break;

              case 2: 
                irsend.sendRaw(derece20_fan2_on, 211, 38);
                Serial.println("20 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 20;
                break;

              case 3: 
                irsend.sendRaw(derece20_fan3_on, 211, 38);
                Serial.println("20 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 20;
                break;

              default:
                break;
            }
                case 21: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 21;
                break;

              case 2: 
                irsend.sendRaw(derece21_fan2_on, 59, 38);
                Serial.println("21 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 21;
                break;

              case 3: 
                irsend.sendRaw(derece21_fan3_on, 211, 38);
                Serial.println("21 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 21;
                break;

              default:
              break;
            }
            break;
            case 22: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece22_fan1_on, 59, 38);
                Serial.println("22 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 22;
                break;

              case 2: 
                irsend.sendRaw(derece22_fan2_on, 59, 38);
                Serial.println("22 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 22;
                break;

              case 3: 
                irsend.sendRaw(derece22_fan3_on, 59, 38);
                Serial.println("22 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 22;
                break;

              default:
              break;
            }

            break;
            case 23: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 23;
                break;

              case 2: 
                irsend.sendRaw(derece23_fan2_on, 59, 38);
                Serial.println("21 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 23;
                break;

              case 3: 
                irsend.sendRaw(derece23_fan3_on, 59, 38);
                Serial.println("23 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 23;
                break;

              default:
              break;
            }
            break;
            case 24: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece24_fan1_on, 59, 38);
                Serial.println("24 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 24;
                break;

              case 2: 
                irsend.sendRaw(derece24_fan2_on, 59, 38);
                Serial.println("24 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 24;
                break;

              case 3: 
                irsend.sendRaw(derece24_fan3_on, 59, 38);
                Serial.println("24 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 24;
                break;

              default:
              break;
            }
            break;
            case 25: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece25_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 25;
                break;

              case 2: 
                irsend.sendRaw(derece25_fan2_on, 59, 38);
                Serial.println("25 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 25;
                break;

              case 3: 
                irsend.sendRaw(derece25_fan3_on, 59, 38);
                Serial.println("21 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 25;
                break;

              default:
              break;
            }
            break;
            case 26: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece26_fan1_on, 59, 38);
                Serial.println("26 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 26;
                break;

              case 2: 
                irsend.sendRaw(derece26_fan2_on, 59, 38);
                Serial.println("26 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 26;
                break;

              case 3: 
                irsend.sendRaw(derece26_fan3_on, 59, 38);
                Serial.println("26 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 26;
                break;

              default:
              break;
            }
            break;
            case 27: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece27_fan1_on, 59, 38);
                Serial.println("27 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 27;
                break;

              case 2: 
                irsend.sendRaw(derece27_fan2_on, 59, 38);
                Serial.println("27 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 27;
                break;

              case 3: 
                irsend.sendRaw(derece27_fan3_on, 59, 38);
                Serial.println("21 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 27;
                break;

              default:
              break;
            }
            break;
            case 28: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece28_fan1_on, 59, 38);
                Serial.println("28 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 28;
                break;

              case 2: 
                irsend.sendRaw(derece28_fan2_on, 59, 38);
                Serial.println("28 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 28;
                break;

              case 3: 
                irsend.sendRaw(derece28_fan3_on, 59, 38);
                Serial.println("28 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 28;
                break;

              default:
              break;
            }
            break;
            case 29:
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece29_fan1_on, 59, 38);
                Serial.println("29 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 29;
                break;

              case 2:
                irsend.sendRaw(derece29_fan2_on, 59, 38);
                Serial.println("29 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 29;
                break;

              case 3: 
                irsend.sendRaw(derece29_fan3_on, 59, 38);
                Serial.println("29 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 29;
                break;

              default:
              break;
            }
            break;
            case 30: 
            switch (fanhizi) 
            {

              case 1: 
                irsend.sendRaw(derece30_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 30;
                break;

              case 2: 
                irsend.sendRaw(derece30_fan2_on, 59, 38);
                Serial.println("30 derece Fan 2 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 30;
                break;

              case 3: 
                irsend.sendRaw(derece30_fan3_on, 59, 38);
                Serial.println("30 derece Fan 2 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 30;
                break;

              default:
              break;
            }
            break;

          default:
            break;
        }
      }

      if (fanhizi != fandurumu) 
      {
        switch (fanhizi)
        {
          case 1: // fan1
            
            switch (sicaklik) 
            {

              case 18: 
                irsend.sendRaw(derece18_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 18;
                break;

              case 19: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 19;
                break;

              case 20: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 20;
                break;

                case 21: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("21 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 21;
                break;

                case 22: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("22 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 22;
                break;

                case 23: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 23;
                break;

                case 24: 
                irsend.sendRaw(derece24_fan1_on, 59, 38);
                Serial.println("24 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 24;
                break;

                case 25: 
                irsend.sendRaw(derece25_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 25;
                break;

                case 26: 
                irsend.sendRaw(derece26_fan1_on, 59, 38);
                Serial.println("26 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 226;
                break;

                case 27: 
                irsend.sendRaw(derece27_fan1_on, 59, 38);
                Serial.println("27 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 27;
                break;

                case 28: 
                irsend.sendRaw(derece28_fan1_on, 59, 38);
                Serial.println("28 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 28;
                break;

                case 29: 
                irsend.sendRaw(derece29_fan1_on, 59, 38);
                Serial.println("29 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 29;
                break;

                case 30: 
                irsend.sendRaw(derece30_fan1_on, 59, 38);
                Serial.println("30 derece Fan 1 Komutu gönderildi");
                fandurumu = 1;
                sicaklikdurumu = 30;
                break;
               
              default:
                break;
            }

            break;

          case 2: //fan2
            
            switch (sicaklik) 
            {

              case 18: 
                irsend.sendRaw(derece18_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 18;
                break;

              case 19: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 19;
                break;

              case 20: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 20;
                break;

                case 21: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("21 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 21;
                break;

                case 22: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("22 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 22;
                break;

                case 23: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 23;
                break;

                case 24: 
                irsend.sendRaw(derece24_fan1_on, 59, 38);
                Serial.println("24 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 24;
                break;

                case 25: 
                irsend.sendRaw(derece25_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 25;
                break;

                case 26: 
                irsend.sendRaw(derece26_fan1_on, 59, 38);
                Serial.println("26 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 226;
                break;

                case 27: 
                irsend.sendRaw(derece27_fan1_on, 59, 38);
                Serial.println("27 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 27;
                break;

                case 28: 
                irsend.sendRaw(derece28_fan1_on, 59, 38);
                Serial.println("28 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 28;
                break;

                case 29: 
                irsend.sendRaw(derece29_fan1_on, 59, 38);
                Serial.println("29 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 29;
                break;

                case 30: 
                irsend.sendRaw(derece30_fan1_on, 59, 38);
                Serial.println("30 derece Fan 1 Komutu gönderildi");
                fandurumu = 2;
                sicaklikdurumu = 30;
                break;

              default:
                break;
            }

            break;
          case 3: 
            switch (sicaklik) 
            {

              case 18: 
                irsend.sendRaw(derece18_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 18;
                break;

              case 19: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 19;
                break;

              case 20: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("20 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 20;
                break;

                case 21: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("21 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 21;
                break;

                case 22: 
                irsend.sendRaw(derece21_fan1_on, 59, 38);
                Serial.println("22 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 22;
                break;

                case 23: 
                irsend.sendRaw(derece23_fan1_on, 59, 38);
                Serial.println("23 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 23;
                break;

                case 24: 
                irsend.sendRaw(derece24_fan1_on, 59, 38);
                Serial.println("24 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 24;
                break;

                case 25:
                irsend.sendRaw(derece25_fan1_on, 59, 38);
                Serial.println("25 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 25;
                break;

                case 26: 
                irsend.sendRaw(derece26_fan1_on, 59, 38);
                Serial.println("26 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 226;
                break;

                case 27: 
                irsend.sendRaw(derece27_fan1_on, 59, 38);
                Serial.println("27 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 27;
                break;

                case 28: 
                irsend.sendRaw(derece28_fan1_on, 59, 38);
                Serial.println("28 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 28;
                break;

                case 29: 
                irsend.sendRaw(derece29_fan1_on, 59, 38);
                Serial.println("29 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 29;
                break;

                case 30: 
                irsend.sendRaw(derece30_fan1_on, 59, 38);
                Serial.println("30 derece Fan 1 Komutu gönderildi");
                fandurumu = 3;
                sicaklikdurumu = 30;
                break;

              default:
                break;
            }

            break;

          default:
            
            break;
        }
      }
    }//else klima açık durumu
  }
} 

if(isitma!=isitmadurumu)
{
  if(isitma==1)
  {
    irsend.sendRaw(derece27_heat_fan3_on,59,38);
    Serial.println("Isıtma Modunu Açma Komutu gönderildi");
          isitmadurumu = 1;
  }
  else
  {
    rsend.sendRaw(derece27_fan3_on, 211, 38);
          Serial.println("Isıtma Modunu Kapatma Komutu gönderildi");
          isitmadurumu = 0;
          fandurumu = 1;
          sicaklikdurumu = 27;
  }
}

void fangetir() 
{

  if (Firebase.getString(veritabanim, "/fanhizi")) //Alınacak veri tipine göre getInt, getBool, getFloat, getDouble, getString olarak kullanılabilir.
  {

    String str = veritabanim.stringData();
    str = str.substring(2, 3);


    fanhizi = str.toInt();

  } else {
    

    Serial.print("veri çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }

}

void sicaklikgetir() 
{

  if (Firebase.getString(veritabanim, "/sicaklik")) //Alınacak veri tipine göre getInt, getBool, getFloat, getDouble, getString olarak kullanılabilir.
  {
    

    String str = veritabanim.stringData();
    str = str.substring(2, 4);
    ;
    sicaklik = str.toInt();
    

  } else {
    

    Serial.print("veri çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }

}
void isitmagetir()
{
  if(Firebase.getString(veritabanim,"/isitma"))
  {
    String str=veritabanim.stringData();
    butonstatus=str.toInt();
  }
  else
  {
    Serial.print("veri çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }
}
void onoffgetir() 
{

  if (Firebase.getString(veritabanim, "/onoff")) //Alınacak veri tipine göre getInt, getBool, getFloat, getDouble, getString olarak kullanılabilir.
  {
    
    String str = veritabanim.stringData();
    butonstatus = str.toInt();
    


  } else 
  {
    

    Serial.print("veri çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }

}


void varitabaniguncelle(String veriadi, int veridegeri) 
{
  veriadi = "/" + veriadi;
  String yazilacak = String(veridegeri);
  if (Firebase.setString(veritabanim, veriadi, yazilacak))
  {
  
    Serial.println("veri gönderimi başarılı");

  } else 
  {
   

    Serial.print("veri gönderilemedi, ");
    Serial.println(veritabanim.errorReason());
  }


}
