# xNucleo-IKS4A1_LSM6DSV16X_mbedOS_001-SensorFusion

This application shows how to use LSM6DSV16X Sensor Fusion features for reading quaternions.

---
### Description:

This code demonstrates a sensor fusion application using the LSM6DSV16X inertial measurement sensor with an mbed microcontroller. It performs sensor fusion to compute quaternion data, and optionally Euler angles, from raw sensor readings.

### How it works:

- **Initialization**: 
  - It includes necessary libraries for communication (`mbed.h`) and for interfacing with the LSM6DSV16X sensor (`plt_iks4a1.h`).
  - It defines macros for algorithm frequency and period, and includes the value of pi.
  - It initializes objects for serial communication (`Serial`), timer (`Timer`), and the LSM6DSV16X sensor.

- **Sensor Fusion Setup**:
  - It configures the LSM6DSV16X sensor for sensor fusion:
    - Sets the full-scale range (FS) for accelerometer and gyroscope.
    - Sets the output data rate (ODR) for accelerometer and gyroscope.
    - Enables accelerometer and gyroscope.
    - Configures sensor fusion parameters.
    - Sets the sensor in stream mode for continuous data streaming.

- **Main Loop**:
  - It enters an infinite loop where it continuously reads data from the sensor's FIFO buffer.
  - Within each iteration:
    - It checks the number of samples available in the FIFO buffer.
    - If samples are available, it reads the rotation vector data from the FIFO.
    - It computes the quaternion data from the rotation vector.
    - It computes the elapsed time within the loop cycle and waits for the algorithm period.
    - It prints the quaternion data over the serial connection.
    - Optionally, it computes and prints Euler angles (roll, pitch, yaw) from the quaternion data.

---

This description outlines the functionality of the code, including initialization, sensor fusion setup, and the main loop for data processing and output.