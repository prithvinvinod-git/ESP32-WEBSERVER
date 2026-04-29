# Installation Guide

## Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x (recommended) or 1.8.x
- A USB cable that supports data transfer (not charge-only)
- ESP32 board plugged into your computer

---

## Step 1 — Install the ESP32 Board Package

1. Open Arduino IDE and go to **File → Preferences**
2. In the *Additional Boards Manager URLs* field, paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click **OK**
4. Go to **Tools → Board → Boards Manager**
5. Search for `esp32` and install the package by **Espressif Systems**

---

## Step 2 — Install Required Libraries

Go to **Tools → Manage Libraries** and search for and install each of the following:

| Library | Author | Version |
|---|---|---|
| Adafruit SSD1306 | Adafruit | ≥ 2.5.7 |
| Adafruit GFX Library | Adafruit | ≥ 1.11.9 |
| Adafruit BusIO | Adafruit | ≥ 1.14.1 |

> `WiFi.h` and `WebServer.h` come bundled with the ESP32 board package — no separate install needed.

---

## Step 3 — Configure Your Wi-Fi Credentials

Open `server-esp.ino` and find these two lines near the top:

```cpp
const char* ssid     = "ssid";
const char* password = "pass";
```

Replace `"ssid"` and `"pass"` with your actual Wi-Fi network name and password.

---

## Step 4 — Select Your Board & Port

1. Go to **Tools → Board → ESP32 Arduino** and select your board model (e.g. *ESP32 Dev Module*)
2. Go to **Tools → Port** and select the COM port your ESP32 is connected to
   - On Windows it will appear as `COM3`, `COM4`, etc.
   - On macOS/Linux it will appear as `/dev/ttyUSB0` or `/dev/cu.usbserial-...`

Recommended upload settings:

| Setting | Value |
|---|---|
| Upload Speed | 921600 |
| Flash Frequency | 80MHz |
| Partition Scheme | Default 4MB |

---

## Step 5 — Upload the Sketch

1. Click the **Upload** button (→ arrow) in Arduino IDE
2. If the upload hangs at *"Connecting..."*, hold the **BOOT** button on your ESP32 for 1–2 seconds while it tries to connect
3. Once uploaded, open **Tools → Serial Monitor** at **115200 baud** to see the IP address printed

---

## Step 6 — Use It

1. The OLED will show *"Connecting..."* while it joins your Wi-Fi, then display the IP address once connected
2. Open a browser on any device on the same network and visit:
   ```
   http://<IP_ADDRESS_ON_OLED>
   ```

---

## Troubleshooting

**OLED shows nothing / "SSD1306 allocation failed" in Serial Monitor**
- Double-check SDA → GPIO 21 and SCL → GPIO 22
- Confirm the OLED I2C address is `0x3C` (most common). If it's `0x3D`, change it in the `display.begin()` call in the sketch.

**Can't connect to Wi-Fi**
- Make sure the SSID and password are correct (case-sensitive)
- ESP32 only supports 2.4 GHz networks — 5 GHz will not work

**Browser can't reach the server**
- Your phone/computer must be on the same Wi-Fi network as the ESP32
- Try pinging the IP shown on the OLED to confirm connectivity
