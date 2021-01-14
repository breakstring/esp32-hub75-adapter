#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include <SPI.h>
#include <SD.h>
#define SD_CS    5   //SS
MatrixPanel_I2S_DMA matrix;

// Change these to whatever suits
#define R1_PIN  25
#define G1_PIN  26
#define B1_PIN  27

#define R2_PIN  14
#define G2_PIN  17 
#define B2_PIN  13

#define A_PIN   32
#define B_PIN   33 
#define C_PIN   21
#define D_PIN   15
#define E_PIN   2	// required for 1/32 scan panels, set to 0,2,or 12 base your jumper setup
          
#define LAT_PIN 4
#define OE_PIN  22

#define CLK_PIN 16

void setup()
{
  Serial.begin(115200);
    // MUST DO THIS FIRST!
  //matrix.begin();  // Use default pins supplied within ESP32-HUB75-MatrixPanel-I2S-DMA.h
  matrix.begin(R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN );  // or custom pins
  
  // Draw a single white pixel
  //matrix.drawPixel(0,0, matrix.color565(255,0,0)); // can do this after .begin() only
  matrix.setTextSize(1);

  if(!SD.begin(SD_CS)){
      Serial.println("Card Mount Failed");
      return;
  } else {
    Serial.println("SD Card mounted with success");
    #if ESP32
      uint8_t cardType = SD.cardType();

      if(cardType == CARD_NONE){
          Serial.println("No SD card attached");
          return;
      }

      Serial.print("SD Card Type: ");
      if(cardType == CARD_MMC){
          Serial.println("MMC");
      } else if(cardType == CARD_SD){
          Serial.println("SDSC");
      } else if(cardType == CARD_SDHC){
          Serial.println("SDHC");
      } else {
          Serial.println("UNKNOWN");
      }

      uint64_t cardSize = SD.cardSize();
    int cardSizeInMB = cardSize/(1024 * 1024);
     
    Serial.printf("Card size: %u MB \n", cardSizeInMB);
 
    uint64_t bytesAvailable = SD.totalBytes(); 
    int spaceAvailableInMB = bytesAvailable/(1024 * 1024);
 
    Serial.printf("Space available: %u MB \n", spaceAvailableInMB);
 
    uint64_t spaceUsed = SD.usedBytes(); 
    Serial.printf("Space used: %u bytes", spaceUsed);

    #endif   
  }
}

void recoardNewData(fs::FS &fs, const char * path, const char * message)
{
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.println(message)) {
    Serial.println("Data appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
void loop()
{
  //matrix.fillScreen(matrix.color565(255,255,255));
  //delay(2000);
  //matrix.fillScreen(matrix.color565(random(256),random(256),random(256)));
  matrix.clearScreen();

  matrix.setTextColor(matrix.color565(random(255),random(255),random(255))); 
  matrix.setCursor(0,20);
  matrix.println("Hello, \nWorld!");
  recoardNewData(SD,"/data.txt","Hello world 64*64");
  delay(5000);
}
