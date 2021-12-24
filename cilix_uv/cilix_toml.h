#ifndef _CILIX_TOML_H_
#define _CILIX_TOML_H_

// *** public - start ***
void* cilix_toml_init(char* str, int is_file);
void cilix_toml_destroy(void* ct);

int cilix_toml_get_node_numb(void* ct, void* node, char* path);
int cilix_toml_get_keyv_numb(void* ct, void* node, char* path);
const char* cilix_toml_get_node_name_at(void* ct, void* node, char* path, int idx);
const char* cilix_toml_get_keyv_name_at(void* ct, void* node, char* path, int idx);

void* cilix_toml_get_node_o(void* ct, void* node, char* path);
void cilix_toml_get_keyv_s(void* ct, void* node, char* path, char* key, char* val, char* df);
double cilix_toml_get_keyv_f(void* ct, void* node, char* path, char* key, double df);
long long cilix_toml_get_keyv_i(void* ct, void* node, char* path, char* key, long long df);
void* cilix_toml_get_keyv_r(void* ct, void* node, char* path, char* key);

int cilix_toml_r_elem_numb(void* r);
void cilix_toml_r_elem_s(void* r, int idx, char* val);
double cilix_toml_r_elem_f(void* r, int idx);
long long cilix_toml_r_elem_i(void* r, int idx);
// *** public - end ***

#endif // #ifndef _CILIX_TOML_H_
