#ifndef CGLTF_H_INCLUDED__
#define CGLTF_H_INCLUDED__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t cgltf_size;
typedef float cgltf_float;
typedef int cgltf_int;
typedef int cgltf_bool;

typedef enum cgltf_file_type
{
	cgltf_file_type_invalid,
	cgltf_file_type_gltf,
	cgltf_file_type_glb,
} cgltf_file_type;

typedef struct cgltf_options
{
	cgltf_file_type type;
	cgltf_size json_token_count; /* 0 == auto */
	void* (*memory_alloc)(void* user, cgltf_size size);
	void (*memory_free) (void* user, void* ptr);
	void* memory_user_data;
} cgltf_options;

typedef enum cgltf_result
{
	cgltf_result_success,
	cgltf_result_data_too_short,
	cgltf_result_unknown_format,
	cgltf_result_invalid_json,
	cgltf_result_invalid_options,
	cgltf_result_file_not_found,
	cgltf_result_io_error,
	cgltf_result_out_of_memory,
} cgltf_result;

typedef enum cgltf_buffer_view_type
{
	cgltf_buffer_view_type_invalid,
	cgltf_buffer_view_type_indices,
	cgltf_buffer_view_type_vertices,
} cgltf_buffer_view_type;

typedef enum cgltf_attribute_type
{
	cgltf_attribute_type_invalid,
	cgltf_attribute_type_position,
	cgltf_attribute_type_normal,
	cgltf_attribute_type_tangent,
	cgltf_attribute_type_texcoord_0,
	cgltf_attribute_type_texcoord_1,
	cgltf_attribute_type_color_0,
	cgltf_attribute_type_joints_0,
	cgltf_attribute_type_weights_0,
} cgltf_attribute_type;

typedef enum cgltf_component_type
{
	cgltf_component_type_invalid,
	cgltf_component_type_r_8, /* BYTE */
	cgltf_component_type_r_8u, /* UNSIGNED_BYTE */
	cgltf_component_type_r_16, /* SHORT */
	cgltf_component_type_r_16u, /* UNSIGNED_SHORT */
	cgltf_component_type_r_32u, /* UNSIGNED_INT */
	cgltf_component_type_r_32f, /* FLOAT */
} cgltf_component_type;

typedef enum cgltf_type
{
	cgltf_type_invalid,
	cgltf_type_scalar,
	cgltf_type_vec2,
	cgltf_type_vec3,
	cgltf_type_vec4,
	cgltf_type_mat2,
	cgltf_type_mat3,
	cgltf_type_mat4,
} cgltf_type;

typedef enum cgltf_primitive_type
{
	cgltf_primitive_type_points,
	cgltf_primitive_type_lines,
	cgltf_primitive_type_line_loop,
	cgltf_primitive_type_line_strip,
	cgltf_primitive_type_triangles,
	cgltf_primitive_type_triangle_strip,
	cgltf_primitive_type_triangle_fan,
} cgltf_primitive_type;

typedef enum cgltf_alpha_mode
{
	cgltf_alpha_mode_opaque,
	cgltf_alpha_mode_mask,
	cgltf_alpha_mode_blend,
} cgltf_alpha_mode;

typedef enum cgltf_animation_path_type {
	cgltf_animation_path_type_invalid,
	cgltf_animation_path_type_translation,
	cgltf_animation_path_type_rotation,
	cgltf_animation_path_type_scale,
} cgltf_animation_path_type;

typedef enum cgltf_interpolation_type {
	cgltf_interpolation_type_linear,
	cgltf_interpolation_type_step,
	cgltf_interpolation_type_cubic_spline,
} cgltf_interpolation_type;

typedef enum cgltf_camera_type {
	cgltf_camera_type_invalid,
	cgltf_camera_type_perspective,
	cgltf_camera_type_orthographic,
} cgltf_camera_type;

typedef struct cgltf_buffer
{
	cgltf_size size;
	char* uri;
} cgltf_buffer;

typedef struct cgltf_buffer_view
{
	cgltf_buffer* buffer;
	cgltf_size offset;
	cgltf_size size;
	cgltf_size stride; /* 0 == automatically determined by accessor */
	cgltf_buffer_view_type type;
} cgltf_buffer_view;

typedef struct cgltf_accessor
{
	cgltf_component_type component_type;
	cgltf_bool normalized;
	cgltf_type type;
	cgltf_size offset;
	cgltf_size count;
	cgltf_size stride;
	cgltf_buffer_view* buffer_view;
} cgltf_accessor;

typedef struct cgltf_attribute
{
	cgltf_attribute_type name;
	cgltf_accessor* data;
} cgltf_attribute;

typedef struct cgltf_image 
{
	char* name;
	char* uri;
	cgltf_buffer_view* buffer_view;
	char* mime_type;
} cgltf_image;

typedef struct cgltf_sampler
{
	cgltf_int mag_filter;
	cgltf_int min_filter;
	cgltf_int wrap_s;
	cgltf_int wrap_t;
} cgltf_sampler;

typedef struct cgltf_texture
{
	char* name;
	cgltf_image* image;
	cgltf_sampler* sampler;
} cgltf_texture;

typedef struct cgltf_texture_view
{	
	cgltf_texture* texture;
	cgltf_int texcoord;
	cgltf_float scale; /* equivalent to strength for occlusion_texture */
} cgltf_texture_view;

typedef struct cgltf_pbr_metallic_roughness
{
	cgltf_texture_view base_color_texture;
	cgltf_texture_view metallic_roughness_texture;

	cgltf_float base_color_factor[4];
	cgltf_float metallic_factor;
	cgltf_float roughness_factor;
} cgltf_pbr_metallic_roughness;

typedef struct cgltf_material
{
	char* name;
	cgltf_pbr_metallic_roughness pbr_metallic_roughness;
	cgltf_texture_view normal_texture;
	cgltf_texture_view occlusion_texture;
	cgltf_texture_view emissive_texture;
	cgltf_float emissive_factor[3];
	cgltf_alpha_mode alpha_mode;
	cgltf_float alpha_cutoff;
	cgltf_bool double_sided;
} cgltf_material;

typedef struct cgltf_primitive {
	cgltf_primitive_type type;
	cgltf_accessor* indices;
	cgltf_material* material;
	cgltf_attribute* attributes;
	cgltf_size attributes_count;
} cgltf_primitive;

typedef struct cgltf_mesh {
	char* name;
	cgltf_primitive* primitives;
	cgltf_size primitives_count;
} cgltf_mesh;

typedef struct cgltf_node cgltf_node;

typedef struct cgltf_skin {
	char* name;
	cgltf_node** joints;
	cgltf_size joints_count;
	cgltf_node* skeleton;
	cgltf_accessor* inverse_bind_matrices;
} cgltf_skin;

typedef struct cgltf_camera_perspective {
	cgltf_float aspect_ratio;
	cgltf_float yfov;
	cgltf_float zfar;
	cgltf_float znear;
} cgltf_camera_perspective;

typedef struct cgltf_camera_orthographic {
	cgltf_float xmag;
	cgltf_float ymag;
	cgltf_float zfar;
	cgltf_float znear;
} cgltf_camera_orthographic;

typedef struct cgltf_camera {
	char* name;
	cgltf_camera_type type;
	union {
		cgltf_camera_perspective perspective;
		cgltf_camera_orthographic orthographic;
	};
} cgltf_camera;

typedef struct cgltf_node {
	char* name;
	cgltf_node** children;
	cgltf_size children_count;
	cgltf_skin* skin;
	cgltf_mesh* mesh;
	cgltf_camera* camera;
	cgltf_bool has_translation;
	cgltf_bool has_rotation;
	cgltf_bool has_scale;
	cgltf_bool has_matrix;
	cgltf_float translation[3];
	cgltf_float rotation[4];
	cgltf_float scale[3];
	cgltf_float matrix[16];
} cgltf_node;

typedef struct cgltf_scene {
	char* name;
	cgltf_node** nodes;
	cgltf_size nodes_count;
} cgltf_scene;

typedef struct cgltf_animation_sampler {
	cgltf_accessor* input;
	cgltf_accessor* output;
	cgltf_interpolation_type interpolation;
} cgltf_animation_sampler;

typedef struct cgltf_animation_channel {
	cgltf_animation_sampler* sampler;
	cgltf_node* target_node;
	cgltf_animation_path_type target_path;
} cgltf_animation_channel;

typedef struct cgltf_animation {
	char* name;
	cgltf_animation_sampler* samplers;
	cgltf_size samplers_count;
	cgltf_animation_channel* channels;
	cgltf_size channels_count;
} cgltf_animation;

typedef struct cgltf_data
{
	unsigned version;
	cgltf_file_type file_type;
	void* file_data;

	cgltf_mesh* meshes;
	cgltf_size meshes_count;

	cgltf_material* materials;
	cgltf_size materials_count;

	cgltf_accessor* accessors;
	cgltf_size accessors_count;

	cgltf_buffer_view* buffer_views;
	cgltf_size buffer_views_count;

	cgltf_buffer* buffers;
	cgltf_size buffers_count;

	cgltf_image* images;
	cgltf_size images_count;

	cgltf_texture* textures;
	cgltf_size textures_count;

	cgltf_sampler* samplers;
	cgltf_size samplers_count;

	cgltf_skin* skins;
	cgltf_size skins_count;

	cgltf_camera* cameras;
	cgltf_size cameras_count;

	cgltf_node* nodes;
	cgltf_size nodes_count;

	cgltf_scene* scenes;
	cgltf_size scenes_count;

	cgltf_scene* scene;

	cgltf_animation* animations;
	cgltf_size animations_count;

	const void* bin;
	cgltf_size bin_size;

	void (*memory_free) (void* user, void* ptr);
	void* memory_user_data;
} cgltf_data;

cgltf_result cgltf_parse(
		const cgltf_options* options,
		const void* data,
		cgltf_size size,
		cgltf_data** out_data);

cgltf_result cgltf_parse_file(
		const cgltf_options* options,
		const char* path,
		cgltf_data** out_data);

void cgltf_free(cgltf_data* data);

#endif /* #ifndef CGLTF_H_INCLUDED__ */

/*
 *
 * Stop now, if you are only interested in the API.
 * Below, you find the implementation.
 *
 */

#ifdef __INTELLISENSE__
/* This makes MSVC intellisense work. */
#define CGLTF_IMPLEMENTATION
#endif

#ifdef CGLTF_IMPLEMENTATION

#include <stdint.h> /* For uint8_t, uint32_t */
#include <string.h> /* For strncpy */
#include <stdlib.h> /* For malloc, free */
#include <stdio.h> /* For fopen */


/*
 * -- jsmn.h start --
 * Source: https://github.com/zserge/jsmn
 * License: MIT
 */
typedef enum {
	JSMN_UNDEFINED = 0,
	JSMN_OBJECT = 1,
	JSMN_ARRAY = 2,
	JSMN_STRING = 3,
	JSMN_PRIMITIVE = 4
} jsmntype_t;
enum jsmnerr {
	/* Not enough tokens were provided */
	JSMN_ERROR_NOMEM = -1,
	/* Invalid character inside JSON string */
	JSMN_ERROR_INVAL = -2,
	/* The string is not a full JSON packet, more bytes expected */
	JSMN_ERROR_PART = -3
};
typedef struct {
	jsmntype_t type;
	int start;
	int end;
	int size;
#ifdef JSMN_PARENT_LINKS
	int parent;
#endif
} jsmntok_t;
typedef struct {
	unsigned int pos; /* offset in the JSON string */
	unsigned int toknext; /* next token to allocate */
	int toksuper; /* superior token node, e.g parent object or array */
} jsmn_parser;
static void jsmn_init(jsmn_parser *parser);
static int jsmn_parse(jsmn_parser *parser, const char *js, size_t len, jsmntok_t *tokens, size_t num_tokens);
/*
 * -- jsmn.h end --
 */


static const cgltf_size GltfHeaderSize = 12;
static const cgltf_size GltfChunkHeaderSize = 8;
static const uint32_t GltfMagic = 0x46546C67;
static const uint32_t GltfMagicJsonChunk = 0x4E4F534A;
static const uint32_t GltfMagicBinChunk = 0x004E4942;

static void* cgltf_default_alloc(void* user, cgltf_size size)
{
	return malloc(size);
}

static void cgltf_default_free(void* user, void* ptr)
{
	free(ptr);
}

static void* cgltf_calloc(cgltf_options* options, size_t element_size, cgltf_size count)
{
	if (SIZE_MAX / element_size < count)
	{
		return NULL;
	}
	void* result = options->memory_alloc(options->memory_user_data, element_size * count);
	if (!result)
	{
		return NULL;
	}
	memset(result, 0, element_size * count);
	return result;
}

static cgltf_result cgltf_parse_json(cgltf_options* options, const uint8_t* json_chunk, cgltf_size size, cgltf_data** out_data);

cgltf_result cgltf_parse(const cgltf_options* options, const void* data, cgltf_size size, cgltf_data** out_data)
{
	if (size < GltfHeaderSize)
	{
		return cgltf_result_data_too_short;
	}

	if (options == NULL)
	{
		return cgltf_result_invalid_options;
	}

	cgltf_options fixed_options = *options;
	if (fixed_options.memory_alloc == NULL)
	{
		fixed_options.memory_alloc = &cgltf_default_alloc;
	}
	if (fixed_options.memory_free == NULL)
	{
		fixed_options.memory_free = &cgltf_default_free;
	}

	uint32_t tmp;
	// Magic
	memcpy(&tmp, data, 4);
	if (tmp != GltfMagic)
	{
		if (fixed_options.type == cgltf_file_type_invalid)
		{
			fixed_options.type = cgltf_file_type_gltf;
		}
		else
		{
			return cgltf_result_unknown_format;
		}
	}

	if (fixed_options.type == cgltf_file_type_gltf)
	{
		cgltf_result json_result = cgltf_parse_json(&fixed_options, (const uint8_t*)data, size, out_data);
		if (json_result != cgltf_result_success)
		{
			return json_result;
		}

		(*out_data)->file_type = cgltf_file_type_gltf;

		return cgltf_result_success;
	}

	const uint8_t* ptr = (const uint8_t*)data;
	// Version
	memcpy(&tmp, ptr + 4, 4);
	uint32_t version = tmp;

	// Total length
	memcpy(&tmp, ptr + 8, 4);
	if (tmp > size)
	{
		return cgltf_result_data_too_short;
	}

	const uint8_t* json_chunk = ptr + GltfHeaderSize;

	if (GltfHeaderSize + GltfChunkHeaderSize > size)
	{
		return cgltf_result_data_too_short;
	}

	// JSON chunk: length
	uint32_t json_length;
	memcpy(&json_length, json_chunk, 4);
	if (GltfHeaderSize + GltfChunkHeaderSize + json_length > size)
	{
		return cgltf_result_data_too_short;
	}

	// JSON chunk: magic
	memcpy(&tmp, json_chunk + 4, 4);
	if (tmp != GltfMagicJsonChunk)
	{
		return cgltf_result_unknown_format;
	}

	json_chunk += GltfChunkHeaderSize;

	const void* bin = 0;
	cgltf_size bin_size = 0;

	if (GltfHeaderSize + GltfChunkHeaderSize + json_length + GltfChunkHeaderSize <= size)
	{
		// We can read another chunk
		const uint8_t* bin_chunk = json_chunk + json_length;

		// Bin chunk: length
		uint32_t bin_length;
		memcpy(&bin_length, bin_chunk, 4);
		if (GltfHeaderSize + GltfChunkHeaderSize + json_length + GltfChunkHeaderSize + bin_length > size)
		{
			return cgltf_result_data_too_short;
		}

		// Bin chunk: magic
		memcpy(&tmp, bin_chunk + 4, 4);
		if (tmp != GltfMagicBinChunk)
		{
			return cgltf_result_unknown_format;
		}

		bin_chunk += GltfChunkHeaderSize;

		bin = bin_chunk;
		bin_size = bin_length;
	}

	cgltf_result json_result = cgltf_parse_json(&fixed_options, json_chunk, json_length, out_data);
	if (json_result != cgltf_result_success)
	{
		return json_result;
	}

	(*out_data)->version = version;
	(*out_data)->file_type = cgltf_file_type_glb;
	(*out_data)->bin = bin;
	(*out_data)->bin_size = bin_size;

	return cgltf_result_success;
}

cgltf_result cgltf_parse_file(const cgltf_options* options, const char* path, cgltf_data** out_data)
{
	if (options == NULL)
	{
		return cgltf_result_invalid_options;
	}

	void* (*memory_alloc)(void*, cgltf_size) = options->memory_alloc ? options->memory_alloc : &cgltf_default_alloc;
	void (*memory_free)(void*, void*) = options->memory_free ? options->memory_free : &cgltf_default_free;

	FILE* file = fopen(path, "rb");
	if (!file)
	{
		return cgltf_result_file_not_found;
	}

	fseek(file, 0, SEEK_END);

	long length = ftell(file);
	if (length < 0)
	{
		fclose(file);
		return cgltf_result_io_error;
	}

	fseek(file, 0, SEEK_SET);

	char* file_data = (char*)memory_alloc(options->memory_user_data, length);
	if (!file_data)
	{
		fclose(file);
		return cgltf_result_out_of_memory;
	}

	cgltf_size file_size = (cgltf_size)length;
	cgltf_size read_size = fread(file_data, 1, file_size, file);

	fclose(file);

	if (read_size != file_size)
	{
		memory_free(options->memory_user_data, file_data);
		return cgltf_result_io_error;
	}

	cgltf_result result = cgltf_parse(options, file_data, file_size, out_data);

	if (result != cgltf_result_success)
	{
		memory_free(options->memory_user_data, file_data);
		return result;
	}

	(*out_data)->file_data = file_data;

	return cgltf_result_success;
}

void cgltf_free(cgltf_data* data)
{
	if (!data)
	{
		return;
	}

	data->memory_free(data->memory_user_data, data->accessors);
	data->memory_free(data->memory_user_data, data->buffer_views);

	for (cgltf_size i = 0; i < data->buffers_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->buffers[i].uri);
	}

	data->memory_free(data->memory_user_data, data->buffers);

	for (cgltf_size i = 0; i < data->meshes_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->meshes[i].name);
		for (cgltf_size j = 0; j < data->meshes[i].primitives_count; ++j)
		{
			data->memory_free(data->memory_user_data, data->meshes[i].primitives[j].attributes);
		}
		data->memory_free(data->memory_user_data, data->meshes[i].primitives);
	}

	data->memory_free(data->memory_user_data, data->meshes);

	for (cgltf_size i = 0; i < data->materials_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->materials[i].name);
	}

	data->memory_free(data->memory_user_data, data->materials);

	for (cgltf_size i = 0; i < data->images_count; ++i) 
	{
		data->memory_free(data->memory_user_data, data->images[i].name);
		data->memory_free(data->memory_user_data, data->images[i].uri);
		data->memory_free(data->memory_user_data, data->images[i].mime_type);
	}

	data->memory_free(data->memory_user_data, data->images);

	for (cgltf_size i = 0; i < data->textures_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->textures[i].name);
	}

	data->memory_free(data->memory_user_data, data->textures);

	data->memory_free(data->memory_user_data, data->samplers);

	for (cgltf_size i = 0; i < data->skins_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->skins[i].name);
		data->memory_free(data->memory_user_data, data->skins[i].joints);
	}

	data->memory_free(data->memory_user_data, data->skins);

	for (cgltf_size i = 0; i < data->cameras_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->cameras[i].name);
	}

	data->memory_free(data->memory_user_data, data->cameras);

	for (cgltf_size i = 0; i < data->nodes_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->nodes[i].name);
		data->memory_free(data->memory_user_data, data->nodes[i].children);
	}

	data->memory_free(data->memory_user_data, data->nodes);

	for (cgltf_size i = 0; i < data->scenes_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->scenes[i].name);
		data->memory_free(data->memory_user_data, data->scenes[i].nodes);
	}

	data->memory_free(data->memory_user_data, data->scenes);

	for (cgltf_size i = 0; i < data->animations_count; ++i)
	{
		data->memory_free(data->memory_user_data, data->animations[i].name);
		data->memory_free(data->memory_user_data, data->animations[i].samplers);
		data->memory_free(data->memory_user_data, data->animations[i].channels);
	}

	data->memory_free(data->memory_user_data, data->animations);

	data->memory_free(data->memory_user_data, data->file_data);

	data->memory_free(data->memory_user_data, data);
}

#define CGLTF_CHECK_TOKTYPE(tok_, type_) if ((tok_).type != (type_)) { return -128; }

#define CGLTF_PTRINDEX(type, idx) (type*)(cgltf_size)(idx + 1)
#define CGLTF_PTRFIXUP(var, data, size) if (var) { if ((cgltf_size)var > size) { return -1; } var = &data[(cgltf_size)var-1]; }

static int cgltf_json_strcmp(jsmntok_t const* tok, const uint8_t* json_chunk, const char* str)
{
	CGLTF_CHECK_TOKTYPE(*tok, JSMN_STRING);
	size_t const str_len = strlen(str);
	size_t const name_length = tok->end - tok->start;
	return (str_len == name_length) ? strncmp((const char*)json_chunk + tok->start, str, str_len) : 128;
}

static int cgltf_json_to_int(jsmntok_t const* tok, const uint8_t* json_chunk)
{
	char tmp[128];
	CGLTF_CHECK_TOKTYPE(*tok, JSMN_PRIMITIVE);
	int size = tok->end - tok->start;
	strncpy(tmp, (const char*)json_chunk + tok->start, size);
	tmp[size] = 0;
	return atoi(tmp);
}

static cgltf_float cgltf_json_to_float(jsmntok_t const* tok, const uint8_t* json_chunk)
{
	char tmp[128];
	CGLTF_CHECK_TOKTYPE(*tok, JSMN_PRIMITIVE);
	int size = tok->end - tok->start;
	strncpy(tmp, (const char*)json_chunk + tok->start, size);
	tmp[size] = 0;
	return (cgltf_float)atof(tmp);
}

static cgltf_bool cgltf_json_to_bool(jsmntok_t const* tok, const uint8_t* json_chunk)
{
	int size = tok->end - tok->start;
	return size == 4 && memcmp(json_chunk + tok->start, "true", 4) == 0;
}

static char* cgltf_json_to_string_alloc(cgltf_options* options, jsmntok_t const* tok, const uint8_t* json_chunk)
{
	int size = tok->end - tok->start;
	char* result = (char*)options->memory_alloc(options->memory_user_data, size + 1);
	if (!result)
	{
		return NULL;
	}
	strncpy(result, (const char*)json_chunk + tok->start, size);
	result[size] = 0;
	return result;
}

static int cgltf_skip_json(jsmntok_t const* tokens, int i)
{
	if (tokens[i].type == JSMN_ARRAY)
	{
		int size = tokens[i].size;
		++i;
		for (int j = 0; j < size; ++j)
		{
			i = cgltf_skip_json(tokens, i);
		}
	}
	else if (tokens[i].type == JSMN_OBJECT)
	{
		int size = tokens[i].size;
		++i;
		for (int j = 0; j < size; ++j)
		{
			i = cgltf_skip_json(tokens, i);
			i = cgltf_skip_json(tokens, i);
		}
	}
	else if (tokens[i].type == JSMN_PRIMITIVE
		 || tokens[i].type == JSMN_STRING)
	{
		return i + 1;
	}
	return i;
}

static int cgltf_parse_json_float_array(jsmntok_t const* tokens, int i, const uint8_t* json_chunk, float* out_array, int size)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	if (tokens[i].size != size)
	{
		return -1;
	}
	++i;
	for (int j = 0; j < size; ++j)
	{
		CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
		out_array[j] = cgltf_json_to_float(tokens + i, json_chunk);
		++i;
	}
	return i;
}

static int cgltf_parse_json_primitive(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_primitive* out_prim)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "mode") == 0)
		{
			++i;
			out_prim->type
					= (cgltf_primitive_type)
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "indices") == 0)
		{
			++i;
			out_prim->indices = CGLTF_PTRINDEX(cgltf_accessor, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "material") == 0)
		{
			++i;
			out_prim->material = CGLTF_PTRINDEX(cgltf_material, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "attributes") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);
			out_prim->attributes_count = tokens[i].size;
			out_prim->attributes = (cgltf_attribute*)cgltf_calloc(options, sizeof(cgltf_attribute), tokens[i].size);
			++i;
			for (cgltf_size iattr = 0; iattr < out_prim->attributes_count; ++iattr)
			{
				CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_STRING);
				out_prim->attributes[iattr].name = cgltf_attribute_type_invalid;
				if (cgltf_json_strcmp(tokens+i, json_chunk, "POSITION") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_position;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "NORMAL") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_normal;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "TANGENT") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_tangent;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "TEXCOORD_0") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_texcoord_0;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "TEXCOORD_1") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_texcoord_1;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "COLOR_0") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_color_0;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "JOINTS_0") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_joints_0;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "WEIGHTS_0") == 0)
				{
					out_prim->attributes[iattr].name = cgltf_attribute_type_weights_0;
				}
				++i;
				out_prim->attributes[iattr].data = CGLTF_PTRINDEX(cgltf_accessor, cgltf_json_to_int(tokens + i, json_chunk));
				++i;
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_mesh(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_mesh* out_mesh)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_mesh->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "primitives") == 0)
		{
			++i;
			if (tokens[i].type != JSMN_ARRAY)
			{
				return -1;
			}
			out_mesh->primitives_count = tokens[i].size;
			out_mesh->primitives = (cgltf_primitive*)cgltf_calloc(options, sizeof(cgltf_primitive), tokens[i].size);
			++i;

			for (cgltf_size prim_index = 0;
			     prim_index < out_mesh->primitives_count;
			     ++prim_index)
			{
				i = cgltf_parse_json_primitive(options, tokens, i, json_chunk,
							       &out_mesh->primitives[prim_index]);
				if (i < 0)
				{
					return i;
				}
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_meshes(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->meshes_count = tokens[i].size;
	out_data->meshes = (cgltf_mesh*)cgltf_calloc(options, sizeof(cgltf_mesh), out_data->meshes_count);
	++i;
	for (cgltf_size j = 0; j < out_data->meshes_count; ++j)
	{
		i = cgltf_parse_json_mesh(options, tokens, i, json_chunk, &out_data->meshes[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_accessor(jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_accessor* out_accessor)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "bufferView") == 0)
		{
			++i;
			out_accessor->buffer_view = CGLTF_PTRINDEX(cgltf_buffer_view, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "byteOffset") == 0)
		{
			++i;
			out_accessor->offset =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "componentType") == 0)
		{
			++i;
			int type = cgltf_json_to_int(tokens+i, json_chunk);
			switch (type)
			{
			case 5120:
				type = cgltf_component_type_r_8;
				break;
			case 5121:
				type = cgltf_component_type_r_8u;
				break;
			case 5122:
				type = cgltf_component_type_r_16;
				break;
			case 5123:
				type = cgltf_component_type_r_16u;
				break;
			case 5125:
				type = cgltf_component_type_r_32u;
				break;
			case 5126:
				type = cgltf_component_type_r_32f;
				break;
			default:
				type = cgltf_component_type_invalid;
				break;
			}
			out_accessor->component_type = (cgltf_component_type)type;
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "normalized") == 0)
		{
			++i;
			out_accessor->normalized = cgltf_json_to_bool(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "count") == 0)
		{
			++i;
			out_accessor->count =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "type") == 0)
		{
			++i;
			if (cgltf_json_strcmp(tokens+i, json_chunk, "SCALAR") == 0)
			{
				out_accessor->type = cgltf_type_scalar;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "VEC2") == 0)
			{
				out_accessor->type = cgltf_type_vec2;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "VEC3") == 0)
			{
				out_accessor->type = cgltf_type_vec3;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "VEC4") == 0)
			{
				out_accessor->type = cgltf_type_vec4;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "MAT2") == 0)
			{
				out_accessor->type = cgltf_type_mat2;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "MAT3") == 0)
			{
				out_accessor->type = cgltf_type_mat3;
			}
			else if (cgltf_json_strcmp(tokens+i, json_chunk, "MAT4") == 0)
			{
				out_accessor->type = cgltf_type_mat4;
			}
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_texture_view(jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_texture_view* out_texture_view)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);
	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens + i, json_chunk, "index") == 0)
		{
			++i;
			out_texture_view->texture = CGLTF_PTRINDEX(cgltf_texture, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "texCoord") == 0)
		{
			++i;
			out_texture_view->texcoord = cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "scale") == 0) 
		{
			++i;
			out_texture_view->scale = cgltf_json_to_float(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "strength") == 0)
		{
			++i;
			out_texture_view->scale = cgltf_json_to_float(tokens + i, json_chunk);
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i + 1);
		}
	}

	return i;
}

static int cgltf_parse_json_pbr_metallic_roughness(jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_pbr_metallic_roughness* out_pbr)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);
	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "metallicFactor") == 0)
		{
			++i;
			out_pbr->metallic_factor = 
				cgltf_json_to_float(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "roughnessFactor") == 0) 
		{
			++i;
			out_pbr->roughness_factor =
				cgltf_json_to_float(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "baseColorFactor") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_pbr->base_color_factor, 4);
			if (i < 0)
			{
				return i;
			}
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "baseColorTexture") == 0)
		{
			i = cgltf_parse_json_texture_view(tokens, i + 1, json_chunk,
				&out_pbr->base_color_texture);
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "metallicRoughnessTexture") == 0)
		{
			i = cgltf_parse_json_texture_view(tokens, i + 1, json_chunk,
				&out_pbr->metallic_roughness_texture);
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_image(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_image* out_image)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j) 
	{
		if (cgltf_json_strcmp(tokens + i, json_chunk, "uri") == 0) 
		{
			++i;
			out_image->uri = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "bufferView") == 0)
		{
			++i;
			out_image->buffer_view = CGLTF_PTRINDEX(cgltf_buffer_view, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "mimeType") == 0)
		{
			++i;
			out_image->mime_type = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "name") == 0)
		{
			++i;
			out_image->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i + 1);
		}
	}

	return i;
}

static int cgltf_parse_json_sampler(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_sampler* out_sampler)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens + i, json_chunk, "magFilter") == 0) 
		{
			++i;
			out_sampler->mag_filter
				= cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "minFilter") == 0)
		{
			++i;
			out_sampler->min_filter
				= cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "wrapS") == 0)
		{
			++i;
			out_sampler->wrap_s
				= cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "wrapT") == 0) 
		{
			++i;
			out_sampler->wrap_t
				= cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i + 1);
		}
	}

	return i;
}


static int cgltf_parse_json_texture(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_texture* out_texture)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_texture->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "sampler") == 0)
		{
			++i;
			out_texture->sampler = CGLTF_PTRINDEX(cgltf_sampler, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "source") == 0) 
		{
			++i;
			out_texture->image = CGLTF_PTRINDEX(cgltf_image, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i + 1);
		}
	}

	return i;
}

static int cgltf_parse_json_material(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_material* out_material)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	out_material->pbr_metallic_roughness.base_color_texture.scale = 1.0f;
	out_material->pbr_metallic_roughness.metallic_roughness_texture.scale = 1.0f;
	out_material->pbr_metallic_roughness.base_color_factor[0] = 1.0f;
	out_material->pbr_metallic_roughness.base_color_factor[1] = 1.0f;
	out_material->pbr_metallic_roughness.base_color_factor[2] = 1.0f;
	out_material->pbr_metallic_roughness.base_color_factor[3] = 1.0f;
	out_material->pbr_metallic_roughness.metallic_factor = 1.0f;
	out_material->pbr_metallic_roughness.roughness_factor = 1.0f;
	out_material->emissive_texture.scale = 1.0f;
	out_material->normal_texture.scale = 1.0f;
	out_material->occlusion_texture.scale = 1.0f;
	out_material->alpha_cutoff = 0.5f;

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_material->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "pbrMetallicRoughness") == 0)
		{
			i = cgltf_parse_json_pbr_metallic_roughness(tokens, i + 1, json_chunk, &out_material->pbr_metallic_roughness);
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "emissiveFactor") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_material->emissive_factor, 3);
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "normalTexture") == 0)
		{
			i = cgltf_parse_json_texture_view(tokens, i + 1, json_chunk,
				&out_material->normal_texture);
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "occlusionTexture") == 0)
		{
			i = cgltf_parse_json_texture_view(tokens, i + 1, json_chunk,
				&out_material->occlusion_texture);
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "emissiveTexture") == 0)
		{
			i = cgltf_parse_json_texture_view(tokens, i + 1, json_chunk,
				&out_material->emissive_texture);
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "alphaMode") == 0)
		{
			++i;
			if (cgltf_json_strcmp(tokens + i, json_chunk, "OPAQUE") == 0)
			{
				out_material->alpha_mode = cgltf_alpha_mode_opaque;
			}
			else if (cgltf_json_strcmp(tokens + i, json_chunk, "MASK") == 0)
			{
				out_material->alpha_mode = cgltf_alpha_mode_mask;
			}
			else if (cgltf_json_strcmp(tokens + i, json_chunk, "BLEND") == 0)
			{
				out_material->alpha_mode = cgltf_alpha_mode_blend;
			}
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "alphaCutoff") == 0)
		{
			++i;
			out_material->alpha_cutoff = cgltf_json_to_float(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens + i, json_chunk, "doubleSided") == 0)
		{
			++i;
			out_material->double_sided =
				cgltf_json_to_bool(tokens + i, json_chunk);
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_accessors(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->accessors_count = tokens[i].size;
	out_data->accessors = (cgltf_accessor*)cgltf_calloc(options, sizeof(cgltf_accessor), out_data->accessors_count);
	++i;
	for (cgltf_size j = 0; j < out_data->accessors_count; ++j)
	{
		i = cgltf_parse_json_accessor(tokens, i, json_chunk, &out_data->accessors[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_materials(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->materials_count = tokens[i].size;
	out_data->materials = (cgltf_material*)cgltf_calloc(options, sizeof(cgltf_material), out_data->materials_count);
	++i;
	for (cgltf_size j = 0; j < out_data->materials_count; ++j)
	{
		i = cgltf_parse_json_material(options, tokens, i, json_chunk, &out_data->materials[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_images(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->images_count = tokens[i].size;
	out_data->images = (cgltf_image*)cgltf_calloc(options, sizeof(cgltf_image), out_data->images_count);
	++i;

	for (cgltf_size j = 0; j < out_data->images_count; ++j)
	{
		i = cgltf_parse_json_image(options, tokens, i, json_chunk, &out_data->images[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_textures(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->textures_count = tokens[i].size;
	out_data->textures = (cgltf_texture*)cgltf_calloc(options, sizeof(cgltf_texture), out_data->textures_count);
	++i;

	for (cgltf_size j = 0; j < out_data->textures_count; ++j)
	{
		i = cgltf_parse_json_texture(options, tokens, i, json_chunk, &out_data->textures[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_samplers(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->samplers_count = tokens[i].size;
	out_data->samplers = (cgltf_sampler*)cgltf_calloc(options, sizeof(cgltf_sampler), out_data->samplers_count);
	++i;

	for (cgltf_size j = 0; j < out_data->samplers_count; ++j)
	{
		i = cgltf_parse_json_sampler(options, tokens, i, json_chunk, &out_data->samplers[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_buffer_view(jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_buffer_view* out_buffer_view)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "buffer") == 0)
		{
			++i;
			out_buffer_view->buffer = CGLTF_PTRINDEX(cgltf_buffer, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "byteOffset") == 0)
		{
			++i;
			out_buffer_view->offset =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "byteLength") == 0)
		{
			++i;
			out_buffer_view->size =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "byteStride") == 0)
		{
			++i;
			out_buffer_view->stride =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "target") == 0)
		{
			++i;
			int type = cgltf_json_to_int(tokens+i, json_chunk);
			switch (type)
			{
			case 34962:
				type = cgltf_buffer_view_type_vertices;
				break;
			case 34963:
				type = cgltf_buffer_view_type_indices;
				break;
			default:
				type = cgltf_buffer_view_type_invalid;
				break;
			}
			out_buffer_view->type = (cgltf_buffer_view_type)type;
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_buffer_views(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->buffer_views_count = tokens[i].size;
	out_data->buffer_views = (cgltf_buffer_view*)cgltf_calloc(options, sizeof(cgltf_buffer_view), out_data->buffer_views_count);
	++i;
	for (cgltf_size j = 0; j < out_data->buffer_views_count; ++j)
	{
		i = cgltf_parse_json_buffer_view(tokens, i, json_chunk, &out_data->buffer_views[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_buffer(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_buffer* out_buffer)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "byteLength") == 0)
		{
			++i;
			out_buffer->size =
					cgltf_json_to_int(tokens+i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "uri") == 0)
		{
			++i;
			out_buffer->uri = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_buffers(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->buffers_count = tokens[i].size;
	out_data->buffers = (cgltf_buffer*)cgltf_calloc(options, sizeof(cgltf_buffer), out_data->buffers_count);
	++i;
	for (cgltf_size j = 0; j < out_data->buffers_count; ++j)
	{
		i = cgltf_parse_json_buffer(options, tokens, i, json_chunk, &out_data->buffers[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_skin(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_skin* out_skin)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_skin->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "joints") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
			out_skin->joints_count = tokens[i].size;
			out_skin->joints = (cgltf_node**)cgltf_calloc(options, sizeof(cgltf_node*), tokens[i].size);
			++i;

			for (cgltf_size k = 0; k < out_skin->joints_count; ++k)
			{
				out_skin->joints[k] = CGLTF_PTRINDEX(cgltf_node, cgltf_json_to_int(tokens + i, json_chunk));
				++i;
			}
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "skeleton") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
			out_skin->skeleton = CGLTF_PTRINDEX(cgltf_node, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "inverseBindMatrices") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
			out_skin->inverse_bind_matrices = CGLTF_PTRINDEX(cgltf_accessor, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_skins(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->skins_count = tokens[i].size;
	out_data->skins = (cgltf_skin*)cgltf_calloc(options, sizeof(cgltf_skin), out_data->skins_count);
	++i;
	for (cgltf_size j = 0; j < out_data->skins_count; ++j)
	{
		i = cgltf_parse_json_skin(options, tokens, i, json_chunk, &out_data->skins[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_camera(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_camera* out_camera)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_camera->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "type") == 0)
		{
			++i;
			if (cgltf_json_strcmp(tokens + i, json_chunk, "perspective") == 0)
			{
				out_camera->type = cgltf_camera_type_perspective;
			}
			else if (cgltf_json_strcmp(tokens + i, json_chunk, "orthographic") == 0)
			{
				out_camera->type = cgltf_camera_type_orthographic;
			}
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "perspective") == 0)
		{
			++i;

			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

			int data_size = tokens[i].size;
			++i;

			out_camera->type = cgltf_camera_type_perspective;

			for (int k = 0; k < data_size; ++k)
			{
				if (cgltf_json_strcmp(tokens+i, json_chunk, "aspectRatio") == 0)
				{
					++i;
					out_camera->perspective.aspect_ratio = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "yfov") == 0)
				{
					++i;
					out_camera->perspective.yfov = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "zfar") == 0)
				{
					++i;
					out_camera->perspective.zfar = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "znear") == 0)
				{
					++i;
					out_camera->perspective.znear = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else
				{
					i = cgltf_skip_json(tokens, i+1);
				}
			}
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "orthographic") == 0)
		{
			++i;

			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

			int data_size = tokens[i].size;
			++i;

			out_camera->type = cgltf_camera_type_orthographic;

			for (int k = 0; k < data_size; ++k)
			{
				if (cgltf_json_strcmp(tokens+i, json_chunk, "xmag") == 0)
				{
					++i;
					out_camera->orthographic.xmag = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "ymag") == 0)
				{
					++i;
					out_camera->orthographic.ymag = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "zfar") == 0)
				{
					++i;
					out_camera->orthographic.zfar = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "znear") == 0)
				{
					++i;
					out_camera->orthographic.znear = cgltf_json_to_float(tokens + i, json_chunk);
					++i;
				}
				else
				{
					i = cgltf_skip_json(tokens, i+1);
				}
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_cameras(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->cameras_count = tokens[i].size;
	out_data->cameras = (cgltf_camera*)cgltf_calloc(options, sizeof(cgltf_camera), out_data->cameras_count);
	++i;
	for (cgltf_size j = 0; j < out_data->cameras_count; ++j)
	{
		i = cgltf_parse_json_camera(options, tokens, i, json_chunk, &out_data->cameras[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_node(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_node* out_node)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	out_node->rotation[3] = 1.0f;
	out_node->scale[0] = 1.0f;
	out_node->scale[1] = 1.0f;
	out_node->scale[2] = 1.0f;
	out_node->matrix[0] = 1.0f;
	out_node->matrix[5] = 1.0f;
	out_node->matrix[10] = 1.0f;
	out_node->matrix[15] = 1.0f;

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_node->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "children") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
			out_node->children_count = tokens[i].size;
			out_node->children = (cgltf_node**)cgltf_calloc(options, sizeof(cgltf_node*), tokens[i].size);
			++i;

			for (cgltf_size k = 0; k < out_node->children_count; ++k)
			{
				out_node->children[k] = CGLTF_PTRINDEX(cgltf_node, cgltf_json_to_int(tokens + i, json_chunk));
				++i;
			}
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "mesh") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
			out_node->mesh = CGLTF_PTRINDEX(cgltf_mesh, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "skin") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
			out_node->skin = CGLTF_PTRINDEX(cgltf_skin, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "camera") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_PRIMITIVE);
			out_node->camera = CGLTF_PTRINDEX(cgltf_camera, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "translation") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_node->translation, 3);
			if (i < 0)
			{
				return i;
			}
			out_node->has_translation = 1;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "rotation") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_node->rotation, 4);
			if (i < 0)
			{
				return i;
			}
			out_node->has_rotation = 1;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "scale") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_node->scale, 3);
			if (i < 0)
			{
				return i;
			}
			out_node->has_scale = 1;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "matrix") == 0)
		{
			i = cgltf_parse_json_float_array(tokens, i + 1, json_chunk, out_node->matrix, 16);
			if (i < 0)
			{
				return i;
			}
			out_node->has_matrix = 1;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_nodes(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->nodes_count = tokens[i].size;
	out_data->nodes = (cgltf_node*)cgltf_calloc(options, sizeof(cgltf_node), out_data->nodes_count);
	++i;
	for (cgltf_size j = 0; j < out_data->nodes_count; ++j)
	{
		i = cgltf_parse_json_node(options, tokens, i, json_chunk, &out_data->nodes[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_scene(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_scene* out_scene)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_scene->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "nodes") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
			out_scene->nodes_count = tokens[i].size;
			out_scene->nodes = (cgltf_node**)cgltf_calloc(options, sizeof(cgltf_node*), tokens[i].size);
			++i;

			for (cgltf_size k = 0; k < out_scene->nodes_count; ++k)
			{
				out_scene->nodes[k] = CGLTF_PTRINDEX(cgltf_node, cgltf_json_to_int(tokens + i, json_chunk));
				++i;
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_scenes(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->scenes_count = tokens[i].size;
	out_data->scenes = (cgltf_scene*)cgltf_calloc(options, sizeof(cgltf_scene), out_data->scenes_count);
	++i;
	for (cgltf_size j = 0; j < out_data->scenes_count; ++j)
	{
		i = cgltf_parse_json_scene(options, tokens, i, json_chunk, &out_data->scenes[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static int cgltf_parse_json_animation_sampler(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_animation_sampler* out_sampler)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "input") == 0)
		{
			++i;
			out_sampler->input = CGLTF_PTRINDEX(cgltf_accessor, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "output") == 0)
		{
			++i;
			out_sampler->output = CGLTF_PTRINDEX(cgltf_accessor, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "interpolation") == 0)
		{
			++i;
			if (cgltf_json_strcmp(tokens + i, json_chunk, "LINEAR") == 0)
			{
				out_sampler->interpolation = cgltf_interpolation_type_linear;
			}
			else if (cgltf_json_strcmp(tokens + i, json_chunk, "STEP") == 0)
			{
				out_sampler->interpolation = cgltf_interpolation_type_step;
			}
			else if (cgltf_json_strcmp(tokens + i, json_chunk, "CUBICSPLINE") == 0)
			{
				out_sampler->interpolation = cgltf_interpolation_type_cubic_spline;
			}
			++i;
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_animation_channel(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_animation_channel* out_channel)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "sampler") == 0)
		{
			++i;
			out_channel->sampler = CGLTF_PTRINDEX(cgltf_animation_sampler, cgltf_json_to_int(tokens + i, json_chunk));
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "target") == 0)
		{
			++i;

			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

			int target_size = tokens[i].size;
			++i;

			for (int k = 0; k < target_size; ++k)
			{
				if (cgltf_json_strcmp(tokens+i, json_chunk, "node") == 0)
				{
					++i;
					out_channel->target_node = CGLTF_PTRINDEX(cgltf_node, cgltf_json_to_int(tokens + i, json_chunk));
					++i;
				}
				else if (cgltf_json_strcmp(tokens+i, json_chunk, "path") == 0)
				{
					++i;
					if (cgltf_json_strcmp(tokens+i, json_chunk, "translation") == 0)
					{
						out_channel->target_path = cgltf_animation_path_type_translation;
					}
					else if (cgltf_json_strcmp(tokens+i, json_chunk, "rotation") == 0)
					{
						out_channel->target_path = cgltf_animation_path_type_rotation;
					}
					else if (cgltf_json_strcmp(tokens+i, json_chunk, "scale") == 0)
					{
						out_channel->target_path = cgltf_animation_path_type_scale;
					}
					++i;
				}
				else
				{
					i = cgltf_skip_json(tokens, i+1);
				}
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_animation(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_animation* out_animation)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_OBJECT);

	int size = tokens[i].size;
	++i;

	for (int j = 0; j < size; ++j)
	{
		if (cgltf_json_strcmp(tokens+i, json_chunk, "name") == 0)
		{
			++i;
			out_animation->name = cgltf_json_to_string_alloc(options, tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "samplers") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
			out_animation->samplers_count = tokens[i].size;
			out_animation->samplers = (cgltf_animation_sampler*)cgltf_calloc(options, sizeof(cgltf_animation_sampler), tokens[i].size);
			++i;

			for (cgltf_size k = 0; k < out_animation->samplers_count; ++k)
			{
				i = cgltf_parse_json_animation_sampler(options, tokens, i, json_chunk, &out_animation->samplers[k]);
				if (i < 0)
				{
					return i;
				}
			}
		}
		else if (cgltf_json_strcmp(tokens+i, json_chunk, "channels") == 0)
		{
			++i;
			CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
			out_animation->channels_count = tokens[i].size;
			out_animation->channels = (cgltf_animation_channel*)cgltf_calloc(options, sizeof(cgltf_animation_channel), tokens[i].size);
			++i;

			for (cgltf_size k = 0; k < out_animation->channels_count; ++k)
			{
				i = cgltf_parse_json_animation_channel(options, tokens, i, json_chunk, &out_animation->channels[k]);
				if (i < 0)
				{
					return i;
				}
			}
		}
		else
		{
			i = cgltf_skip_json(tokens, i+1);
		}
	}

	return i;
}

static int cgltf_parse_json_animations(cgltf_options* options, jsmntok_t const* tokens, int i, const uint8_t* json_chunk, cgltf_data* out_data)
{
	CGLTF_CHECK_TOKTYPE(tokens[i], JSMN_ARRAY);
	out_data->animations_count = tokens[i].size;
	out_data->animations = (cgltf_animation*)cgltf_calloc(options, sizeof(cgltf_animation), out_data->animations_count);
	++i;
	for (cgltf_size j = 0; j < out_data->animations_count; ++j)
	{
		i = cgltf_parse_json_animation(options, tokens, i, json_chunk, &out_data->animations[j]);
		if (i < 0)
		{
			return i;
		}
	}
	return i;
}

static cgltf_size cgltf_calc_size(cgltf_type type, cgltf_component_type component_type)
{
	cgltf_size size = 0;

	switch (component_type)
	{
	case cgltf_component_type_r_8:
	case cgltf_component_type_r_8u:
		size = 1;
		break;
	case cgltf_component_type_r_16:
	case cgltf_component_type_r_16u:
		size = 2;
		break;
	case cgltf_component_type_r_32u:
	case cgltf_component_type_r_32f:
		size = 4;
		break;
	case cgltf_component_type_invalid:
	default:
		size = 0;
		break;
	}

	switch (type)
	{
	case cgltf_type_vec2:
		size *= 2;
		break;
	case cgltf_type_vec3:
		size *= 3;
		break;
	case cgltf_type_vec4:
		size *= 4;
		break;
	case cgltf_type_mat2:
		size *= 4;
		break;
	case cgltf_type_mat3:
		size *= 9;
		break;
	case cgltf_type_mat4:
		size *= 16;
		break;
	case cgltf_type_invalid:
	case cgltf_type_scalar:
	default:
		size *= 1;
		break;
	}

	return size;
}

static int cgltf_fixup_pointers(cgltf_data* out_data);

cgltf_result cgltf_parse_json(cgltf_options* options, const uint8_t* json_chunk, cgltf_size size, cgltf_data** out_data)
{
	jsmn_parser parser = { 0 };

	if (options->json_token_count == 0)
	{
		int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, NULL, 0);

		if (token_count <= 0)
		{
			return cgltf_result_invalid_json;
		}

		options->json_token_count = token_count;
	}

	jsmntok_t* tokens = (jsmntok_t*)options->memory_alloc(options->memory_user_data, sizeof(jsmntok_t) * options->json_token_count);

	jsmn_init(&parser);

	int token_count = jsmn_parse(&parser, (const char*)json_chunk, size, tokens, options->json_token_count);

	if (token_count <= 0 || tokens[0].type != JSMN_OBJECT)
	{
		options->memory_free(options->memory_user_data, tokens);
		return cgltf_result_invalid_json;
	}

	cgltf_data* data = (cgltf_data*)options->memory_alloc(options->memory_user_data, sizeof(cgltf_data));
	memset(data, 0, sizeof(cgltf_data));
	data->memory_free = options->memory_free;
	data->memory_user_data = options->memory_user_data;

	// The root is an object.
	int i = 1;

	for (int j = 0; j < tokens[0].size; ++j)
	{
		jsmntok_t const* tok = &tokens[i];
		if (cgltf_json_strcmp(tok, json_chunk, "meshes") == 0)
		{
			i = cgltf_parse_json_meshes(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "accessors") == 0)
		{
			i = cgltf_parse_json_accessors(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "bufferViews") == 0)
		{
			i = cgltf_parse_json_buffer_views(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "buffers") == 0)
		{
			i = cgltf_parse_json_buffers(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "materials") == 0)
		{
			i = cgltf_parse_json_materials(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "images") == 0)
		{
			i = cgltf_parse_json_images(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "textures") == 0)
		{
			i = cgltf_parse_json_textures(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "samplers") == 0)
		{
			i = cgltf_parse_json_samplers(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "skins") == 0)
		{
			i = cgltf_parse_json_skins(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "cameras") == 0)
		{
			i = cgltf_parse_json_cameras(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "nodes") == 0)
		{
			i = cgltf_parse_json_nodes(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "scenes") == 0)
		{
			i = cgltf_parse_json_scenes(options, tokens, i + 1, json_chunk, data);
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "scene") == 0)
		{
			++i;
			data->scene = (cgltf_scene*)(cgltf_size)cgltf_json_to_int(tokens + i, json_chunk);
			++i;
		}
		else if (cgltf_json_strcmp(tok, json_chunk, "animations") == 0)
		{
			i = cgltf_parse_json_animations(options, tokens, i + 1, json_chunk, data);
		}
		else
		{
			i = cgltf_skip_json(tokens, i + 1);
		}

		if (i < 0)
		{
			options->memory_free(options->memory_user_data, tokens);
			cgltf_free(data);
			return cgltf_result_invalid_json;
		}
	}

	options->memory_free(options->memory_user_data, tokens);

	if (cgltf_fixup_pointers(data) < 0)
	{
		cgltf_free(data);
		return cgltf_result_invalid_json;
	}

	*out_data = data;

	return cgltf_result_success;
}

static int cgltf_fixup_pointers(cgltf_data* data)
{
	for (cgltf_size i = 0; i < data->meshes_count; ++i)
	{
		for (cgltf_size j = 0; j < data->meshes[i].primitives_count; ++j)
		{
			CGLTF_PTRFIXUP(data->meshes[i].primitives[j].indices, data->accessors, data->accessors_count);

			for (cgltf_size k = 0; k < data->meshes[i].primitives[j].attributes_count; ++k)
			{
				CGLTF_PTRFIXUP(data->meshes[i].primitives[j].attributes[k].data, data->accessors, data->accessors_count);
			}
		}
	}

	for (cgltf_size i = 0; i < data->accessors_count; ++i)
	{
		CGLTF_PTRFIXUP(data->accessors[i].buffer_view, data->buffer_views, data->buffer_views_count);

		if (data->accessors[i].buffer_view)
		{
			data->accessors[i].stride = data->accessors[i].buffer_view->stride;
		}

		if (data->accessors[i].stride == 0)
		{
			data->accessors[i].stride = cgltf_calc_size(data->accessors[i].type, data->accessors[i].component_type);
		}
	}

	for (cgltf_size i = 0; i < data->textures_count; ++i)
	{
		CGLTF_PTRFIXUP(data->textures[i].image, data->images, data->images_count);
		CGLTF_PTRFIXUP(data->textures[i].sampler, data->samplers, data->samplers_count);
	}

	for (cgltf_size i = 0; i < data->images_count; ++i)
	{
		CGLTF_PTRFIXUP(data->images[i].buffer_view, data->buffer_views, data->buffer_views_count);
	}

	for (cgltf_size i = 0; i < data->materials_count; ++i)
	{
		CGLTF_PTRFIXUP(data->materials[i].normal_texture.texture, data->textures, data->textures_count);
		CGLTF_PTRFIXUP(data->materials[i].emissive_texture.texture, data->textures, data->textures_count);
		CGLTF_PTRFIXUP(data->materials[i].occlusion_texture.texture, data->textures, data->textures_count);

		CGLTF_PTRFIXUP(data->materials[i].pbr_metallic_roughness.base_color_texture.texture, data->textures, data->textures_count);
		CGLTF_PTRFIXUP(data->materials[i].pbr_metallic_roughness.metallic_roughness_texture.texture, data->textures, data->textures_count);
	}

	for (cgltf_size i = 0; i < data->buffer_views_count; ++i)
	{
		CGLTF_PTRFIXUP(data->buffer_views[i].buffer, data->buffers, data->buffers_count);
	}

	for (cgltf_size i = 0; i < data->skins_count; ++i)
	{
		for (cgltf_size j = 0; j < data->skins[i].joints_count; ++j)
		{
			CGLTF_PTRFIXUP(data->skins[i].joints[j], data->nodes, data->nodes_count);
		}

		CGLTF_PTRFIXUP(data->skins[i].skeleton, data->nodes, data->nodes_count);
		CGLTF_PTRFIXUP(data->skins[i].inverse_bind_matrices, data->accessors, data->accessors_count);
	}

	for (cgltf_size i = 0; i < data->nodes_count; ++i)
	{
		for (cgltf_size j = 0; j < data->nodes[i].children_count; ++j)
		{
			CGLTF_PTRFIXUP(data->nodes[i].children[j], data->nodes, data->nodes_count);
		}

		CGLTF_PTRFIXUP(data->nodes[i].mesh, data->meshes, data->meshes_count);
		CGLTF_PTRFIXUP(data->nodes[i].skin, data->skins, data->skins_count);
		CGLTF_PTRFIXUP(data->nodes[i].camera, data->cameras, data->cameras_count);
	}

	for (cgltf_size i = 0; i < data->scenes_count; ++i)
	{
		for (cgltf_size j = 0; j < data->scenes[i].nodes_count; ++j)
		{
			CGLTF_PTRFIXUP(data->scenes[i].nodes[j], data->nodes, data->nodes_count);
		}
	}

	CGLTF_PTRFIXUP(data->scene, data->scenes, data->scenes_count);

	for (cgltf_size i = 0; i < data->animations_count; ++i)
	{
		for (cgltf_size j = 0; j < data->animations[i].samplers_count; ++j)
		{
			CGLTF_PTRFIXUP(data->animations[i].samplers[j].input, data->accessors, data->accessors_count);
			CGLTF_PTRFIXUP(data->animations[i].samplers[j].output, data->accessors, data->accessors_count);
		}

		for (cgltf_size j = 0; j < data->animations[i].channels_count; ++j)
		{
			CGLTF_PTRFIXUP(data->animations[i].channels[j].sampler, data->animations[i].samplers, data->animations[i].samplers_count);
			CGLTF_PTRFIXUP(data->animations[i].channels[j].target_node, data->nodes, data->nodes_count);
		}
	}

	return 0;
}

/*
 * -- jsmn.c start --
 * Source: https://github.com/zserge/jsmn
 * License: MIT
 */
/**
 * Allocates a fresh unused token from the token pull.
 */
static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser,
				   jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *tok;
	if (parser->toknext >= num_tokens) {
		return NULL;
	}
	tok = &tokens[parser->toknext++];
	tok->start = tok->end = -1;
	tok->size = 0;
#ifdef JSMN_PARENT_LINKS
	tok->parent = -1;
#endif
	return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(jsmntok_t *token, jsmntype_t type,
			    int start, int end) {
	token->type = type;
	token->start = start;
	token->end = end;
	token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int jsmn_parse_primitive(jsmn_parser *parser, const char *js,
				size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;
	int start;

	start = parser->pos;

	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		switch (js[parser->pos]) {
#ifndef JSMN_STRICT
		/* In strict mode primitive must be followed by "," or "}" or "]" */
		case ':':
#endif
		case '\t' : case '\r' : case '\n' : case ' ' :
		case ','  : case ']'  : case '}' :
			goto found;
		}
		if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
			parser->pos = start;
			return JSMN_ERROR_INVAL;
		}
	}
#ifdef JSMN_STRICT
	/* In strict mode primitive must be followed by a comma/object/array */
	parser->pos = start;
	return JSMN_ERROR_PART;
#endif

found:
	if (tokens == NULL) {
		parser->pos--;
		return 0;
	}
	token = jsmn_alloc_token(parser, tokens, num_tokens);
	if (token == NULL) {
		parser->pos = start;
		return JSMN_ERROR_NOMEM;
	}
	jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
#ifdef JSMN_PARENT_LINKS
	token->parent = parser->toksuper;
#endif
	parser->pos--;
	return 0;
}

/**
 * Fills next token with JSON string.
 */
static int jsmn_parse_string(jsmn_parser *parser, const char *js,
			     size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;

	int start = parser->pos;

	parser->pos++;

	/* Skip starting quote */
	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c = js[parser->pos];

		/* Quote: end of string */
		if (c == '\"') {
			if (tokens == NULL) {
				return 0;
			}
			token = jsmn_alloc_token(parser, tokens, num_tokens);
			if (token == NULL) {
				parser->pos = start;
				return JSMN_ERROR_NOMEM;
			}
			jsmn_fill_token(token, JSMN_STRING, start+1, parser->pos);
#ifdef JSMN_PARENT_LINKS
			token->parent = parser->toksuper;
#endif
			return 0;
		}

		/* Backslash: Quoted symbol expected */
		if (c == '\\' && parser->pos + 1 < len) {
			int i;
			parser->pos++;
			switch (js[parser->pos]) {
			/* Allowed escaped symbols */
			case '\"': case '/' : case '\\' : case 'b' :
			case 'f' : case 'r' : case 'n'  : case 't' :
				break;
				/* Allows escaped symbol \uXXXX */
			case 'u':
				parser->pos++;
				for(i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0'; i++) {
					/* If it isn't a hex character we have an error */
					if(!((js[parser->pos] >= 48 && js[parser->pos] <= 57) || /* 0-9 */
					     (js[parser->pos] >= 65 && js[parser->pos] <= 70) || /* A-F */
					     (js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
						parser->pos = start;
						return JSMN_ERROR_INVAL;
					}
					parser->pos++;
				}
				parser->pos--;
				break;
				/* Unexpected symbol */
			default:
				parser->pos = start;
				return JSMN_ERROR_INVAL;
			}
		}
	}
	parser->pos = start;
	return JSMN_ERROR_PART;
}

/**
 * Parse JSON string and fill tokens.
 */
static int jsmn_parse(jsmn_parser *parser, const char *js, size_t len,
	       jsmntok_t *tokens, size_t num_tokens) {
	int r;
	int i;
	jsmntok_t *token;
	int count = parser->toknext;

	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c;
		jsmntype_t type;

		c = js[parser->pos];
		switch (c) {
		case '{': case '[':
			count++;
			if (tokens == NULL) {
				break;
			}
			token = jsmn_alloc_token(parser, tokens, num_tokens);
			if (token == NULL)
				return JSMN_ERROR_NOMEM;
			if (parser->toksuper != -1) {
				tokens[parser->toksuper].size++;
#ifdef JSMN_PARENT_LINKS
				token->parent = parser->toksuper;
#endif
			}
			token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
			token->start = parser->pos;
			parser->toksuper = parser->toknext - 1;
			break;
		case '}': case ']':
			if (tokens == NULL)
				break;
			type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PARENT_LINKS
			if (parser->toknext < 1) {
				return JSMN_ERROR_INVAL;
			}
			token = &tokens[parser->toknext - 1];
			for (;;) {
				if (token->start != -1 && token->end == -1) {
					if (token->type != type) {
						return JSMN_ERROR_INVAL;
					}
					token->end = parser->pos + 1;
					parser->toksuper = token->parent;
					break;
				}
				if (token->parent == -1) {
					if(token->type != type || parser->toksuper == -1) {
						return JSMN_ERROR_INVAL;
					}
					break;
				}
				token = &tokens[token->parent];
			}
#else
			for (i = parser->toknext - 1; i >= 0; i--) {
				token = &tokens[i];
				if (token->start != -1 && token->end == -1) {
					if (token->type != type) {
						return JSMN_ERROR_INVAL;
					}
					parser->toksuper = -1;
					token->end = parser->pos + 1;
					break;
				}
			}
			/* Error if unmatched closing bracket */
			if (i == -1) return JSMN_ERROR_INVAL;
			for (; i >= 0; i--) {
				token = &tokens[i];
				if (token->start != -1 && token->end == -1) {
					parser->toksuper = i;
					break;
				}
			}
#endif
			break;
		case '\"':
			r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
			if (r < 0) return r;
			count++;
			if (parser->toksuper != -1 && tokens != NULL)
				tokens[parser->toksuper].size++;
			break;
		case '\t' : case '\r' : case '\n' : case ' ':
			break;
		case ':':
			parser->toksuper = parser->toknext - 1;
			break;
		case ',':
			if (tokens != NULL && parser->toksuper != -1 &&
					tokens[parser->toksuper].type != JSMN_ARRAY &&
					tokens[parser->toksuper].type != JSMN_OBJECT) {
#ifdef JSMN_PARENT_LINKS
				parser->toksuper = tokens[parser->toksuper].parent;
#else
				for (i = parser->toknext - 1; i >= 0; i--) {
					if (tokens[i].type == JSMN_ARRAY || tokens[i].type == JSMN_OBJECT) {
						if (tokens[i].start != -1 && tokens[i].end == -1) {
							parser->toksuper = i;
							break;
						}
					}
				}
#endif
			}
			break;
#ifdef JSMN_STRICT
			/* In strict mode primitives are: numbers and booleans */
		case '-': case '0': case '1' : case '2': case '3' : case '4':
		case '5': case '6': case '7' : case '8': case '9':
		case 't': case 'f': case 'n' :
			/* And they must not be keys of the object */
			if (tokens != NULL && parser->toksuper != -1) {
				jsmntok_t *t = &tokens[parser->toksuper];
				if (t->type == JSMN_OBJECT ||
						(t->type == JSMN_STRING && t->size != 0)) {
					return JSMN_ERROR_INVAL;
				}
			}
#else
			/* In non-strict mode every unquoted value is a primitive */
		default:
#endif
			r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
			if (r < 0) return r;
			count++;
			if (parser->toksuper != -1 && tokens != NULL)
				tokens[parser->toksuper].size++;
			break;

#ifdef JSMN_STRICT
			/* Unexpected char in strict mode */
		default:
			return JSMN_ERROR_INVAL;
#endif
		}
	}

	if (tokens != NULL) {
		for (i = parser->toknext - 1; i >= 0; i--) {
			/* Unmatched opened object or array */
			if (tokens[i].start != -1 && tokens[i].end == -1) {
				return JSMN_ERROR_PART;
			}
		}
	}

	return count;
}

/**
 * Creates a new parser based over a given  buffer with an array of tokens
 * available.
 */
static void jsmn_init(jsmn_parser *parser) {
	parser->pos = 0;
	parser->toknext = 0;
	parser->toksuper = -1;
}
/*
 * -- jsmn.c end --
 */

#endif /* #ifdef CGLTF_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif
