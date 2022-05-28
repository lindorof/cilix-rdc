#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cJSON.h"

#include "cilix_json.h"

#define ISNULLOBJ(obj)if(!obj){return 0;}
#define	ISNULLPTR(obj,name) if (!obj) {	return 0;}else if (obj && !name) {return 0;}

void* cilix_json_init(char* str, long len) {
	cJSON *cJS = 0;
	if (str && (len > 0)) {
		cJS = cJSON_ParseWithLength(str, len);
	}
	else {
		cJS = cJSON_CreateObject();
	}return (void*)cJS;
}

void cilix_json_destroy(void* obj) {
	if (obj) {
		cJSON_Delete((cJSON*)obj);
	}
}

void*  cilix_json_get_obj(void* obj, char* name) {

	ISNULLPTR(obj, name)

		return (void*)cJSON_GetObjectItem((cJSON*)obj, name);
}

char*  cilix_json_get_str(void* obj, char* name) {

	ISNULLPTR(obj, name)

		return  cJSON_GetObjectItem((cJSON*)obj, name)->valuestring;
}

int    cilix_json_get_int(void* obj, char* name) {

	ISNULLPTR(obj, name)

		return  cJSON_GetObjectItem((cJSON*)obj, name)->valueint;
}

double cilix_json_get_double(void* obj, char* name) {

	ISNULLPTR(obj, name)

		return  cJSON_GetObjectItem((cJSON*)obj, name)->valuedouble;
}

void*  cilix_json_get_arr(void* obj, char* name) {

	ISNULLPTR(obj, name)

		return (void*)cJSON_GetObjectItem(obj, name);
}

int    cilix_json_get_arr_size(void* obj) {

	ISNULLOBJ(obj)

		return cJSON_GetArraySize((cJSON*)obj);
}

void*  cilix_json_get_arr_obj(void* obj, int idx) {

	ISNULLOBJ(obj)

		return (void*)cJSON_GetArrayItem((cJSON*)obj, idx);
}

char*  cilix_json_get_arr_str(void* obj, int idx) {

	ISNULLOBJ(obj)

		return 0;
}

int    cilix_json_get_arr_int(void* obj, int idx) {

	ISNULLOBJ(obj)

		return 0;
}

double cilix_json_get_arr_double(void* obj, int idx) {

	ISNULLOBJ(obj)

		return 0;
}


void* cilix_json_add_obj(void* obj, char* name) {

	ISNULLPTR(obj, name)

		cJSON* cJSItem = cJSON_CreateObject();

	cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)obj, name, cJSItem);
	if (!bRet) {
		cJSON_Delete(cJSItem);
		return 0;
	}return (void*)cJSItem;
}

void* cilix_json_add_str(void* obj, char* name, char* val) {
	ISNULLPTR(obj, name)

		cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateString(val));
	if (!bRet) {
		return 0;
	}return obj;
}

void* cilix_json_add_int(void* obj, char* name, int val) {
	ISNULLPTR(obj, name)

		cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateNumber((int)val));
	if (!bRet) {
		return 0;
	}return obj;
}

void* cilix_json_add_double(void* obj, char* name, double val) {
	ISNULLPTR(obj, name)

		cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateNumber(val));
	if (!bRet) {
		return 0;
	}return obj;
}

void* cilix_json_add_arr(void* obj, char* name) {
	ISNULLPTR(obj, name)

		cJSON* cJSArray = cJSON_AddArrayToObject((cJSON*)obj, name);
	return (void*)cJSArray;
}

void* cilix_json_add_arr_obj(void* obj, char* name) {
	ISNULLPTR(obj, name)

		cJSON* cJSItem = cJSON_CreateObject();
	cJSON_AddItemToArray(cJSON_GetObjectItem(obj, name), cJSItem);

	return (void*)cJSItem;
}

void* cilix_json_add_arr_str(void* obj, char* name, char* val) {

	ISNULLPTR(obj, name)

		cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateString(val));
	return obj;
}

void* cilix_json_add_arr_int(void* obj, char* name, int val) {

	ISNULLPTR(obj, name)

		cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateNumber((double)val));
	return 0;
}

void* cilix_json_add_arr_double(void* obj, char* name, double val) {

	ISNULLPTR(obj, name)

		cJSON_AddItemToObject((cJSON*)obj, name, cJSON_CreateNumber(val));
	return 0;
}

char* cilix_json_print(void* obj) {
	if (!obj) {
		return 0;
	}return  cJSON_Print(obj);
}

void cilix_json_print_free(char* text) {
	if (text) {
		free(text);
	}
}
