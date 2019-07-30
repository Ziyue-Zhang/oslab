#include<vfs.h>
extern ssize_t tty_write();
void shell_task(void *name){
  device_t *tty = dev_lookup(name);
  while(1){
    char line[128], text[128];
    sprintf(text, "(%s) $ ", name); tty_write(tty, 0, text, strlen(text));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    sprintf(text, "Echo: %s.\n", line); tty_write(tty, 0, text,strlen(text));
  }
}
void tty_task(void *name){
  device_t *tty = dev_lookup(name);
  char line[128], text[128];
  while(1){
    sprintf(text, "(%s) $ ", name); 
    tty->ops->write(tty, 0, text, strlen(text));
    int nread = tty->ops->read(tty, 0 ,line, sizeof(line));
    line[nread - 1] = '\0';
    sprintf(text, "Echo: %s.\n", line); tty_write(tty, 0, text,strlen(text));
  }
}
/*void shell_thread(int tty_id) {
  char buf[128];
  sprintf(buf, "/dev/tty%d", tty_id);
  int stdin = vfs->open(buf, O_RDONLY);
  int stdout = vfs->open(buf, O_WRONLY);
  while (1) {
    if (got_a_line()) {
    } else {
      ssize_t nread = fs->read(stdin, buf, sizeof(buf));
    }
  }
}*/