#include "pixo_displaylib.h"
#include <Adafruit_DotStarMatrix.h>

// Constructor for the library. Use this for init-logic,
// and setting of private variables
pixo_displaylib::pixo_displaylib(int max_brightness) : Adafruit_DotStarMatrix::Adafruit_DotStarMatrix(int w, int h, uint8_t d, uint8_t c, uint8_t matrixType, uint8_t ledType)
{
    BRIGHTNESS = max_brightness;

    Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(ROWS, ROWS, DATAPIN, CLOCKPIN, MCONFIG, BGR)
}

void pixo_displaylib::begin() {
    matrix.begin();
    matrix.setBrightness(BRIGHTNESS);

    pixo_displaylib::PXL pxl;
    pixo_displaylib::PIXOLS pixols;

    // Initialize our pixel structure
    for (int cnt = 0; cnt < TOTALPXLS; cnt++) {
        pixols[cnt].active = false;
        pixols[cnt].id = cnt;
    }
}

// Returns the number of pixols who's active state is true
int pixo_displaylib::activePixelCount() {
    int totalActive = 0;
    for (int cnt = 0; cnt < TOTALPXLS; cnt++) {
        if (pixols[cnt].active == true) totalActive++;
    }
    return totalActive;
}

// Sets the active state of all pixols to false
void pixo_displaylib::resetActiveState() {
    for (int i = 0; i < TOTALPXLS; i++) 
        pixols[i].active = false;
}

/*
 * showPxl, displays a Pixol to the display with no animation
 * args:
 * - pxl[TOTALPXLS][3] - Standard Pixol image
 * - dlytime - The amount of time (in ms) to keep the image on the display
 * - color (optional) - Whether or not to set the active pixel's color to its stored value
 */
void pixo_displaylib::showPxl(int pxl[TOTALPXLS][3], int dlytime, bool color=false) {
    resetActiveState();

    for (int i=0; i < TOTALPXLS; i++){
        if (pxl[i][0] != 0 && pxl[i][1] != 0 && pxl[i][2] != 0) {
            pixols[i].active = true;
        }

        if (color && pixols[i].active) {
            matrix.setPassTrhuColor(matrix.color(pixols[i].r,
                                                 pixols[i].g,
                                                 pixols[i].b))
        }

        if (!color) {
            matrix.setPixelColor(i, pxl[i][0], pxl[i][1], pxl[i][2]);
        }else {
            if (pixols[i].active) {
                

                matrix.setPixelColor(i, pxl[i][], 
                                        pxl[i][pixols[i].g],
                                        pxl[i][pixols[i].b]);
            }
            
        }
    }

    matrix.show();
    delay(dlytime);
  
    matrix.fillScreen(0);
    matrix.show();
}

// Notification functions

/*
 * notifyPopupPxl, draws a Pixol that scrolls into view, and back down
 * args:
 * - pxl[TOTALPXLS][3] - Standard Pixol image
 * - dlytime - The amount of time (in ms) to keep the image on the display
 */
void pixo_displaylib::notifyPopupPxl(int pxl[TOTALPXLS][3], int dlytime) {
    for (int j = 15; j >= 0; j--) {
        for (int i = 0; i < TOTALPXLS; i++) {
            matrix.setPixelColor(i + (j * 16),
                pxl[i][0], pxl[i][1], pxl[i][2]);
        }
        matrix.show();
        delay(50);
    }

    delay(dlytime);

    for (int j = 0; j <= 16; j++) {
        for (int i = 0; i < TOTALPXLS; i++) {
            matrix.setPixelColor(i + (j * 16),
                pxl[i][0], pxl[i][1], pxl[i][2]);
        }
        matrix.show();
        delay(50);
    }

    delay(100);
}

/*
 * notifyScrollPxl, draws a Pixol that scrolls upward into view, and upwards out; Loop this to scroll X times
 * args:
 * - pxl[TOTALPXLS][3] - Standard Pixol image
 * - dlytime - The amount of time (in ms) to keep the image on the display
 * - loops (optional) - The total amount of times this should loop through
 */
void pixo_displaylib::notifyScrollPxl(int pxl[TOTALPXLS][3], int dlytime, int loops=0) {
    for (int x=0; x<=loops; x++) {
        for (int j = 15; j >= 0; j--) {
            for (int i = 0; i < TOTALPXLS; i++) {
                matrix.setPixelColor(i + (j * 16), 
                    pxl[i][0], pxl[i][1], pxl[i][2]);
            }
            matrix.show();
            delay(15);
        }

        delay(dlytime);

        for (int j = 0; j >= -16; j--) {
            for (int i = 0; i < TOTALPXLS; i++) {
                matrix.setPixelColor(i + (j * 16),
                    pxl[i][0], pxl[i][1], pxl[i][2]);
            }
            matrix.show();
            delay(15);
        }

        delay(100);
    }
}

/*
 * notifyBlinkPxl, flash a Pixol on screen repeatedly
 * args:
 * - pxl[TOTALPXLS][3] - Standard Pixol image, displayed all at once
 * - dlytime - The amount of time (in ms) to keep the image on the display
 * - loops (optional) - The total amount of times this should blink
 * - color (optional) - Whether or not to set the active pixel's color to its stored value
 */
void pixo_displaylib::notifyBlinkPxl(int pxl[TOTALPXLS][3], int dlytime, int loops=0, bool color=false) {
    for (int i = 0; i <= loops; i++) {
        showPxl(pxl, dlytime, color);
        showPxl(darkness, dlytime, color);
    }
}

// "Animation" or picture transition functions

/*
 * scanChangePxl, Change from one Pixol to another with a scanline appearence
 * args:
 * - src[TOTALPXLS][3] - Standard Pixo-Style icon that we change from, displayed all at once
 * - dst[TOTALPXLS][3] - Standard Pixo-Style icon that we change to, displayed one pixel at a time
 * - dlytime - The amount of time (in ms) to keep the image on the display
 * - anitime - The amount of time (in ms) between drawing each pixel during the transition; the lower this number, the faster the transition
 */
void pixo_displaylib::scanChangePxl(int src[TOTALPXLS][3], int dst[TOTALPXLS][3], int dlytime, int anitime) {
    for (int i=0; i<TOTALPXLS; i++){
        matrix.setPixelColor(i, src[i][0], src[i][1], src[i][2]);
    }
    matrix.show();

    delay(dlytime);

    for (int i=0; i<TOTALPXLS; i++){
        matrix.setPixelColor(i, dst[i][0], dst[i][1], dst[i][2]);
        matrix.show();
        delay(anitime);
    }

    delay(dlytime);
}

/*
 * randChangePxl, Change from one Pixol to another with a random pixel selection
 * args:
 * - src[TOTALPXLS][3] - Standard Pixo-Style icon that we change from, displayed all at once
 * - dst[TOTALPXLS][3] - Standard Pixo-Style icon that we change to, displayed one pixel at a time
 * - dlytime - The amount of time (in ms) to keep the image on the display
 * - anitime - The amount of time (in ms) between drawing each pixel during the transition; the lower this number, the faster the transition
 */
void pixo_displaylib::randChangePxl(int src[TOTALPXLS][3], int dst[TOTALPXLS][3], int dlytime, int anitime) {
    resetActiveState();

    // Image that we are animating from
    for (int i = 0; i < TOTALPXLS; i++){
        pixols[i].active = false;
        matrix.setPixelColor(i, src[i][0], src[i][1], src[i][2]);
    }
    matrix.show();

    delay(dlytime);
    
    // Image that we are animating to
    while (activePixelCount() < TOTALPXLS) {
        randomSeed(A1);
        int selected = random(0, TOTALPXLS);

        // Select only pixels that have not already been set
        if (pixols[selected].active) {
            while (true) {
                selected = random(0, TOTALPXLS);
                if (!pixols[selected].active) {
                    break;
                }
            }
        }

        pixols[selected].active = true;
        matrix.setPixelColor(selected, dst[selected][0],
                                       dst[selected][1],
                                       dst[selected][2]);
        matrix.show();
        delay(anitime);

        if (activePixelCount() >= TOTALPXLS) break;
    }

    delay(dlytime);
}