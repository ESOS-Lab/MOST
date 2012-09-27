Mobile Storage Analyzer
----------------

Written by Kisung Lee <kisunglee@hanyang.ac.kr> (based on e2fsprogs).

Reference [Lee12] LEE, Kisung, and WON, Youjip 
"Smart layers and dumb result: Io characterization of an android-based smartphone" 
In EMSOFT 2012: In Proc. of International Conference on Embedded Software (Oct.7-12 2012). Tampere, Finland 

Kernel part (MOST/kernel)
----------------
* Kernel source is based on Linux kernel 2.6.35.7. (Defined by CONFIG_MOST)
* kernel\trace\blktrace.c
* block\blk-core.c


User program (MOST/progs/most.tar.gz)
----------------
Build
-----
* 1) cd build
* 2) ../configure
* 3) ./config.most
* 4) make
* 5) Program(most) is built in build/most/.


Usage
-----
    # most [filesystem (EXT4/FAT32)] [partition path] [input file] [output file]
    # most EXT4 /dev/block/mmcblk0p1 trace_in.log trace_out.log

2012-08-27, Kisung Lee <kisunglee@hanyang.ac.kr>
