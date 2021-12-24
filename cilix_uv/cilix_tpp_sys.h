#ifndef _CILIX_TPP_SYS_H_
#define _CILIX_TPP_SYS_H_

#define CILIX_TPP_SYS_LEN_BYTES             (8)
#define CILIX_TPP_SYS_SEP                   ("^")
#define CILIX_TPP_SYS_LEN_2_A(s,l)          sprintf((s), "%08X", (unsigned int)(l))
#define CILIX_TPP_SYS_A_2_LEN(s,l)          (l) = strtoul((s), 0, 16)

#define CILIX_TPP_SYS_ASSEM_LEN(pkp, bl)    ( CILIX_TPP_SYS_LEN_BYTES + strlen(pkp?pkp:"") + strlen(CILIX_TPP_SYS_SEP) + bl )

typedef int (*cilix_tpp_sys_packet_cb)(void* seq, void* pe, char* pkp, long pkpl, char* pks, long pksl, int pki);

typedef struct cilix_tpp_sys_s {
    void* seq;
    cilix_tpp_sys_packet_cb seq_packet_cb;

    char* data;
    long len;
} cilix_tpp_sys_t;

// *** public - start ***
cilix_tpp_sys_t* cilix_tpp_sys_init(void* pseq, cilix_tpp_sys_packet_cb pcb);
void cilix_tpp_sys_fini(cilix_tpp_sys_t* ts);
void cilix_tpp_sys_parse(cilix_tpp_sys_t* ts, void* pe, char* data, long len);
void cilix_tpp_sys_assem(char* data, void* base, unsigned long baselen, char* protocol);
// *** public - end ***

#endif // #ifndef _CILIX_TPP_SYS_H_
