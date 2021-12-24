#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cJSON.h"

#include "cilix_json.h"

void* cilix_json_init(char* str, long len) {
    return 0;
}

void cilix_json_destroy(void* obj) {

}

void*  cilix_json_get_obj(void* obj, char* name) {
    return 0;
}

char*  cilix_json_get_str(void* obj, char* name) {
    return 0;
}

int    cilix_json_get_int(void* obj, char* name) {
    return 0;
}

double cilix_json_get_double(void* obj, char* name) {
    return 0;
}

void*  cilix_json_get_arr(void* obj, char* name) {
    return 0;
}

int    cilix_json_get_arr_size(void* obj) {
    return 0;
}

void*  cilix_json_get_arr_obj(void* obj, int idx) {
    return 0;
}

char*  cilix_json_get_arr_str(void* obj, int idx) {
    return 0;
}

int    cilix_json_get_arr_int(void* obj, int idx) {
    return 0;
}

double cilix_json_get_arr_double(void* obj, int idx) {
    return 0;
}


void* cilix_json_add_obj(void* obj, char* name) {
    return 0;
}

void* cilix_json_add_str(void* obj, char* name, char* val) {
    return 0;
}

void* cilix_json_add_int(void* obj, char* name, int val) {
    return 0;
}

void* cilix_json_add_double(void* obj, char* name, double val) {
    return 0;
}

void* cilix_json_add_arr(void* obj, char* name) {
    return 0;
}

void* cilix_json_add_arr_obj(void* obj, char* name) {
    return 0;
}

void* cilix_json_add_arr_str(void* obj, char* name, char* val) {
    return 0;
}

void* cilix_json_add_arr_int(void* obj, char* name, int val) {
    return 0;
}

void* cilix_json_add_arr_double(void* obj, char* name, double val) {
    return 0;
}

char* cilix_json_print(void* obj) {
    return 0;
}

void cilix_json_print_free(char* text) {

}
