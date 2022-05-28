#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cilix_json.h"

void cilix_test_json(void){
    void* mSet =  cilix_json_init("",0);
    void* mJSChild = cilix_json_add_obj(mSet,"LFSReadRaw");
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
   cilix_json_add_arr_double(mJSSit2,"c",2.11);


    char * data = cilix_json_print(mSet);
    printf(">>> data:\n%s\n<<<", data);

    void* mGet = cilix_json_init(data,strlen(data));

    void* mJSOut= cilix_json_get_obj(mGet,"LFSReadRaw");
    
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
     printf("c:%.2f\n",cilix_json_get_double(mJSOut2,"c"));


}