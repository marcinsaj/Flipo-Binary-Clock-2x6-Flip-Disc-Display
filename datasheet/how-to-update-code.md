## FOR INTERMEDIATE USERS ONLY! 
> [!TIP]
> The heart of the clock is the ATmega 328 microcontroller (default 16 MHz external crystal) - [schematic - in development]().
> The [clock code](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/tree/main/examples) was written in the Arduino IDE and uses [MiniCore](https://github.com/MCUdude/MiniCore).

> [!CAUTION]
> If you want to upload your own code, there is an ISP connector on board for programming. If you use a programmer that allows you to power the microcontroller to be programmed, all you have to do is turn off (slide switch) the clock during programming. If the programmer (e.g. Atmel-ICE ISP) requires an additional power supply for the microcontroller to be programmed, **the clock controller board _should be disconnected_ from the flip-disc display board during programming - this is very important because the flip-disc display may be damaged during programming**.
---
![](https://github.com/marcinsaj/Flipo-Binary-Clock-2x6-Flip-Disc-Display/blob/main/extras/flipo-2x6-flip-disc-binary-clock-isp-atmega328.webp)
