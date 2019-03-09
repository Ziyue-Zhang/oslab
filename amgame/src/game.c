#include <game.h>
#include <klib.h>

void init_screen();
void splash();
void read_keyy();
void clean();
void display();
int color, mw, mh, x, y, px, py;
void draw_rectt(int x, int y, int w, int h, uint32_t color) ;
int main() {
  // Operating system is a C program
  _ioe_init();
  init_screen();
  color=0xffffff;
  //splash();
  display();
  while (1) {
    read_keyy();
     //splash();
     display();
  }
  return 0;
}

void read_keyy() {
  _DEV_INPUT_KBD_t event = { .keycode = _KEY_NONE };
  #define KEYNAME(key) \
    [_KEY_##key] = #key,
  static const char *key_names[] = {
    _KEYS(KEYNAME)
  };
  _io_read(_DEV_INPUT, _DEVREG_INPUT_KBD, &event, sizeof(event));
  if (event.keycode != _KEY_NONE && event.keydown) {
    /*if(strcmp(key_names[event.keycode], "1") == 0) {
        color=(color+0xf)%(0xffffff+1);
        splash();
        }
    else if(strcmp(key_names[event.keycode], "2") == 0){
        color=(color-0xff00)%(0xffffff+1);
        splash();
        }
    else if(strcmp(key_names[event.keycode], "3") == 0) {
        color=(color+0xff00)%(0xffffff+1);
        splash();
    }*/
    if(strcmp(key_names[event.keycode], "UP") == 0) {
        if(py > 0)
        py--;
        clean();
        display();
    }
    else if(strcmp(key_names[event.keycode], "LEFT") == 0) {
        if(px > 0)
        px--;
        clean();
        display();
    }
    else if(strcmp(key_names[event.keycode], "RIGHT") == 0) {
        if(px < mw - 1)
        px++;
        clean();
        display();
    }
    else if(strcmp(key_names[event.keycode], "DOWN") == 0) {
        if(py < mh - 1)
        py++;
        clean();
        display();
    }
    else if(strcmp(key_names[event.keycode], "SPACE") == 0) {
        clean();
    }   
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }
}

int w, h;

void init_screen() {
  _DEV_VIDEO_INFO_t info = {0};
  _io_read(_DEV_VIDEO, _DEVREG_VIDEO_INFO, &info, sizeof(info));
  w = info.width;
  h = info.height;
  mw = w / SIDE;
  mh = h / SIDE;
  px = 0;
  py = 0;
}

void draw_rectt(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: allocated on stack
  _DEV_VIDEO_FBCTL_t event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  _io_write(_DEV_VIDEO, _DEVREG_VIDEO_FBCTL, &event, sizeof(event));
}

void splash() {
  for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if ((x & 1) ^ (y & 1)) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, color); // white
      }
    }
  }
}
void clean() {
    for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0x000000);
    }
  }
}
void display() {
    for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if (x == px && y == py) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffff00);
      }
    }
  }
}
