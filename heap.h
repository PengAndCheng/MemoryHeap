#ifndef __CPMEMP_H__
#define __CPMEMP_H__

#ifdef __cplusplus
extern "C" {
#endif


//链表节点 freerots的命名
typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK * pxNextFreeBlock; /*<< The next free block in the list. The next free block in the list 表中的下一个空闲块。列表中的下一个空闲块*/
    size_t xBlockSize;                     /*<< The size of the free block. The size of the free block 空闲块的大小。空闲块的大小*/
} BlockLink_t;




/* xHeapStats改名heapStats避免冲突。 */
typedef struct heapStats
{
    /* 当前可用的堆大小——这是所有空闲块的总和，而不是可以分配的最大块。 */
    size_t xAvailableHeapSpaceInBytes;

    /* 调用vPortGetHeapStats()时堆中所有空闲块的最大大小(以字节为单位)。 */
    size_t xSizeOfLargestFreeBlockInBytes;

    /* 调用vPortGetHeapStats()时堆中所有空闲块的最小大小(以字节为单位)。 */
    size_t xSizeOfSmallestFreeBlockInBytes;

    /* 调用vPortGetHeapStats()时堆中可用内存块的数量。 */
    size_t xNumberOfFreeBlocks;

    /* 自系统启动以来堆中的总空闲内存(所有空闲块的总和)的历史最小值。 */
    size_t xMinimumEverFreeBytesRemaining;

    /* 返回有效内存块的pvPortMalloc()调用次数。 */
    size_t xNumberOfSuccessfulAllocations;

    /* 成功释放内存块的vPortFree()调用次数。 */
    size_t xNumberOfSuccessfulFrees;
} HeapStats_t;


//声明堆枚举
typedef enum {
#define HEAP_CFG(namestr,   name,   size) HEAP_##name,
#include "heap_std.h"
#undef HEAP_CFG //C语言对#ifndef的预处理，只检查关键字，后面的"(namestr,   name,   size)"，认为是多余的字符
  HEAP_MAX
} HEAP_t;

typedef struct heap
{
    size_t configTOTAL_HEAP_SIZE;
    uint8_t* ucHeap;
    BlockLink_t xStart;
    BlockLink_t *pxEnd;

    size_t xFreeBytesRemaining;             //表示内存堆剩余大小
    size_t xMinimumEverFreeBytesRemaining;  //记录最小的那个空闲内存块大小
    size_t xNumberOfSuccessfulAllocations;   //成功的申请一次内存统计
    size_t xNumberOfSuccessfulFrees;         //表示成功释放一次统计

    HEAP_t HEAP;
    char* namep;
    char is_init;
}Heap_t;



#define HEAP_CFG(namestr,   name,   size) void * name##_malloc(unsigned int xWantedSize);
#include "heap_std.h"
#undef HEAP_CFG

#define HEAP_CFG(namestr,   name,   size) void name##_free(void * pv);
#include "heap_std.h"
#undef HEAP_CFG

#define HEAP_CFG(namestr,   name,   size) void name##_heap_info(void);
#include "heap_std.h"
#undef HEAP_CFG

#ifdef __cplusplus
}
#endif

#endif /* __CPMEMP_H__ */
