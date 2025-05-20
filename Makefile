#clang -O2 -target bpf -c syscall_count.c -o syscall_count.bpf.o
all:
	clang -O2 -g -target bpf -D__TARGET_ARCH_x86 -idirafter /usr/lib/llvm-14/lib/clang/14.0.0/include -idirafter /usr/local/include -idirafter /usr/include/x86_64-linux-gnu -idirafter /usr/include -I/home/wwj/ebpffile/libbpf-bootstrap/examples/c/../../vmlinux.h/include/x86 -isystem /home/wwj/ebpffile/libbpf-bootstrap/examples/c/build/libbpf -c syscall_count.c -o syscall_count.bpf.o
	gcc syscall_count_user.c -o syscall_count_user -lelf -lbpf

