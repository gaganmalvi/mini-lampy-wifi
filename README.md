# Lampy: Unleash the Glow!

Welcome, curious creator! Lampy isn’t just a lamp; it transforms your desk into a realm of glowing rainbows, twinkling fireflies, and fiery infernos. The magic? Open-source love combined with some spicy, absurd creativity.

## 🌟 What Can Lampy Do?
Lampy creates jaw-dropping lighting effects, including:

- **🔥 Flickering Fires**: For those cozy rainy days.
- **🌈 Infinite Rainbows**: Pure magic.
- **🌟 Twinkling Fireflies**: Nature-inspired calmness.
- **🌊 Ocean Waves**: Flowing tranquility.
- **🌌 Shooting Stars**: Wish upon them, maybe?
- **🧵 Matrix Rain**: Get lost in code-inspired mystique.

Adjust speed, brightness, or custom settings directly via code tweaks. Whether you're a tinkerer, an artist, or a curious coder, Lampy lets you light the way your own way!

## 🛠️ Hardware Requirements
- ESP32 microcontroller (e.g., Xiao ESP32C3)
- WS2812B LED strip (72 LEDs)
- Power supply for the LEDs
- Breadboard and jumper wires for connections

## 🚀 Getting Started

### Hardware Setup
1. Connect the WS2812B LED strip to the ESP32:
   - Data pin to `D0`
   - Power and ground to a suitable 5V power supply.
2. Ensure the ESP32 is powered correctly and connected to your computer for programming.

### Software Setup
1. Install the following Arduino libraries:
   - `WiFi`
   - `ESPAsyncWebServer`
   - `FastLED`
   - `Preferences`
   - `ESPmDNS`
2. Upload the Lampy code to the ESP32 using the Arduino IDE.

## 🔧 First-Time Configuration

1. Power on the ESP32 and connect to the Wi-Fi hotspot named `Lampy-Hotspot` (password: `password123`).
2. Open a browser and navigate to `192.168.4.1` to access the Wi-Fi setup page.
3. Enter your Wi-Fi credentials and submit the form.
4. The ESP32 will reboot and connect to the specified Wi-Fi network.

## 🌐 Accessing Lampy

1. Once connected to the same network as the ESP32, access Lampy through `http://lampy.local`.
2. Use the web interface to control the LEDs.

## 🎛️ Controls and Features

### Web Interface
Lampy’s control panel is your gateway to creative lighting magic:

- **On/Off**: Toggle the LEDs on or off with smooth fade-in and fade-out effects.
- **Brightness**: Adjust the brightness using a slider.
- **Animation Modes**: Choose your vibe from the dropdown menu:
  - Fire
  - Rainbow
  - Firefly
  - Water
  - Matrix Rain
  - Aster
  - Mandarin
- **Direction**: Flip the direction of animations forward or backward.
- **Speed**: Adjust the speed of animations with a slider.

### Animation Details
1. **Fire**: Simulates cozy, flickering flames.
2. **Rainbow**: Creates a moving rainbow pattern that’s pure magic.
3. **Firefly**: Twinkling lights mimic nature's calm glow.
4. **Water**: Mimics tranquil, flowing waves.
5. **Matrix Rain**: Get lost in cascading green code.
6. **Aster**: A dynamic field of floral-inspired patterns.
7. **Mandarin**: Alternates between orange and green, like a mandarin orchard.

## 💡 Tips and Tricks
- **Brightness Tuning**: Experiment with the brightness slider to create the perfect ambiance.
- **Custom Animations**: Feeling adventurous? Add your own animation magic by editing the `.ino` file.
- **Seamless Transitions**: Lampy ensures smooth transitions between on/off states for a professional touch.

## ⚙️ Advanced Features
- **Automatic Network Reconnection**: Lampy automatically reconnects to your saved Wi-Fi network. If it can’t, it reverts to hotspot mode.
- **Modular Animations**: All animations are separate functions, making it easy to add your own creative effects.
- **Dynamic Control Server**: The control server runs a sleek Vue.js app styled with TailwindCSS for a modern look.

## 🚧 Todos and Cautions
- **Animation Migration**: The modes are super unstable and need to be migrated from [lampy GitHub repository](https://github.com/Absurd-Industries/lampy) to FastLED.
- **UI Stability**: The UI is unstable and will be fine-tuned over time (e.g., slider interaction needs smoothing).
- **Work-in-Progress**: The current experience is a placeholder for what’s possible. Please report issues, bugs, or improvement ideas!

## 🛡️ License
Lampy is powered by the **GPL v3 License**. Feel free to share, modify, and tinker as long as you keep the love open-source.

## 🎉 About absurd.industries
We’re all about bending rules and making technology FUN! Explore more of our absurd creations:

- 🌐 [Website](https://absurd.industries)
- 💬 [Discord Community](https://discord.gg/absurdindustries)
- 📸 [Instagram](https://instagram.com/absurd.industries)

Created with chaos, code, and a sprinkle of absurdity by absurd.industries!

