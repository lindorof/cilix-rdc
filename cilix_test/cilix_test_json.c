#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cilix_json.h"

void cilix_test_json(void){
    void* mSet =  cilix_json_init("",0);
	
	cilix_json_add_str(mSet, "test1", NULL);
	cilix_json_add_str(mSet, NULL, NULL);
	cilix_json_add_str(mSet, NULL, "adf");
	cilix_json_add_str(mSet, "asldf", "adf");

	void* mJSChild = cilix_json_add_arr(mSet,"PARA");

	void* mJSChild_1 = cilix_json_add_arr(mJSChild, NULL);
	cilix_json_add_str(mJSChild_1, NULL, "TRACK2:MII = 59");
	cilix_json_add_str(mJSChild_1,NULL , "COUNTRY = 280");
	cilix_json_add_str(mJSChild_1, NULL, "ISSUERID = 50050500");
	cilix_json_add_str(mJSChild_1, NULL, "LUHNT3 = 1");

	void* mJSChild_2 = cilix_json_add_arr(mJSChild, "");
	cilix_json_add_str(mJSChild_2, "", "TRACK3:MII = 30");
	cilix_json_add_str(mJSChild_2, "", "COUNTRY = 281");
	cilix_json_add_str(mJSChild_2, "", "ISSUERID = 51050500");
	cilix_json_add_str(mJSChild_2, "", "LUHNT3 = 2");
	/* void* mJSChild = cilix_json_add_obj(mSet,"LFSReadRaw");
    cilix_json_add_int(mJSChild,"Len",10);
    cilix_json_add_str(mJSChild,"Data","123456789");
   
    cilix_json_add_arr(mJSChild,"sites");

   void* mJSSit1= cilix_json_add_arr_obj(mJSChild,"sites");
   cilix_json_add_arr_str(mJSSit1,"a","234");
   cilix_json_add_arr_int(mJSSit1,"b",2);
   cilix_json_add_arr_double(mJSSit1,"c",2.11);

   void* mJSSit2 = cilix_json_add_arr_obj(mJSChild,"sites");
   cilix_json_add_arr_str(mJSSit2,"a","234");
   cilix_json_add_arr_int(mJSSit2,"b",2);
   cilix_json_add_arr_double(mJSSit2,"c",2.11);*/

    char * data = cilix_json_print(mSet);
    printf(">>> data:\n%s\n<<<", data);
	
    void* mGet = cilix_json_init(data,strlen(data));
	char* test1 = cilix_json_get_str(mSet, "test1");
	printf(">>> test1:\n%lu\n<<<", strlen(test1));

	void* mJSOut = cilix_json_get_obj(mGet, "PARA");
	int iSize = cilix_json_get_arr_size(mJSOut);
	printf("PARA Size:%d\n", iSize);
	for (int i = 0; i < iSize; i++) {
		void* mJSOut1 = cilix_json_get_arr_obj(mJSOut, i);
		int iSize_ = cilix_json_get_arr_size(mJSOut1);
		printf("%d Size:%d\n",i, iSize_);
		for (int j = 0; j < iSize_; j++) {
			printf("i = %d,j = %d data:%s\n", i,j,cilix_json_get_arr_str(mJSOut1, j));
		}
	}
	

    /*void* mJSOut= cilix_json_get_obj(mGet,"LFSReadRaw");
    
    printf("Len:%d\n",cilix_json_get_int(mJSOut,"Len"));
    printf("Data:%s\n",cilix_json_get_str(mJSOut,"Data"));
    printf("Data:%s\n",cilix_json_get_str(mJSOut,"Data"));

    void* mJSArr = cilix_json_get_obj(mJSOut,"sites");
    
    printf("ArrSize:%d\n",cilix_json_get_arr_size(mJSArr));

    void* mJSOut1  = cilix_json_get_arr_obj(mJSArr,0);
    printf("a:%s\n",cilix_json_get_str(mJSOut1,"a"));
    printf("b:%d\n",cilix_json_get_int(mJSOut1,"b"));
     printf("c:%.2f\n",cilix_json_get_double(mJSOut1,"c"));

    void* mJSOut2  = cilix_json_get_arr_obj(mJSArr,1);
    printf("a:%s\n",cilix_json_get_str(mJSOut2,"a"));
    printf("b:%d\n",cilix_json_get_int(mJSOut2,"b"));
     printf("c:%.2f\n",cilix_json_get_double(mJSOut2,"c*/

  return ;
}