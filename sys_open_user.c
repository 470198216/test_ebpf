typedef unsigned int u32;
typedef unsigned long long u64;
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

int main()
{
    struct bpf_object *obj;
    struct bpf_program *prog;
    int prog_fd, map_fd;
    struct bpf_map *map;
    u32 key;
    u64 value;

    // 加载 eBPF 对象
    obj = bpf_object__open("sys_open_bpf.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        return -1;
    }

    // 加载 eBPF 程序
    bpf_object__load(obj);

    // 获取 eBPF 程序和映射
    prog = bpf_object__find_program_by_name(obj, "count_syscall");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program\n");
        return -1;
    }
    prog_fd = bpf_program__fd(prog);

    //map_fd = bpf_find_map(__stringify("syscall_count_map"), obj);
    //if (map_fd < 0) {
    //    fprintf(stderr, "Failed to find BPF map\n");
    //    return -1;
    //}
    map = bpf_object__find_map_by_name(obj, "syscall_count_map");
    if (!map) {
        fprintf(stderr, "Failed to find BPF map\n");
        return -1;
    }
    map_fd = bpf_map__fd(map);

    // 运行一段时间后打印结果
    sleep(15);

    // 遍历映射并打印结果
    while (bpf_map_get_next_key(map_fd, &key, sizeof(key)) == 0) {
        bpf_map_lookup_elem(map_fd, &key, &value);
        printf("PID %u: %llu syscalls\n", key, value);
        key++; // 更新 key 以获取下一个条目
    }

    // 清理资源
    bpf_object__close(obj);

    return 0;
}
