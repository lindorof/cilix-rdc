#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cJSON.h"
#include "base64.h"

#include "cilix_allocator.h"

#include "cilix_json.h"

#define ISNULLOBJ(obj)if(!obj){return 0;}
#define	ISNULLPTR(obj,name) if (!obj) {	return 0;}else if (obj && !name) {return 0;}

typedef struct json_data_s {
	cJSON 	*cJS;
	void 	*buff;
	void	*data;
}json_data_t;

void* cilix_json_init(char* str, long len) {
	json_data_t* obj = (json_data_t*)cilix_alloc_buf(sizeof(json_data_t));

	if (str && (len > 0)) {
		obj->cJS = cJSON_ParseWithLength(str, len);
	}
	else {
		obj->cJS = cJSON_CreateObject();
	}obj->buff = obj;
	return obj;
}

void cilix_json_destroy(void* obj) {
	json_data_t* ct = (json_data_t*)obj;
	if (!ct) return;
	if (ct->cJS) {
		cJSON_Delete(ct->cJS);
	}cilix_alloc_fini(ct);
}

void*  cilix_json_get_obj(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	json_data_t* child = (json_data_t*)cilix_alloc_more(sizeof(json_data_t), ct->buff);
	child->cJS = cJSON_GetObjectItem((cJSON*)ct->cJS, name);
	child->buff = ct->buff;
	return (void*)child;
}

char*  cilix_json_get_str(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	return  cJSON_GetObjectItem((cJSON*)ct->cJS, name)->valuestring;
}

char*  cilix_json_get_gstr(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	char* str = cJSON_GetObjectItem((cJSON*)ct->cJS, name)->valuestring;
	int len = strlen(str);
	int dLen = b64d_size(len);
	char* deData = cilix_alloc_more(dLen, ct->buff);
	dLen = b64_decode((unsigned char *)str, len, deData);
	deData[dLen] = '\0';
	return deData;
}

char*  cilix_json_get_64str(void* obj, char* name, int* len) {
	json_data_t* ct = (json_data_t*)obj;
	char* str = cJSON_GetObjectItem((cJSON*)ct->cJS, name)->valuestring;
	int tlen = strlen(str);
	int eLen = b64d_size(tlen);
	char* deData = cilix_alloc_more(eLen + 1, ct->buff);
	eLen = b64_decode((unsigned char *)str, tlen, deData);
	deData[eLen] = '\0';
	if (len) {
		*len = eLen;
	}
	return deData;
}

int    cilix_json_get_int(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	return  cJSON_GetObjectItem((cJSON*)ct->cJS, name)->valueint;
}

double cilix_json_get_double(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	return  cJSON_GetObjectItem((cJSON*)ct->cJS, name)->valuedouble;
}

void*  cilix_json_get_arr(void* obj, char* name) {
	return (void*)cJSON_GetObjectItem(obj, name);
}

int    cilix_json_get_arr_size(void* obj) {
	json_data_t* ct = (json_data_t*)obj;
	return cJSON_GetArraySize((cJSON*)ct->cJS);
}

void*  cilix_json_get_arr_obj(void* obj, int idx) {
	json_data_t* ct = (json_data_t*)obj;
	json_data_t* child = (json_data_t*)cilix_alloc_more(sizeof(json_data_t), ct->buff);
	child->cJS = (void*)cJSON_GetArrayItem((cJSON*)ct->cJS, idx);
	child->buff = ct->buff;
	return child;
}

char*  cilix_json_get_arr_str(void* obj, int idx) {
	json_data_t *ct = (json_data_t *)obj;
	return cJSON_GetArrayItem((cJSON *)ct->cJS, idx)->valuestring;
}

char*  cilix_json_get_arr_gstr(void* obj, int idx) {
	json_data_t* ct = (json_data_t*)obj;
	char* str = cJSON_GetArrayItem((cJSON *)ct->cJS, idx)->valuestring;
	int len = strlen(str);
	int dLen = b64d_size(len);
	char* deData = cilix_alloc_more(dLen, ct->buff);
	dLen = b64_decode((unsigned char *)str, len, deData);
	deData[dLen] = '\0';
	return deData;
}

char*  cilix_json_get_arr_64str(void* obj, int idx, int* len) {
	json_data_t* ct = (json_data_t*)obj;
	char* str = cJSON_GetArrayItem((cJSON *)ct->cJS, idx)->valuestring;
	int tlen = strlen(str);
	int eLen = b64d_size(tlen);
	char* deData = cilix_alloc_more(eLen + 1, ct->buff);
	eLen = b64_decode((unsigned char *)str, tlen, deData);
	deData[eLen] = '\0';
	if (len) {
		*len = eLen;
	}
	return deData;
}

int    cilix_json_get_arr_int(void* obj, int idx) {
	json_data_t *ct = (json_data_t *)obj;
	return cJSON_GetArrayItem((cJSON *)ct->cJS, idx)->valueint;
}

double cilix_json_get_arr_double(void* obj, int idx) {
	json_data_t *ct = (json_data_t *)obj;
	return cJSON_GetArrayItem((cJSON *)ct->cJS, idx)->valueint;
}


void* cilix_json_add_obj(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	json_data_t* child = (json_data_t*)cilix_alloc_more(sizeof(json_data_t), ct->buff);
	cJSON* cJSItem = cJSON_CreateObject();
	cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)(ct->cJS), name, cJSItem);
	if (!bRet) {
		cJSON_Delete(cJSItem);
		return 0;
	}
	child->cJS = cJSItem;
	child->buff = ct->buff;
	return (void*)child;
}

void* cilix_json_add_str(void* obj, char* name, char* val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(val));
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_gstr(void* obj, char* name, char* val){
	json_data_t* ct = (json_data_t*)obj;
	int len = strlen(val);
	int eLen = b64e_size(len);
	char *enData = (char *)malloc(sizeof(char *) * eLen + 1);
	memset(enData, 0, sizeof(char *) * eLen + 1);
	b64_encode((unsigned char *)val, len, enData);
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(enData));
	free(enData);
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_64str(void* obj, char* name, char* val, int len) {
	json_data_t* ct = (json_data_t*)obj;
	int eLen = b64e_size(len);
	char *enData = (char *)malloc(sizeof(char *) * eLen + 1);
	memset(enData, 0, sizeof(char *) * eLen + 1);
	b64_encode((unsigned char *)val, len, enData);
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(enData));
	free(enData);
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_int(void* obj, char* name, int val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateNumber((int)val));
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_double(void* obj, char* name, double val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateNumber(val));
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_arr(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	json_data_t* child = (json_data_t*)cilix_alloc_more(sizeof(json_data_t), ct->buff);
	cJSON* cJSArray;
	if (!name)cJSArray = cJSON_AddArrayToObject((cJSON*)ct->cJS, "");
	else cJSArray = cJSON_AddArrayToObject((cJSON*)ct->cJS, name);
	child->cJS = cJSArray;
	child->buff = ct->buff;
	return (void*)child;
}

void* cilix_json_add_arr_obj(void* obj, char* name) {
	json_data_t* ct = (json_data_t*)obj;
	json_data_t* child = (json_data_t*)cilix_alloc_more(sizeof(json_data_t), ct->buff);
	cJSON* cJSItem = cJSON_CreateObject();
	cJSON_bool bRet = cJSON_AddItemToObject((cJSON*)(ct->cJS), name, cJSItem);
	if (!bRet) {
		cJSON_Delete(cJSItem);
		return 0;
	}
	child->cJS = cJSItem;
	child->buff = ct->buff;
	return (void*)child;
}

void* cilix_json_add_arr_str(void* obj, char* name, char* val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(val));
	return obj;
}

void* cilix_json_add_arr_gstr(void* obj, char* name, char* val) {
	json_data_t* ct = (json_data_t*)obj;
	int len = strlen(val);
	int eLen = b64e_size(len);
	char *enData = (char *)malloc(sizeof(char *) * eLen + 1);
	memset(enData, 0, sizeof(char *) * eLen + 1);

	b64_encode((unsigned char *)val, len, enData);

	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(enData));
	free(enData);
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_arr_64str(void* obj, char* name, char* val, int len) {
	json_data_t* ct = (json_data_t*)obj;
	int eLen = b64e_size(len);
	char *enData = (char *)malloc(sizeof(char *) * eLen + 1);
	memset(enData, 0, sizeof(char *) * eLen + 1);
	b64_encode((unsigned char *)val, len, enData);
	cJSON_bool bRet = cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateString(enData));
	free(enData);
	if (!bRet) {
		return 0;
	}return (void*)obj;
}

void* cilix_json_add_arr_int(void* obj, char* name, int val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateNumber((double)val));
	return 0;
}

void* cilix_json_add_arr_double(void* obj, char* name, double val) {
	json_data_t* ct = (json_data_t*)obj;
	cJSON_AddItemToObject(ct->cJS, name, cJSON_CreateNumber(val));
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
