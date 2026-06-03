# Smart Parking Assist System 🚗

https://github.com/user-attachments/assets/107bd262-9fd1-4afa-9c2b-6afa1bf3a55b

A real-time, non-blocking parking sensor built with Arduino and an HC-SR04 ultrasonic module. It measures distance and gives instant visual (LED) and audio (Buzzer) feedback. The system uses short, intentional delays only for buzzer timing and sensor cooldown.

## 🛠️ Components
* Arduino UNO
* HC-SR04 Ultrasonic Sensor
* 3x LEDs (Red, Yellow, Green)
* 1x Active Buzzer
* 3x Resistors
* Breadboard & Jumpers

## 🔌 Pin Mapping
| Component | Arduino Pin |
| :--- | :--- |
| **HC-SR04 Trig** | Pin 12 |
| **HC-SR04 Echo** | Pin 13 |
| **Buzzer** | Pin 5 |
| **Red LED** | Pin 2 |
| **Yellow LED** | Pin 3 |
| **Green LED** | Pin 4 |

<img width="1771" height="757" alt="image" src="https://github.com/user-attachments/assets/291ad7c4-e5a5-49c4-9831-b404c140e5bd" />

## ⚙️ How It Works
The system reads distance continuously (with a short 100ms gap to avoid sound wave overlap) and splits the area into 3 zones:
1. **Safe (> 50 cm):** Green LED ON. Path is clear.
2. **Caution (20 - 50 cm):** Yellow LED ON. Buzzer gives short beeps (Slow down).
3. **Danger (≤ 20 cm):** Red LED ON. Continuous solid beep (STOP!).

### 🔬 Custom Math Calibration
Instead of relying on the textbook speed of sound constant (0.017 cm/µs), I calibrated this setup manually based on actual hardware testing.

During testing, a physical distance of **5 cm** returned a ping travel time of **274 µs**. 
Therefore, the custom formula used in the code is:

$$\text{Distance} = \text{pingTravelTime} \times \left(\frac{5.0}{274.0}\right)$$

Using this empirical data automatically cancels out hardware inaccuracies (like the sensor's internal crystal oscillator) and adjusts for the room's current temperature and humidity. 

## 🚀 How to Run
1. Wire up the components as shown in the circuit diagram.
2. Open the `.ino` file in the Arduino IDE.
3. Compile and upload to your Arduino UNO.
4. Open the Serial Monitor (9600 baud rate) to see the live distance feed.
