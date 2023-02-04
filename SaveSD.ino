//???
// SSD1306  display(0x3c, 22, 21);
void SaveSD(){
  String octeto="";
  
  SPI.begin(CLK_SD, MISO_SD, MOSI_SD, CS_SD);
  
  file = SD.open("/octetos.csv", FILE_APPEND);

  file.println(); // Próxima linha
  file.print(timestamp); // Insere o timestamp
  file.print(";"); // Caractere separador
  
  // Formação da linha para o CSV
  octetos_="";
  for(int i=0;i<8;i++)
    octetos_+=octetos[i];

  file.print(octetos_); // Insere o octeto
  
  // Fecha arquivo
  file.close();
  
  // Finaliza comunicação SPI
  SPI.end();
}
