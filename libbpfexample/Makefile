
all:
	mkdir -p ./.output
	clang -g -O2 -target bpf -D__TARGET_ARCH_x86                  \
             -I.output -I../../libbpf/include/uapi -I/home/wwj/ebpffile/libbpf-bootstrap/vmlinux.h/include/x86/ -I/home/wwj/ebpffile/libbpf-bootstrap/blazesym/capi/include -idirafter /usr/lib/llvm-14/lib/clang/14.0.0/include -idirafter /usr/local/include -idirafter /usr/include/x86_64-linux-gnu -idirafter /usr/include                     \
             -c lsm.bpf.c -o .output/lsm.tmp.bpf.o
	/home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool gen object .output/lsm.bpf.o .output/lsm.tmp.bpf.o
	/home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool gen skeleton .output/lsm.bpf.o > .output/lsm.skel.h
	cc -g -Wall -I.output -I../../libbpf/include/uapi -I../../vmlinux.h/include/x86/ -I/home/wwj/ebpffile/libbpf-bootstrap/blazesym/capi/include -c lsm.c -o .output/lsm.o
	cc -g -Wall .output/lsm.o /home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/libbpf.a   -lelf -lz -o lsm

clean:
	rm -rfv lsm
	rm -rfv .output

