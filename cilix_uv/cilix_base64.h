#ifndef __CILIX_BASE64_H__
#define __CILIX_BASE64_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

// *** public - start ***

int cilix_base64_ensize(int);

int cilix_base64_encode(char* SourceData, int SourceLen, char* Base64);

int cilix_base64_desize(int);

int cilix_base64_decode(char* Base64, char* DeData, int *DeDataLen);


// *** public - end ***

#endif  // #__CILIX_BASE64_H__