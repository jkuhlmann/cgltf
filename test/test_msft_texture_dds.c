// Copyright 2022 Google LLC
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("usage: %s <gltf-file>\n", argv[0]);
        return -1;
    }

    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, argv[1], &data);

    if (result != cgltf_result_success)
        return -1;
    result = cgltf_load_buffers(&options, data, argv[1]);
    if (result != cgltf_result_success)
        return -1;
    result = cgltf_validate(data);
    if (result != cgltf_result_success)
        return -1;

    for (cgltf_size i = 0; i < data->textures_count; i++)
    {
        const cgltf_texture *texture = &data->textures[i];
        if (!data->textures[i].has_dds)
        {
            continue;
        }

        if (texture->dds_image == NULL)
        {
            fprintf(stderr, "Texture has_dds but has no DDS source.\n");
            return -1;
        }

        if (texture->dds_image < data->images ||
                texture->dds_image >= data->images + data->images_count) {
            fprintf(stderr, "DDS image pointer is invalid.\n");
            return -1;
        }
    }

    cgltf_free(data);

    return result;
}
