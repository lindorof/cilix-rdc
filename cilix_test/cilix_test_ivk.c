#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cilix_rdc_ivk/cilix_rdc_ivk.h"

void cilix_test_ivk(char *pyh, char *drv,char *func, char *in, int len)
{
    void* rdc = rdc_init(pyh,drv);
	int ret = 0; char* data = 0;
	data = rdc_invoke(rdc, func, in, len, &ret);
    fprintf(stderr, "ret:%d data:%s \n ", ret,data );
	rdc_invoke_free(rdc,data);
	rdc_fini(rdc);
}