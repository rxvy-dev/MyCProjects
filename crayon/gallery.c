/* gallery.c - put pictures anywhere
 *
 *     gcc gallery.c -o gallery && ./gallery
 */
#include "crayon.h"

int main(void) {
    window(700, 450);
    background("lightblue");

    image("star.png", -250, 120);          /* picture, x, y */
    image("star.png", -150, 120, 0.5);     /* half size */
    image("turtle.png", 0, 0, 3);          /* 3 times bigger */
    image("coin.png", 200, 120);
    image("coin.png", 240, 120);
    image("coin.png", 280, 120);
    image("player.png", 230, -60, 2);

    textsize(36);
    bold(1);
    text_center("{rainbow}My Gallery", 0, 180);
    bold(0);
    textsize(18);
    text_center("{darkgray}image(\"turtle.png\", 0, 0, 3);", 0, -150);
}
