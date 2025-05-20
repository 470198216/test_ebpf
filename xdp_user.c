#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

int main()
{
    struct bpf_object *obj;
    struct bpf_program *prog;
    int prog_fd;

    // 加载eBPF对象
    obj = bpf_object__open("xdp_pass.bpf.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        return -1;
    }

    // 加载eBPF程序
    bpf_object__load(obj);

    // 获取eBPF程序
    prog = bpf_object__find_program_by_name(obj, "xdp_pass");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program\n");
        return -1;
    }
    prog_fd = bpf_program__fd(prog);

    // 将XDP程序绑定到网络接口
    bpf_set_link_xdp_fd(1, prog_fd, 0);

    return 0;
}
