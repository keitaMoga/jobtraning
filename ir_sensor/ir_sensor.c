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
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include "ir_sensor.h"

#define IR_SNSR_SLAVE_ADDR  (0x40)
#define IR_SNSR_REGISTER_SB (0x35) // Shift Bit(0x01:128cm 0x02:64cm)
#define IR_SNSR_REGISTER_D1 (0x5E) // Distance[11:4]
#define IR_SNSR_REGISTER_D2 (0x5F) // Distance[3:0]
// *Distance[11:4]*16+Distance[3:0])/16/2^Shift Bit

static int read_register(uint8_t reg, uint8_t *data)
{
        int fd;
        if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
                printf("Faild to open i2c port\n");
                return 1;
        }

        if (ioctl(fd, I2C_SLAVE, IR_SNSR_SLAVE_ADDR) < 0) {
                printf("Unable to get bus access to talk to slave\n");
                close(fd);
                return 1;
        }
        /* write address */
        if ((write(fd, &reg, 1)) != 1) {
                fprintf(stderr, "Error writing to i2c slave\n");
                close(fd);
                return 1;
        }

        /* read data */
        if (read(fd, data, 1) != 1) {
                fprintf(stderr, "Error reading from i2c slave\n");
                close(fd);
                return 1;
        }
        close(fd);
        return 0;
}

int measure_distance(void)
{
        uint8_t distance_reg1;
        uint8_t distance_reg2;
        int distance;

        if (0 != read_register(IR_SNSR_REGISTER_D1, &distance_reg1)) {
                printf("file open error\n");
                return -1;
        }
        if (0 != read_register(IR_SNSR_REGISTER_D2, &distance_reg2)) {
                printf("file open error\n");
                return -1;
        }
        distance = ((distance_reg1 * 16 + distance_reg2) / 16) / pow(2, 0x01);
        return distance;
}

#ifdef __STANDALONE__
int main(void)
{
        int distance = 0;

        do {
                distance = measure_distance();
                printf("distance = %dcm\n", distance);
                usleep(100000);
        } while (1);
        return 0;
}
#endif
