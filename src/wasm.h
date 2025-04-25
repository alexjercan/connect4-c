#ifndef WASM_H
#define WASM_H

extern int js_width();
extern int js_height();
extern int js_random(int a, int b);

extern void js_clear_canvas();
extern void js_draw_rect(int x, int y, int w, int h, int color);
extern void js_draw_circle(int x, int y, int r, int color);
extern void js_log_message(const char *message, int length);

extern int js_canvas_clicked_x();

#endif // WASM_H
