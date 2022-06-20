#include "cilix_base64.h"

int cilix_base64_ensize(int InSize){
    return b64e_size(InSize);
}

int cilix_base64_encode(char* SourceData, int SourceLen, char* Base64){

    if((SourceData==0)||(Base64 == 0)||(SourceLen<=0)){
        return -1;
    }

    int out_size = b64e_size(SourceLen)+1;
    
    unsigned char *out_data = malloc( (sizeof(char) * out_size) );

    memset(out_data,0,out_size);

    out_size = b64_encode((unsigned char*)SourceData,SourceLen,out_data);
    
    memcpy(Base64,out_data,out_size);

    free(out_data);

    return out_size;
}

int cilix_base64_desize(int InSize){
    return b64d_size(InSize);
}

int cilix_base64_decode(char* Base64, char* DeData, int *DeDataLen){

     if((Base64==0)||(DeData == 0)||(DeDataLen ==0)){
        return -1;
    }

    unsigned int len = strlen((char*)Base64);

    int out_size = b64d_size(len) +1;

    unsigned char *out = malloc( sizeof(char) * out_size);
   
    memset(out,0,out_size);

    out_size = b64_decode((unsigned char*)Base64,len,out);

    memcpy(DeData,out,out_size);
    
    *DeDataLen =     out_size;
    
    free(out);

    return out_size;
}
