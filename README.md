# IoT-Capstone
This is an IoT project for a hydroponics monitoring system designed to automate the observation of nutrient solution water levels and flow within a NFT (Nutrient Film Technique) hydroponics setup. This is built using an ESP32 microcontroller, the system collects data from an ultrasonic water level sensor and two simulated flow meters to detect low solution levels and potential leaks in the system. This data is transmitted to the Adafruit IO cloud dashboard using the MQTT protocol for a real-time visualisation of systems and alerts for when a leak is detected or the input and output flow meters deviate too much.
The project is prototyped and tested using the Wokwi simulator, which allows for a full demonstration of the system without the physical hardware.
## Key Features
* Ultrasonic Water Level Monitoring:
  * Measures the distance to the nutrient solution surface in the reservoir.
* Flow Monitoring (Leak Detection):
  * Compares input and output flow rates to detect abnormal deviations that can indicate leaks.
* Adafruit IO Dashboard Integration:
  * Publishes data and alerts to an online cloud dashboard using MQTT.
* Alerts:
  * Sends notifcations to an alerts feed when water level drops too low or when leaks are detected.
* Efficient Publishing:
  * Sensor values are only published when they change, to conserve bandwidth and prevent hitting publish limits.

## Simulation Components
This simulation uses the following virtual components in Wokwi:
* ESP32 DevKit v4,
* HC-SR04 Ultrasonic Sensor,
* Two slide potentiometers (simulating the flow meters),
* Wi-Fi and MQTT connectivity to Adafruit IO.

## Instructions
1. Open Wokwi - Go to [link](https://wokwi.com).
2. Create a new project.
3. Use the provided diagram.json and .ino code from the GitHub repo to paste them into the relevant files.
4. Create an Adafruit IO Account ([link](https://www.adafruit.com)) and add three feeds:
   * water-level,
   * flow,
   * alerts.
5. Get your Adafruit credentials:
   * IO Username,
   * AIO Key.
6. Replace the #define IO_USERNAME and #define IO_KEY values in the Wokwi code with your Adafruit credentials.
7. Run the simulation:
   * Click the "Start Simulation" button in Wokwi.
   * Adjust the slide potentiometers to simulate different flow rates.
   * Adjust the ultrasonic sensor distance to simulate different water levels.
   * Watch the Serial Monitor for output (every 30 seconds if a value changed).
   * Check the Adafruit IO dashboard for published data and alerts.

## Future Improvements
* Replace simulated flow meters with real flow meters when using actual hardware.
* Use SSL/TLS with MQTT for encrypted communication.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
