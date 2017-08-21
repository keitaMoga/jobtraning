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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include "lcd.h"

#define INIT_WAIT_TIME_USEC (40)
#define I2C_LCD_SLAVE_ADDR  (0x3e)
#define LCD_REGISTER        (0x00)
#define PATH_MAX            (64)
#define CTL_LCD_INSTR       (0x00)
#define CTL_LCD_DATA        (0x40)
#define LCD_PACKET_RS_BIT   (0x40)
#define PACKET_LENGTH_BYTE  (2)
#define LCD_LINE_SIZE       (16)

#define SECOND_LINE_HEAD_ADDR (0x40)

/* LCD instruction code */
#define LCD_INST_SET_FC1    (0x38)  // Function set1 (Instruction table 0 : Can set Cursor/Display Shift/CGRAM)
#define LCD_INST_SET_FC2    (0x39)  // Function set2 (Instruction table 1 : Can set OSC/ICON etc)
#define LCD_INST_INT_OSC    (0x14)  // Internal OSC frequency
#define LCD_INST_SET_CRT    (0x73)  // Contrast set
#define LCD_INST_CTR_PIC    (0x56)  // Power/ICON/Contrast control
#define LCD_INST_CTR_FLR    (0x6c)  // Follower control
#define LCD_INST_CTR_CLR    (0x01)  // Clear Display
#define LCD_INST_CTR_PWR    (0x0c)  // Display ON/OFF control
#define LCD_INST_CTR_SDA    (0x80)  // Set DDRAM Address

enum packet_type {
        LCD_PACKET_TYPE_INST,
        LCD_PACKET_TYPE_DATA,
};

typedef struct {
        uint8_t ctrl;
        uint8_t data;
} packet;

static int lcd_set(enum packet_type type, uint8_t data)
{
        int fd, ret;
        packet p;

        fd = open("/dev/i2c-1", O_RDWR);
        if (fd < 0) {
                printf("Faild to open i2c port\n");
                return fd;
        }

        ret = ioctl(fd, I2C_SLAVE, I2C_LCD_SLAVE_ADDR);
        if (ret < 0) {
                printf("Unable to get bus access to talk to slave\n");
                close(fd);
                return ret;
        }

        p.ctrl = CTL_LCD_INSTR;
        if (type == LCD_PACKET_TYPE_DATA)
                p.ctrl |= LCD_PACKET_RS_BIT;
        p.data = data;

        ret = write(fd, &p, PACKET_LENGTH_BYTE);
        close(fd);

        if (ret < 0)
                return ret;
        if (ret != PACKET_LENGTH_BYTE) {
                printf("Error writing to i2c slave\n");
                close(fd);
                return -1;
        }

        if (type == LCD_PACKET_TYPE_INST)
                usleep(INIT_WAIT_TIME_USEC);

        return 0;
}

void lcd_init(void)
{
        uint8_t INIT_INST[] = {
                LCD_INST_SET_FC1, LCD_INST_SET_FC2, LCD_INST_INT_OSC,
                LCD_INST_SET_CRT, LCD_INST_CTR_PIC, LCD_INST_CTR_FLR,
                LCD_INST_SET_FC1, LCD_INST_CTR_CLR, LCD_INST_CTR_PWR
        };
        int i;

        for (i = 0; i < sizeof(INIT_INST) / sizeof(INIT_INST[0]); i++)
                lcd_set(LCD_PACKET_TYPE_INST, INIT_INST[i]);
}

static int lcd_send_string(const char *string)
{
        int ret;
        int i;

        for (i = 0; string[i]; i++) {
                ret = lcd_set(LCD_PACKET_TYPE_DATA, string[i]);
                if (ret < 0)
                        return ret;
        }
        return 0;
}

void lcd_disp(const char *first_line, const char *second_line)
{
        /* CLS */
        lcd_set(LCD_PACKET_TYPE_INST, LCD_INST_CTR_CLR);

        lcd_send_string(first_line);

        /* CR+LF */
        lcd_set(LCD_PACKET_TYPE_INST, LCD_INST_CTR_SDA | SECOND_LINE_HEAD_ADDR);

        lcd_send_string(second_line);
}
