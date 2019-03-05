/*
    Pixo-Pixel Display Library
    The MIT License (MIT)
    Copyright (c) 2019 c0de (c0de@c0defox.es)
 
    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:
 
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
 
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    -----------------------------------------------------------------------------

    This is a wrapper for the DotStar libraries, and makes a lot of assumptions
    that I think are fine. Just in case they aren't, I'm open to feedback
    and/or pull requests to help improve this library.
*/

#ifndef pixo_displaylib_h
#define pixo_displaylib_h

// Load GFX and DotStar libraries
#include "Arduino.h"
#include <Adafruit_DotStarMatrix.h>

// Pre-assuming these values as this is the Pixo-Pixel configuration
#define DATAPIN    19
#define CLOCKPIN   18
#define TOTALPXLS  256 // The total number of LEDs
#define ROWS       16
#define MCONFIG    0x00000000
#define BGR        (2 | (1 << 2) | (0 << 4))

class pixo_displaylib : public Adafruit_DotStarMatrix {
  public:
    pixo_displaylib(int max_brightness);
    void begin(); // Initializes the pixol structure
    int activePixelCount(); // Returns the number of active pixels
    void resetActiveState(); // Sets all pixels to inactive
    void showPxl(int pxl[TOTALPXLS][3], int dlytime, bool color=false); // Displays the image on screen
    void notifyPopupPxl(int pxl[TOTALPXLS][3], int dlytime); // Pops an image up and back down
    void notifyScrollPxl(int pxl[TOTALPXLS][3], int dlytime, int loops=0); // Makes an image scroll upwards
    void notifyBlinkPxl(int pxl[TOTALPXLS][3], int dlytime, int loops=0, bool color=false); // Makes an image blink
    void scanChangePxl(int src[TOTALPXLS][3], int dst[TOTALPXLS][3], int dlytime, int anitime); // Transition from one image to another, one pixel at a time
    void randChangePxl(int src[TOTALPXLS][3], int dst[TOTALPXLS][3], int dlytime, int anitime); // Transition from one image to another, one pixel at a time, randomly
  private:
    int BRIGHTNESS; // Sets the brightness
    struct PXL {
      byte id: 1;
      byte r: 1;
      byte g: 1;
      byte b: 1;
      byte active: 1;
    }; // Structure that contains settings of each pixel
    struct PXL PIXOLS[TOTALPXLS]; // Structure that contains all pixels
    Adafruit_DotStarMatrix matrix;
};

#endif