# SSH-Ramdisk-tool

An ssh ramdisk tool that lets you boot the restoreramdisk for your specified ios/ipados version (FOR CHECKM8 COMPATIBLE DEVICES ONLY!)

```
For this to work you need macOS catalina and higher, and  xcode installed!!!

I tried linux but unfortunately kernel64patcher uses a library that is only found in mac. So it doesn't work on linux.
```

I'm still working on the ssh part so if anyone got any suggestions feel free to tell me!
It seems A8 and A9 have a problem booting ramdisk.

Compiling:
```c++
g++ main.cc -o RamdiskMaker -std=c++11
```

Usage:
```c++
Usage:
    -d <identifier>
    -i <version>
    -b <board>
    -s <blob>
    -p <pwn the device and remove sigchecks>
```
If you have issues with the tool just tell me i will try and fix it :)

(I know the code for dependencies is crappy im doing my best learning c++ :) )

Thanks to everyone whose dependencies im using :)

