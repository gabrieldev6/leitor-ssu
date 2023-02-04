void loraSend(){
  // Envio de linha do CSV via LORA
  SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,CS_LoRa); //Abrindo serial LoRa
  
  // send packet
  LoRa.beginPacket();
  LoRa.print(timestamp);
  LoRa.print(";");
  LoRa.print(octetos_);
  LoRa.endPacket();
    
  SPI.end();
}
