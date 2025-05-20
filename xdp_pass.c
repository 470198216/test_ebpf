#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
//
//SEC("kprobe/sys_open")
//int BPF_PROG(trace_open)
//{
//    // 你的 eBPF 程序逻辑
//    return 0;
//}
//
//char _license[] SEC("license") = "GPL";
//
//
SEC("xdp")
int xdp_pass(struct xdp_md *ctx)
{
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
