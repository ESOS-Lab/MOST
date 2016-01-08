Mobile Storage Analyzer
=======================

Written by Kisung Lee <kisunglee@hanyang.ac.kr> (based on e2fsprogs).

### Reference: 
 * Kisung Lee and Youjip Won "Smart layers and dumb result: Io characterization of an android-based smartphone" 
In EMSOFT 2012: In Proc. of International Conference on Embedded Software (Oct.7-12 2012). Tampere, Finland 
<http://www.esos.hanyang.ac.kr/files/publication/conferences/international/Smart_Layers_and_Dumb_Result.pdf>

### Acknowledgement:
 * This work is supported by IT R&D program MKE/KEIT (No. 10041608, Embedded System Software for New-memory based Smart Device).

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
        # blkparse -f "%5T.%9t\t%a\t%d\t%8S\t%4n\t%4p\t%20C\n" -q -i result > result.p
    4. Run MOST
        # most EXT4 /dev/block/mmcblk0p12 result.p result.po


2012-08-27, Kisung Lee <kisunglee@hanyang.ac.kr>

MOST Viewer
==============

Requirement tools
-------------------
    1. python
        # apt-get install python
    2. R
        # apt-get install r-base

Usage
-------
    # python test2.py [option]
    
example
--------
    # python test2.py [-g graph_type] [-rw Read or Write] [-t time] [-y y-axis] [-l legend] [-i input] [-o output] [-s size] [-p point]
    * graph_type : f(Cumulative Bar Plot-file_type)/ b(Cumulative Bar Plot-block_type)/ t(time_plot+simple bar plot)/ all
    * Read or Write : r(read)/ w(write)/ rw(both read and write) 1. time_plot(graph) only 2. rw is default
    * time : sec(start):sec(end) # default - whole of the <result.po> data
    * y-axis - t(time plot) : s(sector size)/ n(sector number)/ all(default) 
	 - b(block type)/f(file type) : s(I/O Size)/ c(I/O Count)/ all(default)
    * legend : f(file_type)/ b(block_type) # time_plot(graph) only, not implemented.
    * input : # 1. name of the MOST output file(*.po), 2. default : result.po
	    3. also setted as directory names : <name>-<time> ex> result.po-Wed,Sep,30,16:08:04,2015
    * output # 1. name and location of the graph 2. file extension together
    * size : width:height # 1. To control the ratio, size of the Graph file 2. units : inch  3. default is 7:7
    * point : c(color)/ s(shape) # 1. time_plot(graph) only, 2. color is default

manual
--------
    link : https://drive.google.com/file/d/0B_qhp_YDu7dwalI1RWhsLWxXV0k/view?usp=sharing
    
    
2016-01-08, Kyeongyeol Choe <chl4651@hanyang.ac.kr>
