struct file_id{
	__u64 inode;
	__u32 dev;
	__u32 rdev;
	__u32 pid;
	__u32 tid;
};

struct file_stat{
	__u64 reads;
	__u64 read_bytes;
	__u64 writes;
	__u64 write_bytes;
	__u32 pid;
	__u32 tid;
	char filename[PATH_MAX];
	char com[TASK_COMM_LEN];
	char type;
}

const volatile pid_t target_pid = 0;

/*
 * BPF_CORE_READ() is used to simplif BPF CO-RE relocatable read, especially 
 * when there are few pointer caasing steps.
 * E.G., what in non-BPF world(or in BPF W/ bcc) would be something like:
 * 	int x = s->a.b.c->d.e->f->g;
 * can be succinctly achieved using BPF_CORE_READ as:
 * 	int x = BPF_CORE_READ(s, a.b.c, d.e, f, g);
 * BPF_CORE_READ will decompose above statement into 4 bpf_core_read(BPFCO-RE 
 * relocatable bpf_probe_read_kernel() wrapper) calls, logically
 * equivalent to:
 * 1. const void* __t = s->a.b.c;
 * 2. __t = __t->d.e;
 * 3. __t = __t->f;
 * 4. return __t->g;
 * Equivalence is logical, becase there is a heavy type casting/preservation
 * involved, as well as all the reads are happening througt
 * bpf_probe_read_kernel() calls using __builtin_preserve_asscess_index() to emit CO-RE relocations
 * N.B. only up to 9 "field asscessors" are supported , which should be more 
 * than enough for any practical purpose
*/
#define BPF_CORE_READ(src, a, ...) ({ 			\
		__type((src), a, ##__VA_ARGS__) __r;	\
		BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);	\
		__r;	\
		})

const volatile bool regular_file_only = true;
static struct file_stat zero_value;

static void get_file_path(struct file* file, char* buf, size_t size)
{
	struct qstr dname;
	dname = BPF_CORE_READ(file, f_path.dentry, d_name);
	bpf_core_read(buf, size, dname.name);
}

static int probe_entry(strct pt_regs* ctx, struct file* file, size_t count, enum op op)
{
	__u64 pid_tgid = bpf_get_current_pid_tgid();
	__u32 pid = pid_tgid >> 32;
	__u32 tid = pid_tgid;
	int mode;
	struct file_id key = {};
	struct file_stat* valuep;

	if(target_pid && target_pid != pid)
		return 0;
	mode = BPF_CORE_READ(file, f_inode, i_mode);
	if(regular_file_only && !S_ISREG(mode))
		return 0;
	key.dev = BPF_CORE_READ(file, f_inode, i_sb, s_dev);
	key.rdev = BPF_CORE_READ(file, f_inode, i_rdev);
	key.inode = BPF_CORE_READ(file, f_inode, i_ino);
	key.pid = pid;
	key.tid = tid;

	valuep = bpf_map_lookup_or_try_init(&entries, &key, &zero_value);
	if(!valuep)
		return 0;

	valuep->pid = pid;
	valuep->tid = tid;
	//bpf_get_current_comm在kernel/bpf/helpers.c: BPF_CALL_2(bpf_get_current_comm, char *, buf, u32, size)
	/*
	#define BPF_CALL_x(x, attr, name, ...)                                         \
        static __always_inline                                                 \
        u64 ____##name(__BPF_MAP(x, __BPF_DECL_ARGS, __BPF_V, __VA_ARGS__));   \
        typedef u64 (*btf_##name)(__BPF_MAP(x, __BPF_DECL_ARGS, __BPF_V, __VA_ARGS__)); \
        attr u64 name(__BPF_REG(x, __BPF_DECL_REGS, __BPF_N, __VA_ARGS__));    \
        attr u64 name(__BPF_REG(x, __BPF_DECL_REGS, __BPF_N, __VA_ARGS__))     \
        {                                                                      \
                return ((btf_##name)____##name)(__BPF_MAP(x,__BPF_CAST,__BPF_N,__VA_ARGS__));\
        }                                                                      \
        static __always_inline                                                 \
        u64 ____##name(__BPF_MAP(x, __BPF_DECL_ARGS, __BPF_V, __VA_ARGS__))
	 */
	/* 在vmlinux.h有对应的入口 */
	bpf_get_current_comm(&valuep->comm, sizeof(valuep->comm));
	get_file_path(file, valuep->filename, sizeof(valuep->filename));
}


SEC("kprobe/vfs_read")
int BPF_KPROBE(vfs_read_entry, struct file* file, char* buf, size_t count,
		loff_t* pos)
{
	return probe_entry(ctx, file, count, READ);
}
