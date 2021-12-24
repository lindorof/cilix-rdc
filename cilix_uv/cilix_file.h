#ifndef _CILIX_FILE_H_
#define _CILIX_FILE_H_

// *** public - start ***
int cilix_file_exist(char* f);
long cilix_file_len(char* f);
void cilix_file_move(char* oldf, char* newf, int copy, int overwrite);
int cilix_file_remove(char* f);
// *** public - end ***

#endif // #ifndef _CILIX_FILE_H_
