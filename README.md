# The Ruin Machine

### ESP32-Based Gambling Simulator

<img src="https://raw.githubusercontent.com/Arceus-Labs/The-Ruin-Machine/main/Assets/TheRuinMachineCover.png" alt="The Ruin Machine Cover" width="600"/>

[![ESP32](https://img.shields.io/badge/ESP32-Dev%20Module-E7352C?style=flat-square&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D?style=flat-square&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-success?style=flat-square)]()

A physical gambling simulator that proves all betting strategies lose to math. Educational tool for teaching probability theory through hands-on experience.

[Web Version](https://github.com/Arceus-Labs/TRM-Web)

## About

Think you can beat the house? This device will prove you wrong. The Ruin Machine is a handheld ESP32-based gambling simulator that demonstrates through real-time gameplay why every betting strategy eventually fails against the house edge.

Press a button, watch your virtual bankroll disappear, and learn why mathematics is undefeated. Perfect for teaching probability theory, expected value, and the gambler's ruin theorem.

<table>
<tr>
<td width="50%">

### Key Features

- Real-time Gambling Simulation - True random using ESP32 hardware RNG
- OLED Display - Live bankroll, stats & win rate
- Audio Feedback - Buzzer for wins, losses & alerts
- Multiple Strategies - Martingale & Flat Bet modes
- Quick Reset - Hold button 5s to restart
- Non-blocking Code - Smooth responsive operation

</td>
<td width="50%">

### What It Demonstrates

- House edge always wins long-term
- Martingale strategy fails exponentially
- Win rate converges to 49.5%
- Gambler's Ruin theorem in action
- Expected Value calculations
- Law of Large Numbers

</td>
</tr>
</table>

## Hardware Components

<details>
<summary><b>Click to expand full components list</b></summary>

| Component | Model/Type | Qty | Purpose |
|:----------|:-----------|:---:|:--------|
| Microcontroller | ESP32 DevKit V1 | 1 | Main processing unit |
| Display | 0.96" OLED SSD1306 | 1 | Real-time visualization |
| Buzzer | Active Buzzer 5V | 1 | Audio feedback |
| Push Buttons | 6mm Tactile | 2 | User input (BET & MODE) |
| Resistors | 10kΩ | 2 | Button pull-ups (optional) |

</details>

## Pin Configuration

```
┌──────────────────────────────────────────────────────────────┐
│                      ESP32 Pin Mapping                       │
├─────────────────┬─────────────┬──────────────────────────────┤
│    Component    │   GPIO Pin  │            Notes             │
├─────────────────┼─────────────┼──────────────────────────────┤
│ I2C SDA (OLED)  │     21      │ Default I2C Data             │
│ I2C SCL (OLED)  │     22      │ Default I2C Clock            │
│ BET Button      │     13      │ Internal pull-up enabled     │
│ MODE Button     │     12      │ Internal pull-up enabled     │
│ Buzzer          │      4      │ Active buzzer                │
└─────────────────┴─────────────┴──────────────────────────────┘
```

## The Math

<details>
<summary><b>Click to expand mathematical proof</b></summary>

### House Edge

```
Win Probability: 49.5%
Loss Probability: 50.5%
House Edge: 1%
```

### Expected Value

```
EV = (0.495 × ₹100) - (0.505 × ₹100)
EV = -₹1 per bet

Over 1000 bets → Expected loss = ₹1,000
```

### Martingale Fallacy

| Consecutive Losses | Required Bet | Total Risk | Potential Profit |
|:------------------:|:------------:|:----------:|:----------------:|
| 5 | ₹3,200 | ₹6,300 | ₹100 |
| 10 | ₹102,400 | ₹204,700 | ₹100 |

Risk everything to win ₹100

</details>

## Usage

### Button Controls

| Button | Action | Function |
|:------:|:-------|:---------|
| BET | Press | Place a bet and flip |
| MODE | Tap | Switch strategy (Martingale ↔ Flat) |
| MODE | Hold 5s | Reset game to starting bankroll |

### Betting Strategies

| Strategy | Code | Description |
|:--------:|:----:|:------------|
| Martingale | `[M]` | Double bet after each loss |
| Flat Bet | `[F]` | Constant ₹100 bet |

### Display Information

```
=RUIN MACHINE=
Rs.10000          ← Current bankroll
Games:0           ← Games played & win rate
Bet: Rs.100       ← Current bet amount
W:0 L:0 [M]       ← Win/Loss streaks & strategy
```

### Buzzer Feedback

| Sound | Meaning |
|:-----:|:--------|
| High-Low ascending | WIN |
| Low descending | LOSS |
| Triple low beep | BANKRUPTCY |
| Quick double beep | Strategy changed |

## Quick Start

### Install Libraries

```
Required Arduino Libraries:
├── Adafruit SSD1306
├── Adafruit GFX Library
└── Wire (built-in)
```

### Upload to ESP32

1. Open `TheRuinMachine.ino` in Arduino IDE
2. Select **Board**: `ESP32 Dev Module`
3. Select correct **Port**
4. Click **Upload**

### Wire Components

Connect OLED to I2C (SDA→21, SCL→22), buttons to GPIO 12 & 13, buzzer to GPIO 4.

## Bill of Materials

| Component | Qty | Est. Cost |
|:----------|:---:|----------:|
| ESP32 DevKit V1 | 1 | $5.00 |
| SSD1306 OLED 0.96" | 1 | $4.00 |
| Active Buzzer | 1 | $0.50 |
| Tactile Buttons | 2 | $0.20 |
| Breadboard + Wires | 1 set | $3.00 |

### Total Estimated Cost: ~$13

## Links

| Resource | Link |
|:--------:|:----:|
| GitHub | [Arceus-Labs/The-Ruin-Machine](https://github.com/Arceus-Labs/The-Ruin-Machine) |
| Web Version | [Arceus-Labs/TRM-Web](https://github.com/Arceus-Labs/TRM-Web) |
| Slide Deck | [Google Drive](https://drive.google.com/file/d/19qsP3uPnF_kbo4SKc0cpCnif0oX9e3MH/view?usp=sharing) |
| Overview Video | [Google Drive](https://drive.google.com/file/d/1EbXJda__bJuiQr_99y3YCqOkgEXG2dLL/view?usp=sharing) |
| Report | [Notion](https://crocus-zenobia-863.notion.site/The-Ruin-Machine-Technical-Report-2dc1ebfe2064806a9625f49a9871aaf3?pvs=73) |

## Disclaimer

This is an educational tool demonstrating probability theory. If you or someone you know has a gambling problem, please seek help.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Made with mathematical certainty by [Arceus Labs](https://github.com/Arceus-Labs)

*Remember: The house always wins. Math is undefeated.*
