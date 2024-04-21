/*
MIT License

Copyright (c) [2024] 
Organization: Perlatecnica APS ETS
Author: Mauro D'Angelo
Example to use the LSM6DSV16X library with Sensor Fusion Low Power

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
#include "plt_iks4a1.h"
#include <cmath>

#define ALGO_FREQ  120U /* Algorithm frequency 120Hz */
#define ALGO_PERIOD  (1000U / ALGO_FREQ) /* Algorithm period [ms] */
#define PI 3.14159265358979323846

#define PRINT_QUATERNIONS
//#define PRINT_EULER_ANGLES

LSM6DSV16X lsm6dsv16x(I2C_SDA, I2C_SCL); 
Serial pc(USBTX, USBRX);
Timer t;

uint8_t status = 0;
uint32_t k = 0;
unsigned long startTime, elapsedTime;


uint8_t tag = 0;
float quaternions[4] = {0};
float q0, q1, q2, q3 = 0;

// main() runs in its own thread in the OS
int main()
{
    pc.baud(115200);

    // Initialize LSM6DSV16X.
    lsm6dsv16x.begin();
    
    // Enable Sensor Fusion
    status |= lsm6dsv16x.Set_X_FS(4);
    status |= lsm6dsv16x.Set_G_FS(2000);
    status |= lsm6dsv16x.Set_X_ODR(120.0f);
    status |= lsm6dsv16x.Set_G_ODR(120.0f);
    lsm6dsv16x.Enable_X();
    lsm6dsv16x.Enable_G();

    lsm6dsv16x.Reset_SFLP();
    status |= lsm6dsv16x.Set_SFLP_ODR(120.0f);
    status |= lsm6dsv16x.Enable_Rotation_Vector();
    status |= lsm6dsv16x.FIFO_Set_Mode(LSM6DSV16X_STREAM_MODE);

    if (status != LSM6DSV16X_OK) {
        pc.printf("LSM6DSV16X Sensor failed to init/configure\r\n");
        while (1);
    }
    
    pc.printf("LSM6DSV16X Sensor Fusion Demo\r\n");

    t.start();
    while (true) {
        uint16_t fifo_samples;
        
        // Get start time of loop cycle
        startTime = t.read_ms();

        // Check the number of samples inside FIFO
        if (lsm6dsv16x.FIFO_Get_Num_Samples(&fifo_samples) != LSM6DSV16X_OK) {
            pc.printf("LSM6DSV16X Sensor failed to get number of samples inside FIFO");
            while (1);
        }

        // Read the FIFO if there is one stored sample
        if (fifo_samples > 0) {
            for (int i = 0; i < fifo_samples; i++) {
                lsm6dsv16x.FIFO_Get_Tag(&tag);
                if (tag == 0x13u) {
                    lsm6dsv16x.FIFO_Get_Rotation_Vector(&quaternions[0]);
                    q0 = quaternions[3];
                    q1 = quaternions[0];
                    q2 = quaternions[1];
                    q3 = quaternions[2];
                                        
                    // Compute the elapsed time within loop cycle and wait
                    elapsedTime = t.read_ms() - startTime;

                    if ((long)(ALGO_PERIOD - elapsedTime) > 0) {
                        thread_sleep_for(ALGO_PERIOD - elapsedTime);
                    }
                    t.reset();

                    // Print Quaternion data
                    #ifdef PRINT_QUATERNIONS
                        pc.printf("%.2f, %.2f, %.2f, %.2f\r\n", q0, q1, q2, q3);
                    #elif PRINT_EULER_ANGLES
                    // Calcolo degli angoli x, y e z utilizzando i quaternioni
                    float roll = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
                    float pitch = asin(2 * (q0 * q2 - q3 * q1));
                    float yaw = atan2(2 * (q0 * q3 + q1 * q2),  1 - 2 * (q2 * q2 + q3 * q3));

                    // Conversione da radianti a gradi
                    roll *= (float)((float)180.0 / (float)PI);
                    pitch *= (float)((float)180.0 / (float)PI);
                    yaw *= (float)((float)180.0 / (float)PI);

                    // Stampa degli angoli
                    pc.printf("%.2f, %.2f, %.2f\r\n", roll, pitch, yaw);

                    // Conversione da radianti a gradi
                    roll *= (float)((float)180.0 / (float)PI);
                    pitch *= (float)((float)180.0 / (float)PI);
                    yaw *= (float)((float)180.0 / (float)PI);
                    #endif
                }
            }
        }

    }
}

