#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"
#undef CGLTF_IMPLEMENTATION

static size_t allocated_buffers = 0;

static void* write_malloc(size_t size)
{
	void* buffer = malloc(size);
	if (buffer)
	{
		++allocated_buffers;
	}
	return buffer;
}

static void write_free(void* buffer)
{
	if (buffer)
	{
		--allocated_buffers;
	}
	free(buffer);
}

#define malloc write_malloc
#define free write_free
#define CGLTF_WRITE_IMPLEMENTATION
#include "../cgltf_write.h"
#undef malloc
#undef free

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("err\n");
		return -1;
	}

	cgltf_options options = {};
	cgltf_data* data0 = NULL;
	cgltf_result result = cgltf_parse_file(&options, argv[1], &data0);

	// Silently skip over files that are unreadable since this is a writing test.
	if (result != cgltf_result_success)
	{
		return cgltf_result_success;
	}

	result = cgltf_write_file(&options, "out.gltf", data0);
	if (result != cgltf_result_success || allocated_buffers != 0)
	{
		cgltf_free(data0);
		return -1;
	}
	cgltf_data* data1 = NULL;
	result = cgltf_parse_file(&options, "out.gltf", &data1);
	if (result != cgltf_result_success)
	{
		return result;
	}
	if (data0->meshes_count != data1->meshes_count) {
		return -1;
	}
	cgltf_free(data1);
	// The successful write created a regular file, so using it as a directory
	// guarantees that opening this path fails without relying on permissions.
	for (int i = 0; i < 2; ++i)
	{
		options.type = i == 0 ? cgltf_file_type_gltf : cgltf_file_type_glb;
		result = cgltf_write_file(&options, "out.gltf/missing.gltf", data0);
		if (result != cgltf_result_file_not_found || allocated_buffers != 0)
		{
			fprintf(stderr, "Failed file open leaked a serialization buffer.\n");
			cgltf_free(data0);
			return -1;
		}
	}
	cgltf_free(data0);
	return cgltf_result_success;
}
