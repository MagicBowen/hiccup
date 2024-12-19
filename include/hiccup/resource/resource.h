#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#define BASIC_MEM_QUOTA 1024   /* 基本内存配额，单位:MB */
#define MAX_NUM_ALLOCATION 8  /* 每个进程的最大资源分配数目 */

/**
 * 资源类型
 */
typedef enum {
    RT_CPU,    /* CPU资源 */
    RT_MEMORY, /* 内存资源 */
    RT_IO      /* IO资源 */
} ResourceType;

/**
 * 资源
 */
typedef struct ResourceTag {
    int capacity;       /* 对于CPU为核数，对于内存是MB容量，对于IO是带宽值 */
    ResourceType type;  
} Resource;

/**
 * 分配项
 */
typedef struct AllocationTag {
    int usage_time;   /* 使用时长(如 S 为单位模拟) */
    Resource* resource;
} Allocation;

/**
 * 进程
 */
typedef struct ProcessTag {
    int numOfAllocations;
    Allocation* allocations[MAX_NUM_ALLOCATION];
} Process;

/**
 * 对进程使用资源进行 cost 及 penalty 计算
 * total为最终计算结果（根据消耗量计算的 cost 值）
 * penalty 基于 cost 计算的惩罚值
 */
void compute_cost(const Process* proc, int* total, int* penalty);

#ifdef __cplusplus
}
#endif

#endif
