#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//display  if u have one or ignore
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


String displayIP = "Connecting...";
String displayStatus = "Init...";
String displayLED = "OFF";


const char* ssid = "ssid";         
const char* password = "pass"; 


WebServer server(80); 
const int ledPin = 2; 


const char index_html[] PROGMEM = R"rawliteral(
    //replace with your html code here
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Documentation | The Ultimate Guide</title>
    <meta name="description" content="Comprehensive documentation and quick start guide for the ESP32 microcontroller.">
    
    <!-- Google Fonts for Typography -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&family=JetBrains+Mono:wght@400;500&display=swap" rel="stylesheet">

    <style>
        /* --- CSS Variables & Reset --- */
        :root {
            /* Colors */
            --primary: #007AFF;
            --primary-dark: #0056b3;
            --secondary: #6c757d;
            --accent-bg: #e7f1ff;
            --bg-body: #f8f9fa;
            --bg-card: #ffffff;
            --text-main: #212529;
            --text-muted: #6c757d;
            --border-color: #e9ecef;
            --code-bg: #282c34;
            --code-text: #abb2bf;
            
            /* Spacing */
            --sidebar-width: 260px;
            --header-height: 60px;
            --spacing-md: 1.5rem;
            --spacing-lg: 3rem;
            
            /* Shadows */
            --shadow-sm: 0 1px 3px rgba(0,0,0,0.05);
            --shadow-md: 0 4px 6px rgba(0,0,0,0.05);
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        html {
            scroll-behavior: smooth;
        }

        body {
            font-family: 'Inter', sans-serif;
            background-color: var(--bg-body);
            color: var(--text-main);
            line-height: 1.6;
        }

        a {
            text-decoration: none;
            color: inherit;
        }

        ul {
            list-style: none;
        }

        /* --- Components --- */
        
        /* Toast Notification */
        .toast {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background-color: #333;
            color: white;
            padding: 12px 24px;
            border-radius: 6px;
            font-size: 0.9rem;
            opacity: 0;
            transform: translateY(20px);
            transition: opacity 0.3s, transform 0.3s;
            z-index: 1000;
            pointer-events: none;
        }
        .toast.show {
            opacity: 1;
            transform: translateY(0);
        }

        /* Buttons */
        .btn {
            display: inline-block;
            padding: 10px 20px;
            border-radius: 6px;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.2s ease;
            font-size: 0.95rem;
        }
        .btn-primary {
            background-color: var(--primary);
            color: white;
            border: none;
        }
        .btn-primary:hover {
            background-color: var(--primary-dark);
        }
        .btn-outline {
            background-color: transparent;
            border: 1px solid var(--border-color);
            color: var(--text-main);
        }
        .btn-outline:hover {
            border-color: var(--primary);
            color: var(--primary);
        }

        /* Code Blocks */
        .code-block {
            background-color: var(--code-bg);
            border-radius: 8px;
            padding: 1.5rem;
            position: relative;
            margin-bottom: 1.5rem;
            overflow-x: auto;
        }
        .code-block pre {
            color: var(--code-text);
            font-family: 'JetBrains Mono', monospace;
            font-size: 0.9rem;
            white-space: pre;
        }
        .copy-btn {
            position: absolute;
            top: 10px;
            right: 10px;
            background: rgba(255,255,255,0.1);
            border: none;
            color: #ccc;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.75rem;
            cursor: pointer;
            font-family: sans-serif;
        }
        .copy-btn:hover {
            background: rgba(255,255,255,0.2);
            color: white;
        }
        
        /* Syntax Highlighting Classes (Manual) */
        .syntax-kwd { color: #c678dd; } /* keyword */
        .syntax-fn { color: #61afef; } /* function */
        .syntax-str { color: #98c379; } /* string */
        .syntax-num { color: #d19a66; } /* number */
        .syntax-com { color: #5c6370; font-style: italic; } /* comment */
        .syntax-type { color: #e5c07b; } /* type */

        /* --- Layout --- */
        
        /* Header */
        .header {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: var(--header-height);
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(10px);
            border-bottom: 1px solid var(--border-color);
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 0 var(--spacing-md);
            z-index: 500;
        }
        .logo {
            font-weight: 700;
            font-size: 1.25rem;
            display: flex;
            align-items: center;
            gap: 10px;
        }
        .logo span { color: var(--primary); }
        
        .mobile-menu-btn {
            display: none;
            background: none;
            border: none;
            font-size: 1.5rem;
            cursor: pointer;
        }

        /* Main Grid */
        .layout-container {
            display: flex;
            max-width: 1200px;
            margin: 0 auto;
            padding-top: calc(var(--header-height) + 20px);
        }

        /* Sidebar */
        .sidebar {
            width: var(--sidebar-width);
            flex-shrink: 0;
            position: sticky;
            top: calc(var(--header-height) + 20px);
            height: calc(100vh - var(--header-height) - 40px);
            overflow-y: auto;
            padding-right: 20px;
            display: flex;
            flex-direction: column;
            gap: 5px;
            /* Hide scrollbar for aesthetics */
            scrollbar-width: none; 
        }
        .sidebar::-webkit-scrollbar { display: none; }
        
        .nav-link {
            display: block;
            padding: 8px 12px;
            border-radius: 6px;
            color: var(--text-muted);
            font-size: 0.95rem;
            font-weight: 500;
            transition: all 0.2s;
        }
        .nav-link:hover {
            background-color: rgba(0,0,0,0.05);
            color: var(--text-main);
        }
        .nav-link.active {
            background-color: var(--accent-bg);
            color: var(--primary);
        }
        .nav-sub-header {
            font-size: 0.75rem;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            color: #9ca3af;
            margin: 20px 0 10px 10px;
            font-weight: 700;
        }

        /* Main Content */
        .main-content {
            flex-grow: 1;
            padding: 0 20px 60px 20px;
            min-width: 0; /* Prevents flex child overflow */
        }

        /* Sections */
        section {
            margin-bottom: 60px;
            scroll-margin-top: calc(var(--header-height) + 20px);
        }

        h1 {
            font-size: 2.5rem;
            margin-bottom: 1rem;
            letter-spacing: -0.02em;
        }
        h2 {
            font-size: 1.75rem;
            margin-bottom: 1rem;
            padding-bottom: 0.5rem;
            border-bottom: 1px solid var(--border-color);
            letter-spacing: -0.01em;
        }
        h3 {
            font-size: 1.25rem;
            margin-top: 1.5rem;
            margin-bottom: 0.75rem;
            color: var(--text-main);
        }
        p {
            margin-bottom: 1rem;
            color: #4a5568;
        }

        /* Cards & Feature Grid */
        .feature-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 2rem;
        }
        .feature-card {
            background: var(--bg-card);
            padding: 20px;
            border-radius: 12px;
            border: 1px solid var(--border-color);
            box-shadow: var(--shadow-sm);
            transition: transform 0.2s;
        }
        .feature-card:hover {
            transform: translateY(-3px);
            box-shadow: var(--shadow-md);
        }
        .feature-icon {
            font-size: 2rem;
            margin-bottom: 10px;
            display: inline-block;
            padding: 10px;
            background: var(--accent-bg);
            border-radius: 8px;
            color: var(--primary);
        }

        /* Pinout Table */
        .pinout-table {
            width: 100%;
            border-collapse: collapse;
            background: var(--bg-card);
            border-radius: 8px;
            overflow: hidden;
            box-shadow: var(--shadow-sm);
            font-size: 0.9rem;
        }
        .pinout-table th, .pinout-table td {
            padding: 12px 16px;
            text-align: left;
            border-bottom: 1px solid var(--border-color);
        }
        .pinout-table th {
            background-color: #f1f3f5;
            font-weight: 600;
            color: var(--text-muted);
        }
        .pinout-table tr:last-child td { border-bottom: none; }
        .chip-pin {
            font-family: 'JetBrains Mono', monospace;
            background: #eee;
            padding: 2px 6px;
            border-radius: 4px;
            font-weight: bold;
        }

        /* Alerts / Callouts */
        .alert {
            padding: 16px;
            border-radius: 8px;
            margin-bottom: 1.5rem;
            border-left: 4px solid;
        }
        .alert-info {
            background-color: #e7f5ff;
            border-color: #339af0;
            color: #1c7ed6;
        }
        .alert-warning {
            background-color: #fff9db;
            border-color: #fcc419;
            color: #f08c00;
        }

        /* Footer */
        footer {
            margin-top: auto;
            border-top: 1px solid var(--border-color);
            padding: 40px 20px;
            text-align: center;
            color: var(--text-muted);
            font-size: 0.9rem;
        }

        /* --- Responsive Queries --- */
        @media (max-width: 768px) {
            .layout-container {
                flex-direction: column;
            }
            .sidebar {
                display: none; /* Hide sidebar on mobile by default */
                position: fixed;
                top: var(--header-height);
                left: 0;
                width: 100%;
                background: white;
                z-index: 400;
                border-bottom: 1px solid var(--border-color);
                height: auto;
                max-height: 50vh;
                padding: 20px;
            }
            .sidebar.active {
                display: flex;
            }
            .mobile-menu-btn {
                display: block;
            }
            h1 { font-size: 2rem; }
            .main-content { padding: 0 15px 40px 15px; }
        }
    </style>
</head>
<body>

    <!-- Header -->
    <header class="header">
        <a href="#" class="logo">
            <!-- Simple SVG Icon -->
            <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M18 10h-1.26A8 8 0 1 0 9 20h9a5 5 0 0 0 0-10z"></path></svg>
            <span>ESP32</span>Docs
        </a>
        <button class="mobile-menu-btn" aria-label="Toggle Menu" onclick="toggleSidebar()">
            ☰
        </button>
    </header>

    <div class="layout-container">
        
        <!-- Sidebar Navigation -->
        <aside class="sidebar" id="sidebar">
            <div class="nav-sub-header">Overview</div>
            <a href="#introduction" class="nav-link active">Introduction</a>
            <a href="#features" class="nav-link">Key Features</a>
            
            <div class="nav-sub-header">Hardware</div>
            <a href="#pinout" class="nav-link">Pinout Guide</a>
            <a href="#specs" class="nav-link">Specifications</a>
            
            <div class="nav-sub-header">Development</div>
            <a href="#setup" class="nav-link">Setup Guide</a>
            <a href="#blink" class="nav-link">First Program: Blink</a>
            <a href="#wifi" class="nav-link">WiFi Connection</a>

            <div class="nav-sub-header">Resources</div>
            <a href="#datasheet" class="nav-link">Datasheets</a>
        </aside>

        <!-- Main Content Area -->
        <main class="main-content">
            
            <!-- Hero / Intro Section -->
            <section id="introduction">
                <h1>ESP32 Documentation</h1>
                <p style="font-size: 1.15rem; color: var(--text-main);">
                    The ESP32 is a series of low-cost, low-power system on a chip microcontrollers with integrated Wi-Fi and dual-mode Bluetooth. This guide covers everything from pinouts to your first "Hello World".
                </p>
                <div style="margin-top: 20px;">
                    <a href="#setup" class="btn btn-primary">Get Started</a>
                    <a href="#pinout" class="btn btn-outline" style="margin-left: 10px;">View Pinout</a>
                </div>
            </section>

            <!-- Features Grid -->
            <section id="features">
                <h2>Key Features</h2>
                <div class="feature-grid">
                    <article class="feature-card">
                        <div class="feature-icon">📶</div>
                        <h3>Dual Connectivity</h3>
                        <p>Features state-of-the-art 2.4 GHz Wi-Fi (802.11b/g/n) and Classic Bluetooth + BLE (4.2).</p>
                    </article>
                    <article class="feature-card">
                        <div class="feature-icon">⚡</div>
                        <h3>High Performance</h3>
                        <p>Dual-core microprocessor (Xtensa LX6) running at 240MHz, capable of handling complex tasks.</p>
                    </article>
                    <article class="feature-card">
                        <div class="feature-icon">🔌</div>
                        <h3>Rich Peripherals</h3>
                        <p>12-bit SAR ADC up to 18 channels, 2× 8-bit DACs, 10× touch sensors, temperature sensor, and more.</p>
                    </article>
                    <article class="feature-card">
                        <div class="feature-icon">🔋</div>
                        <h3>Low Power</h3>
                        <p>Fine-grained clock gating and multiple power modes to meet the requirements of power-sensitive applications.</p>
                    </article>
                </div>
            </section>

            <!-- Pinout Section -->
            <section id="pinout">
                <h2>Pinout Guide</h2>
                <p>The ESP32 DevKit V1 has 30 pins. Below are the most important GPIOs to know for your projects.</p>
                
                <div class="alert alert-warning">
                    <strong>Note:</strong> Avoid using GPIO 0, 2, 12, and 15 during boot if you are not sure what you are doing, as they can affect the boot mode.
                </div>

                <div style="overflow-x: auto;">
                    <table class="pinout-table">
                        <thead>
                            <tr>
                                <th>Pin Name</th>
                                <th>GPIO Number</th>
                                <th>Typical Function</th>
                                <th>Notes</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td><span class="chip-pin">VP</span></td>
                                <td>36</td>
                                <td>ADC1_CH0</td>
                                <td>Input Only</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">VN</span></td>
                                <td>39</td>
                                <td>ADC1_CH3</td>
                                <td>Input Only</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D34</span></td>
                                <td>34</td>
                                <td>ADC1_CH6</td>
                                <td>Input Only</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D35</span></td>
                                <td>35</td>
                                <td>ADC1_CH7</td>
                                <td>Input Only</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D4</span></td>
                                <td>4</td>
                                <td>ADC2_CH0</td>
                                <td>Connected to onboard LED</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D5</span></td>
                                <td>5</td>
                                <td>SS / SPI_CS</td>
                                <td>Boot strapping pin</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">SD3</span></td>
                                <td>10</td>
                                <td>GPIO 10</td>
                                <td>Used for Flash memory</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">SD2</span></td>
                                <td>9</td>
                                <td>GPIO 9</td>
                                <td>Used for Flash memory</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">RX2</span></td>
                                <td>16</td>
                                <td>UART2 RX</td>
                                <td>General Purpose</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">TX2</span></td>
                                <td>17</td>
                                <td>UART2 TX</td>
                                <td>General Purpose</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D22</span></td>
                                <td>22</td>
                                <td>I2C SCL</td>
                                <td>Default I2C Clock</td>
                            </tr>
                            <tr>
                                <td><span class="chip-pin">D21</span></td>
                                <td>21</td>
                                <td>I2C SDA</td>
                                <td>Default I2C Data</td>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </section>

            <!-- Setup Section -->
            <section id="setup">
                <h2>Arduino IDE Setup</h2>
                <p>To get started with the ESP32 using the familiar Arduino environment, follow these steps:</p>

                <h3>1. Install Arduino IDE</h3>
                <p>Download and install the latest version of the <a href="#" style="color:var(--primary); text-decoration:underline;">Arduino IDE</a>.</p>

                <h3>2. Add ESP32 Board Manager URL</h3>
                <p>Open Preferences (<code>File > Preferences</code> or <code>Arduino > Settings</code>) and add the following URL to "Additional Boards Manager URLs":</p>
                
                <div class="code-block">
                    <button class="copy-btn" onclick="copyCode(this)">Copy</button>
                    <pre>https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json</pre>
                </div>

                <h3>3. Install the Board</h3>
                <p>Go to <code>Tools > Board > Boards Manager...</code>. Search for "esp32" and install the package by Espressif Systems.</p>
            </section>

            <!-- Code Example: Blink -->
            <section id="blink">
                <h2>First Program: Blink</h2>
                <p>The onboard LED on the ESP32 DevKit is usually connected to <strong>GPIO 2</strong>.</p>
                
                <div class="code-block">
                    <button class="copy-btn" onclick="copyCode(this)">Copy</button>
                    <pre>
<span class="syntax-com">// Define the LED pin</span>
<span class="syntax-kwd">const int</span> ledPin = <span class="syntax-num">2</span>;

<span class="syntax-kwd">void</span> <span class="syntax-fn">setup</span>() {
  <span class="syntax-com">// Initialize the digital pin as an output</span>
  <span class="syntax-fn">pinMode</span>(ledPin, <span class="syntax-kwd">OUTPUT</span>);
}

<span class="syntax-kwd">void</span> <span class="syntax-fn">loop</span>() {
  <span class="syntax-fn">digitalWrite</span>(ledPin, <span class="syntax-kwd">HIGH</span>);  <span class="syntax-com">// Turn the LED on (HIGH is the voltage level)</span>
  <span class="syntax-fn">delay</span>(<span class="syntax-num">1000</span>);                      <span class="syntax-com">// Wait for a second</span>
  <span class="syntax-fn">digitalWrite</span>(ledPin, <span class="syntax-kwd">LOW</span>);   <span class="syntax-com">// Turn the LED off by making the voltage LOW</span>
  <span class="syntax-fn">delay</span>(<span class="syntax-num">1000</span>);                      <span class="syntax-com">// Wait for a second</span>
}
                    </pre>
                </div>
            </section>

            <!-- Code Example: WiFi -->
            <section id="wifi">
                <h2>Connecting to WiFi</h2>
                <div class="alert alert-info">
                    <strong>Library:</strong> Ensure you have the <code>WiFi.h</code> library included, which comes pre-installed with the ESP32 board package.
                </div>

                <div class="code-block">
                    <button class="copy-btn" onclick="copyCode(this)">Copy</button>
                    <pre>
<span class="syntax-kwd">#include</span> <span class="syntax-str">&lt;WiFi.h&gt;</span>

<span class="syntax-kwd">const char</span>* ssid = <span class="syntax-str">"YOUR_SSID"</span>;
<span class="syntax-kwd">const char</span>* password = <span class="syntax-str">"YOUR_PASSWORD"</span>;

<span class="syntax-kwd">void</span> <span class="syntax-fn">setup</span>() {
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">begin</span>(<span class="syntax-num">115200</span>);
  
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">print</span>(<span class="syntax-str">"Connecting to "</span>);
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">println</span>(ssid);

  <span class="syntax-fn">WiFi</span>.<span class="syntax-fn">begin</span>(ssid, password);

  <span class="syntax-kwd">while</span> (<span class="syntax-fn">WiFi</span>.<span class="syntax-fn">status</span>() != <span class="syntax-kwd">WL_CONNECTED</span>) {
    <span class="syntax-fn">delay</span>(<span class="syntax-num">500</span>);
    <span class="syntax-fn">Serial</span>.<span class="syntax-fn">print</span>(<span class="syntax-str">"."</span>);
  }

  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">println</span>(<span class="syntax-str">""</span>);
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">println</span>(<span class="syntax-str">"WiFi connected."</span>);
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">println</span>(<span class="syntax-str">"IP address: "</span>);
  <span class="syntax-fn">Serial</span>.<span class="syntax-fn">println</span>(<span class="syntax-fn">WiFi</span>.<span class="syntax-fn">localIP</span>());
}

<span class="syntax-kwd">void</span> <span class="syntax-fn">loop</span>() {}
                    </pre>
                </div>
            </section>

             <!-- Resources Section -->
             <section id="datasheet">
                <h2>Technical Resources</h2>
                <ul style="list-style: disc; padding-left: 20px; color: #4a5568;">
                    <li style="margin-bottom: 10px;"><a href="#" style="text-decoration: underline;">ESP32 Datasheet (PDF)</a> - Full technical specifications.</li>
                    <li style="margin-bottom: 10px;"><a href="#" style="text-decoration: underline;">Espressif Documentation Portal</a> - Official API reference.</li>
                    <li style="margin-bottom: 10px;"><a href="#" style="text-decoration: underline;">Schematics & Pinouts</a> - Hardware reference designs.</li>
                </ul>
            </section>

        </main>
    </div>

    <footer>
        <p>&copy; 2023 ESP32 Documentation Showcase. Designed for Community.</p>
    </footer>

    <!-- Toast Notification Element -->
    <div id="toast" class="toast">Code copied to clipboard!</div>

    <script>
        // --- 1. Sidebar Toggle (Mobile) ---
        function toggleSidebar() {
            const sidebar = document.getElementById('sidebar');
            sidebar.classList.toggle('active');
        }

        // --- 2. Copy Code Functionality ---
        function copyCode(button) {
            // Find the pre element next to the button
            const codeBlock = button.nextElementSibling;
            const codeText = codeBlock.innerText;

            // Use Clipboard API
            navigator.clipboard.writeText(codeText).then(() => {
                showToast("Code copied to clipboard!");
            }).catch(err => {
                showToast("Failed to copy code");
                console.error('Async: Could not copy text: ', err);
            });
        }

        // --- 3. Toast Notification Logic ---
        function showToast(message) {
            const toast = document.getElementById('toast');
            toast.innerText = message;
            toast.classList.add('show');
            
            // Hide after 3 seconds
            setTimeout(() => {
                toast.classList.remove('show');
            }, 3000);
        }

        // --- 4. Active Link Highlighter on Scroll ---
        document.addEventListener('DOMContentLoaded', () => {
            const sections = document.querySelectorAll('section');
            const navLinks = document.querySelectorAll('.nav-link');

            // Use Intersection Observer for better performance than scroll events
            const observerOptions = {
                root: null,
                rootMargin: '-20% 0px -60% 0px', // Trigger when section is near top
                threshold: 0
            };

            const observer = new IntersectionObserver((entries) => {
                entries.forEach(entry => {
                    if (entry.isIntersecting) {
                        // Remove active class from all links
                        navLinks.forEach(link => {
                            link.classList.remove('active');
                            // Collapse mobile menu if clicking, but for scroll we just highlight
                        });

                        // Add active class to corresponding link
                        const id = entry.target.getAttribute('id');
                        const activeLink = document.querySelector(`.nav-link[href="#${id}"]`);
                        if (activeLink) {
                            activeLink.classList.add('active');
                        }
                    }
                });
            }, observerOptions);

            sections.forEach(section => {
                observer.observe(section);
            });
        });
    </script>
</body>
</html>
)rawliteral";

//diplay func
void updateOLED() {
  display.clearDisplay();
  

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("ESP32 Web Server");

  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(0, 16);
  display.print("WiFi: ");
  display.println(displayStatus);

  display.setCursor(0, 26);
  display.println("IP: " + displayIP);

 
  display.setCursor(0, 42);
  display.setTextSize(1);
  display.print("Host LED: ");
  
  

  display.setCursor(0, 56);
  display.setTextSize(1);
  display.print("Status: OK");

  display.display();
}



void handleRoot() {
  String html = String(index_html);
  html.replace("%STATE%", displayLED);
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  displayLED = "ON";
  updateOLED(); 
  handleRoot();
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  displayLED = "OFF";
  updateOLED(); 
  handleRoot();
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("OLED Initialized");

  display.clearDisplay();
  display.display();
  

  displayStatus = "Connecting...";
  updateOLED();


  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    displayStatus = (attempts % 2 == 0) ? "Connecting." : "Connecting..";
    updateOLED();
    attempts++;
  }


  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  displayStatus = "Connected";
  displayIP = WiFi.localIP().toString();
  updateOLED();

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
