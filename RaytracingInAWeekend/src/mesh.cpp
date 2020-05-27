#include "mesh.hpp"

#include <ImGui/imgui.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Mesh::Mesh(float* vertices, unsigned int stride, unsigned int pos_offset, unsigned int normal_offset, unsigned int triangle_count, std::shared_ptr<Material> material)
	:material_(material)
{
	name_ = "Mesh from array";
	triangle_list_ = std::make_shared<HittableList>(triangle_count);
	for (auto i = 0u; i < triangle_count; i++)
	{
		//triangle_list_->Add(std::make_shared<Triangle>(glm::vec3(vertices[i * stride + pos_offset], vertices[i * stride + pos_offset + 1u], vertices[i * stride + pos_offset + 2u]), glm::vec3(vertices[i * stride + pos_offset + 5u], vertices[i * stride + pos_offset + 6u], vertices[i * stride + pos_offset + 7u]), glm::vec3(vertices[i * stride + pos_offset + 10u], vertices[i * stride + pos_offset + 11u], vertices[i * stride + pos_offset + 12u]), material_));
		triangle_list_->Add(std::make_shared<Triangle>(vertices + i * stride * 3, pos_offset, normal_offset, stride, material_));
	}
}

Mesh::Mesh(const char* path, std::shared_ptr<Material> material)
{
	name_ = "Mesh at path: ";
	name_.append(path);
	Assimp::Importer import;
	const aiScene* teapot = import.ReadFile(path, aiProcess_Triangulate);
	/*if (!teapot || teapot->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !teapot->mRootNode)
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";*/

	auto tris_count = 0u;
	for (auto i = 0u; i < teapot->mNumMeshes; i++)
		tris_count += teapot->mMeshes[i]->mNumFaces;
	triangle_list_ = std::make_shared<HittableList>(tris_count);

	//glm::vec3 normal;
	glm::vec3 vertices[3];

	for (auto i = 0u; i < teapot->mNumMeshes; i++)
	{
		auto mesh_data = teapot->mMeshes[i];
		for (auto j = 0u; j < mesh_data->mNumFaces; j++)
		{
			auto face = mesh_data->mFaces[j];
			/*normal.x = mesh_data->mNormals[face.mIndices[0]].x;
			normal.y = mesh_data->mNormals[face.mIndices[0]].y;
			normal.z = mesh_data->mNormals[face.mIndices[0]].z;*/

			for (unsigned int w = 0; w < face.mNumIndices; w++)
			{
				vertices[w].x = mesh_data->mVertices[face.mIndices[w]].x;
				vertices[w].y = mesh_data->mVertices[face.mIndices[w]].y;
				vertices[w].z = mesh_data->mVertices[face.mIndices[w]].z;
			}
			triangle_list_->Add(std::make_shared<Triangle>(vertices, material));
		}
	}

	mesh_bvh = std::make_shared<BvhNode>(*triangle_list_, 0.f, 1.f);
}

void Mesh::ImGuiMenu()
{
	if(ImGui::CollapsingHeader(name_.c_str()))
	{
		ImGui::Text("%i Triangles",triangle_list_->objects_.size());
		ImGui::Separator();
	}

}
