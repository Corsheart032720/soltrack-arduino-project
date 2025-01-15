---

# Soltrack Arduino Project

### Sun Detection and Temperature Monitoring System

The Soltrack Arduino Project combines the power of Arduino technology with advanced sensors to deliver a robust environmental monitoring solution. Designed to detect sunlight and monitor ambient temperature, this project showcases the integration of Arduino with various electronic components for automation and environmental applications.

---

## 🌟 Key Features

### **Sun Detection**
- **Photoresistor-Based Detection**: Tracks sunlight intensity and adjusts position using a servo motor.
- **LED Indicators**: Visual output to indicate light intensity thresholds.

### **Temperature Monitoring**
- **Temperature Sensor Integration**: Measures ambient temperature and converts it into Celsius for display.
- **Alert System**: Piezoelectric buzzer triggers an alert for high temperatures or significant changes in temperature.

### **User Interface**
- **LCD Display**: Real-time display of temperature readings.
- **Interactive Controls**: Potentiometer to adjust LCD contrast.

---

## 🛠️ System Architecture

### **Design Circuit View**
The circuit includes:
- Arduino Uno as the central processor.
- Sensors: Photoresistor and TMP36 temperature sensor.
- Actuators: Servo motor and piezo buzzer.
- Output devices: LCD display and LEDs.

### **Design Schematic View**
The schematic outlines:
- Component interconnections and pin configurations.
- Logical pathways for sunlight detection and temperature monitoring.
- Signal processing flow for alert and display systems.

---

## 📋 Components List

| **Name**         | **Quantity** | **Component**              |
|-------------------|--------------|----------------------------|
| U1               | 1            | Arduino Uno R3            |
| SERVO1           | 1            | Positional Micro Servo     |
| R1, R2           | 2            | Photoresistor             |
| D1, D2           | 2            | Yellow LED                |
| R3, R4, R5       | 3            | 1 kΩ Resistor             |
| U2               | 1            | TMP36 Temperature Sensor  |
| PIEZO1           | 1            | Piezo Buzzer              |
| U3               | 1            | LCD 16 x 2                |
| Rpot1            | 1            | 250 kΩ Potentiometer      |

---

## 📸 Screenshots

### **Design Circuit View**
![Circuit Design Placeholder](https://i.postimg.cc/yY0wjtj4/image.png)

### **Design Schematic View**
![Schematic View Placeholder](https://i.postimg.cc/vm5pnBHX/image.png)


---

## 💻 Installation and Usage

### **Hardware Setup**
1. Assemble the circuit based on the Design Circuit View.
2. Connect components to the Arduino Uno as per the pin configurations provided.

### **Software Setup**
1. Clone the repository:
   ```bash
   git clone https://github.com/Corsheart032720/soltrack-arduino-project.git
   ```
2. Open the project in the Arduino IDE.
3. Upload the code to the Arduino Uno.
4. Power the Arduino and observe the system in action.

---

## 🛠️ Contributing

Contributions are welcome! Feel free to:
- Report bugs.
- Suggest features.
- Submit pull requests.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 📧 Contact

For inquiries or support, reach out via email: **annenicolehat@gmail.com**

---
