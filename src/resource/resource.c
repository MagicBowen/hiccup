#include "hiccup/resource/resource.h"

void compute_cost(const Process* proc, int* total, int* levels) {
    int index = 0;
    int cost = 0;
    int exceed = 0;
    
    for (index = 0; index < proc->numOfAllocations && index < MAX_NUM_ALLOCATION; index++) {
        cost = 0;
        exceed = 0;

        switch (proc->allocations[index]->resource->type) {
        case RT_CPU:
            /* 基本cost为50，使用时间大于2s则额外统计 */
            cost += 50;
            if (proc->allocations[index]->usage_time > 2) {
                exceed = proc->allocations[index]->usage_time - 2;
                /* 超时部分每s加收10 */
                cost += exceed * 10;
            }
            break;
        case RT_MEMORY:
            /* 基本cost为30，如果容量超过 BASIC_MEM_QUOTA 则按超出部分和使用时间统计 */
            cost += 30;
            if (proc->allocations[index]->resource->capacity > BASIC_MEM_QUOTA) {
                exceed = proc->allocations[index]->resource->capacity - BASIC_MEM_QUOTA;
                /* 超容部分乘以使用时间 * 2 */
                cost += proc->allocations[index]->usage_time * exceed * 2;
            }
            break;
        case RT_IO:
            /* 基本cost为20，如果使用时间超过3s，则超出每s统计 1.5 * 容量 */
            cost += 20;
            if (proc->allocations[index]->usage_time > 3) {
                exceed = proc->allocations[index]->usage_time - 3;
                cost += exceed * proc->allocations[index]->resource->capacity * 1.5;
            }
            break;
        default:
            break;
        }

        /* 如果是 IO 资源且使用时间 > 12s，对levels计数加1 */
        if ((proc->allocations[index]->resource->type == RT_IO) && 
            (proc->allocations[index]->usage_time > 12)) {
            (*levels) += 1;
        }

        (*total) += cost;
    }
}
