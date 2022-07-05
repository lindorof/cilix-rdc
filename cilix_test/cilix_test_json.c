#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cilix_json.h"

void cilix_test_json(void) {

	void* mSet = cilix_json_init("", 0);
	cilix_json_add_int(mSet, "HS", 1);
	cilix_json_add_int(mSet, "ID", 2);
	cilix_json_add_int(mSet, "CODE", 10);
	cilix_json_add_int(mSet, "TIMEOUT", 0);

	void* mJSAux = cilix_json_add_obj(mSet, "AUX");
	cilix_json_add_str(mJSAux, "LOGIC", "CardReader");
	cilix_json_add_str(mJSAux, "HWND", "/usr/lfs/linter");
	cilix_json_add_str(mJSAux, "REGHWND", "/usr/lfs/linter11");
	cilix_json_add_int(mJSAux, "REGEVT", 10);

	void* lP1 = cilix_json_add_obj(mSet, "PARA");

	cilix_json_add_str(lP1, "FormName", "PrintForm");
	cilix_json_add_gstr(lP1, "TrackData", "PrintForm");
	cilix_json_add_64str(lP1, "WriteMethod", "PrintForm", 9);

	void* mJSArrSit1 = cilix_json_add_arr(lP1, "CardData");

	void* mJSSit1 = cilix_json_add_obj(mJSArrSit1, "");
	cilix_json_add_arr_str(mJSSit1, "a", "45324");
	cilix_json_add_arr_int(mJSSit1, "b", 2);
	cilix_json_add_arr_double(mJSSit1, "c", 2.11);
	cilix_json_add_arr_gstr(mJSSit1, "GFormName", "PrintForm");
	cilix_json_add_arr_64str(mJSSit1, "64FormName", "PrintForm", 9);

	void* mJSSit2 = cilix_json_add_arr_obj(mJSArrSit1, "");
	cilix_json_add_arr_str(mJSSit2, "a", "234");
	cilix_json_add_arr_int(mJSSit2, "b", 2);
	cilix_json_add_arr_double(mJSSit2, "c", 2.11);
	cilix_json_add_arr_gstr(mJSSit2, "GFormName", "PrintForm");
	cilix_json_add_arr_64str(mJSSit2, "64FormName", "PrintForm", 9);


	char * data = cilix_json_print(mSet);
	printf("cilix_json_print:\n%s\n", data);

	void* mGet = cilix_json_init(data, strlen(data));


	printf("HS:%d ID:%d \n", cilix_json_get_int(mGet, "HS"), cilix_json_get_int(mGet, "ID"));
	char *grData = cilix_json_get_gstr(mGet, "GFormName");
	printf("GFormName:%s\n", grData);
	int rlen = 0;
	char *rData = cilix_json_get_64str(mGet, "64FormName", &rlen);
	printf("64FormName:%s,rlen:%d\n", rData, rlen);


	void* mJSOut = cilix_json_get_obj(mGet, "PARA");

	printf("FormName:%s\n", cilix_json_get_str(mJSOut, "FormName"));
	printf("GFormName:%s\n", cilix_json_get_gstr(mJSOut, "GFormName"));

	printf("64FormName:%s\n", cilix_json_get_64str(mJSOut, "64FormName", &rlen));

	void* mJSArr = cilix_json_get_obj(mJSOut, "CardData");

	printf("ArrSize:%d\n", cilix_json_get_arr_size(mJSArr));

	void* mJSOut1 = cilix_json_get_arr_obj(mJSArr, 0);
	printf("a:%s\n", cilix_json_get_str(mJSOut1, "a"));
	printf("b:%d\n", cilix_json_get_int(mJSOut1, "b"));
	printf("c:%.2f\n", cilix_json_get_double(mJSOut1, "c"));
	//printf("c:%.2f\n", cilix_json_get_arr_gstr(mJSOut1, 0));

	void* mJSOut2 = cilix_json_get_arr_obj(mJSArr, 1);
	printf("a:%s\n", cilix_json_get_str(mJSOut2, "a"));
	printf("b:%d\n", cilix_json_get_int(mJSOut2, "b"));
	printf("c:%.2f\n", cilix_json_get_double(mJSOut2, "c"));


}