send-sds provides utilities for sending SDS (MIDI Sample Dump Standard) files to
samplers from Linux using the ALSA MIDI API.

[![Build Status](https://travis-ci.org/briansorahan/send-sds.png)](https://travis-ci.org/briansorahan/send-sds)

To convert an audio file to SDS, I use [libsndfile][1]'s sndfile-convert
utility. _WARNING_: use libsndfile 1.0.23 or later -- previous versions had
bugs that caused the creation of invalid SDS files.

    $ amidi -l
    Dir Device    Name
    IO  hw:1,0,0  Elektron TM-1 MIDI 1

    $ sndfile-convert foo.wav foo.sds

    $ send-sds
    send-sds 2010.10.23
    usage: <alsa-device> <channel-num> <sample-num> <sds-filename>

    $ send-sds hw:1,0,0 0 2 foo.sds

    $

[1]: http://www.mega-nerd.com/libsndfile/
