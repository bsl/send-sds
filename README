libmidisd is a C library that uses alsa and libsndfile
to interface your Linux system with devices supporting
the MIDI Sample Dump Standard.
Right now it is a just a command-line tool to get the
basic functionality tested and working. Eventually I will
package it as a shared library.
This tool was born out of frustration with the lack
of SDS send/receive support on Linux. Much of the code is
taken directly from [bsl/send-sds][1].

### Supported Devices
* Elektron Machinedrum SPS-1 UW (OS 1.63)

### Dependencies
* gcc
* make
* libasound
* [libsndfile][2]

### Commmand Line Interface
##### List MIDI interfaces
    $ midisd iflist
    Dir Device    Name
    IO  hw:1,0,0  Elektron TM-1 MIDI 1

[1]: http://github.com/bsl/send-sds/
[2]: http://www.mega-nerd.com/libsndfile/
