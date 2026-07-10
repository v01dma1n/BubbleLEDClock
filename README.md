# Bubble LED Clock

An 8-digit 7-segment "bubble" LED clock based on the ESP32. It features automatic NTP time synchronization, weather display from OpenWeatherMap, animated scenes, and a web-based configuration portal. This project serves as the primary, full-featured example application for the `ESP32NTPClock` library framework.

<img src="photos/20250917_200239.jpg" alt="Bubble LED Clock" width="500">

-----

## ✨ Features

  * **8-Digit 7-Segment Display:** Utilizes a "bubble" LED display driven by an HT16K33 controller over I2C.
  * **Automatic Time Sync:** Connects to your WiFi and synchronizes its time with public NTP servers.
  * **Weather Display:** Fetches and displays the current temperature and humidity for a configured location from OpenWeatherMap. 
  * **Animated Scenes:** Cycles through displaying the time, date, and weather using a playlist of customizable animations (Slot Machine, Matrix, Scrolling). 
  * **Web Configuration:** On first boot or when WiFi fails, the clock enters an Access Point (AP) mode. You can connect to its WiFi network to configure all settings through a web page on your phone or computer.
  * **Double-Reset:** Manually enter the AP configuration mode at any time by quickly resetting the device twice. 
  * **Real-Time Clock (RTC):** Uses a DS1307 RTC module to keep time when offline and to enable the robust double-reset feature. 

-----

## ⚙️ Hardware Required

  * **ESP32 Module:** An ESP32-WROOM-DA was used in development.
    * HiLetgo ESP32-WROOM-32 USB-C
    * https://www.amazon.com/HiLetgo-ESP-WROOM-32-Bluetooth-ESP32-DevKitC-32-Development
  * **HT16K33 Driver:**
    * Adafruit HT16K33 Breakout
    * https://www.adafruit.com/product/1427
  * **Display:** An 8-digit, 7-segment common anode display.  
    * This can be sourced from 1970s pocket calculator.
  * **RTC Module:** A DS1307 Real-Time Clock module with a backup battery.
    * Ximimark I2C RTC DS1307
    * https://www.amazon.com/Ximimark-DS1307-AT24C32-Module-Arduino
  * **Logic Level Shifter:** A 3.3V to 5V I2C level shifter is recommended if your display and RTC modules are 5V.
  * **Power Supply:** A stable 5V power supply.

-----

## 📚 Libraries & Dependencies

This project is built on the ESP32 framework and requires several libraries to function correctly.

### Project Libraries

These are the custom libraries created specifically for this clock framework.

  * **ESP32NTPClock** v.1.0.0
  * **ESP32NTPClock Display Drivers** v.1.0.0

-----

### Third-Party Libraries

These libraries need to be installed in your Arduino IDE or PlatformIO environment.

  * **[ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer)** v.3.8.0
  * **[AsyncTCP](https://github.com/me-no-dev/AsyncTCP)** v.3.4.7
  * **[RTClib](https://github.com/adafruit/RTClib)** v.2.1.4
  * **[Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)** v.1.17.2
  * **[SimpleFSM](https://www.google.com/search?q=https://github.com/tockn/SimpleFSM)** v.1.3.1

-----

### ESP32 Core Libraries

These libraries are bundled with the ESP32 board support package (v.3.3.0) and do not require separate installation.

  * **DNSServer** v.3.3.0
  * **ESP32 Async UDP** v.3.3.0
  * **FS** v.3.3.0
  * **HTTPClient** v.3.3.0
  * **NetworkClientSecure** v.3.3.0
  * **Networking** v.3.3.0
  * **Preferences** v.3.3.0
  * **SPI** v.3.3.0
  * **WiFi** v.3.3.0
  * **Wire** v.3.3.0

-----

## 🛠️ Configuration

1.  **First Boot:** On the very first boot, the clock will automatically start in Access Point (AP) mode. The display will show a message like "AP MODE...".
2.  **Connect to the AP:** Using your phone or computer, connect to the WiFi network named **`bubble-clock`**. [cite: 667]
3.  **Captive Portal:** Once connected, a configuration page should automatically open. If it doesn't, navigate to **`192.168.4.1`** in your web browser.
4.  **Enter Settings:** Fill in your WiFi SSID and password, your [POSIX timezone string](https://www.google.com/search?q=http://www.timezoneconverter.com/cgi-bin/posix.tz.pl), and your OpenWeatherMap API key and location details.
5.  **Save:** Click "Save and Restart." The clock will reboot and connect to your WiFi network.

To re-enter configuration mode at any time, simply press the **reset button twice** in quick succession (about 1-2 seconds apart).

-----

## 🚀 Building the Project

1.  Install the [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/).
2.  Install the ESP32 board support package.
3.  Install all the libraries listed in the **Libraries & Dependencies** section above.
4.  Place all the `bubble_led_clock` project files into a sketch folder of the same name.
5.  Open `bubble_led_clock.ino`, select your ESP32 board and port, and click **Upload**.

-----

## 📜 License

This project is licensed under the MIT License - see the `LICENSE` file for details.
