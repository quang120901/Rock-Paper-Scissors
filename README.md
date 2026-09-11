# 🪨📄✂️ Rock Paper Scissors — ESP32 IoT Game

A physical Rock-Paper-Scissors machine powered by an ESP32: players pick their move through a **web interface**, the bot picks randomly, and the outcome is revealed simultaneously through **3 servos**, **5 LEDs**, a **20x4 I2C LCD**, and a **buzzer**.

## ✨ Features

- 🌐 Web UI (open from any browser on the same network) to choose Rock / Paper / Scissors
- 🤖 Bot randomly picks one of the three moves
- 🦾 3 servos do a short "shake" build-up for suspense before revealing the bot's move — just like the real hand gesture
- 💡 5 LEDs visually track the cumulative win / lose / tie count
- 🖥️ 20x4 I2C LCD shows the round number, the bot's move, the result, and the running score
- 🔊 Buzzer plays a distinct jingle when the match ends (win / lose / tie)
- 🏆 A match ends once either side reaches 2 wins; a "Play Again" button resets everything

## 🔌 Hardware & Wiring

| Component | ESP32 Pin | Role |
|---|---|---|
| Servo 1 (Rock) | GPIO 14 | Raises up when the bot picks Rock |
| Servo 2 (Paper) | GPIO 27 | Raises up when the bot picks Paper |
| Servo 3 (Scissors) | GPIO 25 | Raises up when the bot picks Scissors |
| Red LEDs x2 | GPIO 4, 17 | Win counter (1 LED = 1 point) |
| Yellow LEDs x2 | GPIO 18, 19 | Loss counter |
| Cyan LED | GPIO 5 | Tie counter |
| LCD 20x4 (I2C) | SDA: GPIO 33 · SCL: GPIO 32 | Shows round-by-round progress |
| Buzzer | GPIO 22 | Plays a jingle at match end |

The full wiring is defined in [`diagram.json`](./diagram.json) — open it directly in [Wokwi](https://wokwi.com) for a visual view of the circuit.

## ⚙️ How It Works

1. On boot, the ESP32 mounts **SPIFFS** (serving `index.html`, `style.css`, and `jquery.min.js` straight from flash storage) and connects to WiFi, running an `ESPAsyncWebServer` at `/`.
2. The player picks a move from the dropdown and clicks **Play** → the page runs a short "Rock... Paper... Scissors... Shoot!" countdown, then sends `GET /play?sspb=<move>`.
3. The server randomly picks the bot's move, rotates the matching servo, updates the LCD, computes the round result, and returns JSON.
4. The web page reads the JSON and adds a new row to the results table (color-coded by outcome).
5. Once either side reaches 2 wins: the LCD shows "GAME OVER", the buzzer plays a jingle, and the page shows a final banner with a "Play Again" button (calls `/reset`).

## 📁 Project Structure

```
include/
├── RPS.h              # Game logic + servo/LED control
├── Display.h           # LCD wrapper (formatting helpers)
├── Buzzer.h             # Buzzer wrapper (bit-banged, no LEDC/tone())
├── WebRoutes.h          # HTTP route declarations
├── LED.h                # Small LED helper class
├── Wifi.h               # WiFi connection + AsyncWebServer instance
└── Credentials.h        # WiFi SSID / password declarations

src/
├── main.cpp             # setup()/loop(), wires all modules together, mounts SPIFFS
├── RPS.cpp
├── Display.cpp
├── Buzzer.cpp
├── WebRoutes.cpp        # All HTTP routes (/, /style.css, /jquery.min.js, /play, /reset...)
├── LED.cpp
├── Wifi.cpp
└── Credentials.cpp

data/
├── index.html           # Web UI (uploaded to SPIFFS)
├── style.css            # Web UI styling (uploaded to SPIFFS)
└── jquery.min.js         # jQuery library, served locally from SPIFFS

diagram.json              # Wokwi circuit diagram
wokwi.toml                # Wokwi simulator config (firmware + port forwarding)
platformio.ini             # PlatformIO build configuration
partitions.csv              # Custom ESP32 partition table (OTA + SPIFFS)
merge_firmware.py            # Build script — see "Building & Running" below
```

## 🧩 Custom Partition Table

This project uses a custom `partitions.csv` instead of the default scheme, so it has room for both OTA updates and a large enough SPIFFS partition for the web assets:

| Partition | Offset | Size | Purpose |
|---|---|---|---|
| `nvs` | `0x9000` | 20 KB | Non-volatile storage (WiFi config, etc.) |
| `otadata` | `0xe000` | 8 KB | Tracks which OTA slot is active |
| `app0` | `0x10000` | 1280 KB | Main firmware slot |
| `app1` | `0x150000` | 1280 KB | Secondary OTA slot |
| `spiffs` | `0x290000` | 1472 KB | Web assets (`index.html`, `style.css`, `jquery.min.js`) |

## 🏗️ Building & Running

This project targets the Wokwi simulator (VS Code extension), which needs a **single merged binary** containing the bootloader, partition table, firmware, and the SPIFFS filesystem image — PlatformIO doesn't produce that automatically. `merge_firmware.py` handles this:

1. Registered in `platformio.ini` as `extra_scripts = post:merge_firmware.py`, so it runs automatically after every build.
2. It rebuilds the SPIFFS filesystem image from `data/` (`pio run --target buildfs`), so the image always matches whatever is currently in `data/`.
3. It merges `bootloader.bin` + `partitions.bin` + `firmware.bin` + `spiffs.bin` into a single `merged-flash.bin` at the project root, using `esptool.py merge_bin`.

So the full workflow is simply:

```bash
pio run -e az-delivery-devkit-v4
```

No need to run `buildfs` or `merge_bin` manually — this single command handles everything and produces an up-to-date `merged-flash.bin`.

`wokwi.toml` then points the simulator at that merged binary, and forwards the ESP32's virtual web server to your local machine:

```toml
[wokwi]
version = 1
firmware = "merged-flash.bin"
elf = ".pio/build/az-delivery-devkit-v4/firmware.elf"

[[net.forward]]
from = "localhost:8180"
to = "target:80"
```

To try it out:
1. Open the project in VS Code with the PlatformIO and Wokwi extensions installed.
2. Build the project (`pio run`, or the PlatformIO Build button).
3. Run **"Wokwi: Start Simulator"** from the Command Palette.
4. Wait for the Serial Monitor to print an IP address — the ESP32 has connected to the virtual WiFi.
5. Open **http://localhost:8180** in your browser to play.

## 🎬 See It In Action

A short recording of the full experience: picking a move on the web page → countdown → servos shaking and revealing the move → LCD updating → LEDs lighting up → match-end jingle.

> 📺 **[Watch the demo video](YOUR_VIDEO_LINK_HERE)**

### 🕹️ Try It Live (No Hardware Needed)
Run the full simulation directly in your browser — no setup required:

> 👉 **[Open the live simulation on Wokwi](https://wokwi.com/projects/471594379588549633)**

## 🛠️ Required Libraries

- `esp32async/AsyncTCP`
- `esp32async/ESPAsyncWebServer`
- `madhephaestus/ESP32Servo`
- `marcoschwartz/LiquidCrystal_I2C`

## 📄 License

Personal / learning project — free to use, modify, and share.