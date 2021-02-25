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

## Usage

### Sinusoid

Sinusoid performs frequency and amplitude tracking on any signal with a strong fundamental
sine wave.

Plug any signal that you want into the input marked **In**

Set the noise **Threshold** to the desired value. This will cause anything below the threshold
in the signal to be ignored.

The **Min** and **Max** Frequency Knobs can be used to select the range of frequencies to be
tracked.

The **Sticky** parameter can be toggled to determine whether the last sounding frequency is
held while there is silence.

The **Gain** parameter amplifies the **Amp** Output.
**Amp** is the estimated amplitude output.
**V/Oct** is the estimated frequency output.
