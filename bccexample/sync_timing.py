#!/bin/python
#https://zhuanlan.zhihu.com/p/687604050 参考

#usage: sync;sync;sync
from __future__ import print_function
from bcc import BPF
from bcc.utils import printb

#load BPF program
b = BPF(text="""
#include <uapi/linux/ptrace.h>

BPF_HASH(last);

int do_trace(struct pt_regs* ctx){
    u64 ts, *tsp, delta, key = 0, countindex = 1, *countp, sumcount;
    //获取hash表中的数据值
    countp = last.lookup(&countindex);
    if(countp != NULL){
        sumcount = *countp + 1;
        last.delete(&countindex);
    }else{
        sumcount = 1;
    }
    last.update(&countindex, &sumcount);

    //attempt to read stored timestamp
    tsp = last.lookup(&key);
    if(tsp != NULL){
        delta = bpf_ktime_get_ns() - *tsp;
        if(delta < 1000000000){
            //output if time is less than 1 second
            //打印到trace文件中，b.trace_fields会去解析使用%s直接打印出来
            bpf_trace_printk("%d, key:%d, count: %u\\n", delta/1000000, key, sumcount);
        }else{
            //打印查看一下中途打印的量以确认
            bpf_trace_printk("%d, key:%d, count: %u\\n", delta/1000000, key, sumcount);
        }
        last.delete(&key);
    }
    //update stored timestamp
    ts = bpf_ktime_get_ns();
    last.update(&key, &ts);
    return 0;
}
""")

b.attach_kprobe(event=b.get_syscall_fnname("sync"), fn_name="do_trace")
print("Tracing for quick sync's... Ctrl-c to end")

#format output
start = 0
count = 0
while 1:
    try:
        (task, pid, cpu, flags, ts, ms) = b.trace_fields()
        if start == 0:
            start = ts
        ts = ts - start

        printb(b"At time %.2f s: multiple syncs detected, last %s ms ago" % (ts, ms))
    except KeyboardInterrupt:
        exit()
