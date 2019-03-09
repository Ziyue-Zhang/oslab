#include <game.h>
#include <klib.h>

void init_screen();
void splash();
void read_keyy();
int color;
void draw_rectt(int x, int y, int w, int h, uint32_t color) ;
int main() {
  // Operating system is a C program
  _ioe_init();
  init_screen();
  color=0xff00ff;
  splash();
  while (1) {
    read_keyy();
     splash();
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
    if(strcmp(key_names[event.keycode], "UP") == 0)
        color=(color+0xf)%(0xffffff+1);
    if(strcmp(key_names[event.keycode], "LEFT") == 0)
        color=(color-0xff00)%(0xffffff+1);
    if(strcmp(key_names[event.keycode], "RIGHT") == 0)
        color=(color+0xff00)%(0xffffff+1);
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
  draw_rectt(0,0,w/2,h/2,0xffffff);
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
