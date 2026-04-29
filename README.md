# ESP32 Web Server with OLED Display

A Wi-Fi enabled web server running on an ESP32 that lets you control an onboard LED from any browser on your local network. Connection status and IP address are shown live on an SSD1306 OLED display.

---

## Features

- Connects to your Wi-Fi and starts an HTTP server 
- Built-in documentation page served at `/` (full HTML with syntax highlighting)
- SSD1306 OLED shows Wi-Fi status, IP address;
- Animated "Connecting..." feedback on the display during boot

---

## Hardware Required

| Component | Details |
|---|---|
| ESP32 Dev Board | Any standard DevKit (WROOM, WROVER, etc.) |
| SSD1306 OLED | 128×64, I2C interface |
| Jumper Wires | For I2C connections |

### Wiring (OLED)

| OLED Pin | ESP32 Pin |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

> The LED is the onboard LED on **GPIO 2** — no external wiring needed unless you want to use a different pin.

---

## Web Endpoints


Open your browser and navigate to the IP address shown on the OLED display ( `http://192.168.x.x`).

---

## Project Structure

```
esp32-web-server/
├── server-esp.ino       # Main Arduino sketch
├── README.md
├── INSTALLATION.md
└── requirements.txt     # Arduino library dependencies
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
