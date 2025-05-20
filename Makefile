#clang -O2 -target bpf -c syscall_count.c -o syscall_count.bpf.o
all:
	sudo apt install clang llvm
	./ecc minimalbpf.c
	sudo ./ecli run package.json

kprobe-link:
	./ecc kprobe-link.bpf.c
	sudo ./ecli run package.json
