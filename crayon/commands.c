/* commands.c - type something and the window does something new
 *
 *     gcc commands.c -o commands && ./commands
 *
 * Try typing:  hello    blue    party    button    turtle    square    circle    clear
 */
#include "crayon.h"

void wave(void) {
    textsize(24);
    text("{rainbow}*wave*", rnd(-300, 250), rnd(-150, 200));
}

void got_typing(void) {
    textsize(24);

    if (typed("hello")) text("{blue}Hello to you too!", -300, 200);

    if (typed("blue"))  background("lightblue");

    if (typed("party")) {
        background("pink");
        textsize(60);
        text_center("{rainbow}PARTY!", 0, 50);
    }

    if (typed("button")) button("I'm new!", rnd(-250, 250), rnd(-120, 180), wave);

    if (typed("turtle")) {
        new_turtle();
        picture("turtle.png");
        go_to(rnd(-300, 300), rnd(-150, 200));
        draggable();
    }

    if (typed("square")) {
        use(0);
        arrow();
        color("red");
        repeat(4) {
            forward(100);
            right(90);
        }
    }

    if (typed("circle")) {
        use(0);
        arrow();
        color("purple");
        circle(60);
    }

    if (typed("clear")) clear();

    /* "say something" writes the rest of what you typed */
    if (command("say")) text(rest(1), rnd(-300, 200), rnd(-150, 200));
}

int main(void) {
    window(800, 550);
    title("Type something!");

    textsize(20);
    text("Type: hello, blue, party, button, turtle, square, circle, clear, say hi", -380, 250);

    inputbox("Type here and press Enter...", 0, -240, 760, got_typing);
}
