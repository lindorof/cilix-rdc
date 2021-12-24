#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "toml.h"

#include "cilix_toml.h"

void* cilix_toml_init(char* str, int is_file) {
    toml_table_t* ret = 0;
    char errbuf[200] = "";

    if (is_file) {
        ret = toml_parse_file_by_name(str, errbuf, sizeof(errbuf));
    } else {
        ret = toml_parse(str, errbuf, sizeof(errbuf));
    } return ret;
}
void cilix_toml_destroy(void* ct) {
    if (ct == 0) return;

    toml_free((toml_table_t*)ct);
}


int cilix_toml_get_node_numb(void* ct, void* node, char* path) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return 0;
    return toml_table_n_tb(tb);
}
int cilix_toml_get_keyv_numb(void* ct, void* node, char* path) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return 0;
    return toml_table_n_kv_ar(tb);
}
const char* cilix_toml_get_node_name_at(void* ct, void* node, char* path, int idx) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return 0;
    return toml_key_in_tb(tb, idx);
}
const char* cilix_toml_get_keyv_name_at(void* ct, void* node, char* path, int idx) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return 0;
    return toml_key_in_kv_ar(tb, idx);
}


void* cilix_toml_get_node_o(void* ct, void* node, char* path) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    return tb;
}
void cilix_toml_get_keyv_s(void* ct, void* node, char* path, char* key, char* val, char* df) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    do {
        if (!tb) break;
        toml_datum_t dt = toml_string_in(tb, key);
        if (!dt.ok) break; {
            strcpy(val, dt.u.s);
            free(dt.u.s);
        } return;
    } while (0);
    strcpy(val, (df?df:""));
}
double cilix_toml_get_keyv_f(void* ct, void* node, char* path, char* key, double df) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return df;
    toml_datum_t dt = toml_double_in(tb, key);
    if (!dt.ok) return df;
    return dt.u.d;
}
long long cilix_toml_get_keyv_i(void* ct, void* node, char* path, char* key, long long df) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return df;
    toml_datum_t dt = toml_int_in(tb, key);
    if (!dt.ok) return df;
    return dt.u.i;
}
void* cilix_toml_get_keyv_r(void* ct, void* node, char* path, char* key) {
    toml_table_t* tb = toml_table_in_by_path((node?node:ct), path);
    if (!tb) return 0;
    toml_array_t* ar = toml_array_in(tb, key);
    if (!ar) return 0;
    return ar;
}


int cilix_toml_r_elem_numb(void* r) {
    return toml_array_nelem((toml_array_t*)r);
}
void cilix_toml_r_elem_s(void* r, int idx, char* val) {
    toml_datum_t dt = toml_string_at((toml_array_t*)r, idx);
    if (dt.ok) {
        strcpy(val, dt.u.s);
        free(dt.u.s);
    }
}
double cilix_toml_r_elem_f(void* r, int idx) {
    toml_datum_t dt = toml_double_at((toml_array_t*)r, idx);
    if (dt.ok) return dt.u.d;
    return 0.0;
}
long long cilix_toml_r_elem_i(void* r, int idx) {
    toml_datum_t dt = toml_int_at((toml_array_t*)r, idx);
    if (dt.ok) return dt.u.i;
    return 0;
}
