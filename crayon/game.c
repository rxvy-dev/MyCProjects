/* game.c - a tiny game where the player IS a turtle
 *
 *     gcc game.c -o game && ./game
 *
 * UP = walk, DOWN = back up, LEFT / RIGHT = turn, SPACE = dash
 * Grab the coins, stay away from the red arrow. Drag the star for a bonus.
 */
#include "crayon.h"

int main(void) {
    window(800, 600);
    title("Turtle Coins");
    background("lightgreen");

    /* the player: a turtle picture, moved with forward/left/right */
    Turtle player = new_turtle();
    picture("turtle.png");

    /* coins: 5 more turtles that look like coins */
    Turtle coins[5];
    for (int i = 0; i < 5; i++) {
        coins[i] = new_turtle();
        picture("coin.png");
        go_to(rnd(-350, 350), rnd(-250, 230));
    }

    /* a star you can drag */
    Turtle star = new_turtle();
    picture("star.png");
    go_to(250, 150);
    draggable();

    /* the enemy: a big red arrow */
    Turtle enemy = new_turtle();
    arrow();
    color("red");
    turtle_size(2);
    penup();
    go_to(-300, 200);

    int score = 0, lives = 3;

    forever {
        if (lives > 0) {
            /* --- move the player like a turtle --- */
            use(player);
            int step = hold(SPACE) ? 8 : 4;
            key(UP)    forward(step);
            key(DOWN)  back(3);
            key(LEFT)  left(5);
            key(RIGHT) right(5);

            /* --- coins --- */
            for (int i = 0; i < 5; i++) {
                use(coins[i]);
                right(4);                              /* coins spin */
                if (touching(player)) {
                    score++;
                    go_to(rnd(-350, 350), rnd(-250, 230));
                }
            }

            /* --- star bonus --- */
            use(star);
            right(1);
            if (touching(player)) {
                score += 5;
                go_to(rnd(-300, 300), rnd(-200, 200));
            }

            /* --- the enemy chases you --- */
            use(player);
            float px = xcor(), py = ycor();
            use(enemy);
            point_to(px, py);
            forward(1.2f + score * 0.05f);
            if (touching(player)) {
                lives--;
                go_to(rnd(-350, 350), 250);
                use(player);
                go_to(0, 0);
            }
        }

        /* --- text --- */
        use(0);
        color("darkgray");
        textsize(26); bold(1);
        text("Score: %d", -380, 265, score);
        bold(0); textsize(18);
        text("Lives: %d", -380, 238, lives);

        if (lives <= 0) {
            textsize(60); bold(1);
            color("red");
            text_center("GAME OVER", 0, 40);
            bold(0); textsize(18);
            if (button("Play again", 0, -30)) {
                score = 0; lives = 3;
                use(player); go_to(0, 0); face(0);
                use(enemy); go_to(-300, 200);
            }
        }
    }
}
