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
using namespace DirectX;

Model::Model() : Object()
{
	name = L"";
	texture = L"white.png";
	color = Color(1.0f, 1.0f, 1.0f);
	lighting = true;
}

Model::Model(const Model& model) : Object(model)
{
	name = model.name;
	texture = model.texture;
	color = model.color;
	lighting = model.lighting;
}

Model::Model(Model&& model) noexcept : Model(model)
{
	String path = PathHelper::GetAssetsPath();
	path += name;

	Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

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

	mTexture.Create(texture);
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

Matrix Model::GetWorldMatrix()
{
	Quaternion q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));
	return XMMatrixAffineTransformation(scale, Vector3(0.0f, 0.0f, 0.0f), q, position);
}

Matrix Model::GetWorldInvTransposeMatrix(Matrix& world)
{
	return XMMatrixTranspose(XMMatrixInverse(0, world));
}

void Model::SetPosition(Vector3 v)
{
	position = v;
}

void Model::SetRotation(Vector3 v)
{
	rotation = v;
}

void Model::SetScale(Vector3 v)
{
	scale = v;
}

Vector3 Model::GetPosition()
{
	return position;
}

Vector3 Model::GetRotation()
{
	return rotation;
}

Vector3 Model::GetScale()
{
	return scale;
}

void Model::Move(Vector3 v)
{
	position += v;
}

void Model::Rotate(Vector3 v)
{
	rotation += v;
}

void Model::Scale(Vector3)
{
	//scale = v;
}

Vector3 Model::GetDirection()
{
	Vector3 v = Vector3(0.0f, 0.0f, 1.0f);
	Quaternion q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));

	return XMVector3Rotate(v, q);
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