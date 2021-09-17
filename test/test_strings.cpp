#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"

static void check(const char* a, const char* b) {
    if (strcmp(a, b) != 0) {
        fprintf(stderr, "Mismatch detected.\n");
        exit(1);
    }
}

int main(int, char**)
{
    char string[64];

    // cgltf_decode_string
    strcpy(string, "");
    cgltf_decode_string(string);
    check(string, "");

    strcpy(string, "nothing to replace");
    cgltf_decode_string(string);
    check(string, "nothing to replace");

    strcpy(string, "\\\" \\/ \\\\ \\b \\f \\r \\n \\t \\u0030");
    cgltf_decode_string(string);
    check(string, "\" / \\ \b \f \r \n \t 0");

    strcpy(string, "test \\u121b\\u130d\\u1294\\u1276\\u127d test");
    cgltf_decode_string(string);
    check(string, "test ማግኔቶች test");

    // cgltf_decode_uri
    strcpy(string, "");
    cgltf_decode_uri(string);
    check(string, "");

    strcpy(string, "nothing to replace");
    cgltf_decode_uri(string);
    check(string, "nothing to replace");

    strcpy(string, "%2F%D0%BA%D0%B8%D1%80%D0%B8%D0%BB%D0%BB%D0%B8%D1%86%D0%B0");
    cgltf_decode_uri(string);
    check(string, "/кириллица");

    strcpy(string, "test%20%E1%88%9B%E1%8C%8D%E1%8A%94%E1%89%B6%E1%89%BD%20test"); 
    cgltf_decode_uri(string);
    check(string, "test ማግኔቶች test");

    strcpy(string, "%%2F%X%AX%%2F%%");
    cgltf_decode_uri(string);
    check(string, "%/%X%AX%/%%");

    return 0;
}
