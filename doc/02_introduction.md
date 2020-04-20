[<< back](01_index.md) | [index](01_index.md) | [forward >> ](03_setup.md)

# Introduction

## Focus

This tutorial focuses on writing Linux drivers for  new peripherals (in this case, an FPGA IP-Core). It does not focus on the FPGA design flow in general or how to build a complete embedded Linux. You may find some documentation covering these points in the [references](99_references.md).

## Improvements

I am not a black-belt Linux developer myself. So the code may leave some room for improvements. Let me know if you see things that can be improved, I will be happy to integrate your suggestions.

## Hardware

This tutorial was written using an [Enclustra ZX5 FPGA module](https://www.enclustra.com/en/products/system-on-chip-modules/mercury-zx5/) on an [Enclustra PE1 base board](https://www.enclustra.com/en/products/base-boards/mercury-pe1-200-300-400/). However, it will work very similar for any Enclustra module. For completely  different HW, the code is still usable but the descriptions in the tutorial may not fit.

## IP-Core

This tutorial is about writing a Linux driver, not about implementing a Vivado IP-Core. Therefore we will used a pre-packaged and very simple IP-Core that is available open source from PSI [5]. This IP-Core stores some version information along with build-dates. So it basically only contains a few registers to read/write and nothing else. Therefore it is ideal for simple examples.



[[<< back](01_index.md) | index](01_index.md) | [forward >> ](03_setup.md)