/*
 * crayon.h - the easiest way to draw, make games and make apps in C.
 * One file. Works on Linux (X11, and Wayland through XWayland).
 *
 *     gcc main.c -o main && ./main
 *
 *     #include "crayon.h"
 *     int main(void) {
 *         window(600, 400);
 *         arrow();
 *         repeat(4) { forward(100); right(90); }
 *         image("cat.png", 150, 0);
 *         text("{red}Hello {blue}world", -250, 150);
 *     }
 *
 * The middle of the window is (0, 0). x goes right, y goes UP.  grid(); shows the numbers.
 * The window stays open until you close it.
 */
#ifndef CRAYON_H
#define CRAYON_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#define TT __attribute__((unused)) static

/* ============================================================ */
/* COLORS   (or make your own: rgb(255, 128, 0)  or  0xFF8000)  */
/* ============================================================ */
#define RED      0xE74C3C
#define GREEN    0x2ECC71
#define BLUE     0x3498DB
#define YELLOW   0xF1C40F
#define ORANGE   0xE67E22
#define PURPLE   0x9B59B6
#define PINK     0xFF6FB5
#define CYAN     0x1ABC9C
#define BROWN    0x8B5A2B
#define BLACK    0x000000
#define WHITE    0xFFFFFF
#define GRAY     0x95A5A6
#define DARKGRAY 0x2C3E50
#define SKY      0x87CEEB
#define GRASS    0x3E8E41
#define rgb(r, g, b) ((unsigned long)(((r) & 255) << 16 | ((g) & 255) << 8 | ((b) & 255)))

/* ============================================================ */
/* KEYS: letters and numbers are just 'a', 'w', '1' ...         */
/* ============================================================ */
#define UP        XK_Up
#define DOWN      XK_Down
#define LEFT      XK_Left
#define RIGHT     XK_Right
#define SPACE     XK_space
#define ENTER     XK_Return
#define ESC       XK_Escape
#define TAB       XK_Tab
#define SHIFT     XK_Shift_L
#define CTRL      XK_Control_L
#define BACKSPACE XK_BackSpace
#define MOUSE     0x10001        /* left mouse button  */
#define RMOUSE    0x10003        /* right mouse button */

/* repeat(4) { ... }      does the inside 4 times */
#define repeat(n) for (int _tt_i = 0; _tt_i < (n); _tt_i++)

/* forever { ... }        runs the inside 60 times a second, until the window closes.
 *                        Inside forever, everything moves instantly (no slow animation). */
#define forever while (update())

/* key('w') { ... }       while the key is held down
 * press(SPACE) { ... }   once, each time the key is pressed */
#define key(k)   if (hold(k))
#define press(k) if (click(k))

#define _TT_PICK4(_1, _2, _3, _4, NAME, ...) NAME
#define _TT_PICK5(_1, _2, _3, _4, _5, NAME, ...) NAME

typedef void (*TtFn)(void);
typedef int Turtle;

/* ---- libX11, libXft and libpng are loaded at runtime, so plain `gcc file.c` works ---- */
#define _TT_XFUNCS(F) \
    F(XOpenDisplay) F(XCreateSimpleWindow) F(XStoreName) F(XSelectInput) \
    F(XInternAtom) F(XSetWMProtocols) F(XAllocSizeHints) F(XSetWMNormalHints) \
    F(XFree) F(XCreateGC) F(XLoadQueryFont) F(XSetFont) F(XCreatePixmap) \
    F(XSetForeground) F(XFillRectangle) F(XMapWindow) F(XNextEvent) \
    F(XCopyArea) F(XSetLineAttributes) F(XFillPolygon) F(XDrawLine) \
    F(XDrawRectangle) F(XDrawString) F(XFlush) F(XFillArc) F(XDrawArc) F(XTextWidth) \
    F(XLookupKeysym) F(XLookupString) F(XCloseDisplay) F(XGetImage) F(XPutImage) \
    F(XPending) F(XkbSetDetectableAutoRepeat) F(XCreateFontCursor) F(XDefineCursor)
#define _TT_DECL(n) __typeof__(&n) n;
static struct { _TT_XFUNCS(_TT_DECL) } _xl;

TT void _tt_loadx(void) {
    static int loaded = 0;
    if (loaded) return;
    void *h = dlopen("libX11.so.6", RTLD_NOW);
    if (!h) { fprintf(stderr, "crayon.h: cannot load libX11 (install the libx11 package)\n"); exit(1); }
#define _TT_LOAD(n) _xl.n = (__typeof__(&n))dlsym(h, #n);
    _TT_XFUNCS(_TT_LOAD)
    loaded = 1;
}
#define XOpenDisplay        (_xl.XOpenDisplay)
#define XCreateSimpleWindow (_xl.XCreateSimpleWindow)
#define XStoreName          (_xl.XStoreName)
#define XSelectInput        (_xl.XSelectInput)
#define XInternAtom         (_xl.XInternAtom)
#define XSetWMProtocols     (_xl.XSetWMProtocols)
#define XAllocSizeHints     (_xl.XAllocSizeHints)
#define XSetWMNormalHints   (_xl.XSetWMNormalHints)
#define XFree               (_xl.XFree)
#define XCreateGC           (_xl.XCreateGC)
#define XLoadQueryFont      (_xl.XLoadQueryFont)
#define XSetFont            (_xl.XSetFont)
#define XCreatePixmap       (_xl.XCreatePixmap)
#define XSetForeground      (_xl.XSetForeground)
#define XFillRectangle      (_xl.XFillRectangle)
#define XMapWindow          (_xl.XMapWindow)
#define XNextEvent          (_xl.XNextEvent)
#define XCopyArea           (_xl.XCopyArea)
#define XSetLineAttributes  (_xl.XSetLineAttributes)
#define XFillPolygon        (_xl.XFillPolygon)
#define XDrawLine           (_xl.XDrawLine)
#define XDrawRectangle      (_xl.XDrawRectangle)
#define XDrawString         (_xl.XDrawString)
#define XFlush              (_xl.XFlush)
#define XFillArc            (_xl.XFillArc)
#define XDrawArc            (_xl.XDrawArc)
#define XTextWidth          (_xl.XTextWidth)
#define XLookupKeysym       (_xl.XLookupKeysym)
#define XLookupString       (_xl.XLookupString)
#define XCloseDisplay       (_xl.XCloseDisplay)
#define XGetImage           (_xl.XGetImage)
#define XPutImage           (_xl.XPutImage)
#define XPending            (_xl.XPending)
#define XkbSetDetectableAutoRepeat (_xl.XkbSetDetectableAutoRepeat)
#define XCreateFontCursor   (_xl.XCreateFontCursor)
#define XDefineCursor       (_xl.XDefineCursor)

/* smooth fonts (libXft) - optional, falls back to plain X fonts */
typedef struct { unsigned short red, green, blue, alpha; } _tt_rcolor;
typedef struct { unsigned long pixel; _tt_rcolor color; } _tt_xcolor;
typedef struct { int ascent, descent, height, max_advance_width; void *charset, *pattern; } _tt_xfont;
typedef struct { unsigned short width, height; short x, y, xOff, yOff; } _tt_glyph;
static struct {
    int ok;
    void *(*open)(Display *, int, const char *);
    void *(*draw_create)(Display *, Drawable, Visual *, Colormap);
    int   (*color)(Display *, Visual *, Colormap, const _tt_rcolor *, _tt_xcolor *);
    void  (*str)(void *, const _tt_xcolor *, void *, int, int, const unsigned char *, int);
    void  (*extents)(Display *, void *, const unsigned char *, int, _tt_glyph *);
} _tt_xft;

/* PNG pictures (libpng) */
typedef struct {
    void *opaque;
    uint32_t version, width, height, format, flags, colormap_entries, warning_or_error;
    char message[64];
} _tt_png_image;
static struct {
    int tried, ok;
    int  (*begin)(_tt_png_image *, const char *);
    int  (*finish)(_tt_png_image *, const void *, void *, int32_t, void *);
    void (*release)(_tt_png_image *);
} _tt_png;

/* ---- tiny math (so no -lm is needed) ---- */
TT float _tt_abs(float v) { return v < 0 ? -v : v; }
TT int _tt_round(float v) { return (int)(v + (v < 0 ? -0.5f : 0.5f)); }
TT float _tt_sind(float deg) {
    deg -= 360.0f * (int)(deg / 360.0f);
    if (deg > 180) deg -= 360;
    if (deg < -180) deg += 360;
    if (deg > 90) deg = 180 - deg;
    if (deg < -90) deg = -180 - deg;
    float r = deg * 3.14159265f / 180, r2 = r * r;
    return r * (1 - r2 / 6 * (1 - r2 / 20 * (1 - r2 / 42 * (1 - r2 / 72 * (1 - r2 / 110)))));
}
TT float _tt_cosd(float deg) { return _tt_sind(deg + 90); }
TT float _tt_sqrt(float v) {
    if (v <= 0) return 0;
    float r = v > 1 ? v : 1;
    for (int i = 0; i < 40; i++) r = 0.5f * (r + v / r);
    return r;
}
TT float _tt_atan2d(float y, float x) {
    if (x == 0 && y == 0) return 0;
    float ax = _tt_abs(x), ay = _tt_abs(y);
    float t = ay < ax ? ay / ax : ax / ay, t2 = t * t;
    float r = t * (0.9998660f + t2 * (-0.3302995f + t2 * (0.1801410f + t2 * (-0.0851330f + t2 * 0.0208351f))));
    if (ay > ax) r = 1.57079633f - r;
    if (x < 0) r = 3.14159265f - r;
    if (y < 0) r = -r;
    return r * 180 / 3.14159265f;
}
TT int _tt_lower(int c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }
TT int _tt_ieq(const char *a, const char *b) {
    while (*a && *b) if (_tt_lower(*a++) != _tt_lower(*b++)) return 0;
    return *a == *b;
}

/* ============================================================ */
/* STATE                                                        */
/* ============================================================ */
#define _TT_MAXT    64
#define _TT_MAXI    64
#define _TT_MAXB    160
#define _TT_MAXTXT  512
#define _TT_FILLMAX 2048

enum { _TT_NONE, _TT_ARROW, _TT_PIC };

typedef struct {
    float x, y, heading;
    int pen, thick, look, img, has_pic, flip, upright, drag, filling, nfill;
    float size;
    unsigned long color;
    XPoint fill[_TT_FILLMAX];
} _TtTurtle;

typedef struct { char name[256]; int w, h; unsigned char *px; } _TtImage;
typedef struct { int size, bold; void *xft; XFontStruct *core; int asc, desc; } _TtFont;
typedef struct {
    char label[96]; char cmd[200];
    float cx, cy;
    int kind;                         /* 0 = bar button, 1 = placed button, 2 = link */
    int imm, used; long last;
    int clicked, size, bold;
    unsigned long bg, fg;
    TtFn fn;
    int x, y, w, h;                   /* in pixels, worked out when drawing */
} _TtButton;
typedef struct { float x, y; unsigned long c; int size, bold, center; char s[200]; } _TtText;

static struct {
    Display *dpy;
    Window win;
    Pixmap canvas, buf;
    GC gc;
    Visual *vis; Colormap cmap;
    void *xd_canvas, *xd_buf;
    Atom del;
    int W, H, depth, closed, at_exit;
    int spd;
    unsigned long bg;

    _TtTurtle t[_TT_MAXT];
    int nt, cur;

    _TtImage img[_TT_MAXI];
    int nimg;

    _TtFont fonts[24];
    int nfonts, tsize, tbold;

    int mx, my;
    float cx, cy;
    int mdown, mtapped, rdown, rtapped, loop_mode;
    int dragging, clicked_t; float dragdx, dragdy;
    unsigned char down[65536], tapped[65536];
    TtFn onclick_fn, oninput_fn;
    struct { int key; TtFn fn; } keys[64];
    int nkeys;

    _TtButton btn[_TT_MAXB];
    int nbtn, nbar;

    _TtText txt[_TT_MAXTXT];
    int ntxt;

    /* the text box */
    int tb_on, tb_imm; long tb_last;
    float tb_x, tb_y, tb_w;
    int focus, submitted, console_on, sent, last_clicked;
    char last_label[96];
    char input[200], entry[200], hint[120], msg[160];

    unsigned long bar_c, btn_c, btn_text, box_c, box_text, box_hint, box_line;
    int coords_on, cursor_now;
    unsigned long cursors[3];
    long frame;
    struct timespec last;
} _tt;

#define _TT_T (&_tt.t[_tt.cur])
#define _TT_SX(px) (_tt_round(_tt.W / 2.0f + (px)))
#define _TT_SY(py) (_tt_round(_tt.H / 2.0f - (py)))
#define _TT_BARH 46
#define _TT_TBH 44

TT void _tt_check(void) {
    if (_tt.closed) { if (_tt.at_exit) return; exit(0); }
    if (!_tt.dpy) { fprintf(stderr, "crayon.h: call window(w, h) first\n"); exit(1); }
}
TT void _tt_delay(void) { struct timespec t = { 0, 8000000 }; nanosleep(&t, NULL); }

/* ============================================================ */
/* FONTS                                                        */
/* ============================================================ */
TT void _tt_load_xft(void) {
    void *h = dlopen("libXft.so.2", RTLD_NOW);
    if (!h) return;
    *(void **)&_tt_xft.open        = dlsym(h, "XftFontOpenName");
    *(void **)&_tt_xft.draw_create = dlsym(h, "XftDrawCreate");
    *(void **)&_tt_xft.color       = dlsym(h, "XftColorAllocValue");
    *(void **)&_tt_xft.str         = dlsym(h, "XftDrawStringUtf8");
    *(void **)&_tt_xft.extents     = dlsym(h, "XftTextExtentsUtf8");
    _tt_xft.ok = _tt_xft.open && _tt_xft.draw_create && _tt_xft.color && _tt_xft.str && _tt_xft.extents;
}

TT _TtFont *_tt_font(int size, int bold) {
    for (int i = 0; i < _tt.nfonts; i++)
        if (_tt.fonts[i].size == size && _tt.fonts[i].bold == bold) return &_tt.fonts[i];
    _TtFont f; memset(&f, 0, sizeof f);
    f.size = size; f.bold = bold;
    char name[160];
    if (_tt_xft.ok) {
        snprintf(name, sizeof name, "sans-serif:pixelsize=%d%s", size, bold ? ":bold" : "");
        f.xft = _tt_xft.open(_tt.dpy, DefaultScreen(_tt.dpy), name);
    }
    if (f.xft) { f.asc = ((_tt_xfont *)f.xft)->ascent; f.desc = ((_tt_xfont *)f.xft)->descent; }
    else {
        const char *pats[] = { "-*-helvetica-%s-r-normal--%d-*-*-*-*-*-iso8859-1",
                               "-*-dejavu sans-%s-r-normal--%d-*-*-*-*-*-*-*",
                               "-*-*-%s-r-normal--%d-*-*-*-*-*-iso8859-1" };
        for (unsigned i = 0; i < 3 && !f.core; i++) {
            snprintf(name, sizeof name, pats[i], bold ? "bold" : "medium", size);
            f.core = XLoadQueryFont(_tt.dpy, name);
        }
        if (!f.core) f.core = XLoadQueryFont(_tt.dpy, "fixed");
        f.asc = f.core ? f.core->ascent : 10; f.desc = f.core ? f.core->descent : 3;
    }
    if (_tt.nfonts >= 24) _tt.nfonts = 23;
    _tt.fonts[_tt.nfonts] = f;
    return &_tt.fonts[_tt.nfonts++];
}
TT int _tt_tw(_TtFont *f, const char *s) {
    int n = (int)strlen(s);
    if (f->xft) { _tt_glyph g; _tt_xft.extents(_tt.dpy, f->xft, (const unsigned char *)s, n, &g); return g.xOff; }
    return f->core ? XTextWidth(f->core, s, n) : n * 8;
}
TT void _tt_str(Drawable d, _TtFont *f, int x, int y, unsigned long c, const char *s) {
    if (f->xft) {
        void *xd = d == _tt.canvas ? _tt.xd_canvas : _tt.xd_buf;
        _tt_rcolor rc = { (unsigned short)(((c >> 16) & 255) * 257), (unsigned short)(((c >> 8) & 255) * 257),
                          (unsigned short)((c & 255) * 257), 0xFFFF };
        _tt_xcolor col;
        _tt_xft.color(_tt.dpy, _tt.vis, _tt.cmap, &rc, &col);
        _tt_xft.str(xd, &col, f->xft, x, y, (const unsigned char *)s, (int)strlen(s));
    } else {
        if (f->core) XSetFont(_tt.dpy, _tt.gc, f->core->fid);
        XSetForeground(_tt.dpy, _tt.gc, c);
        XDrawString(_tt.dpy, d, _tt.gc, x, y, s, (int)strlen(s));
    }
}

/* ============================================================ */
/* PNG PICTURES                                                 */
/* ============================================================ */
TT int _tt_image(const char *file) {
    for (int i = 0; i < _tt.nimg; i++) if (strcmp(_tt.img[i].name, file) == 0) return i;
    if (_tt.nimg >= _TT_MAXI) { fprintf(stderr, "crayon.h: too many different pictures\n"); return 0; }
    if (!_tt_png.tried) {
        _tt_png.tried = 1;
        void *h = dlopen("libpng16.so.16", RTLD_NOW);
        if (!h) h = dlopen("libpng.so", RTLD_NOW);
        if (h) {
            *(void **)&_tt_png.begin   = dlsym(h, "png_image_begin_read_from_file");
            *(void **)&_tt_png.finish  = dlsym(h, "png_image_finish_read");
            *(void **)&_tt_png.release = dlsym(h, "png_image_free");
            _tt_png.ok = _tt_png.begin && _tt_png.finish && _tt_png.release;
        }
        if (!_tt_png.ok) fprintf(stderr, "crayon.h: cannot load libpng (install the libpng package)\n");
    }
    int i = _tt.nimg;
    _TtImage *g = &_tt.img[i];
    snprintf(g->name, sizeof g->name, "%s", file);
    if (_tt_png.ok) {
        _tt_png_image pi;
        memset(&pi, 0, sizeof pi);
        pi.version = 1;
        if (_tt_png.begin(&pi, file)) {
            pi.format = 3;                                 /* RGBA */
            unsigned char *px = (unsigned char *)malloc((size_t)pi.width * pi.height * 4);
            if (px && _tt_png.finish(&pi, NULL, px, 0, NULL)) {
                g->w = (int)pi.width; g->h = (int)pi.height; g->px = px;
                _tt.nimg++;
                return i;
            }
            free(px);
            _tt_png.release(&pi);
        }
        fprintf(stderr, "crayon.h: could not open picture \"%s\" - is it a .png next to your program?\n", file);
    }
    g->w = g->h = 32;                                      /* pink box instead */
    g->px = (unsigned char *)malloc(32 * 32 * 4);
    for (int p = 0; p < 32 * 32; p++) {
        int on = ((p % 32 / 8) + (p / 32 / 8)) % 2;
        g->px[p * 4] = on ? 255 : 60; g->px[p * 4 + 1] = 0; g->px[p * 4 + 2] = on ? 255 : 60; g->px[p * 4 + 3] = 255;
    }
    _tt.nimg++;
    return i;
}

TT void _tt_blit(XImage *im, _TtImage *g, float cx, float cy, float sc, float ang, int flip) {
    if (sc <= 0) return;
    float hw = g->w * sc / 2, hh = g->h * sc / 2;
    float r = _tt_sqrt(hw * hw + hh * hh) + 1;
    int x0 = (int)(cx - r), x1 = (int)(cx + r) + 1, y0 = (int)(cy - r), y1 = (int)(cy + r) + 1;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > im->width) x1 = im->width;
    if (y1 > im->height) y1 = im->height;
    float ca = _tt_cosd(ang), sa = _tt_sind(ang);
    for (int py = y0; py < y1; py++) {
        uint32_t *row = (uint32_t *)(im->data + py * im->bytes_per_line);
        for (int px = x0; px < x1; px++) {
            float mx = px + 0.5f - cx, my = cy - (py + 0.5f);
            float ux = mx * ca + my * sa, uy = -mx * sa + my * ca;
            int ix = (int)(ux / sc + g->w / 2.0f + 1000) - 1000;
            int iy = (int)(-uy / sc + g->h / 2.0f + 1000) - 1000;
            if (ix < 0 || iy < 0 || ix >= g->w || iy >= g->h) continue;
            if (flip) ix = g->w - 1 - ix;
            unsigned char *s = g->px + (iy * g->w + ix) * 4;
            unsigned a = s[3];
            if (!a) continue;
            uint32_t d = row[px];
            unsigned dr = (d >> 16) & 255, dg = (d >> 8) & 255, db = d & 255;
            dr = (s[0] * a + dr * (255 - a)) / 255;
            dg = (s[1] * a + dg * (255 - a)) / 255;
            db = (s[2] * a + db * (255 - a)) / 255;
            row[px] = (d & 0xFF000000u) | dr << 16 | dg << 8 | db;
        }
    }
}

TT float _tt_pic_angle(_TtTurtle *t) { return t->upright ? 0 : t->heading; }

/* is point (x, y) on turtle i? */
TT int _tt_hit(int i, float x, float y) {
    _TtTurtle *t = &_tt.t[i];
    if (t->look == _TT_ARROW) {
        float dx = x - t->x, dy = y - t->y, r = 12 * t->size;
        return dx * dx + dy * dy <= r * r;
    }
    if (t->look != _TT_PIC || t->size <= 0) return 0;
    _TtImage *g = &_tt.img[t->img];
    float mx = x - t->x, my = y - t->y, a = _tt_pic_angle(t);
    float ca = _tt_cosd(a), sa = _tt_sind(a);
    float ux = mx * ca + my * sa, uy = -mx * sa + my * ca;
    float fx = ux / t->size + g->w / 2.0f, fy = -uy / t->size + g->h / 2.0f;
    if (fx < 0 || fy < 0 || fx >= g->w || fy >= g->h) return 0;
    int ix = (int)fx, iy = (int)fy;
    if (t->flip) ix = g->w - 1 - ix;
    return g->px[(iy * g->w + ix) * 4 + 3] > 20;
}
TT int _tt_turtle_at(float x, float y) {
    for (int i = _tt.nt - 1; i >= 0; i--) if (_tt_hit(i, x, y)) return i;
    return -1;
}

/* ============================================================ */
/* SHAPES (pixel helpers)                                       */
/* ============================================================ */
TT void _tt_rrect(Drawable d, int x, int y, int w, int h, int r, unsigned long c, int fill) {
    Display *dp = _tt.dpy;
    if (r * 2 > h) r = h / 2;
    if (r * 2 > w) r = w / 2;
    XSetForeground(dp, _tt.gc, c);
    XSetLineAttributes(dp, _tt.gc, 1, LineSolid, CapButt, JoinMiter);
    if (r <= 0) {
        if (fill) XFillRectangle(dp, d, _tt.gc, x, y, (unsigned)w, (unsigned)h);
        else XDrawRectangle(dp, d, _tt.gc, x, y, (unsigned)w - 1, (unsigned)h - 1);
        return;
    }
    int D = r * 2;
    if (fill) {
        XFillRectangle(dp, d, _tt.gc, x + r, y, (unsigned)(w - D), (unsigned)h);
        XFillRectangle(dp, d, _tt.gc, x, y + r, (unsigned)w, (unsigned)(h - D));
        XFillArc(dp, d, _tt.gc, x, y, D, D, 90 * 64, 90 * 64);
        XFillArc(dp, d, _tt.gc, x + w - D, y, D, D, 0, 90 * 64);
        XFillArc(dp, d, _tt.gc, x, y + h - D, D, D, 180 * 64, 90 * 64);
        XFillArc(dp, d, _tt.gc, x + w - D, y + h - D, D, D, 270 * 64, 90 * 64);
    } else {
        w--; h--;
        XDrawLine(dp, d, _tt.gc, x + r, y, x + w - r, y);
        XDrawLine(dp, d, _tt.gc, x + r, y + h, x + w - r, y + h);
        XDrawLine(dp, d, _tt.gc, x, y + r, x, y + h - r);
        XDrawLine(dp, d, _tt.gc, x + w, y + r, x + w, y + h - r);
        XDrawArc(dp, d, _tt.gc, x, y, D, D, 90 * 64, 90 * 64);
        XDrawArc(dp, d, _tt.gc, x + w - D, y, D, D, 0, 90 * 64);
        XDrawArc(dp, d, _tt.gc, x, y + h - D, D, D, 180 * 64, 90 * 64);
        XDrawArc(dp, d, _tt.gc, x + w - D, y + h - D, D, D, 270 * 64, 90 * 64);
    }
}

/* ============================================================ */
/* BUTTONS / LINKS / TEXT BOX layout                            */
/* ============================================================ */
TT int _tt_tagged(Drawable d, _TtFont *f, int x, int y, unsigned long c, const char *s, int draw);
TT int _tt_btn_visible(_TtButton *b) { return b->used && (!b->imm || b->last == _tt.frame); }
TT int _tt_tb_visible(void) { return _tt.tb_on && (!_tt.tb_imm || _tt.tb_last == _tt.frame); }

TT void _tt_layout(void) {
    int bx = 10;
    _tt.nbar = 0;
    for (int i = 0; i < _tt.nbtn; i++) {
        _TtButton *b = &_tt.btn[i];
        if (!_tt_btn_visible(b)) continue;
        _TtFont *f = _tt_font(b->size, b->kind != 2 || b->bold);
        int tw = _tt_tagged(_tt.buf, f, 0, 0, 0, b->label, 0);
        if (b->kind == 2) {                                   /* link */
            _TtFont *nf = f;
            b->w = tw; b->h = nf->asc + nf->desc + 2;
            b->x = _TT_SX(b->cx); b->y = _TT_SY(b->cy) - nf->asc;
            continue;
        }
        b->w = tw + 28; b->h = f->asc + f->desc + 14;
        if (b->kind == 0) {
            if (b->h > 34) b->h = 34;
            b->x = bx; b->y = (_TT_BARH - b->h) / 2;
            bx += b->w + 8;
            _tt.nbar++;
        } else {
            b->x = _TT_SX(b->cx) - b->w / 2; b->y = _TT_SY(b->cy) - b->h / 2;
        }
    }
}
TT int _tt_tb_px(int *x, int *y, int *w) {
    *w = (int)_tt.tb_w; *x = _TT_SX(_tt.tb_x) - *w / 2; *y = _TT_SY(_tt.tb_y) - _TT_TBH / 2;
    return _tt_tb_visible();
}

/* ============================================================ */
/* DRAWING THE SCREEN                                           */
/* ============================================================ */
TT int _tt_light(unsigned long c) { return ((c >> 16) & 255) + ((c >> 8) & 255) + (c & 255) > 380; }
TT unsigned long _tt_shade(unsigned long c) {
    unsigned r = (c >> 16) & 255, g = (c >> 8) & 255, b = c & 255;
    if (_tt_light(c)) { r = r * 9 / 10; g = g * 9 / 10; b = b * 9 / 10; }
    else { r += (255 - r) / 5; g += (255 - g) / 5; b += (255 - b) / 5; }
    return rgb(r, g, b);
}

/* color names you can use as text:  color("red");  "{red}" inside text */
TT unsigned long _tt_color_name(const char *s, int *ok) {
    static const struct { const char *n; unsigned long c; } names[] = {
        {"red", RED}, {"green", GREEN}, {"blue", BLUE}, {"yellow", YELLOW}, {"orange", ORANGE},
        {"purple", PURPLE}, {"pink", PINK}, {"cyan", CYAN}, {"brown", BROWN}, {"black", BLACK},
        {"white", WHITE}, {"gray", GRAY}, {"grey", GRAY}, {"darkgray", DARKGRAY}, {"darkgrey", DARKGRAY},
        {"lightgray", 0xDADCE0}, {"lightgrey", 0xDADCE0}, {"sky", SKY}, {"grass", GRASS},
        {"navy", 0x1F3A68}, {"lime", 0x7ED957}, {"gold", 0xF5B700}, {"teal", 0x16A085},
        {"magenta", 0xE84393}, {"violet", 0x8E44AD}, {"darkgreen", 0x1E7A46}, {"darkblue", 0x1A3D8F},
        {"darkred", 0x9B1C1C}, {"lightblue", 0xAED6F1}, {"lightgreen", 0xC8F7C5}, {"beige", 0xF5EBDC},
        {"silver", 0xBDC3C7}, {"maroon", 0x7B1E1E}, {"olive", 0x808000}, {"linkblue", 0x1A0DAB},
    };
    *ok = 1;
    for (unsigned i = 0; i < sizeof names / sizeof *names; i++) if (_tt_ieq(s, names[i].n)) return names[i].c;
    if (s[0] == '#' && strlen(s) == 7) return strtoul(s + 1, NULL, 16);
    *ok = 0;
    return 0;
}
TT unsigned long _tt_cname(const char *s) {
    int ok;
    unsigned long c = _tt_color_name(s, &ok);
    if (!ok) fprintf(stderr, "crayon.h: I don't know the color \"%s\" (try \"red\", \"blue\", \"#ff8800\" ...)\n", s);
    return c;
}
TT unsigned long _tt_cnum(unsigned long c) { return c; }
/* lets every color work as a name ("red") or a number (RED, 0xFF0000) */
#define _TT_COL(c) _Generic((c), char *: _tt_cname, const char *: _tt_cname, default: _tt_cnum)(c)

/* text with colors inside: "{red}Hello {blue}there"   "{rainbow}Turtlr"   "{#ff8800}orange" */
TT int _tt_tagged(Drawable d, _TtFont *f, int x, int y, unsigned long c, const char *s, int draw) {
    static const unsigned long rb[] = { 0x2E9E5B, 0xE67E22, 0x8E44AD, 0x3498DB, 0xE74C3C, 0x16A085, 0xF1C40F };
    int x0 = x, rainbow = 0, ri = 0, n = 0;
    char seg[256];
#define _TT_FLUSH() do { seg[n] = 0; if (n) { if (draw) _tt_str(d, f, x, y, c, seg); x += _tt_tw(f, seg); } n = 0; } while (0)
    for (const char *p = s; *p; p++) {
        if (*p == '{') {
            const char *q = strchr(p, '}');
            if (q && q - p < 24) {
                char name[24];
                memcpy(name, p + 1, (size_t)(q - p - 1)); name[q - p - 1] = 0;
                int ok = 0;
                unsigned long col = _tt_color_name(name, &ok);
                if (_tt_ieq(name, "rainbow")) { _TT_FLUSH(); rainbow = 1; p = q; continue; }
                if (ok) { _TT_FLUSH(); c = col; rainbow = 0; p = q; continue; }
            }
        }
        if (rainbow && *p != ' ') {
            _TT_FLUSH();
            char one[2] = { *p, 0 };
            if (draw) _tt_str(d, f, x, y, rb[ri % 7], one);
            x += _tt_tw(f, one);
            ri++;
            continue;
        }
        if (n < 254) seg[n++] = *p;
    }
    _TT_FLUSH();
#undef _TT_FLUSH
    return x - x0;
}

TT void _tt_refresh(void) {
    if (_tt.closed || !_tt.dpy) return;
    Display *d = _tt.dpy;
    _tt_layout();
    XCopyArea(d, _tt.canvas, _tt.buf, _tt.gc, 0, 0, _tt.W, _tt.H, 0, 0);

    /* 1. turtles that are pictures */
    int need = 0;
    for (int i = 0; i < _tt.nt && !need; i++) need = _tt.t[i].look == _TT_PIC;
    if (need) {
        XImage *im = XGetImage(d, _tt.buf, 0, 0, _tt.W, _tt.H, AllPlanes, ZPixmap);
        if (im && im->bits_per_pixel == 32) {
            for (int i = 0; i < _tt.nt; i++) {
                _TtTurtle *t = &_tt.t[i];
                if (t->look == _TT_PIC)
                    _tt_blit(im, &_tt.img[t->img], _tt.W / 2.0f + t->x, _tt.H / 2.0f - t->y, t->size, _tt_pic_angle(t), t->flip);
            }
            XPutImage(d, _tt.buf, _tt.gc, im, 0, 0, 0, 0, _tt.W, _tt.H);
        }
        if (im) XDestroyImage(im);
    }

    /* 2. turtles that are arrows */
    XSetLineAttributes(d, _tt.gc, 1, LineSolid, CapButt, JoinMiter);
    for (int k = 0; k < _tt.nt; k++) {
        _TtTurtle *t = &_tt.t[k];
        if (t->look != _TT_ARROW) continue;
        float ca = _tt_cosd(t->heading), sa = _tt_sind(t->heading), s = t->size;
        float pts[3][2] = {{12 * s, 0}, {-8 * s, 7 * s}, {-8 * s, -7 * s}};
        XPoint p[4];
        for (int i = 0; i < 3; i++) {
            p[i].x = (short)(_TT_SX(t->x) + pts[i][0] * ca - pts[i][1] * sa);
            p[i].y = (short)(_TT_SY(t->y) - (pts[i][0] * sa + pts[i][1] * ca));
        }
        p[3] = p[0];
        XSetForeground(d, _tt.gc, t->color);
        XFillPolygon(d, _tt.buf, _tt.gc, p, 3, Convex, CoordModeOrigin);
        XSetForeground(d, _tt.gc, _tt_light(t->color) ? 0x333333 : 0xFFFFFF);
        for (int i = 0; i < 3; i++) XDrawLine(d, _tt.buf, _tt.gc, p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
    }

    /* 3. text */
    for (int i = 0; i < _tt.ntxt; i++) {
        _TtText *t = &_tt.txt[i];
        _TtFont *f = _tt_font(t->size, t->bold);
        int x = _TT_SX(t->x);
        if (t->center) x -= _tt_tagged(_tt.buf, f, 0, 0, t->c, t->s, 0) / 2;
        _tt_tagged(_tt.buf, f, x, _TT_SY(t->y), t->c, t->s, 1);
    }

    /* 4. mouse coordinates */
    if (_tt.coords_on) {
        char s[64];
        snprintf(s, sizeof s, "x: %d   y: %d", _tt_round(_tt.mx - _tt.W / 2.0f), _tt_round(_tt.H / 2.0f - _tt.my));
        _TtFont *f = _tt_font(14, 1);
        int tw = _tt_tw(f, s), bx = _tt.W - tw - 22, by = (_tt.nbar ? _TT_BARH : 0) + 8;
        _tt_rrect(_tt.buf, bx, by, tw + 14, 26, 6, 0x222222, 1);
        _tt_str(_tt.buf, f, bx + 7, by + 18, WHITE, s);
    }

    /* 5. bar, buttons and links */
    if (_tt.nbar) {
        XSetForeground(d, _tt.gc, _tt.bar_c);
        XFillRectangle(d, _tt.buf, _tt.gc, 0, 0, _tt.W, _TT_BARH);
        XSetForeground(d, _tt.gc, _tt_shade(_tt_shade(_tt.bar_c)));
        XDrawLine(d, _tt.buf, _tt.gc, 0, _TT_BARH, _tt.W, _TT_BARH);
    }
    for (int i = 0; i < _tt.nbtn; i++) {
        _TtButton *b = &_tt.btn[i];
        if (!_tt_btn_visible(b)) continue;
        int hover = _tt.mx >= b->x && _tt.mx < b->x + b->w && _tt.my >= b->y && _tt.my < b->y + b->h;
        if (b->kind == 2) {
            _TtFont *f = _tt_font(b->size, b->bold);
            _tt_tagged(_tt.buf, f, b->x, b->y + f->asc, b->fg, b->label, 1);
            if (hover) { XSetForeground(d, _tt.gc, b->fg); XFillRectangle(d, _tt.buf, _tt.gc, b->x, b->y + f->asc + 2, (unsigned)b->w, 1); }
            continue;
        }
        _TtFont *f = _tt_font(b->size, 1);
        unsigned long bgc = hover ? _tt_shade(b->bg) : b->bg;
        _tt_rrect(_tt.buf, b->x, b->y, b->w, b->h, 7, bgc, 1);
        _tt_rrect(_tt.buf, b->x, b->y, b->w, b->h, 7, _tt_shade(_tt_shade(b->bg)), 0);
        _tt_tagged(_tt.buf, f, b->x + 14, b->y + (b->h + f->asc - f->desc) / 2, b->fg, b->label, 1);
    }

    /* 6. the text box */
    int tx, ty, tw;
    if (_tt_tb_px(&tx, &ty, &tw)) {
        _TtFont *f = _tt_font(17, 0);
        _tt_rrect(_tt.buf, tx, ty, tw, _TT_TBH, _TT_TBH / 2, _tt.box_c, 1);
        _tt_rrect(_tt.buf, tx, ty, tw, _TT_TBH, _TT_TBH / 2, _tt.focus ? BLUE : _tt.box_line, 0);
        if (_tt.focus) _tt_rrect(_tt.buf, tx + 1, ty + 1, tw - 2, _TT_TBH - 2, _TT_TBH / 2 - 1, BLUE, 0);
        int base = ty + (_TT_TBH + f->asc - f->desc) / 2;
        if (!_tt.focus && !_tt.input[0]) {
            _tt_str(_tt.buf, f, tx + 22, base, _tt.box_hint, _tt.msg[0] ? _tt.msg : _tt.hint);
        } else {
            const char *s = _tt.input;
            while (*s && _tt_tw(f, s) > tw - 50) s++;
            _tt_str(_tt.buf, f, tx + 22, base, _tt.box_text, s);
            int cw = _tt_tw(f, s);
            if (_tt.focus) {
                struct timespec now; clock_gettime(CLOCK_MONOTONIC, &now);
                if (now.tv_nsec < 550000000L) {
                    XSetForeground(d, _tt.gc, _tt.box_text);
                    XDrawLine(d, _tt.buf, _tt.gc, tx + 23 + cw, ty + 11, tx + 23 + cw, ty + _TT_TBH - 11);
                }
            }
            if (_tt.msg[0] && _tt_tw(f, _tt.msg) + cw + 70 < tw)
                _tt_str(_tt.buf, f, tx + tw - 22 - _tt_tw(f, _tt.msg), base, _tt.box_hint, _tt.msg);
        }
    }

    XCopyArea(d, _tt.buf, _tt.win, _tt.gc, 0, 0, _tt.W, _tt.H, 0, 0);
    XFlush(d);
}

TT void _tt_show(void) { if (!_tt.loop_mode) _tt_refresh(); }

/* ============================================================ */
/* WINDOW                                                       */
/* ============================================================ */
TT void done(void);
TT void _tt_atexit(void) {
    if (_tt.dpy && !_tt.closed) { _tt.at_exit = 1; done(); }
}

TT void _tt_new_turtle_at(int i) {
    _TtTurtle *t = &_tt.t[i];
    memset(t, 0, sizeof *t);
    t->pen = 1; t->thick = 2; t->look = _TT_NONE; t->size = 1; t->color = BLACK;
}

/* window(800, 600);   makes the window */
TT void window(int w, int h) {
    _tt_loadx();
    _tt.dpy = XOpenDisplay(NULL);
    if (!_tt.dpy) { fprintf(stderr, "crayon.h: no display found\n"); exit(1); }
    Display *d = _tt.dpy;
    int scr = DefaultScreen(d);
    _tt.W = w; _tt.H = h; _tt.depth = DefaultDepth(d, scr);
    _tt.vis = DefaultVisual(d, scr); _tt.cmap = DefaultColormap(d, scr);
    _tt.spd = 5; _tt.bg = WHITE; _tt.tsize = 18;
    _tt.nt = 1; _tt.cur = 0; _tt_new_turtle_at(0);
    _tt.dragging = -1; _tt.clicked_t = -1; _tt.frame = 1;
    snprintf(_tt.hint, sizeof _tt.hint, "Type here...");
    _tt.bar_c = 0xF1F3F4;
    _tt.btn_c = 0xF8F9FA; _tt.btn_text = 0x202124;
    _tt.box_c = WHITE; _tt.box_text = 0x202124; _tt.box_hint = 0x80868B; _tt.box_line = 0xC9CDD2;

    _tt.win = XCreateSimpleWindow(d, RootWindow(d, scr), 0, 0, w, h, 0, 0, WHITE);
    XStoreName(d, _tt.win, "Crayon");
    XSelectInput(d, _tt.win, ExposureMask | KeyPressMask | KeyReleaseMask |
                 ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    _tt.del = XInternAtom(d, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(d, _tt.win, &_tt.del, 1);
    XkbSetDetectableAutoRepeat(d, True, NULL);

    XSizeHints *hints = XAllocSizeHints();
    hints->flags = PMinSize | PMaxSize;
    hints->min_width = hints->max_width = w;
    hints->min_height = hints->max_height = h;
    XSetWMNormalHints(d, _tt.win, hints);
    XFree(hints);

    _tt.gc = XCreateGC(d, _tt.win, 0, NULL);
    _tt.canvas = XCreatePixmap(d, _tt.win, w, h, _tt.depth);
    _tt.buf    = XCreatePixmap(d, _tt.win, w, h, _tt.depth);
    XSetForeground(d, _tt.gc, WHITE);
    XFillRectangle(d, _tt.canvas, _tt.gc, 0, 0, w, h);

    _tt_load_xft();
    if (_tt_xft.ok) {
        _tt.xd_canvas = _tt_xft.draw_create(d, _tt.canvas, _tt.vis, _tt.cmap);
        _tt.xd_buf    = _tt_xft.draw_create(d, _tt.buf, _tt.vis, _tt.cmap);
        if (!_tt.xd_canvas || !_tt.xd_buf) _tt_xft.ok = 0;
    }
    _tt.cursors[0] = XCreateFontCursor(d, 68);   /* arrow  */
    _tt.cursors[1] = XCreateFontCursor(d, 60);   /* hand   */
    _tt.cursors[2] = XCreateFontCursor(d, 152);  /* I-beam */

    XMapWindow(d, _tt.win);
    XEvent e;
    do { XNextEvent(d, &e); } while (e.type != Expose);
    clock_gettime(CLOCK_MONOTONIC, &_tt.last);
    atexit(_tt_atexit);                        /* keep the window open at the end */
    _tt_refresh();
}

/* title("My Game"); */
TT void title(const char *fmt, ...) {
    _tt_check();
    char b[256];
    va_list a; va_start(a, fmt); vsnprintf(b, sizeof b, fmt, a); va_end(a);
    XStoreName(_tt.dpy, _tt.win, b);
}

/* ============================================================ */
/* MOVING                                                       */
/* ============================================================ */
TT void _tt_fill_add(void) {
    _TtTurtle *t = _TT_T;
    if (t->filling && t->nfill < _TT_FILLMAX) {
        t->fill[t->nfill].x = (short)_TT_SX(t->x);
        t->fill[t->nfill].y = (short)_TT_SY(t->y);
        t->nfill++;
    }
}
TT int _tt_animate(void) { return _tt.spd > 0 && !_tt.loop_mode; }

/* forward(100);   go forward (draws a line when the pen is down) */
TT void forward(float distance) {
    _tt_check();
    _TtTurtle *t = _TT_T;
    float sign = distance < 0 ? -1 : 1, rem = _tt_abs(distance);
    float step = _tt_animate() ? _tt.spd * 2.0f : rem;
    while (rem > 0) {
        float s = rem < step ? rem : step;
        rem -= s;
        float ox = t->x, oy = t->y;
        t->x += _tt_cosd(t->heading) * s * sign;
        t->y += _tt_sind(t->heading) * s * sign;
        if (t->pen) {
            XSetForeground(_tt.dpy, _tt.gc, t->color);
            XSetLineAttributes(_tt.dpy, _tt.gc, t->thick, LineSolid, CapRound, JoinRound);
            XDrawLine(_tt.dpy, _tt.canvas, _tt.gc, _TT_SX(ox), _TT_SY(oy), _TT_SX(t->x), _TT_SY(t->y));
        }
        if (_tt_animate()) { _tt_refresh(); _tt_delay(); }
    }
    _tt_fill_add();
    _tt_show();
}
/* back(50);       go backwards */
TT void back(float distance) { forward(-distance); }

TT void _tt_turn(float deg) {
    _tt_check();
    _TtTurtle *t = _TT_T;
    float sign = deg < 0 ? -1 : 1, rem = _tt_abs(deg);
    int anim = _tt_animate() && t->look != _TT_NONE;      /* invisible turtles turn instantly */
    float step = anim ? _tt.spd * 3.0f : rem;
    while (rem > 0) {
        float s = rem < step ? rem : step;
        rem -= s;
        t->heading += sign * s;
        if (anim) { _tt_refresh(); _tt_delay(); }
    }
    t->heading -= 360.0f * (int)(t->heading / 360.0f);
    _tt_show();
}
/* left(90);  right(45);   turn (any number works: 1, 120, 720 ...) */
TT void left(float degrees)  { _tt_turn(degrees); }
TT void right(float degrees) { _tt_turn(-degrees); }

/* face(90);       point a direction: 0 = right, 90 = up, 180 = left, 270 = down */
TT void face(float degrees) { _tt_check(); _TT_T->heading = degrees; _tt_show(); }

/* point_to(x, y); turn to look at a spot (try point_to(mouse_x(), mouse_y())) */
TT void point_to(float x, float y) { face(_tt_atan2d(y - _TT_T->y, x - _TT_T->x)); }

/* go_to(100, 50); teleport there (never draws a line) */
TT void go_to(float x, float y) { _tt_check(); if (_tt.dragging == _tt.cur) _tt.dragging = -1; _TT_T->x = x; _TT_T->y = y; _tt_fill_add(); _tt_show(); }

/* walk_to(100, 50); turn and walk there (draws a line) */
TT void walk_to(float x, float y) {
    _tt_check();
    _TtTurtle *t = _TT_T;
    float dx = x - t->x, dy = y - t->y, dist = _tt_sqrt(dx * dx + dy * dy);
    if (dist < 0.5f) return;
    float a = _tt_atan2d(dy, dx) - t->heading;
    a -= 360.0f * (int)(a / 360.0f);
    if (a > 180) a -= 360;
    if (a < -180) a += 360;
    _tt_turn(a);
    forward(dist);
}

/* go_home();      back to the middle, facing right */
TT void go_home(void) { go_to(0, 0); face(0); }

/* xcor(), ycor(), heading()   where the turtle is and where it points */
TT float xcor(void)    { return _TT_T->x; }
TT float ycor(void)    { return _TT_T->y; }
TT float heading(void) { return _TT_T->heading; }

/* distance_to(x, y)  how far away a spot is */
TT float distance_to(float x, float y) { float dx = x - _TT_T->x, dy = y - _TT_T->y; return _tt_sqrt(dx * dx + dy * dy); }

/* ============================================================ */
/* DRAWING                                                      */
/* ============================================================ */

/* color("red");  color(RED);  color("#ff8800");   pen color (also text and the arrow) */
TT void _tt_color(unsigned long c) { _tt_check(); _TT_T->color = c; _tt_show(); }
#define color(c) _tt_color(_TT_COL(c))
/* thickness(5);   pen width */
TT void thickness(int n) { _TT_T->thick = n < 1 ? 1 : n; }
/* penup();  pendown();   stop / start drawing lines */
TT void penup(void)   { _TT_T->pen = 0; }
TT void pendown(void) { _TT_T->pen = 1; }
/* speed(1) slow ... speed(10) fast, speed(0) instant */
TT void speed(int n) { _tt.spd = n < 0 ? 0 : n; }

/* circle(50);     walk around a circle (circle(-50) bends right) */
TT void circle(float radius) {
    _tt_check();
    int old = _tt.spd;
    if (_tt.spd > 0 && _tt.spd < 8) _tt.spd = 8;
    float len = 2 * 3.14159265f * _tt_abs(radius) / 36, turn = radius < 0 ? -10.0f : 10.0f;
    for (int i = 0; i < 36; i++) { forward(len); _tt_turn(turn); }
    _tt.spd = old;
}

/* dot(20);        a filled dot where the turtle is */
TT void dot(int size) {
    _tt_check();
    XSetForeground(_tt.dpy, _tt.gc, _TT_T->color);
    XFillArc(_tt.dpy, _tt.canvas, _tt.gc, _TT_SX(_TT_T->x) - size / 2, _TT_SY(_TT_T->y) - size / 2,
             (unsigned)size, (unsigned)size, 0, 360 * 64);
    _tt_show();
}

/* box(x, y, width, height);   a filled rectangle, (x, y) = its middle
 * round_box(x, y, width, height, 12);   with round corners */
TT void round_box(float x, float y, float w, float h, int corner) {
    _tt_check();
    _tt_rrect(_tt.canvas, _TT_SX(x - w / 2), _TT_SY(y + h / 2), (int)w, (int)h, corner, _TT_T->color, 1);
    _tt_show();
}
TT void box(float x, float y, float w, float h) { round_box(x, y, w, h, 0); }
/* outline(x, y, width, height, corner);  just the border of a box */
TT void outline(float x, float y, float w, float h, int corner) {
    _tt_check();
    _tt_rrect(_tt.canvas, _TT_SX(x - w / 2), _TT_SY(y + h / 2), (int)w, (int)h, corner, _TT_T->color, 0);
    _tt_show();
}

/* begin_fill(); ...move around a shape...  end_fill();   colors in the inside */
TT void begin_fill(void) { _TT_T->filling = 1; _TT_T->nfill = 0; _tt_fill_add(); }
TT void end_fill(void) {
    _tt_check();
    _TtTurtle *t = _TT_T;
    if (t->nfill > 2) {
        XSetForeground(_tt.dpy, _tt.gc, t->color);
        XFillPolygon(_tt.dpy, _tt.canvas, _tt.gc, t->fill, t->nfill, Complex, CoordModeOrigin);
    }
    t->filling = 0; t->nfill = 0;
    _tt_show();
}

/* clear();        erase everything that was drawn */
TT void clear(void) {
    _tt_check();
    XSetForeground(_tt.dpy, _tt.gc, _tt.bg);
    XFillRectangle(_tt.dpy, _tt.canvas, _tt.gc, 0, 0, _tt.W, _tt.H);
    if (!_tt.loop_mode) _tt.ntxt = 0;
    _tt_show();
}
/* background("sky");   erase everything and change the background color */
TT void _tt_background(unsigned long c) { _tt.bg = c; clear(); }
#define background(c) _tt_background(_TT_COL(c))
/* reset();        erase everything and send the turtle home */
TT void reset(void) { _TT_T->x = 0; _TT_T->y = 0; _TT_T->heading = 0; clear(); }

/* grid();         a light grid with numbers, to help find coordinates */
TT void grid(void) {
    _tt_check();
    Display *d = _tt.dpy;
    int dark = !_tt_light(_tt.bg);
    unsigned long faint = dark ? 0x3A3A3A : 0xE6E6E6, axis = dark ? 0x777777 : 0xAAAAAA, num = dark ? 0x999999 : 0x999999;
    XSetLineAttributes(d, _tt.gc, 1, LineSolid, CapButt, JoinMiter);
    for (int v = -3000; v <= 3000; v += 50) {
        int sx = _TT_SX(v), sy = _TT_SY(v);
        XSetForeground(d, _tt.gc, v == 0 ? axis : faint);
        if (sx >= 0 && sx < _tt.W) XDrawLine(d, _tt.canvas, _tt.gc, sx, 0, sx, _tt.H);
        if (sy >= 0 && sy < _tt.H) XDrawLine(d, _tt.canvas, _tt.gc, 0, sy, _tt.W, sy);
    }
    _TtFont *f = _tt_font(11, 0);
    char s[16];
    for (int v = -3000; v <= 3000; v += 100) {
        int sx = _TT_SX(v), sy = _TT_SY(v);
        snprintf(s, sizeof s, "%d", v);
        if (sx > 0 && sx < _tt.W - 20) _tt_str(_tt.canvas, f, sx + 3, _TT_SY(0) - 4, num, s);
        if (v && sy > 10 && sy < _tt.H) _tt_str(_tt.canvas, f, _TT_SX(0) + 4, sy - 3, num, s);
    }
    _tt_show();
}
/* show_coords();  shows where the mouse is, in the corner */
TT void show_coords(void) { _tt.coords_on = 1; _tt_show(); }
TT void hide_coords(void) { _tt.coords_on = 0; _tt_show(); }

/* ============================================================ */
/* WHAT THE TURTLE LOOKS LIKE                                   */
/* The turtle is invisible until you give it a look.            */
/* ============================================================ */

/* arrow();                 show the turtle as an arrow */
TT void arrow(void) { _tt_check(); _TT_T->look = _TT_ARROW; _tt_show(); }

/* picture("cat.png");      show the turtle as a picture instead of an arrow.
 *                          It moves with forward/left/right like the arrow does.
 *                          Pictures don't draw lines, unless you call pendown(). */
TT void picture(const char *png_file) {
    _tt_check();
    _TT_T->img = _tt_image(png_file);
    _TT_T->look = _TT_PIC; _TT_T->has_pic = 1;
    _TT_T->pen = 0;
    _tt_show();
}
/* hide();  show();         make it invisible / visible again */
TT void hide(void) { _tt_check(); _TT_T->look = _TT_NONE; _tt_show(); }
TT void show(void) { _tt_check(); _TT_T->look = _TT_T->has_pic ? _TT_PIC : _TT_ARROW; _tt_show(); }
/* turtle_size(2);          twice as big (arrow or picture) */
TT void turtle_size(float s) { _TT_T->size = s; _tt_show(); }
/* flip(1);  flip(0);       mirror the picture (face the other way) */
TT void flip(int on) { _TT_T->flip = on; _tt_show(); }
/* keep_upright();          the picture moves but doesn't spin when you turn */
TT void keep_upright(void) { _TT_T->upright = 1; _tt_show(); }

/* image("cat.png", 100, 50);        draws a picture with its middle at (x, y)
 * image("cat.png", 100, 50, 2);     same, 2 times bigger (0.5 = half size)
 * It stays on the screen like a drawing - clear() or new_screen() erase it. */
TT void _tt_image4(const char *file, double x, double y, double scale) {
    _tt_check();
    if (scale <= 0) return;
    _TtImage *g = &_tt.img[_tt_image(file)];
    int w = (int)(g->w * scale) + 2, h = (int)(g->h * scale) + 2;
    int x0 = _TT_SX(x) - w / 2, y0 = _TT_SY(y) - h / 2, x1 = x0 + w, y1 = y0 + h;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > _tt.W) x1 = _tt.W;
    if (y1 > _tt.H) y1 = _tt.H;
    if (x1 <= x0 || y1 <= y0) return;
    XImage *im = XGetImage(_tt.dpy, _tt.canvas, x0, y0, (unsigned)(x1 - x0), (unsigned)(y1 - y0), AllPlanes, ZPixmap);
    if (!im) return;
    if (im->bits_per_pixel == 32) {
        _tt_blit(im, g, _tt.W / 2.0f + (float)x - x0, _tt.H / 2.0f - (float)y - y0, (float)scale, 0, 0);
        XPutImage(_tt.dpy, _tt.canvas, _tt.gc, im, 0, 0, x0, y0, (unsigned)(x1 - x0), (unsigned)(y1 - y0));
    }
    XDestroyImage(im);
    _tt_show();
}
TT void _tt_image3(const char *file, double x, double y) { _tt_image4(file, x, y, 1); }
#define image(...) _TT_PICK4(__VA_ARGS__, _tt_image4, _tt_image3, 0, 0)(__VA_ARGS__)

/* ============================================================ */
/* MORE TURTLES                                                 */
/* ============================================================ */

/* Turtle cat = new_turtle();    makes another turtle (invisible until arrow() or picture())
 * use(cat);                     now every command controls cat
 * use(0);                       back to the first turtle */
TT Turtle new_turtle(void) {
    _tt_check();
    if (_tt.nt >= _TT_MAXT) { fprintf(stderr, "crayon.h: too many turtles\n"); return _TT_MAXT - 1; }
    int i = _tt.nt++;
    _tt_new_turtle_at(i);
    _tt.cur = i;
    return i;
}
TT void use(Turtle t) { if (t >= 0 && t < _tt.nt) _tt.cur = t; }

/* touching(cat)   is the turtle you are using touching cat? */
TT int touching(Turtle other) {
    if (other < 0 || other >= _tt.nt || other == _tt.cur) return 0;
    _TtTurtle *a = _TT_T, *b = &_tt.t[other];
    if (a->look == _TT_NONE || b->look == _TT_NONE) return 0;
#define _TT_HALF(t, W, H) do { \
        if ((t)->look == _TT_ARROW) { W = H = 9 * (t)->size; } \
        else { _TtImage *g = &_tt.img[(t)->img]; W = g->w * (t)->size / 2; H = g->h * (t)->size / 2; \
               if (!(t)->upright) { float m = W > H ? W : H; W = H = m * 0.85f; } } } while (0)
    float aw, ah, bw, bh;
    _TT_HALF(a, aw, ah);
    _TT_HALF(b, bw, bh);
#undef _TT_HALF
    return _tt_abs(a->x - b->x) < aw + bw && _tt_abs(a->y - b->y) < ah + bh;
}
/* clicked()       was the turtle you are using just clicked? (inside forever)
 * mouse_on()      is the mouse on it right now?
 * draggable();    lets you drag it around with the mouse */
TT int clicked(void) { return _tt.mtapped && _tt.clicked_t == _tt.cur; }
TT int mouse_on(void) { return _tt_hit(_tt.cur, _tt.mx - _tt.W / 2.0f, _tt.H / 2.0f - _tt.my); }
TT void draggable(void) { _TT_T->drag = 1; }

/* ============================================================ */
/* TEXT                                                         */
/* ============================================================ */

/* textsize(30);   size of the next text (and buttons)
 * bold(1);        bold text on,  bold(0) off */
TT void textsize(int size) { _tt.tsize = size < 6 ? 6 : size > 300 ? 300 : size; }
TT void bold(int on) { _tt.tbold = on; }

TT void _tt_addtext(float x, float y, int center, const char *s) {
    _tt_check();
    if (_tt.ntxt >= _TT_MAXTXT) return;
    _TtText *t = &_tt.txt[_tt.ntxt++];
    snprintf(t->s, sizeof t->s, "%s", s);
    t->x = x; t->y = y; t->c = _TT_T->color; t->size = _tt.tsize; t->bold = _tt.tbold; t->center = center;
    _tt_show();
}
/* text("Hello", -300, 200);                 words at a spot (x, y = where the words start)
 * text("{red}Hi {blue}there", 0, 0);        colors inside the words
 * text("{rainbow}Rainbow!", 0, 0);          every letter a different color
 * text("Score: %d", -300, 200, score);      numbers in the words
 *   Outside forever it stays on screen.  Inside forever, call it every frame. */
TT void text(const char *fmt, double x, double y, ...) {
    char b[200]; va_list a; va_start(a, y); vsnprintf(b, sizeof b, fmt, a); va_end(a);
    _tt_addtext((float)x, (float)y, 0, b);
}
/* text_center("Title", 0, 100);   same, but (x, y) is the MIDDLE of the words */
TT void text_center(const char *fmt, double x, double y, ...) {
    char b[200]; va_list a; va_start(a, y); vsnprintf(b, sizeof b, fmt, a); va_end(a);
    _tt_addtext((float)x, (float)y, 1, b);
}
/* say("Hi!");     words where the turtle is */
TT void say(const char *fmt, ...) {
    char b[200]; va_list a; va_start(a, fmt); vsnprintf(b, sizeof b, fmt, a); va_end(a);
    _tt_addtext(_TT_T->x, _TT_T->y, 0, b);
}
/* text_width("Hello")   how wide that text is (with the current textsize) */
TT float text_width(const char *s) { _tt_check(); return (float)_tt_tagged(_tt.buf, _tt_font(_tt.tsize, _tt.tbold), 0, 0, 0, s, 0); }

/* ============================================================ */
/* BUTTONS AND LINKS                                            */
/* ============================================================ */

/* button_color("blue", "white");   color of the next buttons, and of their words
 * bar_color("navy");                color of the top bar */
TT void _tt_button_color(unsigned long c, unsigned long text_c) { _tt.btn_c = c; _tt.btn_text = text_c; }
TT void _tt_bar_color(unsigned long c) { _tt.bar_c = c; _tt_show(); }
#define button_color(c, t) _tt_button_color(_TT_COL(c), _TT_COL(t))
#define bar_color(c) _tt_bar_color(_TT_COL(c))

TT int _tt_btn(const char *label, int kind, float x, float y, TtFn fn) {
    _tt_check();
    int imm = _tt.loop_mode && !fn;
    int found = -1, spare = -1;
    for (int i = 0; i < _tt.nbtn; i++) {
        _TtButton *b = &_tt.btn[i];
        if (!b->used) { if (spare < 0) spare = i; continue; }
        if (b->imm && b->last < _tt.frame - 1 && spare < 0) spare = i;
        if (b->kind != kind || strcmp(b->label, label) != 0) continue;
        if (imm) { if (b->imm && b->last != _tt.frame) { found = i; break; } }
        else if (kind == 0 || (b->cx == x && b->cy == y)) { found = i; break; }
    }
    if (found < 0) {
        if (spare >= 0) found = spare;
        else if (_tt.nbtn < _TT_MAXB) found = _tt.nbtn++;
        else return -1;
        _TtButton *b = &_tt.btn[found];
        memset(b, 0, sizeof *b);
        snprintf(b->label, sizeof b->label, "%s", label);
        b->used = 1; b->kind = kind;
    }
    _TtButton *b = &_tt.btn[found];
    b->cx = x; b->cy = y; b->imm = imm; b->last = _tt.frame;
    b->size = kind == 2 ? _tt.tsize : (_tt.tsize > 24 ? 24 : _tt.tsize < 12 ? 12 : _tt.tsize) - 2;
    b->bold = _tt.tbold;
    if (kind == 2) b->fg = _TT_T->color; else { b->bg = _tt.btn_c; b->fg = _tt.btn_text; }
    if (fn) b->fn = fn;
    if (b->last == _tt.frame && !_tt.loop_mode) b->clicked = 0;
    _tt_show();
    return found;
}
TT int _tt_btn_clicked(int i) { return i >= 0 && _tt.btn[i].clicked; }

/* button("Play", 0, -100, play);    a button at (x, y) (its middle) - runs play() when clicked
 * button("Play", 0, -100);          a button that does nothing by itself:
 *                                   use wait_button() to wait for it,
 *                                   or inside forever: if (button("Play", 0, -100)) { ... }
 * button("Menu", menu);             a button in a bar at the top of the window */
TT void _tt_button2(const char *label, TtFn fn) { _tt_btn(label, 0, 0, 0, fn); }
TT int  _tt_button3(const char *label, double x, double y) { return _tt_btn_clicked(_tt_btn(label, 1, (float)x, (float)y, NULL)); }
TT void _tt_button4(const char *label, double x, double y, TtFn fn) { _tt_btn(label, 1, (float)x, (float)y, fn); }
#define button(...) _TT_PICK4(__VA_ARGS__, _tt_button4, _tt_button3, _tt_button2, 0)(__VA_ARGS__)

/* link("Click me", -300, 100, my_function);    clickable words (uses color(), "{red}" works too)
 * link("Click me", -300, 100);                 same, use wait_button() or forever like a button */
TT int  _tt_link3(const char *label, double x, double y) { return _tt_btn_clicked(_tt_btn(label, 2, (float)x, (float)y, NULL)); }
TT void _tt_link4(const char *label, double x, double y, TtFn fn) { _tt_btn(label, 2, (float)x, (float)y, fn); }
#define link(...) _TT_PICK4(__VA_ARGS__, _tt_link4, _tt_link3, 0, 0)(__VA_ARGS__)

/* remove_button("Go");   take a button away */
TT void remove_button(const char *label) {
    for (int i = 0; i < _tt.nbtn; i++)
        if (_tt.btn[i].used && strcmp(_tt.btn[i].label, label) == 0) _tt.btn[i].used = 0;
    _tt_show();
}

/* ============================================================ */
/* KEYS AND MOUSE                                               */
/* ============================================================ */
TT int _tt_key(int k) { return (k >= 'A' && k <= 'Z' ? k + 32 : k) & 0xFFFF; }

/* click('w')   true once when pressed       hold('w')   true while held down */
TT int click(int k) {
    if (k == MOUSE) return _tt.mtapped;
    if (k == RMOUSE) return _tt.rtapped;
    return _tt.tapped[_tt_key(k)];
}
TT int hold(int k) {
    if (k == MOUSE) return _tt.mdown;
    if (k == RMOUSE) return _tt.rdown;
    if (k == SHIFT) return _tt.down[XK_Shift_L & 0xFFFF] || _tt.down[XK_Shift_R & 0xFFFF];
    if (k == CTRL)  return _tt.down[XK_Control_L & 0xFFFF] || _tt.down[XK_Control_R & 0xFFFF];
    return _tt.down[_tt_key(k)];
}
/* mouse_x(), mouse_y()   where the mouse is        click_x(), click_y()   where you last clicked */
TT float mouse_x(void) { return _tt.mx - _tt.W / 2.0f; }
TT float mouse_y(void) { return _tt.H / 2.0f - _tt.my; }
TT float click_x(void) { return _tt.cx; }
TT float click_y(void) { return _tt.cy; }

/* onkey(UP, my_function);  onclick(my_function);   the function way (no forever needed) */
TT void onkey(int k, TtFn fn) {
    if (_tt.nkeys < 64) { _tt.keys[_tt.nkeys].key = _tt_key(k); _tt.keys[_tt.nkeys].fn = fn; _tt.nkeys++; }
}
TT void onclick(TtFn fn) { _tt.onclick_fn = fn; }

/* ============================================================ */
/* THE TEXT BOX                                                 */
/* ============================================================ */

TT void _tt_textbox(float x, float y, float width, const char *hint) {
    _tt_check();
    _tt.tb_on = 1; _tt.tb_imm = _tt.loop_mode; _tt.tb_last = _tt.frame;
    _tt.tb_x = x; _tt.tb_y = y; _tt.tb_w = width < 80 ? 80 : width;
    if (hint) snprintf(_tt.hint, sizeof _tt.hint, "%s", hint);
    _tt_show();
}
/* inputbox("Type here...", 0, 0, 400, got_it);
 *     a box you can type in, at (x, y) (its middle), 400 wide.
 *     When someone presses Enter, got_it() runs. Inside got_it() use:
 *         entry()            what they typed
 *         typed("hello")     true if they typed exactly hello
 *         command("go")      true if the first word was go     number(1) = second word as a number
 * inputbox("Type here...", 0, 0, 400);   same without a function (for forever: use entered()) */
TT void _tt_inputbox4(const char *hint, double x, double y, double w) { _tt_textbox((float)x, (float)y, (float)w, hint); }
TT void _tt_inputbox5(const char *hint, double x, double y, double w, TtFn fn) {
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    _tt_textbox((float)x, (float)y, (float)w, hint);
    _tt.loop_mode = lm;
    _tt.oninput_fn = fn;
    _tt.focus = 1;                          /* ready to type right away */
    memset(_tt.down, 0, sizeof _tt.down);
    _tt_show();
}
#define inputbox(...) _TT_PICK5(__VA_ARGS__, _tt_inputbox5, _tt_inputbox4, 0, 0, 0)(__VA_ARGS__)
/* remove_inputbox();     take the box away */
TT void remove_inputbox(void) { _tt.tb_on = 0; _tt.focus = 0; _tt.oninput_fn = NULL; _tt_show(); }
/* set_input("hello");    put words in the box */
TT void set_input(const char *s) { snprintf(_tt.input, sizeof _tt.input, "%s", s); _tt_show(); }
/* input_text()           what is in the box (or what was just sent with Enter) */
TT const char *input_text(void) { return _tt.input[0] ? _tt.input : _tt.entry; }
/* typing()                what is in the text box right now (before Enter) */
TT const char *typing(void) { return _tt.input; }
/* message("Saved!");      a little grey note inside the text box */
TT void message(const char *fmt, ...) {
    va_list a; va_start(a, fmt); vsnprintf(_tt.msg, sizeof _tt.msg, fmt, a); va_end(a);
    _tt_show();
}

/* if (entered()) ...          they pressed Enter          entry()   what they sent */
TT int entered(void) { return _tt.submitted; }
TT const char *entry(void) { return _tt.entry; }

/* word(0) = first word, word(1) = second word ...   number(1) = second word as a number
 * rest(1) = everything from the second word on */
TT const char *word(int n) {
    static char out[8][128];
    static int k = 0;
    char *o = out[k++ & 7];
    o[0] = 0;
    const char *s = _tt.entry;
    for (int i = 0; ; i++) {
        while (*s == ' ') s++;
        if (!*s) return o;
        const char *e = s;
        while (*e && *e != ' ') e++;
        if (i == n) {
            int len = (int)(e - s) < 127 ? (int)(e - s) : 127;
            memcpy(o, s, (size_t)len); o[len] = 0;
            return o;
        }
        s = e;
    }
}
TT float number(int n) { return (float)atof(word(n)); }
TT const char *rest(int n) {
    const char *s = _tt.entry;
    for (int i = 0; i < n; i++) { while (*s == ' ') s++; while (*s && *s != ' ') s++; }
    while (*s == ' ') s++;
    return s;
}
/* typed("hello")    they typed exactly that       command("go")   the first word was "go" */
TT int typed(const char *s) { return _tt.submitted && _tt_ieq(_tt.entry, s); }
TT int command(const char *s) { return _tt.submitted && _tt_ieq(word(0), s); }
/* oninput(my_function);   the function way (no forever needed) */
TT void oninput(TtFn fn) { _tt.oninput_fn = fn; }

/* ============================================================ */
/* THE CONSOLE  -  console();  then type turtle commands        */
/* ============================================================ */
TT int run(const char *line);
TT int _tt_run1(const char *line) {
    char saved[200];
    memcpy(saved, _tt.entry, sizeof saved);
    snprintf(_tt.entry, sizeof _tt.entry, "%s", line);
    const char *c = word(0);
    int ok = 1, cok;
    unsigned long col;
#define _TT_IS(a) _tt_ieq(c, a)
#define _TT_N(i, def) (word(i)[0] ? number(i) : (def))
    if (!c[0]) ;
    else if (_TT_IS("forward") || _TT_IS("fd")) forward(_TT_N(1, 50));
    else if (_TT_IS("back") || _TT_IS("bk"))    back(_TT_N(1, 50));
    else if (_TT_IS("left") || _TT_IS("lt"))    left(_TT_N(1, 90));
    else if (_TT_IS("right") || _TT_IS("rt"))   right(_TT_N(1, 90));
    else if (_TT_IS("face"))                    face(number(1));
    else if (_TT_IS("goto") || _TT_IS("go_to") || _TT_IS("jump")) go_to(number(1), number(2));
    else if (_TT_IS("walk") || _TT_IS("walk_to")) walk_to(number(1), number(2));
    else if (_TT_IS("home"))                    go_home();
    else if (_TT_IS("circle"))                  circle(_TT_N(1, 50));
    else if (_TT_IS("dot"))                     dot((int)_TT_N(1, 15));
    else if (_TT_IS("color") || _TT_IS("colour")) { col = _tt_color_name(word(1), &cok); if (cok) color(col); else ok = 0; }
    else if (_TT_IS("bg") || _TT_IS("background")) { col = _tt_color_name(word(1), &cok); if (cok) background(col); else ok = 0; }
    else if (_TT_IS("pen"))                     { if (_tt_ieq(word(1), "up")) penup(); else pendown(); }
    else if (_TT_IS("penup"))                   penup();
    else if (_TT_IS("pendown"))                 pendown();
    else if (_TT_IS("thick") || _TT_IS("thickness")) thickness((int)number(1));
    else if (_TT_IS("speed"))                   speed((int)number(1));
    else if (_TT_IS("fill"))                    { if (_tt_ieq(word(1), "end")) end_fill(); else begin_fill(); }
    else if (_TT_IS("say"))                     say("%s", rest(1));
    else if (_TT_IS("text"))                    text("%s", number(1), number(2), rest(3));
    else if (_TT_IS("textsize"))                textsize((int)number(1));
    else if (_TT_IS("clear"))                   clear();
    else if (_TT_IS("reset"))                   reset();
    else if (_TT_IS("arrow"))                   arrow();
    else if (_TT_IS("picture"))                 { if (word(1)[0]) picture(word(1)); else ok = 0; }
    else if (_TT_IS("hide"))                    hide();
    else if (_TT_IS("size"))                    turtle_size(_TT_N(1, 1));
    else if (_TT_IS("turtle"))                  { new_turtle(); arrow(); }
    else if (_TT_IS("use"))                     use((int)number(1));
    else if (_TT_IS("grid"))                    grid();
    else if (_TT_IS("coords"))                  { if (_tt.coords_on) hide_coords(); else show_coords(); }
    else if (_TT_IS("title"))                   title("%s", rest(1));
    else if (_TT_IS("button")) {
        char lbl[96]; snprintf(lbl, sizeof lbl, "%s", word(1));
        if (!lbl[0]) ok = 0;
        else {
            int lm = _tt.loop_mode; _tt.loop_mode = 0;            /* keep this button */
            int i = _tt_btn(lbl, 0, 0, 0, NULL);
            _tt.loop_mode = lm;
            if (i >= 0) snprintf(_tt.btn[i].cmd, sizeof _tt.btn[i].cmd, "%s", rest(2));
        }
    }
    else if (_TT_IS("remove"))                  remove_button(word(1));
    else if (_TT_IS("help")) {
        printf("Commands (use ; for more than one, and \"repeat 4 ...\" to repeat):\n"
               "  forward 100  back 50  left 90  right 90  face 90  goto x y  walk x y  home\n"
               "  circle 50  dot 20  color red  bg yellow  pen up  pen down  thick 5  speed 0-10\n"
               "  fill ... fill end  say hello  text x y hello  textsize 30  clear  reset\n"
               "  arrow  picture cat.png  hide  size 2  turtle  use 0  grid  coords  title Hi\n"
               "  button NAME commands...   remove NAME\n"
               "Example:  button Square repeat 4 forward 80; left 90\n");
        fflush(stdout);
        message("commands are printed in the terminal");
    }
    else ok = 0;
#undef _TT_IS
#undef _TT_N
    memcpy(_tt.entry, saved, sizeof saved);
    return ok;
}

/* run("repeat 4 forward 80; left 90");   run console commands from your code */
TT int run(const char *line) {
    _tt_check();
    while (*line == ' ') line++;
    if (!*line) return 1;
    char w[16]; int n = 0;
    while (line[n] && line[n] != ' ' && n < 15) { w[n] = line[n]; n++; }
    w[n] = 0;
    if (_tt_ieq(w, "repeat")) {
        const char *p = line + n;
        int times = atoi(p), ok = 1;
        while (*p == ' ') p++;
        while (*p && *p != ' ') p++;
        for (int i = 0; i < times && ok && !_tt.closed; i++) ok = run(p);
        return ok;
    }
    const char *semi = _tt_ieq(w, "button") ? NULL : strchr(line, ';');
    if (semi) {
        char first[200];
        int len = (int)(semi - line) < 199 ? (int)(semi - line) : 199;
        memcpy(first, line, (size_t)len); first[len] = 0;
        int a = run(first);
        return run(semi + 1) && a;
    }
    char one[200];
    snprintf(one, sizeof one, "%s", line);
    for (int i = (int)strlen(one) - 1; i >= 0 && one[i] == ' '; i--) one[i] = 0;
    return _tt_run1(one);
}

/* console();   a text box at the bottom that understands turtle commands (type help) */
TT void console(void) {
    _tt_check();
    _tt.console_on = 1;
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    _tt_textbox(0, -_tt.H / 2.0f + _TT_TBH / 2.0f + 8, _tt.W - 16.0f, "Type a command (try: forward 100, color red, help)");
    _tt.loop_mode = lm;
}

TT void _tt_submit(void) {
    snprintf(_tt.entry, sizeof _tt.entry, "%s", _tt.input);
    _tt.input[0] = 0;
    _tt.submitted = 1;
    _tt.sent++;
    _tt.msg[0] = 0;
    if (_tt.console_on && _tt.entry[0]) {
        char cmd[200]; snprintf(cmd, sizeof cmd, "%s", _tt.entry);
        if (run(cmd)) { if (!_tt.msg[0]) snprintf(_tt.msg, sizeof _tt.msg, "> %.100s", cmd); }
        else snprintf(_tt.msg, sizeof _tt.msg, "? \"%.40s\" - type help", word(0));
    }
    if (_tt.oninput_fn) _tt.oninput_fn();
    if (!_tt.loop_mode) _tt.submitted = 0;
}

/* ============================================================ */
/* THE EVENT LOOP                                               */
/* ============================================================ */
TT void _tt_set_cursor(int c) {
    if (c == _tt.cursor_now) return;
    _tt.cursor_now = c;
    XDefineCursor(_tt.dpy, _tt.win, _tt.cursors[c]);
}
TT int _tt_in_tb(int mx, int my) {
    int x, y, w;
    return _tt_tb_px(&x, &y, &w) && mx >= x && mx < x + w && my >= y && my < y + _TT_TBH;
}
TT int _tt_btn_under(int mx, int my) {
    for (int i = _tt.nbtn - 1; i >= 0; i--) {
        _TtButton *b = &_tt.btn[i];
        if (_tt_btn_visible(b) && mx >= b->x && mx < b->x + b->w && my >= b->y && my < b->y + b->h) return i;
    }
    return -1;
}

TT int _tt_handle(XEvent *e) {
    switch (e->type) {
    case Expose:
        _tt_refresh();
        break;
    case MotionNotify:
        _tt.mx = e->xmotion.x; _tt.my = e->xmotion.y;
        if (_tt.dragging >= 0) {
            _tt.t[_tt.dragging].x = mouse_x() - _tt.dragdx;
            _tt.t[_tt.dragging].y = mouse_y() - _tt.dragdy;
        }
        _tt_set_cursor(_tt_btn_under(_tt.mx, _tt.my) >= 0 ? 1 : _tt_in_tb(_tt.mx, _tt.my) ? 2 : 0);
        if (!_tt.loop_mode) _tt_refresh();
        break;
    case ButtonPress: {
        int mx = e->xbutton.x, my = e->xbutton.y;
        _tt.mx = mx; _tt.my = my;
        if (e->xbutton.button == 3) { _tt.rdown = 1; _tt.rtapped = 1; break; }
        if (e->xbutton.button != 1) break;
        _tt_layout();
        if (_tt_in_tb(mx, my)) {
            if (!_tt.focus) memset(_tt.down, 0, sizeof _tt.down);
            _tt.focus = 1;
            if (!_tt.loop_mode) _tt_refresh();
            break;
        }
        _tt.focus = 0;
        int bi = _tt_btn_under(mx, my);
        if (bi >= 0) {
            _TtButton *b = &_tt.btn[bi];
            b->clicked = 1;
            _tt.last_clicked = bi;
            snprintf(_tt.last_label, sizeof _tt.last_label, "%s", b->label);
            TtFn fn = b->fn;
            char cmd[200]; snprintf(cmd, sizeof cmd, "%s", b->cmd);
            if (fn) fn();
            if (cmd[0]) run(cmd);
            if (!_tt.loop_mode) { _tt.btn[bi].clicked = 0; _tt_refresh(); }
            break;
        }
        if (_tt.nbar && my < _TT_BARH) break;
        _tt.mdown = 1; _tt.mtapped = 1;
        _tt.cx = mx - _tt.W / 2.0f; _tt.cy = _tt.H / 2.0f - my;
        _tt.clicked_t = _tt_turtle_at(_tt.cx, _tt.cy);
        if (_tt.clicked_t >= 0 && _tt.t[_tt.clicked_t].drag) {
            _tt.dragging = _tt.clicked_t;
            _tt.dragdx = _tt.cx - _tt.t[_tt.dragging].x;
            _tt.dragdy = _tt.cy - _tt.t[_tt.dragging].y;
        }
        if (_tt.onclick_fn) _tt.onclick_fn();
        if (!_tt.loop_mode) _tt_refresh();
        break;
    }
    case ButtonRelease:
        if (e->xbutton.button == 1) { _tt.mdown = 0; _tt.dragging = -1; }
        if (e->xbutton.button == 3) _tt.rdown = 0;
        break;
    case KeyPress: {
        KeySym ks = XLookupKeysym(&e->xkey, 0);
        if (_tt.focus && !_tt_tb_visible()) _tt.focus = 0;
        if (_tt.focus) {
            char chars[16] = {0}; KeySym sym;
            int n = XLookupString(&e->xkey, chars, sizeof chars - 1, &sym, NULL);
            int len = (int)strlen(_tt.input);
            if (ks == XK_Return || ks == XK_KP_Enter) _tt_submit();
            else if (ks == XK_Escape) _tt.focus = 0;
            else if (ks == XK_BackSpace) { if (len) _tt.input[len - 1] = 0; }
            else if ((e->xkey.state & ControlMask) && (ks == 'u' || ks == 'l')) _tt.input[0] = 0;
            else for (int i = 0; i < n; i++)
                if (chars[i] >= 32 && chars[i] < 127 && len < (int)sizeof _tt.input - 1) { _tt.input[len++] = chars[i]; _tt.input[len] = 0; }
            if (!_tt.loop_mode) _tt_refresh();
            break;
        }
        if ((ks == XK_Return || ks == XK_KP_Enter) && _tt_tb_visible()) {
            _tt.focus = 1;
            memset(_tt.down, 0, sizeof _tt.down);
            if (!_tt.loop_mode) _tt_refresh();
            break;
        }
        int k = (int)(ks & 0xFFFF);
        _tt.down[k] = 1; _tt.tapped[k] = 1;
        for (int i = 0; i < _tt.nkeys; i++) if (_tt.keys[i].key == k) _tt.keys[i].fn();
        break;
    }
    case KeyRelease:
        _tt.down[XLookupKeysym(&e->xkey, 0) & 0xFFFF] = 0;
        break;
    case ClientMessage:
        if ((Atom)e->xclient.data.l[0] == _tt.del) {
            XCloseDisplay(_tt.dpy);
            _tt.dpy = NULL; _tt.closed = 1;
            return 0;
        }
        break;
    }
    return 1;
}

/* used by `forever` */
TT int update(void) {
    if (_tt.closed) return 0;
    _tt_check();
    _tt.loop_mode = 1;
    _tt_refresh();
    _tt.ntxt = 0;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long us = (now.tv_sec - _tt.last.tv_sec) * 1000000L + (now.tv_nsec - _tt.last.tv_nsec) / 1000L;
    if (us < 16666) { struct timespec t = { 0, (16666 - us) * 1000L }; nanosleep(&t, NULL); }
    clock_gettime(CLOCK_MONOTONIC, &_tt.last);

    memset(_tt.tapped, 0, sizeof _tt.tapped);
    _tt.mtapped = _tt.rtapped = _tt.submitted = 0;
    _tt.clicked_t = -1;
    for (int i = 0; i < _tt.nbtn; i++) _tt.btn[i].clicked = 0;
    while (_tt.dpy && XPending(_tt.dpy)) {
        XEvent e; XNextEvent(_tt.dpy, &e);
        if (!_tt_handle(&e)) return 0;
    }
    if (_tt.focus && !_tt_tb_visible()) _tt.focus = 0;
    _tt.frame++;
    return 1;
}

/* done();   wait for buttons, keys and clicks (you only need this if you use onkey/onclick
 *           style functions in the middle of a program - the window stays open anyway) */
TT void done(void) {
    if (_tt.closed || !_tt.dpy) return;
    _tt.loop_mode = 0;
    _tt_refresh();
    for (;;) {
        while (_tt.dpy && !XPending(_tt.dpy) && _tt.focus) {      /* blink the text cursor */
            struct timespec t = { 0, 50000000 }; nanosleep(&t, NULL);
            _tt_refresh();
        }
        XEvent e;
        XNextEvent(_tt.dpy, &e);
        if (!_tt_handle(&e)) return;
    }
}

/* ============================================================ */
/* THE EASY WAY: ask, wait, pick  (no loops needed)             */
/* ============================================================ */

TT void _tt_wait_tick(void) {
    while (_tt.dpy && XPending(_tt.dpy)) {
        XEvent e; XNextEvent(_tt.dpy, &e);
        if (!_tt_handle(&e)) exit(0);
    }
    if (_tt.closed) exit(0);
    _tt_refresh();
    struct timespec t = { 0, 15000000 }; nanosleep(&t, NULL);
}

/* str("Hi %s, you are %d", name, age)   makes a piece of text (like Python f-strings) */
TT const char *str(const char *fmt, ...) {
    static char out[16][256];
    static int k = 0;
    char *o = out[k++ & 15];
    va_list a; va_start(a, fmt); vsnprintf(o, 256, fmt, a); va_end(a);
    return o;
}

/* same(a, "yes")   true if two texts are the same (CAPS don't matter) */
TT int same(const char *a, const char *b) { return _tt_ieq(a, b); }

/* const char *name = userinput("What's your name?");
 *   shows a text box at the bottom, WAITS until you type and press Enter, gives back what you typed
 * userinput_at("Search...", 0, 50);   same, but the box is at (x, y)
 * numberinput("How old are you?")     same, but gives back a number */
TT const char *userinput_at(const char *question, double x, double y) {
    _tt_check();
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    float w = _tt.W - 40.0f;
    if (w > 580) w = 580;
    int had = _tt.tb_on; float ox = _tt.tb_x, oy = _tt.tb_y, ow = _tt.tb_w; TtFn ofn = _tt.oninput_fn;
    char ohint[120]; snprintf(ohint, sizeof ohint, "%s", _tt.hint);
    _tt.oninput_fn = NULL;
    _tt_textbox((float)x, (float)y, w, question);
    _tt.input[0] = 0;
    _tt.focus = 1;
    memset(_tt.down, 0, sizeof _tt.down);
    int before = _tt.sent;
    while (_tt.sent == before) _tt_wait_tick();
    _tt.focus = 0;
    _tt.tb_on = had; _tt.tb_x = ox; _tt.tb_y = oy; _tt.tb_w = ow; _tt.oninput_fn = ofn;
    snprintf(_tt.hint, sizeof _tt.hint, "%s", ohint);
    _tt.loop_mode = lm;
    _tt_refresh();
    return str("%s", _tt.entry);
}
TT const char *userinput(const char *question) { return userinput_at(question, 0, -_tt.H / 2.0f + _TT_TBH / 2.0f + 14); }
TT float numberinput(const char *question) { return (float)atof(userinput(question)); }

/* pick("Yes", "No")                   shows buttons at the bottom, WAITS for a click,
 *                                     gives back 1 for the first button, 2 for the second ...
 * pick_at(0, -100, "Play", "Quit")    the buttons in a row, around (x, y)
 * pick_list(-300, 100, "a", "b", "c") clickable words in a list going down (uses color()) */
TT int _tt_pick(float x, float y, int kind, const char *first, ...) {
    _tt_check();
    const char *labels[16];
    int n = 0, ids[16];
    va_list a; va_start(a, first);
    for (const char *l = first; l && n < 16; l = va_arg(a, const char *)) labels[n++] = l;
    va_end(a);
    if (!n) return 0;
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    if (y < -99999) y = -_tt.H / 2.0f + 40;
    if (kind == 1) {                                   /* a row of buttons, centered on x */
        int size = (_tt.tsize > 24 ? 24 : _tt.tsize < 12 ? 12 : _tt.tsize) - 2;
        _TtFont *f = _tt_font(size, 1);
        float total = 0;
        for (int i = 0; i < n; i++) total += _tt_tw(f, labels[i]) + 28 + (i ? 14 : 0);
        float bx = x - total / 2;
        for (int i = 0; i < n; i++) {
            float w = (float)_tt_tw(f, labels[i]) + 28;
            ids[i] = _tt_btn(labels[i], 1, bx + w / 2, y, NULL);
            bx += w + 14;
        }
    } else {                                           /* a list of links going down */
        float gap = _tt.tsize * 1.9f;
        for (int i = 0; i < n; i++) ids[i] = _tt_btn(labels[i], 2, x, y - i * gap, NULL);
    }
    int got = 0;
    _tt.last_clicked = -1;
    while (!got) {
        _tt_wait_tick();
        for (int i = 0; i < n; i++) if (ids[i] >= 0 && _tt.last_clicked == ids[i]) got = i + 1;
    }
    for (int i = 0; i < n; i++) if (ids[i] >= 0) _tt.btn[ids[i]].used = 0;
    _tt.loop_mode = lm;
    _tt_refresh();
    return got;
}
#define pick(...)            _tt_pick(0, -999999, 1, __VA_ARGS__, (const char *)NULL)
#define pick_at(x, y, ...)   _tt_pick((x), (y), 1, __VA_ARGS__, (const char *)NULL)
#define pick_list(x, y, ...) _tt_pick((x), (y), 2, __VA_ARGS__, (const char *)NULL)

/* wait_click();   waits until you click somewhere (then use click_x(), click_y()) */
TT void wait_click(void) {
    _tt_check();
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    _tt.mtapped = 0;
    while (!_tt.mtapped) _tt_wait_tick();
    _tt.mtapped = 0;
    _tt.loop_mode = lm;
}

/* int k = wait_key();   waits for a key, gives it back:  if (wait_key() == 'y') ... */
TT int wait_key(void) {
    _tt_check();
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    memset(_tt.tapped, 0, sizeof _tt.tapped);
    for (;;) {
        _tt_wait_tick();
        for (int k = 0; k < 65536; k++)
            if (_tt.tapped[k]) { memset(_tt.tapped, 0, sizeof _tt.tapped); _tt.loop_mode = lm; return k; }
    }
}

/* const char *b = wait_button();   waits until ANY button or link is clicked, gives back its words
 *   if (same(wait_button(), "Yes")) ... */
TT const char *wait_button(void) {
    _tt_check();
    int lm = _tt.loop_mode; _tt.loop_mode = 0;
    _tt.last_clicked = -1;
    while (_tt.last_clicked < 0) _tt_wait_tick();
    _tt.loop_mode = lm;
    return str("%s", _tt.last_label);
}

/* clicked_text()    the words on the button or link that was clicked last
 *                   (handy when many links run the same function) */
TT const char *clicked_text(void) { return _tt.last_label; }

/* new_screen();     wipes EVERYTHING: drawings, text, buttons, links, the input box,
 *                   and hides all turtles. Great for going to a new "page". */
TT void new_screen(void) {
    _tt_check();
    for (int i = 0; i < _tt.nbtn; i++) _tt.btn[i].used = 0;
    _tt.tb_on = 0; _tt.focus = 0; _tt.oninput_fn = NULL; _tt.msg[0] = 0;
    for (int i = 0; i < _tt.nt; i++) _tt.t[i].look = _TT_NONE;
    _tt.dragging = -1;
    _tt.ntxt = 0;
    XSetForeground(_tt.dpy, _tt.gc, _tt.bg);
    XFillRectangle(_tt.dpy, _tt.canvas, _tt.gc, 0, 0, _tt.W, _tt.H);
    _tt_show();
}

/* copy(name, "hello");   puts text into a char array:  char name[100]; copy(name, entry()); */
#define copy(dst, src) snprintf((dst), sizeof(dst), "%s", (src))

/* rnd(1, 6)       a random whole number from 1 to 6 */
TT int rnd(int lo, int hi) {
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL) ^ (unsigned)clock()); seeded = 1; }
    if (hi < lo) { int t = lo; lo = hi; hi = t; }
    return lo + rand() % (hi - lo + 1);
}
/* wait_for(0.5);  wait half a second */
TT void wait_for(float seconds) {
    _tt_check();
    struct timespec end, now;
    clock_gettime(CLOCK_MONOTONIC, &end);
    long ns = (long)(seconds * 1e9f);
    end.tv_sec += ns / 1000000000L; end.tv_nsec += ns % 1000000000L;
    if (end.tv_nsec >= 1000000000L) { end.tv_sec++; end.tv_nsec -= 1000000000L; }
    for (;;) {
        while (_tt.dpy && XPending(_tt.dpy)) { XEvent e; XNextEvent(_tt.dpy, &e); if (!_tt_handle(&e)) exit(0); }
        _tt_refresh();
        clock_gettime(CLOCK_MONOTONIC, &now);
        if (now.tv_sec > end.tv_sec || (now.tv_sec == end.tv_sec && now.tv_nsec >= end.tv_nsec)) break;
        struct timespec t = { 0, 10000000 }; nanosleep(&t, NULL);
    }
}
/* frames()        how many frames have gone by (60 a second) */
TT long frames(void) { return _tt.frame; }

/* dark_mode();    dark background, bar, buttons and text box */
TT void dark_mode(void) {
    _tt_check();
    _tt.bar_c = 0x1E1E1E;
    _tt.btn_c = 0x303134; _tt.btn_text = 0xE8EAED;
    _tt.box_c = 0x303134; _tt.box_text = 0xE8EAED; _tt.box_hint = 0x9AA0A6; _tt.box_line = 0x5F6368;
    for (int i = 0; i < _tt.nt; i++) if (_tt.t[i].color == BLACK) _tt.t[i].color = WHITE;
    background(0x202124);
}

#endif
