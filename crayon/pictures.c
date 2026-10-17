/* pictures.c - a PNG instead of the arrow, plus two turtles at once
 *
 *     gcc pictures.c -o pictures && ./pictures
 */
#include "crayon.h"

int main(void) {
    window(700, 500);
    title("Picture turtles");

    /* turtle 1: a turtle picture that draws a green star */
    picture("turtle.png");
    pendown();                      /* pictures don't draw lines unless you say pendown() */
    color("green");
    thickness(4);
    go_to(-280, 60);
    repeat(5) {
        forward(150);
        right(144);
    }

    /* turtle 2: the normal arrow, drawing a flower with any numbers you like */
    new_turtle();                   /* a second turtle - from now on, commands move this one */
    arrow();
    color("purple");
    go_to(170, 0);
    speed(9);
    repeat(12) {
        circle(45);
        right(30);
    }

    color("darkgray");
    textsize(22);
    text_center("Two turtles - one is a {green}PNG{darkgray}, one is an {purple}arrow", 0, -210);
}
