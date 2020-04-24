#pragma once
#include "textures.hpp"

class CubeMap
{
public:
	CubeMap(const char* dir_name);
	~CubeMap();

	[[nodiscard]] glm::vec3 Value(glm::vec3 dir) const 
	{
		//Based on L. Spiro's code shared at https://www.gamedev.net/forums/topic/687535-implementing-a-cube-map-lookup-function/
		glm::vec3 dir_abs = abs(dir);

		if(dir_abs.z >= dir_abs.x && dir_abs.z >= dir_abs.y)
			return faces_[dir.z < 0.0f ? 5 : 4]->Value(glm::vec2(dir.z > 0.0f ? -dir.x : dir.x, dir.y) * 0.5f / dir_abs.z + 0.5f, dir);
		if(dir_abs.y >= dir_abs.x)
			return faces_[dir.y < 0.0f ? 3 : 2]->Value(glm::vec2(-dir.x, dir.y > 0.0f ? -dir.z : -dir.z) * 0.5f / dir_abs.y + 0.5f, dir);
		return faces_[dir.x > 0.0f ? 1 : 0]->Value(glm::vec2(dir.x > 0.0f ? dir.z : -dir.z, dir.y) * 0.5f / dir_abs.x + 0.5f, dir);
		
	}
	
private:
	ImageTexture* faces_[6]{};
};
