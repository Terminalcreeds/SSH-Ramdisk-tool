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

Options:
```c++
     -d device identifier e.g. iPad7,5
     -i iOS version e.g. 14.8
     -b boardconfig e.g. j71bAP
     -s SHSH Blob file
     -p Pwn the device and remove sigchecks
     -v Just boot the device verbose
     -c Boot the device with custom boot logo
```
If you have issues with the tool just tell me i will try and fix it :)

Note: you need to specify -v/-c after -d. -i, -b and -s. -p can be used apart. After -c opt drag n drop your logo. 

[ -v / -c will NOT boot ramdisk!! && If you selected -v opt keep in mind to specify the iOS version your device is on else it won't boot :) ]

I haven't tested the custom bootlogo one yet so it may not work.

Feel free to contribute on this project!

Thanks to everyone whose dependencies im using :)
