#include <Wire.h>
#include <LoRa.h>
#include <SD.h>
#include <SPI.h>

#include <SSD1306.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "RTCDS1307.h"

//Pinos do LoRa
#define SCK_LoRa  5
#define MISO_LoRa 19
#define MOSI_LoRa 27
#define CS_LoRa   18
#define SS_LoRa      18
#define RST_LoRa     12
#define DI0_LoRa     26
#define BAND_LoRa    433E6


// Pinos para comunicação SPI com o cartão SD
#define MOSI_SD 15
#define MISO_SD 2
#define CLK_SD  14
#define CS_SD   13

//Pino RTC
#define SCL_RTC 21
#define SDA_RTC 11

File file;

// Variáveis para manipulação dos octetos
byte octetos[9]="12345678"; //  Vetor de Bytes que acondiciona os dados recebidos da SSU.
byte oux_byte=0;  // Resultado do OUX dos primeiros 7 Bytes recebidos da SSU até o momento. Ao final, deve-se realizar a inversão desta variável para a comparação com o oitavo Byte de verificação.
unsigned int indice_octeto=0; // Índice que aponta para próxima região livre do vetor de octetos.
String octetos_=" ";

// Flags
bool verificacao_ok=false;  // Flag que indica verificação de dados vindos da SSU está ok.
bool ocioso=true;   // Flag que sinaliza a não recepção de dados da SSU.

// Inicialização da porta 
HardwareSerial SSU(1);
unsigned int tempo=0;  // tempo contato entre dados recebidos da SSU a fim de verificar o timeout ou fim de recepção.

// Configurações do Relógio de Tempo Real (RTC)
//RTCDS1307 rtc(0x68);

// RTC - Variáveis gglobais
uint8_t year, month, weekday, day, hour, minute, second;
bool period = 0;
String timestamp = "tempo";

// Declaração de funções
void dateNow();
void loraSend();
void SaveSD();

void setup() {
  Serial.println("Iniciando a comunicação serial...");
  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer
  Serial.println("Comunicação serial iniciada");

  
  Serial.println("Iniciando a comunicação com a SSU...");
  SSU.begin(110,SERIAL_8N1, 23, 22);
  while (!SSU);
  Serial.println("SSU Inicializada!");

 
  Serial.println("Iniciando a comunicação LoRa...");
  SPI.begin(SCK_LoRa, MISO_LoRa, MOSI_LoRa, CS_LoRa);
  LoRa.setPins(SS_LoRa, RST_LoRa, DI0_LoRa);
  if (!LoRa.begin(BAND_LoRa)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Comunicação LoRa iniciada!");
  SPI.end();

//  Serial.println("RTC Iniciado");
//  rtc.begin();
//  rtc.setDate(1970, 1, 1);
//  rtc.setTime(0, 0, 0);
//  Serial.println("RTC setado com sucesso!");

//  Serial.println("Iniciando o SD card...");
//  SPI.begin(CLK_SD, MISO_SD, MOSI_SD, CS_SD);
//  while(!SD.begin(13)) {
//    Serial.println("Card SD!");
//    delay(500);
//  }
//  Serial.println("SD card iniciado com sucesso!");
//  
//  if(SD.exists("/octetos.csv"))
//    SD.remove("/octetos.csv");
//  SPI.end();
}

void loop() {
  //Timeout: tempo entre dados da SSU excedeu o tempo limite de 50ms. Fim de transmissão SSU.
  //TIMEOUT Observado: 290.0 ms
  //Tempo entre caracteres: 12.0 ms
  // Caso dê timeout ou todos os 8 octetos sejam recebidos
/*
  if((unsigned int)millis()-tempo>50.0 && !ocioso)
  { 
    Serial.println("Time out!");
    
    // Verificação de Bytes: (true) Bytes recebidos com sucesso, (false) descartar Bytes.
    verificacao_ok=((octetos[7]==((byte)~oux_byte))?true:false);
    //verificacao_ok=true;
    
    if(verificacao_ok)
    {
      // Formação da linha para o CSV
      octetos_="";
      for(int i=0;i<8;i++)
        octetos_+=octetos[i];
        
      //dateNow(); // Coleta a data e hora
      
      // Envia a linha pela serial para o computador
      Serial.print(timestamp);
      Serial.print(";");
      Serial.println(octetos_); 

      //SaveSD();

    }
  
    //Zera contagem de octetos para nova recepção.  
    indice_octeto=0;
    
    //Limpa o Byte de verificação.
    oux_byte=0;
  
    // Limpa o buffer de recepção.
    for(int i=0;i<8;i++)
      octetos[i]=0;
  
    //Baixa a flag de recebimento OK.
    verificacao_ok=false;
    
    // Flag que indica fim de atividade de recebimento de dados.
    ocioso=true;
  }
*/
      
  // Caso chegue algum Byte vindo da SSU
  if(SSU.available())
  {
    Serial.println("Recebido da SSU!"); 
    
    //Flag que indica recebimento de dados da SSU.
    ocioso=false;

    // Lê Byte vindo da SSU
    byte c = (byte)SSU.read();

    /*
    //Em caso de a contagem de ser um octeto da transmissão atual
    if(indice_octeto<8)
    {
      //Concatena os bytes recebidos da SSU no vetor octetos.
      octetos[indice_octeto]=c;
    
      if(indice_octeto<7)
      {
        // Faz-se o ou exclusivo dele com os dados recebidos, com exceção do próprio Byte de verificação.
        oux_byte^=c;
      }
      
      // Incrementa posição do índice para a próxima posição disponível.
      indice_octeto++;

      */
      //alteração gabriel: 0302
      Serial.print("Tempo desde a última recepção: ");      
      Serial.print(millis()-tempo);
      Serial.println(" ms");
      Serial.print("Dado recebido: ");
      Serial.println(String(c));
      // Reinicia a contagem de tempo para avaliação do Timeout.
      tempo=millis();
    }
    
  }
  //indice_octeto=0;
//}
