#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "uv.h"

void cilix_test_toml(char* f);
void cilix_test_proc(void);
void cilix_test_signal(void);
void cilix_test_sys(void);
void cilix_test_base64(void);
void cilix_test_timeout(void);
void cilix_test_json(void);

#define _ARGIS(s) if (argc > 2 && CILIX_STRICMP(argv[2],s)==0)
#define _ARGPARA(i) (argc > (3+i) ? argv[3+i] : "")

int cilix_test_x(int argc, char **argv)
{
    _ARGIS("-toml") {
        cilix_test_toml(_ARGPARA(0));
    }

    _ARGIS("-proc") {
        cilix_test_proc();
    }

    _ARGIS("-sig") {
        cilix_test_signal();
    }

    _ARGIS("-sys") {
        cilix_test_sys();
    }

    _ARGIS("-base64") {
        cilix_test_base64();
    }

    _ARGIS("-timeout") {
        cilix_test_timeout();
    }
	_ARGIS("-json") {
		cilix_test_json();
	}
    return 0;  
}

