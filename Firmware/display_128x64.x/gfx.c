#include <plib.h>
#include <xc.h>
#include "types.h"

#include "SSD1306.h"

#include "gfx.h"

extern const unsigned char font[];

#define swap(x,y) { x = x + y; y = x - y; x = x - y; }

#define WIDTH   128
#define HEIGHT  64 // This is the 'raw' display w/h - never changes
static int16_t _width, _height; // Display w/h as modified by current rotation
static uint16_t cursor_x, cursor_y;
static uint16_t textcolor, textbgcolor;
static uint8_t textsize, rotation;
static bool wrap; // If set, 'wrap' text at right edge of display

static uint16_t strokeColor;
static bool useStroke;
static uint16_t fillColor;
static bool useFill;


void gfx_init(void)
{
 _width = WIDTH;
 _height = HEIGHT;
 rotation = 0;
 cursor_y = cursor_x = 0;
 textsize = 1;
 textcolor = WHITE;
 textbgcolor = WHITE;
 useStroke = true;
 strokeColor = WHITE;
 wrap = true;
}

// Draw a circle outline
void drawCircle(int16_t x0, int16_t y0, int16_t r,
                uint16_t color)
{
 int16_t f = 1 - r;
 int16_t ddF_x = 1;
 int16_t ddF_y = -2 * r;
 int16_t x = 0;
 int16_t y = r;

 drawPixel(x0, y0 + r, color);
 drawPixel(x0, y0 - r, color);
 drawPixel(x0 + r, y0, color);
 drawPixel(x0 - r, y0, color);

 while(x < y)
  {
   if (f >= 0)
    {
     y--;
     ddF_y += 2;
     f += ddF_y;
    }
   x++;
   ddF_x += 2;
   f += ddF_x;

   drawPixel(x0 + x, y0 + y, color);
   drawPixel(x0 - x, y0 + y, color);
   drawPixel(x0 + x, y0 - y, color);
   drawPixel(x0 - x, y0 - y, color);
   drawPixel(x0 + y, y0 + x, color);
   drawPixel(x0 - y, y0 + x, color);
   drawPixel(x0 + y, y0 - x, color);
   drawPixel(x0 - y, y0 - x, color);
  }
}

void drawCircleHelper(int16_t x0, int16_t y0,
                      int16_t r, uint8_t cornername, uint16_t color)
{
 int16_t f = 1 - r;
 int16_t ddF_x = 1;
 int16_t ddF_y = -2 * r;
 int16_t x = 0;
 int16_t y = r;

 while(x < y)
  {
   if (f >= 0)
    {
     y--;
     ddF_y += 2;
     f += ddF_y;
    }
   x++;
   ddF_x += 2;
   f += ddF_x;
   if (cornername & 0x4)
    {
     drawPixel(x0 + x, y0 + y, color);
     drawPixel(x0 + y, y0 + x, color);
    }
   if (cornername & 0x2)
    {
     drawPixel(x0 + x, y0 - y, color);
     drawPixel(x0 + y, y0 - x, color);
    }
   if (cornername & 0x8)
    {
     drawPixel(x0 - y, y0 + x, color);
     drawPixel(x0 - x, y0 + y, color);
    }
   if (cornername & 0x1)
    {
     drawPixel(x0 - y, y0 - x, color);
     drawPixel(x0 - x, y0 - y, color);
    }
  }
}

// Used to do circles and roundrects

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                      uint8_t cornername, int16_t delta, uint16_t color)
{

 int16_t f = 1 - r;
 int16_t ddF_x = 1;
 int16_t ddF_y = -2 * r;
 int16_t x = 0;
 int16_t y = r;

 while(x < y)
  {
   if (f >= 0)
    {
     y--;
     ddF_y += 2;
     f += ddF_y;
    }
   x++;
   ddF_x += 2;
   f += ddF_x;

   if (cornername & 0x1)
    {
     drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
     drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
    }
   if (cornername & 0x2)
    {
     drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
     drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
    }
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r,
                uint16_t color)
{
 drawFastVLine(x0, y0 - r, 2 * r + 1, color);
 fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Bresenham's algorithm - thx wikpedia

void drawLine(int16_t x0, int16_t y0,
              int16_t x1, int16_t y1,
              uint16_t color)
{
 int16_t steep = abs(y1 - y0) > abs(x1 - x0);
 if (steep)
  {
   swap(x0, y0);
   swap(x1, y1);
  }

 if (x0 > x1)
  {
   swap(x0, x1);
   swap(y0, y1);
  }

 int16_t dx, dy;
 dx = x1 - x0;
 dy = abs(y1 - y0);

 int16_t err = dx / 2;
 int16_t ystep;

 if (y0 < y1)
  {
   ystep = 1;
  }
 else
  {
   ystep = -1;
  }

 for(; x0 <= x1; x0++)
  {
   if (steep)
    {
     drawPixel(y0, x0, color);
    }
   else
    {
     drawPixel(x0, y0, color);
    }
   err -= dy;
   if (err < 0)
    {
     y0 += ystep;
     err += dx;
    }
  }
}

// Draw a rectangle

void drawRect(int16_t x, int16_t y,
              int16_t w, int16_t h,
              uint16_t color)
{
 drawFastHLine(x, y, w, color);
 drawFastHLine(x, y + h - 1, w, color);
 drawFastVLine(x, y, h, color);
 drawFastVLine(x + w - 1, y, h, color);
}

/*
void drawFastVLine(int16_t x, int16_t y,
                   int16_t h, uint16_t color)
{
 // Update in subclasses if desired!
 drawLine(x, y, x, y + h - 1, color);
}

void drawFastHLine(int16_t x, int16_t y,
                   int16_t w, uint16_t color)
{
 // Update in subclasses if desired!
 drawLine(x, y, x + w - 1, y, color);
}
*/

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
              uint16_t color)
{
 // Update in subclasses if desired!
 int16_t i;
 for(i = x; i < x + w; i++)
  {
   drawFastVLine(i, y, h, color);
  }
}

void fillScreen(uint16_t color)
{
 fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle

void drawRoundRect(int16_t x, int16_t y, int16_t w,
                   int16_t h, int16_t r, uint16_t color)
{
 // smarter version
 drawFastHLine(x + r, y, w - 2 * r, color); // Top
 drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
 drawFastVLine(x, y + r, h - 2 * r, color); // Left
 drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
 // draw four corners
 drawCircleHelper(x + r, y + r, r, 1, color);
 drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
 drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
 drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle

void fillRoundRect(int16_t x, int16_t y, int16_t w,
                   int16_t h, int16_t r, uint16_t color)
{
 // smarter version
 fillRect(x + r, y, w - 2 * r, h, color);

 // draw four corners
 fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
 fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle

void drawTriangle(int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color)
{
 drawLine(x0, y0, x1, y1, color);
 drawLine(x1, y1, x2, y2, color);
 drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle

void fillTriangle(int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color)
{

 int16_t a, b, y, last;

 // Sort coordinates by Y order (y2 >= y1 >= y0)
 if (y0 > y1)
  {
   swap(y0, y1);
   swap(x0, x1);
  }
 if (y1 > y2)
  {
   swap(y2, y1);
   swap(x2, x1);
  }
 if (y0 > y1)
  {
   swap(y0, y1);
   swap(x0, x1);
  }

 if (y0 == y2)
  { // Handle awkward all-on-same-line case as its own thing
   a = b = x0;
   if (x1 < a) a = x1;
   else if (x1 > b) b = x1;
   if (x2 < a) a = x2;
   else if (x2 > b) b = x2;
   drawFastHLine(a, y0, b - a + 1, color);
   return;
  }

 int16_t
 dx01 = x1 - x0,
 dy01 = y1 - y0,
 dx02 = x2 - x0,
 dy02 = y2 - y0,
 dx12 = x2 - x1,
 dy12 = y2 - y1,
 sa = 0,
 sb = 0;

 // For upper part of triangle, find scanline crossings for segments
 // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
 // is included here (and second loop will be skipped, avoiding a /0
 // error there), otherwise scanline y1 is skipped here and handled
 // in the second loop...which also avoids a /0 error here if y0=y1
 // (flat-topped triangle).
 if (y1 == y2) last = y1; // Include y1 scanline
 else last = y1 - 1; // Skip it

 for(y = y0; y <= last; y++)
  {
   a = x0 + sa / dy01;
   b = x0 + sb / dy02;
   sa += dx01;
   sb += dx02;
   /* longhand:
   a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
   b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
   if (a > b) swap(a, b);
   drawFastHLine(a, y, b - a + 1, color);
  }

 // For lower part of triangle, find scanline crossings for segments
 // 0-2 and 1-2.  This loop is skipped if y1=y2.
 sa = dx12 * (y - y1);
 sb = dx02 * (y - y0);
 for(; y <= y2; y++)
  {
   a = x1 + sa / dy12;
   b = x0 + sb / dy02;
   sa += dx12;
   sb += dx02;
   /* longhand:
   a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
   b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
   if (a > b) swap(a, b);
   drawFastHLine(a, y, b - a + 1, color);
  }
}

void drawBitmap(int16_t x, int16_t y,
                const uint8_t *bitmap, int16_t w, int16_t h,
                uint16_t color)
{

 int16_t i, j, byteWidth = (w + 7) / 8;

 for(j = 0; j < h; j++)
  {
   for(i = 0; i < w; i++)
    {
     if (bitmap[j * byteWidth + i / 8] & (128 >> (i & 7)))
      {
       drawPixel(x + i, y + j, color);
      }
    }
  }
}


// Draw a character
void drawChar(int16_t x, int16_t y, unsigned char c,
              uint16_t color, uint16_t bg, uint8_t size)
{

 if ((x >= _width) || // Clip right
     (y >= _height) || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0)) // Clip top
  return;

 int8_t i, j;
 for(i = 0; i < 6; i++)
  {
   uint8_t line;
   if (i == 5) line = 0x0;
   else line = font[(c * 5) + i];
   for(j = 0; j < 8; j++)
    {
     if (line & 0x1)
      {
       if (size == 1) drawPixel(x + i, y + j, color); // default size
       else fillRect(x + (i * size), y + (j * size), size, size, color); // big size
      }
     else
     if (bg != color)
      {
       if (size == 1) drawPixel(x + i, y + j, bg); // default size
       else fillRect(x + i*size, y + j*size, size, size, bg); // big size
      }
     line >>= 1;
    }
  }
}

void setCursor(int16_t x, int16_t y)
{
 cursor_x = x;
 cursor_y = y;
}

void setTextSize(uint8_t s)
{
 textsize = (s > 0) ? s : 1;
}


void setTextColor(uint16_t c)
{
 // For 'transparent' background, we'll set the bg
 // to the same as fg instead of using a flag
 textcolor = textbgcolor = c;
}

void setTextBgColor(uint16_t c, uint16_t b)
{
 textcolor = c;
 textbgcolor = b;
}


void setTextWrap(bool w)
{
 wrap = w;
}

uint8_t getRotation(void)
{
 return rotation;
}

void setRotation(uint8_t x)
{
 rotation = (x & 3);
 switch(rotation)
  {
   case 0:
   case 2:
    _width = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)

int16_t width(void)
{
 return _width;
}

int16_t height(void)
{
 return _height;
}


//void invertDisplay(bool i)
//{
// // Do nothing, must be subclassed if supported
//}

uint16_t newColor(uint8_t r, uint8_t g, uint8_t b)
{
 return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


//void background(uint8_t red, uint8_t green, uint8_t blue)
//{
// background(newColor(red, green, blue));
//}

void background(uint16_t c)
{
 fillScreen(c);
}

//void stroke(uint8_t red, uint8_t green, uint8_t blue)
//{
// stroke(newColor(red, green, blue));
//}

void stroke(uint16_t c)
{
 useStroke = true;
 strokeColor = c;
 setTextColor(c);
}

void noStroke()
{
 useStroke = false;
}

void noFill()
{
 useFill = false;
}

//void fill(uint8_t red, uint8_t green, uint8_t blue)
//{
// fill(newColor(red, green, blue));
//}

void fill(uint16_t c)
{
 useFill = true;
 fillColor = c;
}

size_t write(uint8_t c)
{
 if (c == '\n')
  {
   cursor_y += textsize * 8;
   cursor_x = 0;
  }
 else if (c == '\r')
  {
   // skip em
  }
 else
  {
   drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
   cursor_x += textsize * 6;
   if (wrap && (cursor_x > (_width - textsize * 6)))
    {
     cursor_y += textsize * 8;
     cursor_x = 0;
    }
  }
 return 1;
}

uint8_t text(const char * text, int16_t x, int16_t y)
{
 char c;
 uint8_t n = 0;
 //if (!useStroke) return;
 setTextWrap(false);
 //setTextColor(strokeColor);
 setCursor(x, y);
 while (c = *text++) {write(c); n++;}
 return n;
}

uint8_t textWrap(const char * text, int16_t x, int16_t y)
{
 char c;
 uint8_t n = 0;
 //if (!useStroke) return;
 setTextWrap(true);
 //setTextColor(strokeColor);
 setCursor(x, y);
 while (c = *text++) {write(c); n++;}
 return n;
}

void textSize(uint8_t size)
{
 setTextSize(size);
}

void point(int16_t x, int16_t y)
{
 if (!useStroke) return;
 drawPixel(x, y, strokeColor);
}

void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
 if (!useStroke) return;

 if (x1 == x2) drawFastVLine(x1, y1, y2 - y1, strokeColor);
 else
 if (y1 == y2) drawFastHLine(x1, y1, x2 - x1, strokeColor);
 else drawLine(x1, y1, x2, y2, strokeColor);
}

void rect(int16_t x, int16_t y, int16_t width, int16_t height)
{
 if (useFill) fillRect(x, y, width, height, fillColor);
 if (useStroke) drawRect(x, y, width, height, strokeColor);
}

void rrect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius)
{
 if (radius == 0) rect(x, y, width, height);
 if (useFill) fillRoundRect(x, y, width, height, radius, fillColor);
 if (useStroke) drawRoundRect(x, y, width, height, radius, strokeColor);
}

void circle(int16_t x, int16_t y, int16_t r)
{
 if (r == 0) return;

 if (useFill) fillCircle(x, y, r, fillColor);
 if (useStroke) drawCircle(x, y, r, strokeColor);
}

void triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
{
 if (useFill) fillTriangle(x1, y1, x2, y2, x3, y3, fillColor);
 if (useStroke) drawTriangle(x1, y1, x2, y2, x3, y3, strokeColor);
}
