#include <game.h>
#include <klib.h>
#define FPS 20
void init_screen();
void splash();
void read_keyy();
void clean();
void display();
void reset();
int color, mw, mh, x, y, px, py, rx, ry, x1, y1, x2, y2;
void draw_rectt(int x, int y, int w, int h, uint32_t color) ;
int main() {
  // Operating system is a C program
  _ioe_init();
  init_screen();
  color=0xffffff;
  //splash();
  display();
  /*while (1) {
    read_keyy();
     //splash();
     //display();
     if(px == rx && py == ry) {
        printf("You win!\n");
        reset();
     }
  }*/
  int next_frame = 0;
  while (1) {
  while (uptime() < next_frame) ; // 等待一帧的到来
  read_keyy();
  if(px == rx && py == ry) {
      printf("You win!\n");
      reset();
  }
  else if((px == x1 && py == y1) || (px == x2 && py == y2)) {
      printf("You lost!\n");
      reset();
  }
  //printf("1fps\n");
  next_frame += 1000 / FPS; // 计算下一帧的时间
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
    if(strcmp(key_names[event.keycode], "1") == 0) {
        color=(color+0xf)%(0xffffff+1);
        //splash();
        clean();
        display();
        }
    else if(strcmp(key_names[event.keycode], "2") == 0){
        color=(color-0xff00)%(0xffffff+1);
        //splash();
        clean();
        display();
        }
    else if(strcmp(key_names[event.keycode], "3") == 0) {
        color=(color+0xff00)%(0xffffff+1);
        //splash();
        clean();
        display();
    }
    else if(strcmp(key_names[event.keycode], "UP") == 0) {
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
  while(!rx && !ry) {
    rx = rand() % mw;
    ry = rand() % mh;
  }
  while((!x1 && !y1) || ((x1 == rx) && (y1 == ry))) {
    x1 = rand() % mw;
    y1 = rand() % mh;
  }
  while((!x2 && !y2) || ((x2 == rx) && (y2 == ry)) || ((x2 == x1) && (y2 == y1))) {
    x2 = rand() % mw;
    y2 = rand() % mh;
  }
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
      if (x == rx && y == ry) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xff0000);
	  }
      else if (x == x1 && y == y1) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0x00ff00);
      }
      else if (x == x2 && y == y2) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0x0000ff);
      }
      else if (x == px && y == py) {
        //draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
		draw_rectt(x * SIDE, y * SIDE, SIDE, SIDE, color);
      }
    }
  }
}
void reset() {
    px=py=0;
    clean();
    rx = rand() % mw;
    ry = rand() % mh;
    while(!rx && !ry) {
        rx = rand() % mw;
        ry = rand() % mh;
    }
    while((!x1 && !y1) || ((x1 == rx) && (y1 == ry))) {
    x1 = rand() % mw;
    y1 = rand() % mh;
    }
    while((!x2 && !y2) || ((x2 == rx) && (y2 == ry)) || ((x2 == x1) && (y2 == y1))) {
    x2 = rand() % mw;
    y2 = rand() % mh;
    }
    display();
}
