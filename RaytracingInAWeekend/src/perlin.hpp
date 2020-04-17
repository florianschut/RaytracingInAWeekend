#pragma once
#include <glm/vec3.hpp>
#include "random.hpp"

class Perlin
{
public:
	Perlin()
	{
		random_vec_ = Generate();
		perm_x_ = GeneratePerm();
		perm_y_ = GeneratePerm();
		perm_z_ = GeneratePerm();
	}
	
	float Noise(const glm::vec3& p) const
	{
		float u = p.x - floor(p.x);
		float v = p.y - floor(p.y);
		float w = p.z - floor(p.z);

		int i = static_cast<int>(floor(p.x));
		int j = static_cast<int>(floor(p.y));
		int k = static_cast<int>(floor(p.z));
		glm::vec3 c[2][2][2];
		
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = random_vec_[
					perm_x_[(i + di) & 255] ^
					perm_y_[(j + dj) & 255] ^
					perm_z_[(k + dk) & 255]];
		return TrilinearInterp(c, u, v, w);
	}

	float Turb(const glm::vec3& p, int depth = 7) const
	{
		float accum = 0.f;
		glm::vec3 temp_p = p;
		float weight = 1.f;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * Noise(temp_p);
			weight *= 0.5f;
			temp_p *= 2.f;
		}
		return fabs(accum);
	}
	
private:
	static glm::vec3* Generate()
	{
		glm::vec3* p = new glm::vec3[256];
		for (auto i = 0; i < 256; ++i)
		{
			float random_x = 2.f * utility::RandomFloat() - 1.f;
			float random_y = 2.f * utility::RandomFloat() - 1.f;
			float random_z = 2.f * utility::RandomFloat() - 1.f;
			p[i] = glm::normalize(glm::vec3(random_x, random_y, random_z));
		}
		return p;
	}

	static void Permute(int* p, int n)
	{
		for (auto i = n - 1; i > 0; i--)
		{
			auto target = static_cast<int>(utility::RandomFloat() * (i + 1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static int* GeneratePerm()
	{
		int* p = new int[256];
		for (auto i = 0; i < 256; i++)
			p[i] = i;
		Permute(p, 256);
		return p;
	}

	static float TrilinearInterp(glm::vec3 c[2][2][2], float u, float v, float w)
	{
		float uu = u * u * (3.f - 2.f * u);
		float vv = v * v * (3.f - 2.f * v);
		float ww = w * w * (3.f - 2.f * w);
		float accumulated = 0.f;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
				{
					glm::vec3 weight_v(u -i, v - j, w - k);
					accumulated += 
						(i * uu + (1.f - i) * (1.f - uu)) *
						(j * vv + (1.f - j) * (1.f - vv)) *
						(k * ww + (1.f - k) * (1.f - ww)) * glm::dot(c[i][j][k], weight_v);
				}
		return accumulated;
	}

	
	
	glm::vec3* random_vec_;
	int* perm_x_;
	int* perm_y_;
	int* perm_z_;
};
