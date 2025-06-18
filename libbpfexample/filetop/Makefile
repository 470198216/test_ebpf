default: all

OUTPUT := .output
OUTPUT_ABS := $(abspath $(OUTPUT))
COMMON_LIBS_OBJ := $(patsubst %.c,$(OUTPUT_ABS)/%.o,$(wildcard libs/*.c))
IBBPF_OBJ ?= $(OUTPUT)/libbpf.a

$(OUTPUT)/libs:
	mkdir -p $@

CFLAGS := -g -O0 -Wall  -Wunused-variable -Wmissing-field-initializers -Werror -Werror=undef
INCLUDES := -I.output -I.output/include -I../vmlinux/x86/ -Iinclude -I.

# Build common libs objects
$(OUTPUT_ABS)/libs/%.o: libs/%.c $(LIBBPF_OBJ) | $(OUTPUT)/libs
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(COMMON_LIBS_OBJ)
	mkdir -p ./.output
	make -C /home/wwj/ebpffile/libbpf-bootstrap/libbpf/src BUILD_STATIC_ONLY=1                        \
        OBJDIR=/home/wwj/ebpffile/libbpfexample/filetop/.output/libbpf PREFIX=/home/wwj/ebpffile/libbpfexample/filetop/.output DESTDIR=       \
        LIBDIR=/home/wwj/ebpffile/libbpfexample/filetop/.output                                           \
        install install_uapi_headers
	clang -g -O2 -target bpf -D__TARGET_ARCH_x86                  \
             -I include -I.output -I/home/wwj/ebpffile/libbpf-bootstrap/libbpf/include/uapi -I/home/wwj/ebpffile/libbpf-bootstrap/vmlinux.h/include/x86/ -I/home/wwj/ebpffile/libbpf-bootstrap/blazesym/capi/include -idirafter /usr/lib/llvm-14/lib/clang/14.0.0/include -idirafter /usr/local/include -idirafter /usr/include/x86_64-linux-gnu -idirafter /usr/include                     \
             -c filetop.bpf.c -o .output/filetop.tmp.bpf.o
	/home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool gen object .output/filetop.bpf.o .output/filetop.tmp.bpf.o
	/home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool gen skeleton .output/filetop.bpf.o > .output/filetop.skel.h
	cc -g -Wall -I./include -I./.output/include -I.output -I/home/wwj/ebpffile/libbpf-bootstrap/libbpf/include/uapi -I/home/wwj/ebpffile/libbpf-bootstrap/vmlinux.h/include/x86/ -I. -c filetop.c -o .output/filetop.o -DSKEL_NAME=filetop_bpf
	cc -g -Wall .output/filetop.o $(COMMON_LIBS_OBJ) /home/wwj/ebpffile/libbpf-bootstrap/examples/c/.output/libbpf.a   -lelf -lz -o filetop
	#../ecc lsm-connect.bpf.c	
	#sudo ../ecli package.json
clean:
	rm -rfv filetop
	rm -rfv .output

