<div align="center">
  <h2>yasb4arm</h2>
  <h4>Yet Another Secure Bootloader for ARM micro-Controllers</h4>
</div>
## About

yasb4arm is a secure bootloader project designed for arm core microcontrollers, initally developed for STM32 family of microcontrollers.
This project is a collection of 3 projects; one for bootloader firmware, one for image creation and signing of binaries, one for flash utility.

## yasb4arm-Bootlader

This is the core of yasb4arm.
After power up (HW and SW resets), bootloader checks for a valid signed  boot image. If exists, it initializes an auto-boot sequence. Otherwise it automatically falls into boot command shell.
It is also possible to interrupt auto-boot procedure to jump into boot command shell.

## yasb4arm-ImageGenerator

This project generates encripted and criptographically signed boot images from intel-hex formatted binaries.

## yasb4arm-FlashUtil

This project is an example project to burn flash with signed images. Its purpose is to demonstrate communication via Bootloader's boot command shell. You can (should) use this project as an example/tutorial for your own Flash Utility

## Requirements

- [AIR Embedded Workbench for ARM 6.50 or later](https://www.iar.com/iar-embedded-workbench/)
- [Visual Studio 2013 or later](https://www.visualstudio.com/)

---

> GitHub [@u-onder](https://github.com/u-onder)
