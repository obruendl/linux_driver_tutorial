# General Information

## Maintainer
Oliver Bründler [oli.bruendler@gmx.ch]

## Changelog
See [Changelog](Changelog.md)

## Purpose of the Repository

When getting into Linux driver development, I had a quite hard time to find examples helping with the very first steps. Obviously most black-belt driver developers see these things as too easy to explain. 

I read the note "there are plenty of examples in the kernel" many times. Just that complex drivers that contain 5'000 lines of code are maybe not the very simplest way to enter the topic. Especially because for newbies it may not be so easy to find out what is generic and what is specific to one driver.

I built my very simplest example of different driver types and because this would have helped me a lot, I will make it available as tutorial. 

## Tutorial

To follow the turiorial, start with [./doc/01_index.md](./doc/01_index.md)

The tutorial is based on Enclustra FPGA modules. However, the code does not contain any platform specific parts. 

## Cloning
Because the repository contains submodules, it must be cloned with the *--recurse-submodules* option:

```
git clone --recurse-submodules https://github.com/obruendl/linux_driver_tutorial
```



