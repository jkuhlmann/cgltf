#include "jsmn.h"

#define CGLTF_IMPLEMENTATION
#define CGLTF_EXTERNAL_JSMN
#include "../cgltf.h"

int main(int argc, char** argv)
{
	((void) argc);	// unused
	((void) argv);	// unused

	// DM: nothing to do in this sample - we only care that that it compiles when you point it towards an external jsmn implementation

	return 0;
}