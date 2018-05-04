

#define CGLTF_IMPLEMENTATION
#include "../cgltf.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("err\n");
		return -1;
	}

	FILE* f = fopen(argv[1], "rb");
	if (!f)
	{
		return -2;
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	void* buf = malloc(size);
	fread(buf, size, 1, f);

	fclose(f);

	cgltf_options options = {0};
	cgltf_data data;
	cgltf_result result = cgltf_parse(&options, buf, size, &data);

	printf("Result: %d\n", result);
	if (result == cgltf_result_success)
	{
		printf("Type: %u\n", data.file_type);
		printf("Version: %d\n", data.version);
		printf("Meshes: %lu\n", data.meshes_count);
	}

	free(buf);

	cgltf_free(&data);

	return result;
}
