/* 
 * File:   gfx.h
 * Author: Dmitry Orlov
 *
 * Created on April 3, 2015, 10:10 PM
 */

#ifndef GFX_H
#define	GFX_H

extern void gfx_init(void);

// Draw a circle outline
extern void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
extern void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
extern void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
extern void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern void fillScreen(uint16_t color);

extern void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
extern void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
extern void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
extern void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
extern void setCursor(int16_t x, int16_t y);
extern void setTextColor(uint16_t c);
extern void setTextBgColor(uint16_t c, uint16_t bg);
extern void setTextSize(uint8_t s);
extern void setTextWrap(bool w);
extern void setRotation(uint8_t r);

extern size_t write(uint8_t);

extern int16_t height(void);
extern int16_t width(void);
extern uint8_t getRotation(void);

/// transforms a color in 16-bit form given the RGB components.
/// The default implementation makes a 5-bit red, a 6-bit
/// green and a 5-bit blue (MSB to LSB). Devices that use
/// different scheme should override this.
extern uint16_t newColor(uint8_t red, uint8_t green, uint8_t blue);
extern  void background(uint16_t c);
extern void fill(uint16_t c);
extern void noFill();
extern void stroke(uint16_t c);

extern void noStroke();

extern uint8_t text(const char * text, int16_t x, int16_t y);
extern uint8_t textWrap(const char * text, int16_t x, int16_t y);
extern void textSize(uint8_t size);

extern void circle(int16_t x, int16_t y, int16_t r);

extern void point(int16_t x, int16_t y);

extern void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

extern void quad(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t x4, int16_t y4);

extern void rect(int16_t x, int16_t y, int16_t width, int16_t height);

extern void rrect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius);

extern void triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);


#endif	/* GFX_H */

