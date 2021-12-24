#ifndef _CILIX_JSON_H_
#define _CILIX_JSON_H_

// *** public - start ***

// str : nil to create json root
// len : <=0 to use strlen(str)
void* cilix_json_init(char* str, long len);
void cilix_json_destroy(void* obj);

void*  cilix_json_get_obj(void* obj, char* name);
char*  cilix_json_get_str(void* obj, char* name);
int    cilix_json_get_int(void* obj, char* name);
double cilix_json_get_double(void* obj, char* name);

void*  cilix_json_get_arr(void* obj, char* name);
int    cilix_json_get_arr_size(void* obj);
void*  cilix_json_get_arr_obj(void* obj, int idx);
char*  cilix_json_get_arr_str(void* obj, int idx);
int    cilix_json_get_arr_int(void* obj, int idx);
double cilix_json_get_arr_double(void* obj, int idx);

void* cilix_json_add_obj(void* obj, char* name);
void* cilix_json_add_str(void* obj, char* name, char* val);
void* cilix_json_add_int(void* obj, char* name, int val);
void* cilix_json_add_double(void* obj, char* name, double val);

void* cilix_json_add_arr(void* obj, char* name);
void* cilix_json_add_arr_obj(void* obj, char* name);
void* cilix_json_add_arr_str(void* obj, char* name, char* val);
void* cilix_json_add_arr_int(void* obj, char* name, int val);
void* cilix_json_add_arr_double(void* obj, char* name, double val);

char* cilix_json_print(void* obj);
void  cilix_json_print_free(char* text);

// *** public - end ***

#endif // #ifndef _CILIX_JSON_H_
