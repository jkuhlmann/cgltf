/* How to fuzz:

clang main.c -O2 -g -fsanitize=address,fuzzer -o fuzz
cp -r data temp
./fuzz temp/ -dict=gltf.dict -jobs=12 -workers=12

*/
#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"
#include <stdlib.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
	cgltf_options options = {cgltf_file_type_invalid};
	cgltf_data* data = NULL;
	cgltf_result res = cgltf_parse(&options, Data, Size, &data);
	if (res != cgltf_result_success)
	{
		return 0;
	}

	cgltf_validate(data);

	/* Load buffers: for GLB this binds the embedded BIN chunk;
	 * for glTF with data URIs this decodes base64 data.
	 * This is required before accessing accessor data. */
	cgltf_load_buffers(&options, data, NULL);

	/* Exercise accessor read paths including sparse accessor handling.
	 * Recent CVEs (CVE-2026-32845, CVE-2026-75148) and other reported
	 * issues (#295, #299) reside in these code paths. */
	for (cgltf_size i = 0; i < data->accessors_count; i++)
	{
		cgltf_accessor* acc = &data->accessors[i];

		/* cgltf_accessor_unpack_floats: exercises sparse values iteration */
		cgltf_size float_count = cgltf_accessor_unpack_floats(acc, NULL, 0);
		if (float_count > 0 && float_count < 1024 * 1024)
		{
			cgltf_float* floats = (cgltf_float*)calloc(float_count, sizeof(cgltf_float));
			if (floats)
			{
				cgltf_accessor_unpack_floats(acc, floats, float_count);
				free(floats);
			}
		}

		/* cgltf_accessor_read_float: exercises cgltf_find_sparse_index */
		if (acc->count > 0 && acc->count < 4096)
		{
			cgltf_size num_components = cgltf_num_components(acc->type);
			if (num_components > 0 && num_components <= 16)
			{
				cgltf_float out[16];
				cgltf_size limit = acc->count < 64 ? acc->count : 64;
				for (cgltf_size j = 0; j < limit; j++)
				{
					cgltf_accessor_read_float(acc, j, out, num_components);
				}
			}
		}
	}

	cgltf_free(data);
	return 0;
}
