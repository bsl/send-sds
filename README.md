Forked from [bsl/send-sds][1].

libmidisds will one day be a cross-platform C library
for interfacing your Linux system with
devices supporting the MIDI Sample Dump Standard.
Right now it is a just a command-line tool.

### Dependencies
* gcc
* make
* libasound
* [libsndfile][2]

### Commmand Line Interface
##### List MIDI interfaces
    $ midisds iflist
    Dir Device    Name
    IO  hw:1,0,0  Elektron TM-1 MIDI 1
##### Send an SDS file
    $ midisds send hw:1,0,0 sample.{wav,flac,aiff,sds,...}

[1]: http://github.com/bsl/send-sds/
[2]: http://www.mega-nerd.com/libsndfile/
