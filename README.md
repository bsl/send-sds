# Overview

send-sds provides utilities for sending SDS (MIDI Sample Dump Standard) files to
samplers from Linux using the ALSA MIDI API.

[![Build Status](https://travis-ci.org/briansorahan/send-sds.png)](https://travis-ci.org/briansorahan/send-sds)

## Getting Started

### Install dependencies

#### Debian/Ubuntu/Mint systems

```shell
sudo apt-get update
sudo apt-get install libsndfile1-dev sndfile-tools libasound2-dev alsa-utils
```

### Build send-sds

```shell
make
```

### Install send-sds

```shell
sudo install send-sds /usr/local/bin
```

If you just type `send-sds` at the command line now you should see a brief help message:

```
send-sds 2010.10.23
usage: send-sds <alsa-device> <channel-num> <sample-num> <sds-filename>
```

## Creating SDS files

You must have `sndfile-tools` and `alsa-utils` already installed, since we will be using `sndfile-convert` and `amidi`.

### Convert a wav file

```shell
sndfile-convert foo.wav foo.sds
```

### List MIDI devices

```shell
amidi -l
```

I (used to) own an Elektron Machinedrum that I connected to my computer through the Elektron TurboMIDI interface.
`amidi` shows something like this with that setup:

```
Dir Device    Name
IO  hw:1,0,0  Elektron TM-1 MIDI 1
```

Take note of the `Device` column for the device you want to send an audio sample to.

### Send a mono SDS file to sample slot 2

```shell
send-sds hw:1,0,0 0 2 foo.sds
```

*Note* there is a small SDS file checked into this repo that you can use for testing.

If you want to test with this file, try:

```shell
$ send-sds hw:1,0,0 0 2 layered_finger_snap_mono.sds
```

[1]: http://www.mega-nerd.com/libsndfile/
