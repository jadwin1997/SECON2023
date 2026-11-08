# SECON 2023

Autonomous robot software developed for the 2023 SECON competition. This robot successfully ran at competition, navigating the field using wall-distance sensing and gyro heading while coordinating item collection, colored-chip delivery, and recycling deposits.

The core of the project is a custom sensor-based navigation algorithm: two ultrasonic sensors estimate alignment with a wall, a front distance sensor detects approaching walls, and an integrated gyro heading tracks progress around the field. Distance feedback and a filtered wall-angle estimate produce differential-drive steering commands.

## Final competition code

**[LibraryTesting__LEDStart.ino](src/testing/Library/LibraryTesting__LEDStart/LibraryTesting__LEDStart.ino)** is the final competition sketch, despite its location under `testing`.

Its hardware interface lives alongside it:

- [SECON_PROTOBOARD_V1.h](src/testing/Library/LibraryTesting__LEDStart/SECON_PROTOBOARD_V1.h) — sensor and motor interface, pin assignments, and controller state.
- [SECON_PROTOBOARD_V1.cpp](src/testing/Library/LibraryTesting__LEDStart/SECON_PROTOBOARD_V1.cpp) — sensor acquisition, motor control, distance variance, and serial telemetry.

`src/competition/competition.ino` is an empty placeholder, rather than the final program.

## Hardware

| Component | Role |
| --- | --- |
| Arduino Nano | Runs navigation and controls the drive motors |
| MPU-6050 | Supplies Z-axis angular velocity and three-axis acceleration over I²C |
| Two ultrasonic sensors | Measure wall distance and estimate the robot's angle relative to the wall |
| Front Sharp IR distance sensor | Detects approaching walls and initiates turns |
| TCS34725 color sensor | Detects the starting LED |
| Two independently driven motors | Provide differential-drive motion |
| Arduino Mega | Receives commands for intake and deposit mechanisms over software serial |

The final hardware library reads the front sensor using `SharpIR`, although some sketch comments refer to LiDAR. Accelerometer readings are acquired each navigation cycle; the steering and mission logic in this version use the distance sensors and gyro.

### Navigation-controller connections

Pin numbers below come from the final sketch and its local hardware library.

| Connection | Nano pins / address |
| --- | --- |
| Ultrasonic sensor 1 | Trigger D10, echo D9 |
| Ultrasonic sensor 2 | Trigger D6, echo D5 |
| Front IR sensor | A7; SharpIR model setting `20150` |
| Left motor | PWM D11, direction D4 and D2 |
| Right motor | PWM D3, direction D8 and D7 |
| MPU-6050 | I²C, address `0x68` |
| TCS34725 | I²C, library default address |
| Mega communication | RX D13, TX D12, 9600 baud |
| USB serial telemetry | 9600 baud |

## How navigation works

### Wall-distance and angle feedback

The two wall-facing ultrasonic sensors are separated by **12 cm**. Their echo-time difference is converted to an estimated wall angle:

```text
wall_angle = atan2(echo_1 - echo_2 - offset, 12 × 2 × 29.1)
```

The result is converted to degrees. Using echo times retains more measurement resolution than whole-centimeter distances. The sketch applies a scalar Kalman-filter update to this angle estimate to smooth wall-alignment feedback.

A separate distance-error calculation aims to maintain an initial **12 cm wall clearance**. The steering calculation blends that distance-control term with the filtered wall-angle estimate:

```text
steering = -0.9 × distance_control + 0.1 × filtered_wall_angle
```

This combines correction toward the desired clearance with correction toward alignment with the wall. The sketch also calculates filtered ultrasonic echo times, though those values are not used in the final steering calculation.

### Front-wall turns and motor control

When the front sensor reports a distance below **39 cm**, the controller progressively shifts the steering command toward a turn. Otherwise, it follows the wall using the blended feedback above.

Steering is converted into left and right motor commands. The base speed decreases as the steering magnitude increases, helping reduce oscillation during corrections and allowing tighter turns. Motor commands are constrained to the signed PWM range of **−255 to 255**.

### Gyro-based field progress

The controller integrates the MPU-6050's Z-axis angular velocity using a nominal **0.05-second timestep**. This accumulated heading triggers chip deliveries and recycling maneuvers at specific points around the field.

Wall feedback handles continuous steering, while gyro heading provides mission progress. Heading is reset at selected maneuvers where the robot is expected to have realigned with the field.

### Stuck recovery

The hardware library calculates the variance of the average ultrasonic distance over batches of seven readings. Sustained low variance is treated as a sign that the robot may be stuck.

After repeated low-variance cycles, the controller increases its target speed and briefly drives both motors forward at a higher PWM value. These short kicks help the robot escape stalled positions. Normal target speed is restored when distance variation returns.

## Competition sequence

1. **Initialize:** configure the motors, wake the MPU-6050, sample the gyro, and initialize serial communication and the color sensor.
2. **Wait for the start:** monitor the color sensor's normalized red channel for the starting LED. Start after detection or a 10-second timeout; skip the wait if the color sensor cannot initialize.
3. **Navigate and collect:** follow the wall, turn away from front walls, and send intake commands to the mechanism controller.
4. **Deliver the red chip:** when integrated heading passes approximately **−47°**, stop, reverse toward the wall to align, and send the red-chip command.
5. **Deliver the green chip:** when heading passes approximately **−89°**, perform another reverse alignment and send the green-chip command. Reset heading to **−180°** afterward.
6. **Deposit recycling:** when heading passes **−275°**, perform a timed turning and driving sequence, send the deposit command, and reposition to sweep items into the corner.
7. **Run a second circuit:** reset heading and increase target wall clearance by **15 cm** after the deposit. The chip-delivery flags remain set, so those deliveries happen only once.
8. **Finish:** after the second recycling deposit, stop the drive motors and continue sending the deposit command once per second.

These maneuvers combine sensor-guided travel with timed movements tuned for the competition robot and field.

### Mechanism-controller commands

The Nano sends single ASCII digits to the Mega through `SoftwareSerial.print()` at 9600 baud.

| Command | Intended action |
| --- | --- |
| `0` | Intake items |
| `1` | Deposit recycling/items |
| `2` | Drop the red chip |
| `3` | Drop the green chip |

The final navigation sketch sends these commands; the receiving mechanism firmware is not included in that sketch's folder.

## Opening and uploading

1. Open `src/testing/Library/LibraryTesting__LEDStart/LibraryTesting__LEDStart.ino` in the Arduino IDE. Keep the accompanying `.h` and `.cpp` files in the same sketch folder.
2. Install the external libraries referenced by the sketch and hardware header:
   - `DFRobot_TCS34725`
   - `NewPing`
   - `SharpIR`
   - `TFMini` — still included by the hardware header, although the final front-distance implementation uses SharpIR.
3. Use the Arduino AVR board package, which provides `Wire` and `SoftwareSerial`.
4. Select the Arduino Nano, the processor/bootloader matching the board, and its serial port, then compile and upload.
5. Open the Serial Monitor at **9600 baud** to inspect distance readings, integrated heading, motor commands, and distance variance.

This repository preserves the original competition source. Library versions and the exact board/bootloader configuration were not pinned, so reproducing the build may require matching the original library APIs. Running the full mission requires the corresponding robot hardware and mechanism controller.

### Main tuning values

| Setting | Competition value | Purpose |
| --- | --- | --- |
| `dt` | `0.05` s | Nominal gyro integration timestep |
| `bot_size` | `12` cm | Ultrasonic sensor spacing |
| `target_distance` | `12` cm initially | Wall-following clearance |
| `target_speed` | `120` | Initial drive PWM target |
| `Kp` | `0.8` | Distance-control weighting |
| Distance / wall-angle blend | `0.9 / 0.1` | Relative steering contributions |
| Front-wall threshold | `39` cm | Initiates obstacle-turn behavior |
| Low-variance threshold | `12.0` | Stuck-detection threshold |
| Start timeout | `10,000` ms | Fallback when no starting LED is detected |

The navigation loop includes a 50 ms delay plus sensor reads and processing, and mission maneuvers use blocking delays. The integration timestep is therefore a nominal tuning value rather than a measured loop period.

## Repository layout

```text
src/
├── testing/
│   ├── Library/
│   │   ├── LibraryTesting__LEDStart/  Final competition navigation code
│   │   └── ...                       Earlier iterations and hardware tests
│   └── wall_follower/                Earlier wall-following sketch
├── competition/                      Placeholder sketch
├── _EXAMPLES/                        Individual hardware examples
├── python/colordetect/               Computer-vision experiments
└── SAViM/                            Separate C++/Qt project
docs/                                Generated Doxygen documentation
style/                               Documentation styling
Doxyfile                             Doxygen configuration
doxygen.sh                           Documentation-generation script
```

The repository captures the development process as well as the competition result: experiments, successive controller iterations, and the final hardware-tuned autonomous routine.
