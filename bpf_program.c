#include <linux/bpf.h>
#define SEC(NAME) __attribute__((section(NAME), used))

static int (*bpf_trace_printk)(const char *fmt, int fmt_size,
                               ...) = (void *)BPF_FUNC_trace_printk;

SEC("ksyscall/kill")
int bpf_prog(struct pt_regs *ctx) {
  char msg[] = "Hello, BPF World!";
  bpf_trace_printk(msg, sizeof(msg));
  return 0;
}

SEC("socket")
int socket_prog(struct __sk_buff *skb) {
  return 0;
}

char _license[] SEC("license") = "GPL";

