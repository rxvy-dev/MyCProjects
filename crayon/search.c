/* search.c - "Turtlr", a pretend search engine
 *
 *     gcc search.c -o search && ./search
 *
 * Every page is just a function. A button or link says which function to run next.
 */
#include "crayon.h"

char search[200];        /* what you searched for */

void home(void);
void results(void);


/* --- runs when you press Enter or click "Turtlr Search" --- */
void do_search(void) {
    copy(search, input_text());
    results();
}

/* --- the I'm Feeling Lucky button --- */
void lucky(void) {
    copy(search, input_text());
    new_screen();
    textsize(40);
    bold(1);
    text_center("{green}Lucky you!", 0, 60);
    bold(0);
    textsize(20);
    text_center("You searched for: {blue}%s", 0, 0, search);
    link("< Back home", -40, -80, home);
}

/* --- a pretend website (opens when you click a result) --- */
void website(void) {
    new_screen();

    color("navy");
    box(0, 290, 1000, 60);                    /* dark bar at the top */
    textsize(20);
    bold(1);
    text("{white}turtlepedia.org", -470, 282);
    bold(0);

    textsize(15);
    link("{silver}< Back to results", 330, 282, results);

    color("black");
    textsize(30);
    bold(1);
    text(clicked_text(), -430, 190);          /* the title of the link you clicked */
    bold(0);

    textsize(18);
    text("This page is pretend. It was made by search.c just now.", -430, 130);
    text("You searched for {blue}%s{black}.", -430, 95, search);

    color("lightblue");
    round_box(-200, -40, 460, 100, 14);
    textsize(17);
    color("black");
    text("{blue}Did you know?", -410, -20);
    text("Everything here is text(), link(), button() and box().", -410, -50);
}

/* --- the results page --- */
void results(void) {
    new_screen();

    /* small colorful logo - click it to go home */
    textsize(30);
    bold(1);
    link("{#2e9e5b}T{orange}u{purple}r{blue}t{red}l{teal}r", -480, 270, home);
    bold(0);

    /* search box at the top, with what you searched in it */
    inputbox("", -90, 282, 560, do_search);
    set_input(search);

    /* line under the top */
    color("lightgray");
    box(0, 230, 1000, 1);

    textsize(13);
    color("gray");
    text("About 1,240,000 results for \"%s\"", -370, 200, search);

    /* 5 results: a site name, a blue title you can click, a grey sentence */
    textsize(13);
    color("black");
    text("turtlepedia.org", -370, 160);
    text("learnstuff.net", -370, 80);
    text("funfacts.io", -370, 0);
    text("howto.zone", -370, -80);
    text("talkforum.com", -370, -160);

    textsize(20);
    color("linkblue");
    link(str("%s - Turtlepedia, the free encyclopedia", search), -370, 135, website);
    link(str("What is %s? A simple guide for beginners", search), -370, 55, website);
    link(str("10 things you didn't know about %s", search), -370, -25, website);
    link(str("%s: tips, tricks and common mistakes", search), -370, -105, website);
    link(str("Is %s worth it? - Forum discussion", search), -370, -185, website);

    textsize(14);
    color("darkgray");
    text("%s is something lots of people search for. Here is its history and more.", -370, 113, search);
    text("New to %s? Start here. We explain the basics in plain words.", -370, 33, search);
    text("Number 7 will surprise you. Fun facts about %s.", -370, -47, search);
    text("Everything we learned after trying %s for a whole year.", -370, -127, search);
    text("We asked 500 people what they think about %s.", -370, -207, search);
}

/* --- the home page --- */
void home(void) {
    new_screen();

    /* top right corner */
    textsize(14);
    color("black");
    link("Mail", 250, 278);
    link("Images", 295, 278);
    button_color("#1a73e8", "white");
    button("Sign in", 440, 284);

    /* the big logo: every letter its own color */
    textsize(92);
    bold(1);
    text_center("{#2e9e5b}T{orange}u{purple}r{blue}t{red}l{teal}r", 0, 95);
    bold(0);

    /* the search box and the two buttons */
    inputbox("Search Turtlr or type a URL", 0, 10, 580, do_search);
    set_input("");
    button_color("#f8f9fa", "#3c4043");
    textsize(14);
    button("Turtlr Search", -85, -55, do_search);
    button("I'm Feeling Lucky", 85, -55, lucky);

    textsize(13);
    color("gray");
    text_center("Made with crayon.h  -  you can drag the turtle", 0, -110);

    /* a little turtle picture at the bottom */
    use(1);
    show();
    use(0);

    /* grey footer */
    color("#f2f2f2");
    box(0, -300, 1000, 40);
    textsize(14);
    color("gray");
    text("About     Advertising     Business     How Turtlr works", -470, -305);
    text("Privacy     Terms     Settings", 280, -305);
}

int main(void) {
    window(1000, 640);
    title("Turtlr Search");

    new_turtle();               /* turtle number 1: the little turtle picture */
    picture("turtle.png");
    go_to(-300, -258);
    draggable();
    use(0);

    home();
}
