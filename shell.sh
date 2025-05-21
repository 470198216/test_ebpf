#!/bin/bash
#clang -O2 -target bpf -I/usr/src/linux-headers-$(uname -r)/include/ -c sys_open.c -o sys_open.bpf.o
clang -O2 -target bpf -c xdp_pass.c -o xdp_pass.bpf.o
gcc xdp_user.c -o xdp_user -lelf -lbpf


clang -O2 -target bpf -c syscall_count.c -o syscall_count.bpf.o
gcc syscall_count_user.c -o syscall_count_user -lelf -lbpf


clang -O2 -target bpf -c sys_open_bpf.c -o sys_open_bpf.o
gcc sys_open_user.c -o sys_open_user -lelf -lbpf

sudo bpftrace syscall_count.bt

rm xdp_user sys_open_user sys_open_bpf.o xdp_pass.bpf.o syscall_count.bpf.o syscall_count_user -rfv

clang -O2 -target bpf -c bpf_program.c -o bpf_program.o
gcc loader.c -o loader -lelf -lbpf

#(可用)
sudo python3 open.py


# 列举所有的sys_enter_* 可用节点
bpftrace -l 'tracepoint:syscalls:sys_enter_*'

#程序初始化加载时候触发
bpftrace -e 'BEGIN { printf("hello world\n"); }'


#在sys_enter_openat触发打印，并且打印出对应的文件名称
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_openat { printf("%s %s\n", comm, str(args->filename)); }'

#在sys_enter系统调用入口中统计程序系统调用的次数
#解析如下:
#This summarizes syscalls by process name, printing a report on Ctrl-C.
#
#@: This denotes a special variable type called a map, which can store and summarize data in different ways. You can add an optional variable name after the @, eg "@num", either to improve readability, or to differentiate between more than one map.
#[]: The optional brackets allow a key to be set for the map, much like an associative array.
#count(): This is a map function – the way it is populated. count() counts the number of times it is called. Since this is saved by comm, the result is a frequency count of system calls by process name.
#Maps are automatically printed when bpftrace ends.
#
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm] = count(); }'

#返回sys_read退出时候的返回值情况直方图
bpftrace -e 'tracepoint:syscalls:sys_exit_read /pid == 1/ { @bytes = hist(args->ret); }'

#获取操作系统read接口的返回值直方图
bpftrace -e 'kretprobe:vfs_read { @bytes = lhist(retval, 0, 2000, 200); }'

#统计read系统调用接口的时间消耗， 以进程名称生成直方图
bpftrace -e 'kprobe:vfs_read { @start[tid] = nsecs; } kretprobe:vfs_read /@start[tid]/ { @ns[comm] = hist(nsecs - @start[tid]); delete(@start[tid]); }'

#获取sched中调度接口的调用次数，5秒后退出
bpftrace -e 'tracepoint:sched:sched* { @[probe] = count(); } interval:s:5 { exit(); }'


#每99hz捕获一次内核堆栈kstack以及其调用数
bpftrace -e 'profile:hz:99 { @[kstack] = count(); }'

#打印任务切换堆栈以及调度情况
bpftrace -e 'tracepoint:sched:sched_switch { @[kstack] = count(); }'

#统计块设备请求读写的统计直方图
#ret = register_trace_block_rq_issue(blk_add_trace_rq_issue, NULL);
bpftrace -e 'tracepoint:block:block_rq_issue { @ = hist(args->bytes); }'

#打印open系统调用的参数量
bpftrace path.bt
