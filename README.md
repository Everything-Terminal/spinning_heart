# ♥ Terminal Spinning Heart

A beautiful 3D spinning and beating heart rendered entirely in the terminal using pure C.

This project uses a classic implicit heart surface equation, ray casting, diffuse lighting, rim lighting, and ANSI truecolor escape codes to create a smooth animated heart with no external graphics libraries.

---

## 🎥 Watch the Build

See the full ASMR coding session where I build this from scratch:

[![Watch on YouTube](https://img.shields.io/badge/Watch%20on-YouTube-red?style=for-the-badge&logo=youtube)](https://youtu.be/RtVUp827Uyk)

---

## ✨ Features

- ♥ Real 3D heart shape using an implicit surface equation
- 🔄 Smooth spinning animation
- 💓 Gentle heartbeat pulse
- 💡 Diffuse lighting for depth
- ✨ Rim lighting for edge glow
- 🌈 Truecolor gradient from dark red to bright pink
- 🖥 Flicker-free terminal rendering
- 🧹 Clean exit with terminal restoration
- ⚙️ No external graphics libraries

---

## 📸 What It Does

The program renders a 3D heart in the terminal by checking whether each screen cell intersects the heart surface.

For every pixel, it:

1. Casts a ray into the scene
2. Rotates the ray based on the current spin angle
3. Checks the implicit heart equation
4. Approximates the surface normal
5. Applies diffuse and rim lighting
6. Renders the cell using a truecolor red/pink gradient

The heart also gently pulses like a heartbeat using a sine wave.

---

## 🧱 Requirements

- Linux or macOS terminal
- GCC
- Math library (`-lm`)
- Truecolor-capable terminal

Most modern terminals support truecolor, including GNOME Terminal, Kitty, Alacritty, WezTerm, and tmux with truecolor support.

---

## 🚀 Build

Compile:

```bash
gcc -std=c11 -Wall -Wextra spinning_heart.c -o spinning_heart -lm
```

Or with GNU extensions:

```bash
gcc -std=gnu11 -Wall -Wextra spinning_heart.c -o spinning_heart -lm
```

---

## 🎮 Run

```bash
./spinning_heart
```

---

## 🕹 Controls

| Input    | Action                    |
| -------- | ------------------------- |
| `Ctrl+C` | Quit and restore terminal |

---

## 🧠 How It Works

The heart is defined using the classic implicit heart surface:

```text
(x² + 9/4 y² + z² − 1)³ − x² z³ − 9/80 y² z³ = 0
```

A point is inside the heart when this value is less than or equal to zero.

The renderer uses:

- Ray casting through each terminal cell
- Y-axis rotation for the spinning effect
- Central differences to estimate the surface normal
- Diffuse lighting for shading
- Rim lighting to highlight the edges
- ANSI truecolor escape codes for smooth red/pink gradients
- `usleep()` to control frame timing
- `sigaction()` to handle `Ctrl+C` cleanly

---

## 🎨 Colors

The heart uses truecolor ANSI backgrounds:

- Shadow: dark red around `rgb(90, 10, 30)`
- Midtone: warm red
- Highlight: bright pink around `rgb(255, 50, 100)`

The terminal background remains empty space for contrast.

---

## ⚙️ Tuning

You can adjust these values inside `spinning_heart.c`:

| Value             | Effect                     |
| ----------------- | -------------------------- |
| `W`               | Terminal width             |
| `H`               | Terminal height            |
| `A += 0.02f`      | Spin speed                 |
| `T += 0.05f`      | Heartbeat clock speed      |
| `0.06f` in `beat` | Heartbeat strength         |
| `usleep(35000)`   | Frame rate, roughly 28 fps |

---

## 📁 Project Structure

```text
terminal-spinning-heart/
├── spinning_heart.c
├── README.md
└── .gitignore
```

---

## 🛠 Possible Future Improvements

- Adjustable size
- Different color themes
- Faster/slower heartbeat mode
- Custom message under the heart
- Smooth terminal resize support
- Point-cloud rendering mode
- Multiple hearts
- Music-synced heartbeat
