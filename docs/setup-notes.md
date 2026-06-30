# Setup Notes

## Arduino Libraries Required
Install these libraries in Arduino IDE:

- ESP32 board package
- ESP32Servo

## Board Selection
Use an ESP32 development board such as:

- ESP32 Dev Module
- DOIT ESP32 DEVKIT V1

## Testing Steps
1. Upload the code to ESP32.
2. Open Serial Monitor at 115200 baud.
3. Check the IP address printed by ESP32.
4. Open the IP address in a browser.
5. Check temperature and pH readings.
6. Press "Feed Fish Now" to test servo movement.
7. Check alert LED behaviour when readings go outside safe limits.
