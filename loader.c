//在Linux6.8.12上已经废弃，如下只在linux4.x内核
//#include "bpf_load.h"
//#include <stdio.h>
//
//int main(int argc, char **argv) {
//  if (load_bpf_file("bpf_program.o") != 0) {
//    printf("The kernel didn't load the BPF program\n");
//    return -1;
//  }
//
//  read_trace_pipe();
//
//  return 0;
//}
//
//
#include <bpf/libbpf.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct bpf_object *obj;
    struct bpf_program *prog;
    int prog_fd, map_fd;
    struct bpf_map *map;
    //u32 key;
    //u64 value;

    // 加载 eBPF 对象
    obj = bpf_object__open("bpf_program.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        return -1;
    }

    // 加载 eBPF 程序
    bpf_object__load(obj);

    // 获取 eBPF 程序和映射
    //prog = bpf_object__find_program_by_name(obj, "bpf_prog");
    //if (!prog) {
    //    fprintf(stderr, "Failed to find BPF program\n");
    //    return -1;
    //}
    //prog_fd = bpf_program__fd(prog);
    //printf("BPF program loaded successfully with FD: %d\n", prog_fd);

    //map_fd = bpf_find_map(__stringify("syscall_count_map"), obj);
    //if (map_fd < 0) {
    //    fprintf(stderr, "Failed to find BPF map\n");
    //    return -1;
    //}
    //map = bpf_object__find_map_by_name(obj, "syscall_count_map");
    //if (!map) {
    //    fprintf(stderr, "Failed to find BPF map\n");
    //    return -1;
    //}
    //map_fd = bpf_map__fd(map);

    // 运行一段时间后打印结果
    sleep(15);

    // 遍历映射并打印结果
    //while (bpf_map_get_next_key(map_fd, &key, sizeof(key)) == 0) {
    //    bpf_map_lookup_elem(map_fd, &key, &value);
    //    printf("PID %u: %llu syscalls\n", key, value);
    //    key++; // 更新 key 以获取下一个条目
    //}

    // 清理资源
    bpf_object__close(obj);

    return 0;

}
