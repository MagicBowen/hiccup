#include "catch2/catch.hpp"
#include "hiccup/resource/resource.h"

TEST_CASE( "Test computing of cost and penalty of process" ) {
    Resource r1{4, RT_CPU};
    Resource r2{2048, RT_MEMORY};
    Resource r3{100, RT_STORAGE};

    Allocation a1{3, &r1};  // CPU     使用3s
    Allocation a2{2, &r2};  // Memory  使用2s
    Allocation a3{14, &r3}; // Storage 使用14s

    Process proc;
    proc.numOfAllocations = 0;
    proc.allocations[proc.numOfAllocations++] = &a1;
    proc.allocations[proc.numOfAllocations++] = &a2;
    proc.allocations[proc.numOfAllocations++] = &a3;

    int total = 0;
    int penalty = 0;
    compute_cost(&proc, &total, &penalty);

    // 手动计算期望值:
    // CPU     : base 50 + (3-2)*10 = 50+10=60
    // MEM     : base 30 + (2 * (2048-1024)*2)=30 + (2*1024*2)=30+4096=4126
    // STORAGE : base 20 + (14-3)*100*1.5=20+(11*100*1.5)=20+1650=1670
    // total = 60 + 4126 + 1670 = 5856
    // STORAGE 资源 >12S, penalty=1
    REQUIRE(total == 5856);
    REQUIRE(penalty == 1);
}
