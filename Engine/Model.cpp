#include "Model.h"

#include "PathHelper.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Mesh.h"

#include <d3d11.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace Assimp;

Model::Model() : Object()
{
	mModelPath = L"";
	mTexturePath = L"white.png";
	mColor = Color(1.0f, 1.0f, 1.0f);
	mLighting = true;
	mMetallic = 0.0f;
	mRoughness = 1.0f;
}

Model::Model(const Model& model) : Object(model)
{
	mModelPath = model.mModelPath;
	mTexturePath = model.mTexturePath;
	mColor = model.mColor;
	mLighting = model.mLighting;
	mMetallic = model.mMetallic;
	mRoughness = model.mRoughness;
}

Model::Model(Model&& model) noexcept : Model(model)
{
	String path = PathHelper::GetAssetsPath();
	path += mModelPath;

	Importer importer;
	const aiScene* scene = importer.ReadFile(StringConverter(path), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

	mArr.Resize(scene->mNumMeshes);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		static Array<Vertex> vertex;
		vertex.Resize(mesh->mNumVertices);

		static Array<unsigned int> index;
		index.Resize(mesh->mNumFaces * 3);

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			static Vector3 v;

			v.x = mesh->mVertices[j].x;
			v.y = mesh->mVertices[j].y;
			v.z = mesh->mVertices[j].z;
			vertex[j].position = v;

			if (mesh->HasNormals())
			{
				v.x = mesh->mNormals[j].x;
				v.y = mesh->mNormals[j].y;
				v.z = mesh->mNormals[j].z;
				vertex[j].normal = v;
			}
			else vertex[j].normal = Vector3::Zero();

			if (mesh->mTextureCoords[0])
			{
				v.x = mesh->mTextureCoords[0][j].x;
				v.y = mesh->mTextureCoords[0][j].y;
				vertex[j].uv = Vector2(v.x, v.y);
			}
			else vertex[j].uv = Vector2(0.0f, 0.0f);
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				index[j * 3 + k] = mesh->mFaces[j].mIndices[k];
			}
		}

		mArr[i] = new Mesh(vertex, index);
	}

	mTexture.Create(mTexturePath);
}

void Model::SetModelPath(const String& modelPath)
{
	mModelPath = modelPath;
}

String Model::GetModelPath()
{
	return mModelPath;
}

void Model::SetTexturePath(const String& texturePath)
{
	mTexturePath = texturePath;
}

String Model::GetTexturePath()
{
	return mTexturePath;
}

void Model::SetColor(const Color& color)
{
	mColor = color;
}

void Model::SetColorR(float r)
{
	mColor.r = r;
}

void Model::SetColorG(float g)
{
	mColor.g = g;
}

void Model::SetColorB(float b)
{
	mColor.b = b;
}

Color Model::GetColor()
{
	return mColor;
}

void Model::SetLighting(bool lighting)
{
	mLighting = lighting;
}

bool Model::GetLighting()
{
	return mLighting;
}

void Model::SetMetallic(float metallic)
{
	mMetallic = metallic;
}

float Model::GetMetallic()
{
	return mMetallic;
}

void Model::SetRoughness(float roughness)
{
	mRoughness = roughness;
}

float Model::GetRoughness()
{
	return mRoughness;
}

void Model::BindTexture()
{
	mTexture.Bind();
}

void Model::Draw()
{
	int size = mArr.Size();
	for (int i = 0; i < size; i++)
	{
		mArr[i]->Draw();
	}
}

Model::~Model()
{
	int size = mArr.Size();
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(mArr[i]);
	}

	mTexture.Release();
}