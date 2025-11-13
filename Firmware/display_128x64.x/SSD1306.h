#ifndef SSD1306__H
#define	SSD1306__H

#define BLACK 0
#define WHITE 1

//#define SSD1306_I2C_ADDRESS   0x78 //0x3C	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

/*=========================================================================
SSD1306 Displays
-----------------------------------------------------------------------
The driver is used in multiple displays (128x64, 128x32, etc.).
Select the appropriate display below to create an appropriately
sized framebuffer, etc.

SSD1306_128_64  128x64 pixel display

SSD1306_128_32  128x32 pixel display

-----------------------------------------------------------------------*/
#define SSD1306_128_64
//   #define SSD1306_128_32
#define GTK_LOGO
//#define LOGO
//#define ADAFRUIT_LOGO
//#define SSD1306_DIRECT_DRAW
/*=========================================================================*/

extern bool ssd1306_init(void); //SSD1306_SWITCHCAPVCC

#ifdef SSD1306_DIRECT_DRAW
extern bool ssd1306_Goto(uint8_t x, uint8_t y);
extern bool ssd1306_Putch(char c);
extern bool ssd1306_Puts(char* str);
#endif

extern void clearDisplay(void);
extern bool invertDisplay(uint8_t i);
extern bool display();

extern bool startscrollright(uint8_t start, uint8_t stop);
extern bool startscrollleft(uint8_t start, uint8_t stop);

extern bool startscrolldiagright(uint8_t start, uint8_t stop);
extern bool startscrolldiagleft(uint8_t start, uint8_t stop);
extern bool stopscroll(void);
extern bool dim(bool dimm);
bool isDim(void);


extern void drawPixel(int16_t x, int16_t y, uint16_t color);

extern void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
extern void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

#endif