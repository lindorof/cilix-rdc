#ifndef _CILIX_OS_CONFIG_H_
#define _CILIX_OS_CONFIG_H_

#include "Config.h"

/***************************************************/

#ifdef _CILIX_OS_Windows

//***** slash
#define CILIX_PATH_SLASH '\\'

//***** socket
#define CILIX_SOCKET_ADDR_H <winsock2.h>

//***** string
#define CILIX_STRING_H <string.h>
#define CILIX_STRICMP stricmp

#endif

/***************************************************/

#ifdef _CILIX_OS_Darwin

//***** slash
#define CILIX_PATH_SLASH '/'

//***** socket
#define CILIX_SOCKET_ADDR_H <netinet/in.h>

//***** string
#define CILIX_STRING_H <strings.h>
#define CILIX_STRICMP strcasecmp

#endif

/***************************************************/

#ifdef _CILIX_OS_Linux

//***** slash
#define CILIX_PATH_SLASH '/'

//***** socket
#define CILIX_SOCKET_ADDR_H <netinet/in.h>

//***** string
#define CILIX_STRING_H <string.h>
#define CILIX_STRICMP strcasecmp

#endif

/***************************************************/

#endif // #ifndef _CILIX_OS_CONFIG_H_