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
#include "lcd.h"
#include "led.h"
#include "ir_sensor.h"

static void led_status_start(void)
{
        led_set_value(LED_01, LED_TURN_ON);
        led_set_value(LED_02, LED_TURN_OFF);
}

static void led_status_stop(void)
{
        led_set_value(LED_01, LED_TURN_OFF);
        led_set_value(LED_02, LED_TURN_ON);
}

int main(void)
{
        int distance, previous = 0;
        char lcdstr[16];

        lcd_init();
        led_init();
        button_init();
        led_status_stop();
        lcd_disp("Start measuring", "by button push.");

        do {
                if (is_pressed(SW_02)) {
                        led_status_start();
                        distance = measure_distance();
                        led_status_stop();
                        if (distance < 0)
                                return distance;

                        if (previous != distance) {
                                sprintf(lcdstr, "distance = %dcm", distance);
                                lcd_disp("", lcdstr);
                                previous = distance;
                        }
                }
                usleep(100000);
        } while (1);

        return 0;
}
