# Overview

send-sds provides utilities for sending SDS (MIDI Sample Dump Standard) files to
samplers from Linux using the ALSA MIDI API.

## Getting Started

### Install dependencies

#### Debian/Ubuntu/Mint systems

```shell
sudo apt-get update
sudo apt-get install libsndfile1-dev sndfile-tools libasound2-dev alsa-utils
```

#### Red Hat/CentOS/Fedora systems

```shell
sudo dnf install alsa-lib-devel libsndfile-devel

# For sndfile-convert on Fedora:
sudo dnf install libsndfile-utils
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

## Sending SDS files

### Create an SDS file from a wav file

You must have `sndfile-tools` and `alsa-utils` already installed, since we will be using `sndfile-convert` and `amidi`.

```shell
sndfile-convert foo.wav foo.sds
```

### List MIDI devices

Connect your Elektron Machinedrum to your computer via a MIDI interface and run:

```shell
amidi -l
```

Take note of the `Device` column for the device you want to send an audio sample to.

For example, if your Elektron Machinedrum is connected to the computer
via an Elektron TurboMIDI interface the output would look something like this:

```
Dir Device    Name
IO  hw:1,0,0  Elektron TM-1 MIDI 1
```

In this case, `hw:1,0,0` is the device you will send data to.


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
