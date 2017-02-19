<div align="center">
  <h2>seeBoot</h2>
  <h4>Secure Encrypted Embedded Bootloader</h4>
  <h5>for</h5>
  <h4>micro-Controllers</h4>
</div>
## About

seeBoot is a secure and encrypted embedded bootloader project designed for microcontrollers, initally developed for STM32 family of microcontrollers.
This project is a collection of 3 projects; one for bootloader firmware, one for signing binaries and image creation, one for microcontroller flash utility.

## seeBoot-Bootloader

This is the core of seeBoot.
After power up (HW and SW resets), bootloader checks for a valid signed  boot image. If exists, it initializes an auto-boot sequence. Otherwise it automatically falls into boot command shell.
It is also possible to interrupt auto-boot procedure to jump into boot command shell.

## seeBoot-ImageGenerator

This project generates encrypted and criptographically signed boot images from intel-hex formatted binaries.

## seeBoot-FlashUtil

This project is an example project to burn flash with signed images. Its purpose is to demonstrate communication via Bootloader's boot command shell. You can (should) use this project as an example/tutorial for your own Flash Utility

## Requirements

- [AIR Embedded Workbench for ARM 6.50 or later](https://www.iar.com/iar-embedded-workbench/)
- [Visual Studio 2013 or later](https://www.visualstudio.com/)

---

> GitHub [@u-onder](https://github.com/u-onder)
