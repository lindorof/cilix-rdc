#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "cilix_timeout.h"

void cilix_test_timeout(void) {
	void* tm = cilix_timeout_init(5000);
	printf("cilix_timeout_init [%p]\n", tm);

	for (int i=0; i<7; i++) {
		cilix_timeout_sleep(1000);
		printf("cilix_timeout_flag[%d] %d", i, cilix_timeout_flag(tm));
	}

	cilix_timeout_destroy(tm);
	printf("cilix_timeout_destroy");
}
