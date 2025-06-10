#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"

// Performs matrix-vector multiplication, as in (4x4) * (4x1) = (4x1)
static void transform(const cgltf_double matrix[16], const cgltf_double source[4], cgltf_double target[4]) {
    target[0] = matrix[0] * source[0] + matrix[4] * source[1] + matrix[ 8] * source[2] + matrix[12] * source[3];
    target[1] = matrix[1] * source[0] + matrix[5] * source[1] + matrix[ 9] * source[2] + matrix[13] * source[3];
    target[2] = matrix[2] * source[0] + matrix[6] * source[1] + matrix[10] * source[2] + matrix[14] * source[3];
    target[3] = matrix[3] * source[0] + matrix[7] * source[1] + matrix[11] * source[2] + matrix[15] * source[3];
}

static void set(cgltf_double target[3], double x, double y, double z) {
    target[0] = x;
    target[1] = y;
    target[2] = z;
}

static void check(cgltf_double target[3], double x, double y, double z) {
    if (target[0] != x || target[1] != y || target[2] != z) {
        fprintf(stderr, "Mismatch detected.\n");
        exit(1);
    }
}

int main(int, char**)
{
    cgltf_node node = {};

    cgltf_double matrix[16];
    cgltf_double source[4] = {1, 2, 3, 1};
    cgltf_double target[4];

    set(node.scale, 1, 1, 1);
    set(node.translation, 1, 0, 0);
    cgltf_node_transform_local(&node, matrix);
    transform(matrix, source, target);
    check(target, 2, 2, 3);

    set(node.scale, 3, 1, 1);
    set(node.translation, 0, 0, 0);
    cgltf_node_transform_local(&node, matrix);
    transform(matrix, source, target);
    check(target, 3, 2, 3);

    set(node.scale, 1, 3, 1);
    set(node.translation, 1, 0, 0);
    cgltf_node_transform_local(&node, matrix);
    transform(matrix, source, target);
    check(target, 2, 6, 3);

    return 0;
}
