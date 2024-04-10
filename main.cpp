/*
MIT License

Copyright (c) [2024] 
Organization: Perlatecnica APS ETS
Author: Mauro D'Angelo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mbed.h"
#include "LSM6DSV16X.h"

LSM6DSV16X lsm6dsv16x(I2C_SDA, I2C_SCL); 
Serial pc(USBTX, USBRX);

uint8_t status = 0;
uint32_t k = 0;

uint8_t tag = 0;
float quaternions[4] = {0};

// main() runs in its own thread in the OS
int main()
{
    // Initialize LSM6DSV16X.
    lsm6dsv16x.begin();
    lsm6dsv16x.Enable_X();
    lsm6dsv16x.Enable_G();

    // Enable Sensor Fusion
    status |= lsm6dsv16x.Set_X_FS(4);
    status |= lsm6dsv16x.Set_G_FS(2000);
    status |= lsm6dsv16x.Set_X_ODR(120.0f);
    status |= lsm6dsv16x.Set_G_ODR(120.0f);
    status |= lsm6dsv16x.Set_SFLP_ODR(120.0f);
    status |= lsm6dsv16x.Enable_Rotation_Vector();
    status |= lsm6dsv16x.FIFO_Set_Mode(LSM6DSV16X_STREAM_MODE);


    while (true) {

    }
}

