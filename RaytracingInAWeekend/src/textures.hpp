#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "perlin.h"

class Texture
{
public:
	virtual ~Texture() {};
	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const = 0;
	glm::vec3 Value(const glm::vec2 uv, const glm::vec3& p) const { return Value(uv.x, uv.y, p); };
};

class ConstantTexture: public Texture
{
public:
	ConstantTexture(const glm::vec3 col = glm::vec3(1.f)) : color_(col){}
	glm::vec3 Value(float, float, const glm::vec3&) const override { return color_; }
	
private:
	glm::vec3 color_;
};

class CheckerTexture: public Texture
{
public:
	CheckerTexture(Texture* texture1 = new ConstantTexture(glm::vec3(1.f)), Texture* texture2 = new ConstantTexture(glm::vec3(0.f))): even_(texture1), odd_(texture2){};
	~CheckerTexture() { delete even_; delete odd_; }

	glm::vec3 Value(float u, float v, const glm::vec3& p) const override
	{
		const auto sines = sin(10.f * p.x) * sin(10.f * p.y) * sin(10.f * p.z);
		if (sines < 0.f)
			return even_->Value(u, v, p);
		else 
			return odd_->Value(u, v, p);
	}
	
private:
	Texture* even_;
	Texture* odd_;
};

class ImageTexture: public Texture
{
public:
	ImageTexture(unsigned char* img_data, const int width, const int height) :img_data_(img_data), width_(width), height_(height) {}
	~ImageTexture();
	glm::vec3 Value(float u, float v, const glm::vec3& p) const override;

private:
	unsigned char* img_data_;
	const int width_, height_;
};

inline glm::vec3 ImageTexture::Value(const float u, const float v, const glm::vec3&) const
{
	int i = static_cast<int>(u * width_);
	int j = static_cast<int>((1.f - v) * height_ - 0.001f);

	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > width_ - 1) i = width_ - 1;
	if (j > height_ - 1) j = height_ - 1;
	float r = static_cast<float>(static_cast<int>(img_data_[3 * i + 3 * width_ * j])) / 255.f;
	float g = static_cast<float>(static_cast<int>(img_data_[3 * i + 3 * width_ * j + 1])) / 255.f;
	float b = static_cast<float>(static_cast<int>(img_data_[3 * i + 3 * width_ * j + 2])) / 255.f;
	return glm::vec3(r, g, b);
}

inline ImageTexture::~ImageTexture()
{
	delete[] img_data_;
}

class NoiseTexture: public Texture
{
public:
	NoiseTexture(float scale = 1.f): scale_(scale) {};
	glm::vec3 Value(float, float, const glm::vec3& p) const
	{
		return glm::vec3(1) * 0.5f * (1 + sin(scale_ * p.z + 10 * noise_.Turb(p)));
	}
private:
	Perlin noise_;
	float scale_;
};