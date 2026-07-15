#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"

#include <cstring>

static void check(const char* a, const char* b, cgltf_size size) {
    if (strcmp(a, b) != 0 || strlen(a) != size) {
        fprintf(stderr, "Mismatch detected.\n");
        exit(1);
    }
}

static void check_extras_copy(cgltf_size capacity, const char* expected)
{
    static const char json[] = "abcd";
    cgltf_data data = {};
    cgltf_extras extras = {};
    unsigned char storage[8];
    cgltf_size size = capacity;

    data.json = json;
    extras.end_offset = sizeof(json) - 1;

    memset(storage, 0xcc, sizeof(storage));
    storage[capacity] = 0x58;

    cgltf_result result = cgltf_copy_extras_json(&data, &extras, (char*)storage, &size);
    if (result != cgltf_result_success || storage[capacity] != 0x58 ||
        (capacity > 0 && strcmp((char*)storage, expected) != 0))
    {
        fprintf(stderr, "Extras copy mismatch for capacity %zu.\n", capacity);
        exit(1);
    }
}

int main(int, char**)
{
    char string[64];
    cgltf_size size;

    // cgltf_decode_string
    strcpy(string, "");
    size = cgltf_decode_string(string);
    check(string, "", size);

    strcpy(string, "nothing to replace");
    size = cgltf_decode_string(string);
    check(string, "nothing to replace", size);

    strcpy(string, "\\\" \\/ \\\\ \\b \\f \\r \\n \\t \\u0030");
    size = cgltf_decode_string(string);
    check(string, "\" / \\ \b \f \r \n \t 0", size);

    strcpy(string, "test \\u121b\\u130d\\u1294\\u1276\\u127d test");
    size = cgltf_decode_string(string);
    check(string, "test ማግኔቶች test", size);

    // cgltf_decode_uri
    strcpy(string, "");
    size = cgltf_decode_uri(string);
    check(string, "", size);

    strcpy(string, "nothing to replace");
    size = cgltf_decode_uri(string);
    check(string, "nothing to replace", size);

    strcpy(string, "%2F%D0%BA%D0%B8%D1%80%D0%B8%D0%BB%D0%BB%D0%B8%D1%86%D0%B0");
    size = cgltf_decode_uri(string);
    check(string, "/кириллица", size);

    strcpy(string, "test%20%E1%88%9B%E1%8C%8D%E1%8A%94%E1%89%B6%E1%89%BD%20test"); 
    size = cgltf_decode_uri(string);
    check(string, "test ማግኔቶች test", size);

    strcpy(string, "%%2F%X%AX%%2F%%");
    size = cgltf_decode_uri(string);
    check(string, "%/%X%AX%/%%", size);

    // cgltf_copy_extras_json
    static const char extras_json[] = "abcd";
    cgltf_data data = {};
    cgltf_extras extras = {};

    data.json = extras_json;
    extras.end_offset = sizeof(extras_json) - 1;
    size = 0;

    if (cgltf_copy_extras_json(&data, &extras, NULL, &size) != cgltf_result_success || size != sizeof(extras_json))
    {
        fprintf(stderr, "Extras size mismatch.\n");
        exit(1);
    }

    check_extras_copy(5, "abcd");
    check_extras_copy(4, "abc");
    check_extras_copy(3, "ab");
    check_extras_copy(2, "a");
    check_extras_copy(1, "");
    check_extras_copy(0, NULL);

    return 0;
}
