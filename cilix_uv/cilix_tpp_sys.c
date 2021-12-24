#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_tpp_sys.h"

void cilix_tpp_sys_cat(cilix_tpp_sys_t* ts, char* data, long len);
void cilix_tpp_sys_left(cilix_tpp_sys_t* ts, char* data, long len);

cilix_tpp_sys_t* cilix_tpp_sys_init(void* pseq, cilix_tpp_sys_packet_cb pcb) {
    cilix_tpp_sys_t* ts = (cilix_tpp_sys_t*)malloc(sizeof(cilix_tpp_sys_t));

    ts->seq = pseq;
    ts->seq_packet_cb = pcb;

    ts->data = 0;
    ts->len = 0;

    return ts;
}

void cilix_tpp_sys_fini(cilix_tpp_sys_t* ts) {
    if (ts == 0) return;

    free(ts->data);
    free(ts);
}

void cilix_tpp_sys_parse(cilix_tpp_sys_t* ts, void* pe, char* data, long len) {
    cilix_tpp_sys_cat(ts, data, len);

    char* fs = ts->data;
    long fl = ts->len;

    for (int fpki=0; fl>0; fpki++) {
        char* fsep = 0;
        if ((fsep = strstr(fs, CILIX_TPP_SYS_SEP)) == 0) break;
        if (fsep-fs < CILIX_TPP_SYS_LEN_BYTES) break;

        long fpkl = 0;
        char fpkl_a[CILIX_TPP_SYS_LEN_BYTES + 10] = "";
        memcpy(fpkl_a, fs, CILIX_TPP_SYS_LEN_BYTES);
        CILIX_TPP_SYS_A_2_LEN(fpkl_a, fpkl);

        char* fpks = fsep + strlen(CILIX_TPP_SYS_SEP);
        if (fpks+fpkl > fs+fl) break;

        char* fpkp = fs+CILIX_TPP_SYS_LEN_BYTES;
        long fpkpl = fsep-(fs+CILIX_TPP_SYS_LEN_BYTES);

        int stop = 0;
        if (ts->seq_packet_cb) {
            stop = ts->seq_packet_cb(ts->seq, pe, fpkp, fpkpl, fpks, fpkl, fpki);
        }

        fl = (fs+fl) - (fpks+fpkl);
        fs = fpks+fpkl;

        if (stop) break;
    }

    cilix_tpp_sys_left(ts, fs, fl);
}

void cilix_tpp_sys_assem(char* data, void* base, unsigned long baselen, char* protocol) {
    unsigned long idx = 0; {
        CILIX_TPP_SYS_LEN_2_A(data + idx, baselen);
        idx += CILIX_TPP_SYS_LEN_BYTES;
    } {
        strcpy(data + idx, (protocol?protocol:""));
        idx += strlen(protocol?protocol:"");
    } {
        strcpy(data + idx, CILIX_TPP_SYS_SEP);
        idx += strlen(CILIX_TPP_SYS_SEP);
    } {
        memcpy(data + idx, base, baselen);
        idx += baselen;
    }
}

inline void cilix_tpp_sys_cat(cilix_tpp_sys_t* ts, char* data, long len) {
    char* nd = (char*)malloc(ts->len + len + 1);
    memset(nd, 0, ts->len + len + 1); {
        memcpy(nd, ts->data, ts->len);
        memcpy(nd + ts->len, data, len);
    }
    free(ts->data); {
        ts->data = nd;
        ts->len += len;
    }
}
inline void cilix_tpp_sys_left(cilix_tpp_sys_t* ts, char* data, long len) {
    if (data == ts->data) return;

    char* nd = 0;
    long nl = 0;
    if (len > 0) {
        nl = len;
        nd = (char*)malloc(len + 1);
        memset(nd, 0, len + 1);
        memcpy(nd, data, len);
    }
    free(ts->data); {
        ts->data = nd;
        ts->len = nl;
    }
}
