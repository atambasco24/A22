#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#include "ImageData.h"

void setup() {
  System_Init();
  if(USE_IIC) {
    Serial.print("Only USE_SPI_4W, Please revise DEV_config.h !!!");
    return 0;
  }
  
  Serial.print(F("OLED_Init()...\r\n"));
  OLED_1in5_rgb_Init();
  Driver_Delay_ms(500); 
  OLED_1in5_rgb_Clear();  
  
  //1.Create a new image size
  UBYTE *BlackImage;
  Serial.print("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, OLED_1in5_RGB_WIDTH, OLED_1in5_RGB_HEIGHT, 270, BLACK);  
  Paint_SetScale(65);

  void Paint_DrawNum(UWORD 0, UWORD 0, double 18, sFONT* Font24, UWORD 1.8, UWORD White, UWORD Black);
}

void loop() {

}
