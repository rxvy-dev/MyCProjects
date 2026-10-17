# 🖍️ crayon.h

**The easiest way to draw, make games and make apps in C.**
One file. No setup. No libraries to link. Just `#include` it and go.

```sh
gcc main.c -o main && ./main
```

```c
#include "crayon.h"

int main(void) {
    window(600, 400);
    arrow();

    forward(100);
    right(90);
    forward(100);
    right(90);
    forward(100);
    right(90);
    forward(100);
    right(90);
}
```

<img src="screenshots/square.png" width="420">

That's the whole program. No event loop, no setup code, no cleanup. The window stays open by itself.

---

## Why crayon?

Most C graphics libraries make you write 50 lines before you see a single pixel —
create a renderer, load fonts, write an event loop, free everything at the end.

With crayon you write what you **mean**:

| You want to... | You write |
|---|---|
| draw a line | `forward(100);` |
| show a picture | `image("cat.png", 0, 0);` |
| write colorful text | `text("{red}Hi {blue}there", 0, 0);` |
| ask a question | `name = userinput("Your name?");` |
| make a button | `button("Play", 0, -100, play);` |
| move a character | `key(UP) forward(4);` |

It's only one header file, and it builds with plain `gcc` — no `-lX11`, no `-lpng`, no CMake.

---

## Draw

```c
window(600, 400);
arrow();               // show the arrow (or nothing = invisible pen)

forward(100);
right(90);             // any number: right(37), left(720) ...
circle(50);
go_to(100, 50);        // jump to a spot
dot(20);
color("red");          // names or "#ff8800"
thickness(5);
background("sky");
```

The middle of the window is `(0, 0)`. `x` goes right, `y` goes up.
Not sure where something is? Call `grid();` and the numbers show up on screen.

## Pictures

```c
image("cat.png", 100, 50);        // picture with its middle at (100, 50)
image("cat.png", 100, 50, 2);     // twice as big
```

<img src="screenshots/gallery.png" width="520">

Or turn the pen itself into a picture — it still moves with `forward`, `left` and `right`:

```c
picture("turtle.png");
repeat(5) { forward(150); right(144); }
```

<img src="screenshots/pictures.png" width="520">

## Colorful text

```c
text("Hello", -200, 100);                         // words, x, y
text("{red}G{yellow}o{blue}o{green}d", 0, 0);     // a color for each part
text("{rainbow}Rainbow!", 0, -50);                // every letter a different color
text("Score: %d", -200, 150, score);              // numbers inside
text_center("Title", 0, 150);                     // centered on x, y
textsize(40);
bold(1);
```

## Ask the user things

The program simply waits for the answer — no callbacks needed.

```c
const char *name = userinput("What's your name?");
text("{rainbow}Hi %s!", 0, 100, name);

int choice = pick("Yes", "No");        // 1 = Yes, 2 = No
```

<img src="screenshots/hello.png" width="520">

## Buttons, links and typing

```c
void play(void) { text("Let's go!", 0, 0); }

button("Play", 0, -100, play);         // words, x, y, what to do
link("Click me", -200, 50, play);
button_color("blue", "white");
```

A box you can type into — whatever you type can make new things appear:

```c
void got_it(void) {
    text(entry(), 0, 0);                              // print what they typed
    if (typed("party"))  background("pink");
    if (typed("button")) button("I'm new!", 0, -100, got_it);
}

inputbox("Type here...", 0, -200, 500, got_it);
```

<img src="screenshots/commands.png" width="520">

## Pages

```c
void settings(void) {
    new_screen();                      // wipe everything
    text("Settings", 0, 100);
    link("< Back", -50, 0, home);
}
```

Every page is just a function. A button or link says which one to open next.
That's how the search engine example below is built.

## Games

```c
Turtle player = new_turtle();
picture("player.png");

forever {
    key(UP)    forward(4);
    key(LEFT)  left(5);
    key(RIGHT) right(5);
    press(SPACE) text("jump!", 0, 0);
}
```

Also: `touching(coin)`, `draggable()`, `clicked()`, `mouse_x()`, `rnd(1, 6)`.

<img src="screenshots/game.png" width="520">

---

## Example: a search engine in ~150 lines

`search.c` is a pretend search engine called **Turtlr**: home page, search box, buttons,
a results page and pretend websites — all made from `text()`, `link()`, `button()`,
`inputbox()` and `box()`.

<img src="screenshots/search-home.png" width="640">
<img src="screenshots/search-results.png" width="640">

```c
void home(void) {
    new_screen();

    textsize(92);
    bold(1);
    text_center("{#2e9e5b}T{orange}u{purple}r{blue}t{red}l{teal}r", 0, 95);
    bold(0);

    inputbox("Search Turtlr or type a URL", 0, 10, 580, do_search);
    button("Turtlr Search", -85, -55, do_search);
    button("I'm Feeling Lucky", 85, -55, lucky);
}
```

---

## All the examples

| file | what it shows |
|---|---|
| `square.c` | the classic square |
| `gallery.c` | pictures anywhere with `image()` |
| `hello.c` | `userinput()`, `pick()` and rainbow text |
| `commands.c` | type words, new things appear |
| `pictures.c` | a picture pen and an arrow pen drawing together |
| `game.c` | a small arcade game |
| `search.c` | Turtlr, the pretend search engine |

```sh
gcc search.c -o search && ./search
```

## Requirements

Linux with X11 or Wayland (XWayland). Uses `libX11`, `libpng` and (optionally) `libXft`
for smooth text — all already on any normal desktop Linux. They're loaded at runtime,
so you never have to link anything.

## Cheat sheet

```
window(w, h)  title("...")  background("color")  grid()  show_coords()
forward(n)  back(n)  left(deg)  right(deg)  face(deg)  go_to(x, y)  walk_to(x, y)  go_home()
color("red")  thickness(n)  penup()  pendown()  speed(0-10)  circle(r)  dot(size)
box(x, y, w, h)  round_box(x, y, w, h, r)  outline(x, y, w, h, r)  begin_fill()  end_fill()
clear()  new_screen()
arrow()  picture("x.png")  hide()  show()  turtle_size(n)  flip(1)  keep_upright()
image("x.png", x, y)  image("x.png", x, y, scale)
text("...", x, y)  text_center("...", x, y)  textsize(n)  bold(1)  say("...")
userinput("question")  numberinput("question")  pick("A", "B")  wait_button()  wait_key()
button("words", x, y, fn)  link("words", x, y, fn)  button_color("bg", "text")
inputbox("hint", x, y, width, fn)  entry()  typed("x")  command("x")  word(n)  number(n)
new_turtle()  use(t)  touching(t)  clicked()  draggable()  xcor()  ycor()
forever { }  key(k) { }  press(k) { }  mouse_x()  mouse_y()  rnd(a, b)  wait_for(sec)
```

## License

MIT — do whatever you want with it.
