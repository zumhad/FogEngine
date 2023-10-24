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
}

Model::Model(const Model& model) : Object(model)
{
	name = model.name;
	texture = model.texture;
	color = model.color;
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
			static Vector3 pos;
			pos.x = mesh->mVertices[j].x;
			pos.y = mesh->mVertices[j].y;
			pos.z = mesh->mVertices[j].z;

			static Vector3 normal;
			normal.x = mesh->mNormals[j].x;
			normal.y = mesh->mNormals[j].y;
			normal.z = mesh->mNormals[j].z;

			static Vector2 uv;
			uv.x = mesh->mTextureCoords[0][j].x;
			uv.y = mesh->mTextureCoords[0][j].y;

			vertex[j] = Vertex(pos, normal, uv);
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
	return XMMatrixAffineTransformation(scale, XMVectorSet(0, 0, 0, 1), q, position);
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