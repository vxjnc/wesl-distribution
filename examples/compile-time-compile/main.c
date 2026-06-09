#include <assert.h>
#include <stdio.h>
#include <wesl.h>

#include "shaders/main.wgsl.h"

int main(void) {
    printf("wesl version: %s\n", wesl_version());
    printf("\nmain wgsl:%s\n", main_wgsl);
    return 0;
}
