typedef unsigned int u32;
typedef unsigned long long u64;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

int main(int argc, char **argv)
{
    struct bpf_object *obj;
    int map_fd, prog_fd;
    struct bpf_map_info map_info = {};
    int map_info_len = sizeof(map_info);
    struct bpf_map *map;
    struct bpf_program *prog;
    u32 key;
    u64 value;

    // 加载eBPF对象
    obj = bpf_object__open("syscall_count.bpf.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        return -1;
    }

    // 加载eBPF程序
    bpf_object__load(obj);

    // 获取eBPF程序和映射
    //prog = bpf_object__find_program_by_name(obj, "count_syscall");
    //if (!prog) {
    //    fprintf(stderr, "Failed to find BPF program\n");
    //    return -1;
    //}
    //prog_fd = bpf_program__fd(prog);

    //map = bpf_object__find_map_by_name(obj, "syscall_count_map");
    //if (!map) {
    //    fprintf(stderr, "Failed to find BPF map\n");
    //    return -1;
    //}
    //map_fd = bpf_map__fd(map);

    // 运行一段时间后打印结果
    sleep(5);

    // 遍历映射并打印结果
    while (bpf_map_get_next_key(map_fd, &key, sizeof(key)) == 0) {
        bpf_map_lookup_elem(map_fd, &key, &value);
        printf("PID %u: %llu syscalls\n", key, value);
    }

    return 0;
}
