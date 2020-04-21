#include "cube_map.hpp"

static const char* const texture_paths[6] = {
	"px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"
};

CubeMap::CubeMap(const char* dir_name)
{
	for (int i = 0; i < 6; i++)
	{
		char path[100];
		strcpy_s(path, dir_name);
		strcat_s(path, texture_paths[i]);
		faces_[i] = new ImageTexture(path);
	}
}

CubeMap::~CubeMap()
{
	delete[] &faces_;
}
