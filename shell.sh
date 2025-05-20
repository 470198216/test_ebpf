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
