# CyShock
## Overview

CyShock is a [VCV Rack](https://github.com/VCVRack/Rack) plugin that currently features 
only my [sine wave frequency/amplitude](https://github.com/kinetikeith/dstrack) tracking
algorithm. The algorithm was originally designed to do live pitch tracking of whistling,
but any signal with a strong enough fundamental frequency should be able to be tracked.

## Building
**This has currently been tested on linux, and not very extensively at that.**

### Linux

Set up your build environment as outlined [here](https://vcvrack.com/manual/Building#setting-up-your-development-environment).

To compile:
```
make dep
make dist
```

To install:
```
make dep
make install
```
