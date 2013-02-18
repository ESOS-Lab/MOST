Mobile Storage Analyzer
=======================

Written by Kisung Lee <kisunglee@hanyang.ac.kr> (based on e2fsprogs).

Reference LEE Kisung and WON Youjip 
"Smart layers and dumb result: Io characterization of an android-based smartphone" 
In EMSOFT 2012: In Proc. of International Conference on Embedded Software (Oct.7-12 2012). Tampere, Finland 

Kernel part (MOST/kernel)
----------------
* Kernel source is based on Linux kernel 2.6.35.7. (Defined by CONFIG_MOST)
* kernel\trace\blktrace.c
* block\blk-core.c


User program (MOST/progs/most/)
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
    
    
Example 
--------
    1. Run blktrace (for 60 secs)
        # blktrace -d /dev/block/mmcblk0p12 -a complete -a issue -o result -w 60
    2. Run android application (for 60 secs)
    3. Run blparse
        # blkparse -f "%5T.9%t\t%a\t%d\t%8S\t%4n\t%4p\t%20C\n" -q -i result > result.p
    4. Run MOST
        # most EXT4 /dev/block/mmcblk0p12 result.p result.po


2012-08-27, Kisung Lee <kisunglee@hanyang.ac.kr>
