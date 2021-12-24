#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "toml.h"
#include "cilix_toml.h"
#include "cilix_system.h"

#define TTAB "    "
#define TEST_TOML_TCK(tc, ...) \
	for (int __tc=0; __tc<tc; __tc ++) { \
		printf(TTAB); \
	} printf(__VA_ARGS__); \
    printf("\n"); \

void cilix_test_toml_node(int tcount, void* toml, void* node) {
	int num = cilix_toml_get_node_numb(toml, node, 0);

	for (int idx=0; idx<num; idx++) {
		const char* node_name = cilix_toml_get_node_name_at(toml, node, 0, idx);
		void* node_o = cilix_toml_get_node_o(toml, node, (char*)node_name);
		TEST_TOML_TCK(tcount+1, "%s", (char*)node_name);
		if (!node_o) continue;

		cilix_test_toml_node(tcount+1, toml, node_o);

		int kvn = cilix_toml_get_keyv_numb(toml, node_o, 0);
		for (int i=0; i<kvn; i++) {
			const char* key_name = cilix_toml_get_keyv_name_at(toml, node_o, 0, i);

			char key_value[500] = "";
			cilix_toml_get_keyv_s(toml, node_o, 0, (key_name?(char*)key_name:"NULL"), key_value, "");

			TEST_TOML_TCK(tcount+2, "%s=%s", (key_name?(char*)key_name:"NULL"), key_value);
		}
	}
}
void cilix_test_toml_f(char* f) {
	char cur_path[1025] = "";
	cilix_path_exe_path(0, cur_path);
	strcat(cur_path, f);

	void* toml = cilix_toml_init(cur_path, 1);
	do {
		printf("cilix_toml_init [%s][%p]\n", cur_path, toml);
		if (!toml) break;

		cilix_test_toml_node(0, toml, 0);
	} while (0);
	cilix_toml_destroy(toml);
}

static void fatal(const char* msg, const char* msg1)
{
	fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
	exit(1);
}

void cilix_test_toml(char* f) {
	if (f && strlen(f) > 0) {
		cilix_test_toml_f(f);
		return;
	}

    // -----------------------------------------------------------------
	// ly
	void* testnull = cilix_toml_init("", 0);
	printf(">>> testnull=0X%lX\n", (long)testnull);
	cilix_toml_destroy(testnull);

	char lytomlpath[1025] = ""; {
		cilix_path_exe_path_patch_name(0, "test.toml", lytomlpath);
	}
	void* lyconf = cilix_toml_init(lytomlpath, 1);
	if (!lyconf) {
		fatal("toml_parse_file_by_name failed - ", strerror(errno));
	}

    char key1[100] = "";
    cilix_toml_get_keyv_s(lyconf, 0, 0, "key1", key1, "!df");
	printf(">>> key1=%s\n", key1);

	int tti = 0;
	char tts[50][50] = {""}; {
		strcpy(tts[tti++], "srvp.ap");
		strcpy(tts[tti++], "srvp.ap.u1");
		strcpy(tts[tti++], "srvp.ap.u2");
		strcpy(tts[tti++], "");
		strcpy(tts[tti++], ".");
		strcpy(tts[tti++], "...");
		strcpy(tts[tti++], ".srvp.ap.u1");
		strcpy(tts[tti++], ".srvp...ap.u1...");
		strcpy(tts[tti++], "ab");
		strcpy(tts[tti++], "srvp");
		strcpy(tts[tti++], "srvp...");
		strcpy(tts[tti++], "srvp...ap");
		strcpy(tts[tti++], "srvp...ap.u4");
		strcpy(tts[tti++], "srvp...ap.");
	}
	for (int i=0; i<tti; i++) {
        void* tt = cilix_toml_get_node_o(lyconf, 0, tts[i]);
		printf(">>> [%s]=%lX\n", tts[i], (long)tt);
	}

    void* srvp_ap = cilix_toml_get_node_o(lyconf, 0, "srvp.ap");
	printf(">>> srvp_ap=%lX\n", (long)srvp_ap);
	if (srvp_ap) {
		{
			//toml_table_t* srvp_ap_u1 = toml_table_in(srvp_ap, "u1");
			//srvp_ap = srvp_ap_u1;
		}
		printf(">>> srvp.ap.ntab : %d\n", cilix_toml_get_node_numb(lyconf, srvp_ap, 0));
		printf(">>> srvp.ap.nkvar : %d\n", cilix_toml_get_keyv_numb(lyconf, srvp_ap, 0));
		for (int i=0; i<=8; i++) {
			const char* keyin = cilix_toml_get_node_name_at(lyconf, srvp_ap, "", i);
			printf(">>> cilix_toml_get_node_name_at[%d][%s]\n", i, (keyin?keyin:""));
		}
		for (int i=0; i<=8; i++) {
			const char* keyin = cilix_toml_get_keyv_name_at(lyconf, srvp_ap, "", i);
			printf(">>> cilix_toml_get_keyv_name_at[%d][%s]\n", i, (keyin?keyin:""));
		}
	}

	// -----------------------------------------------------------------

    FILE* fp;
    char errbuf[200];

	// 1. Read and parse toml file
	fp = fopen("test.toml", "r");
	if (!fp) {
		fatal("cannot open test.toml - ", strerror(errno));
	}

	toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
	fclose(fp);

	if (!conf) {
		fatal("cannot parse - ", errbuf);
	}

	// 2. Traverse to a table.
	toml_table_t* server = toml_table_in(conf, "server");
	if (!server) {
		fatal("missing [server]", "");
	}

	// 3. Extract values
	toml_datum_t host = toml_string_in(server, "host");
	if (!host.ok) {
		fatal("cannot read server.host", "");
	}

	toml_array_t* portarray = toml_array_in(server, "port");
	if (!portarray) {
		fatal("cannot read server.port", "");
	}

	printf("host: %s\n", host.u.s);
	printf("port: ");
	for (int i = 0; ; i++) {
		toml_datum_t port = toml_int_at(portarray, i);
		if (!port.ok) break;
		printf("%d ", (int)port.u.i);
	}
	printf("\n");

	// 4. Free memory
	free(host.u.s);
	toml_free(conf);
}
