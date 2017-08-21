/* Copyright (c) 2017 Atmark Techno, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "button.h"

#define GPIO_DIR_SW1 "/sys/class/gpio/CON9_11/"
#define GPIO_DIR_SW2 "/sys/class/gpio/CON9_12/"

static char button_get_value(enum sw_type type);

int is_pressed(enum sw_type type)
{
  if (button_get_value(type) == '0')
    return 1;
  else
    return 0;
}

static char button_get_value(enum sw_type type)
{
        int fd;
  char *dir_path;
        char val;

        if (type == SW_01)
                dir_path = GPIO_DIR_SW1 "value";
  else
    dir_path = GPIO_DIR_SW2 "value";

  fd = open(dir_path, O_RDWR);
        read(fd, &val, 1);
        close(fd);

        return val;
}

void button_init(void)
{
        int fd;
        fd = open(GPIO_DIR_SW1 "direction", O_RDWR);
        write(fd, "in", 2);
        close(fd);

        fd = open(GPIO_DIR_SW2 "direction", O_RDWR);
        write(fd, "in", 2);
        close(fd);
}

#ifdef __STANDALONE__
int main(void)
{
        int flag_sw1;
        int flag_sw2;

        button_init();

        flag_sw1 = is_pressed(SW_01);
        flag_sw2 = is_pressed(SW_02);
        printf("SW1 is %spressed\n", !flag_sw1 ? "not" : "");
        printf("SW2 is %spressed\n", !flag_sw2 ? "not" : "");

        return 0;
}
#endif
