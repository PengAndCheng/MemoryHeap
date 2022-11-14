
//在RT-THREAD系统下利用FinSH组件进行测试
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#include <stdio.h>

#include "heap.h"

void hi(void){

}
MSH_CMD_EXPORT(hi,hi)

HeapStats_t hspt;
void hsp(void){
    HeapStats_t* hs=&hspt;

    vPortGetHeapStats(hs);
    printf("xAvailableHeapSpaceInBytes=%d;\n",hs->xAvailableHeapSpaceInBytes);
    printf("xSizeOfLargestFreeBlockInBytes=%d;\n",hs->xSizeOfLargestFreeBlockInBytes);
    printf("xSizeOfSmallestFreeBlockInBytes=%d;\n",hs->xSizeOfSmallestFreeBlockInBytes);
    printf("xNumberOfFreeBlocks=%d;\n",hs->xNumberOfFreeBlocks);
    printf("xMinimumEverFreeBytesRemaining=%d;\n",hs->xMinimumEverFreeBytesRemaining);
    printf("xNumberOfSuccessfulAllocations=%d;\n",hs->xNumberOfSuccessfulAllocations);
    printf("xNumberOfSuccessfulFrees=%d;\n",hs->xNumberOfSuccessfulFrees);
}
MSH_CMD_EXPORT(hsp,hsp)






void h1(void){
    hsp();

    char*p1 = pvPortMalloc(12);

    hsp();

    vPortFree(p1);

    hsp();
}
MSH_CMD_EXPORT(h1,h1)

//把内存=申请完
char* cp[300];
int cpnb=0;
void h2(void){

    //hsp();

    for (int i = 0; i < 300; ++i) {
        cp[i]=(char*)pvPortMalloc(1);
        if (cp[i]==NULL) {
            break;
        }
        printf("cp[%d]=%d;\n",i,cp[i]);
        hsp();
        printf("\n");
        cpnb++;
    }

    printf("cpnb=%d;\n",cpnb);
    //hsp();
}
MSH_CMD_EXPORT(h2,h2)

void h3(void){

    //hsp();

    printf("cpnb=%d;\n",cpnb);

    int max=cpnb;
    for (int i = 0; i < max; ++i) {
        vPortFree(cp[i]);
        hsp();
        printf("\n");
        cpnb--;
    }


    printf("cpnb=%d;\n",cpnb);
    //hsp();
}
MSH_CMD_EXPORT(h3,h3)


void h4(void){
    int count = 0xffffffff;
    uint32_t tick1 = rt_tick_get();
    uint32_t tick2 = 0;
    unsigned int ticks = 0;
    printf("---------------count=%d;\n",count);
    while(count--){


        for (int i = 0; i < 300; ++i) {
            cp[i]=(char*)pvPortMalloc(1);
            if (cp[i]==NULL) {
                break;
            }
            cpnb++;
        }

        RT_ASSERT(cpnb==20);


        int max=cpnb;
        for (int i = 0; i < max; ++i) {
            vPortFree(cp[i]);
            cpnb--;
        }


        RT_ASSERT(cpnb==0);

        tick2 = rt_tick_get();
        ticks = tick2-tick1;
        printf("---------------count=%d; ticks=%u;\n",count,ticks);
        rt_thread_mdelay(0);
    }
    printf("---------------h4 end---------------;\n");
}
MSH_CMD_EXPORT(h4,h4)
