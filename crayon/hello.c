/* hello.c - ask questions and show the answers
 *
 *     gcc hello.c -o hello && ./hello
 */
#include "crayon.h"

int main(void) {
    window(700, 450);
    background("beige");

    const char *name = userinput("What's your name?");

    textsize(50);
    bold(1);
    text_center("{rainbow}Hi %s!", 0, 120, name);
    bold(0);

    const char *place = userinput("Where should I write your name? (type: top, middle or bottom)");

    textsize(30);
    if (same(place, "top"))    text_center("{red}%s", 0, 50, name);
    if (same(place, "middle")) text_center("{blue}%s", 0, 0, name);
    if (same(place, "bottom")) text_center("{green}%s", 0, -60, name);

    textsize(18);
    text_center("Do you like turtles?", 0, -120);
    int answer = pick("Yes", "No");             /* waits for a click: 1 = Yes, 2 = No */

    if (answer == 1) {
        picture("turtle.png");
        go_to(0, -60);
        turtle_size(1.5);
        text_center("{green}Me too!", 0, -150);
    } else {
        text_center("{red}Oh no!", 0, -150);
    }
}
