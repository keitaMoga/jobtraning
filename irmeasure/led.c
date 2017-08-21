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
#include "led.h"

#define GPIO_DIR_LED1 "/sys/class/gpio/CON9_1/"
#define GPIO_DIR_LED2 "/sys/class/gpio/CON9_2/"

void led_set_value(enum led_type type, enum led_light_mode mode)
{
        char *dir_path;
        char *value = (mode == LED_TURN_ON) ? "0" : "1";
        int fd;

        if (type == LED_01)
                dir_path = GPIO_DIR_LED1 "value";
        else
                dir_path = GPIO_DIR_LED2 "value";

        fd = open(dir_path, O_RDWR);
        write(fd, value, 1);
        close(fd);
}

void led_init(void)
{
        int fd;

        fd = open(GPIO_DIR_LED1 "direction", O_RDWR);
        write(fd, "out", 3);
        close(fd);

        fd = open(GPIO_DIR_LED2 "direction", O_RDWR);
        write(fd, "out", 3);
        close(fd);
}
