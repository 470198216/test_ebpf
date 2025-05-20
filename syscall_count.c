#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

//struct bpf_map_def SEC("maps") syscall_count_map = {
//    .type = BPF_MAP_TYPE_HASH,
//    .key_size = sizeof(u32),
//    .value_size = sizeof(u64),
//    .max_entries = 1024,
//};
// 将字符串常量定义为全局变量
static const char fmt[] = "sys_open called by PID %u\n";

//SEC("kprobe/sys_enter")
//int tgkill_entry(struct pt_regs *ctx)
SEC("ksyscall/tgkill")
int BPF_KSYSCALL(tgkill_entry, pid_t tgid, pid_t tid, int sig)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    //u64 *count = bpf_map_lookup_elem(&syscall_count_map, &pid);
    //if (count)
    //    *count += 1;
    //else{
    //    bpf_map_update_elem(&syscall_count_map, &pid, &((u64){1}), BPF_ANY);
    //}

    // 使用全局变量作为格式字符串
    bpf_trace_printk(fmt, sizeof(fmt), pid);
    return 0;
}

char _license[] SEC("license") = "GPL";
