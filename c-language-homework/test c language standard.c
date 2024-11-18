#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    // 检查是否定义了__STDC_VERSION__宏
    #ifdef __STDC_VERSION__
        // 根据__STDC_VERSION__的值确定使用的C标准
        #if __STDC_VERSION__ == 199901L
            printf("C99 standard is being used.\n");
        #elif __STDC_VERSION__ == 201112L
            printf("C11 standard is being used.\n");
        #else
            printf("C standard version is %ld.\n", __STDC_VERSION__);
        #endif
    #else
        printf("C standard version is not defined. Assuming C89/C90.\n");
    #endif

    return 0;
}
