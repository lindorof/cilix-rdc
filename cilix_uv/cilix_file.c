#include <stdio.h>
#include <stdlib.h>

#include "cilix_os_config.h"
#include CILIX_STRING_H

#include "cilix_file.h"

#include "Config.h"

long private_cilix_file_len(FILE* fp) {
    if(!fp) return 0;

    fseek(fp, 0, SEEK_END);
    return ftell(fp);
}

long cilix_file_len(char* f) {
    FILE* fp = fopen(f, "rb");
    if (!fp) return 0;

    long fp_len = private_cilix_file_len(fp);
    fclose(fp);
    return fp_len;
}

int cilix_file_exist(char* f) {
    FILE* fp = fopen(f, "rb");
    if (!fp) return 0;

    fclose(fp);
    return 1;
}

void cilix_file_move(char* oldf, char* newf, int copy, int overwrite) {
    if (!cilix_file_exist(oldf)) return;
    if (cilix_file_exist(newf) && !overwrite) return;

    FILE* fp_oldf = fopen(oldf, "rb");
    long fp_oldf_len = private_cilix_file_len(fp_oldf);
    char* fp_oldf_data = 0; {
        fp_oldf_data = (char*)malloc(fp_oldf_len);
        fseek(fp_oldf, 0, SEEK_SET);
        fread(fp_oldf_data, fp_oldf_len, 1, fp_oldf);
    } fclose(fp_oldf);

    FILE* fp_newf = fopen(newf, "wb");
    fwrite(fp_oldf_data, fp_oldf_len, 1, fp_newf);
    fclose(fp_newf);

    if (!copy) cilix_file_remove(oldf);
}

int cilix_file_remove(char* f) {
    int ret = remove(f);
    return (ret?0:1);
}
