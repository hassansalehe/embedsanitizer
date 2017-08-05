#ifndef TSAN_INTERFACE_H
#define TSAN_INTERFACE_H

// This header should NOT include any other headers.
// All functions in this header are extern "C" and start with __tsan_.

#ifdef __cplusplus
extern "C" {
#endif

// This function should be called at the very beginning of the process,
// before any instrumented code is executed and before any call to malloc.
void __tsan_init();

void __tsan_read1(void *addr);

void __tsan_read2(void *addr);

void __tsan_read4(void *addr);

void __tsan_read8(void *addr);

void __tsan_read16(void *addr);

void __tsan_write1(void *addr);

void __tsan_write2(void *addr);

void __tsan_write4(void *addr);

void __tsan_write8(void *addr);

void __tsan_write16(void *addr);

void __tsan_func_entry(void *call_pc);
void __tsan_func_exit();

void __tsan_thread_create(void * child_id);

void __tsan_thread_join(void * child_id);

void __tsan_thread_lock(void * lock);

void __tsan_thread_unlock(void * lock);

void __tsan_vptr_update(void **vptr_p, void *new_val);
void __tsan_vptr_read(void **vptr_p);

/** Code adapted from tsan of LLVM*/
typedef char  __tsan_atomic8;
typedef short __tsan_atomic16;  // NOLINT
typedef int   __tsan_atomic32;

typedef unsigned char   a8;
typedef unsigned short  a16;  // NOLINT
typedef unsigned int    a32;


typedef enum {
  __tsan_memory_order_relaxed,
  __tsan_memory_order_consume,
  __tsan_memory_order_acquire,
  __tsan_memory_order_release,
  __tsan_memory_order_acq_rel,
  __tsan_memory_order_seq_cst
} __tsan_memory_order;

__tsan_atomic8  __tsan_atomic8_load(const volatile  __tsan_atomic8 *a,  __tsan_memory_order mo);
__tsan_atomic16 __tsan_atomic16_load(const volatile __tsan_atomic16 *a, __tsan_memory_order mo);
__tsan_atomic32 __tsan_atomic32_load(const volatile __tsan_atomic32 *a, __tsan_memory_order mo);

void __tsan_unaligned_read2(const void *addr);
void __tsan_unaligned_read4(const void *addr);
void __tsan_unaligned_read8(const void *addr);
void __tsan_unaligned_read16(const void *addr);

void __tsan_unaligned_write2(void *addr);
void __tsan_unaligned_write4(void *addr);
void __tsan_unaligned_write8(void *addr);
void __tsan_unaligned_write16(void *addr);

a8 __tsan_atomic32_fetch_add(volatile a8 *a, a8 v, __tsan_memory_order mo);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TSAN_INTERFACE_H