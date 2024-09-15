# Flipo-Binary-Clock-2x6-Flip-Disc-Display
<a href="https://www.buymeacoffee.com/marcinsaj"><img src="https://github.com/marcinsaj/marcinsaj/blob/main/Buy-me-a-coffee.png" /></a>

---
## Specification
- fully functional clock, just need to connect the power supply
- two buttons for time settings
- toggle power switch (on/off)
- accurate real-time clock (RTC) on board
- the RTC clock memory is backed up by a supercapacitor, so the clock does not require an additional battery in the event of a power failure or turn off
- ISP programming connector for intermediate users - more about it below in the description
- clock dimensions: 
- the clock requires a 12V 1.5A power supply
- [2x6 flip-disc display repository](https://github.com/marcinsaj/Flipo-2x6-Flip-Disc-Display)
- Project website - in development
---
> [!CAUTION]
> FOR INTERMEDIATE USERS ONLY! 
The heart of the clock is the ATmega 328 microcontroller (default 16 MHz external crystal). The [clock code](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/tree/main/examples) was written in the Arduino IDE and uses [MiniCore](https://github.com/MCUdude/MiniCore). On the clock board there is an ISP connector for programming. If you use a programmer that allows you to power the microcontroller to be programmed, all you have to do is turn off the clock during programming. If the programmer (e.g. Atmel-ICE ISP) requires an additional power supply for the microcontroller to be programmed, **the clock controller board _should be disconnected_ from the flip-disc display board during programming - this is very important because the flip-disc display may be damaged during programming**.
---

Binary Clock Controller PCB:
![Binary Clock Controller PCB](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/blob/main/extras/top-binary-clock-2x6-flip-disc-display-pcb.png)
![Binary Clock Controller PCB](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/blob/main/extras/bottom-binary-clock-2x6-flip-disc-display-pcb.png)

2x6 Flip-disc Display PCB:
![2x6 Flip-disc Display PCB](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/blob/main/extras/top-2x6-flip-disc-display-pcb.png)
![2x6 Flip-disc Display PCB](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/blob/main/extras/bottom-2x6-flip-disc-display-pcb.png)
