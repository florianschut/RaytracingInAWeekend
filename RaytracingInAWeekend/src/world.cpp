#include "world.hpp"

#include <stb_img/stb_image.h>

EquirectBackground::EquirectBackground(const char* path, float rotation)
{
	img_data_ = stbi_loadf(path,&width_, &height_, &comp_, 0);
	if (img_data_ == nullptr)
		std::cerr << "Texture at path " << path << " not found\n";

	rotation_deg_ = rotation;
	if (rotation_deg_ == 0.f)
		return;
	
	float s = sin((rotation / 180.f) * static_cast<float>(M_PI));
	float c = cos((rotation / 180.f) * static_cast<float>(M_PI));
	rotation_ = glm::mat2(c, -s, s, c);
	rotation_ *= 0.5f;
	rotation_ += 0.5f;
	rotation_ = rotation_ * 2.f - 1.f;
}

EquirectBackground::~EquirectBackground()
{
	delete[] img_data_;
};
