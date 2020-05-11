#include "textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_img/stb_image.h>

ImageTexture::ImageTexture(const char* path)
{
	img_data_ = stbi_loadf(path, &width_, &height_, &comp_, 0);
	if (img_data_ == nullptr)
		std::cerr << "Texture at path " << path << " not found\n";
}

ImageTexture::ImageTexture(float* img_data, const int width, const int height) :img_data_(img_data), width_(width), height_(height) {}

ImageTexture::~ImageTexture()
{
	delete[] img_data_;
}
