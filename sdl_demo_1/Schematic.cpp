#include "Schematic.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "Material.h"
#include "Util.h"
#include <glm/vec4.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	void Schematic::Init()
	{
		aiLogStream stream;
		// get a handle to the predefined STDOUT log stream and attach
		// it to the logging system. It remains active for all further
		// calls to aiImportFile(Ex) and aiApplyPostProcessing.
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
		aiAttachLogStream(&stream);
		// ... same procedure, but this stream now writes the
		// log messages to assimp_log.txt
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
		aiAttachLogStream(&stream);

		//initTorchic();

		initMillenniumFalcon();

		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
		aiDetachAllLogStreams();
	}

	SchematicNode* Schematic::GetSchematicByName(const char *name)
	{
		return schematicNodeMap[name];
	}

	// PRIVATE
	std::unordered_map<const char*, SchematicNode*> Schematic::schematicNodeMap;

	SchematicNode* Schematic::buildSchematic(const aiScene *scene, aiNode *node)
	{
		SchematicNode *schematicNode = new SchematicNode();

		/*aiVector3D scaling;
		aiQuaterniont<float> rotation;
		aiVector3D position;

		node->mTransformation.Decompose(scaling, rotation, position);*/

		schematicNode->transform = mat4::tmat4x4(
			vec4(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4),
			vec4(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4),
			vec4(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4),
			vec4(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4)
		);

		for (uint i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			std::vector<vec4> vertices;
			/*for (int j = 0; j < mesh->mNumVertices; ++j)
				vertices.push_back(vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f));*/
			for (int j = 0; j < mesh->mNumFaces; ++j)
			{
				auto face = mesh->mFaces[j];
				if (face.mNumIndices == 3)
				{
					vertices.push_back(vec4(mesh->mVertices[face.mIndices[0]].x, mesh->mVertices[face.mIndices[0]].y, mesh->mVertices[face.mIndices[0]].z, 1.0f));
					vertices.push_back(vec4(mesh->mVertices[face.mIndices[1]].x, mesh->mVertices[face.mIndices[1]].y, mesh->mVertices[face.mIndices[1]].z, 1.0f));
					vertices.push_back(vec4(mesh->mVertices[face.mIndices[2]].x, mesh->mVertices[face.mIndices[2]].y, mesh->mVertices[face.mIndices[2]].z, 1.0f));
				}
			}

			std::vector<vec4> normals;
			if (mesh->HasNormals())
			{
				//for (int j = 0; j < mesh->mNumVertices; ++j)
				//	normals.push_back(vec4(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 1.0f));
				for (int j = 0; j < mesh->mNumFaces; ++j)
				{
					auto face = mesh->mFaces[j];
					if (face.mNumIndices == 3)
					{
						normals.push_back(vec4(mesh->mNormals[face.mIndices[0]].x, mesh->mNormals[face.mIndices[0]].y, mesh->mNormals[face.mIndices[0]].z, 0.0f));
						normals.push_back(vec4(mesh->mNormals[face.mIndices[1]].x, mesh->mNormals[face.mIndices[1]].y, mesh->mNormals[face.mIndices[1]].z, 0.0f));
						normals.push_back(vec4(mesh->mNormals[face.mIndices[2]].x, mesh->mNormals[face.mIndices[2]].y, mesh->mNormals[face.mIndices[2]].z, 0.0f));
					}
				}
			}
			else
				normals = std::vector<vec4>(vertices);

			std::vector<vec4> tangents;
			std::vector<vec4> binormals;
			if (mesh->HasTangentsAndBitangents())
			{
				/*for (int j = 0; j < mesh->mNumVertices; ++j)
				{
					tangents.push_back(vec4(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z, 1.0f));
					binormals.push_back(vec4(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z, 1.0f));
				}*/
				for (int j = 0; j < mesh->mNumFaces; ++j)
				{
					auto face = mesh->mFaces[j];
					if (face.mNumIndices == 3)
					{
						tangents.push_back(vec4(mesh->mTangents[face.mIndices[0]].x, mesh->mTangents[face.mIndices[0]].y, mesh->mTangents[face.mIndices[0]].z, 0.0f));
						tangents.push_back(vec4(mesh->mTangents[face.mIndices[1]].x, mesh->mTangents[face.mIndices[1]].y, mesh->mTangents[face.mIndices[1]].z, 0.0f));
						tangents.push_back(vec4(mesh->mTangents[face.mIndices[2]].x, mesh->mTangents[face.mIndices[2]].y, mesh->mTangents[face.mIndices[2]].z, 0.0f));

						binormals.push_back(vec4(mesh->mBitangents[face.mIndices[0]].x, mesh->mBitangents[face.mIndices[0]].y, mesh->mBitangents[face.mIndices[0]].z, 0.0f));
						binormals.push_back(vec4(mesh->mBitangents[face.mIndices[1]].x, mesh->mBitangents[face.mIndices[1]].y, mesh->mBitangents[face.mIndices[1]].z, 0.0f));
						binormals.push_back(vec4(mesh->mBitangents[face.mIndices[2]].x, mesh->mBitangents[face.mIndices[2]].y, mesh->mBitangents[face.mIndices[2]].z, 0.0f));
					}
				}
			}
			else
			{
				tangents = std::vector<vec4>(vertices);
				binormals = std::vector<vec4>(vertices);
			}

			std::vector<vec2> textureCoordinates;
			if (mesh->HasTextureCoords(0))
			{
				for (int j = 0; j < mesh->mNumFaces; ++j)
				{
					auto face = mesh->mFaces[j];
					if (face.mNumIndices == 3)
					{
						textureCoordinates.push_back(vec2(mesh->mTextureCoords[0][face.mIndices[0]].x, mesh->mTextureCoords[0][face.mIndices[0]].y));
						textureCoordinates.push_back(vec2(mesh->mTextureCoords[0][face.mIndices[1]].x, mesh->mTextureCoords[0][face.mIndices[1]].y));
						textureCoordinates.push_back(vec2(mesh->mTextureCoords[0][face.mIndices[2]].x, mesh->mTextureCoords[0][face.mIndices[2]].y));
					}
				}
			}
			//for (int j = 0; j < mesh->mNumVertices; ++j)
			//for (int j = 0; j < vertices.size(); ++j)
			//	textureCoordinates.push_back(vec2(0.0f, 0.0f));

			/*vertices = Util::TransformVertices(vertices, schematicNode->transform);
			normals = Util::TransformVertices(normals, schematicNode->transform);
			tangents = Util::TransformVertices(tangents, schematicNode->transform);
			binormals = Util::TransformVertices(binormals, schematicNode->transform);*/

			int vOffset = Shader::Vertices.size();
			int vCount = vertices.size();

			Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());
			Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
			Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
			Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
			Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoordinates.begin(), textureCoordinates.end());

			auto aiMat = scene->mMaterials[mesh->mMaterialIndex];

			//schematicNode->vertexMeshes.push_back(VertexMesh(Material::Plastic(vec4(0.5f, 0.5f, 0.5f, 1.0f)), vOffset, vCount, false));
			//schematicNode->vertexMeshes.push_back(VertexMesh(Material::Plastic(vec4(1.0f, 0.0f, 0.0f, 1.0f)), vOffset, vCount, false));
			schematicNode->vertexMeshes.push_back(VertexMesh(Material::Chrome(), vOffset, vCount, false));
		}

		// recursively build children schematic nodes
		for (uint i = 0; i < node->mNumChildren; ++i)
		{
			SchematicNode *childNode = buildSchematic(scene, node->mChildren[i]);
			if (childNode != NULL)
				schematicNode->children.push_back(childNode);
		}

		return schematicNode;
	}

	void Schematic::initTorchic()
	{
		const aiScene *scene = NULL;

		if ((scene = aiImportFile("model\\torchic.blend", aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
		//if ((scene = aiImportFile("model\\torchic.blend", aiProcessPreset_TargetRealtime_Fast)) != NULL)
		{
			SchematicNode *schematic = buildSchematic(scene, scene->mRootNode);

			schematicNodeMap.emplace("torchic", schematic);
		}
		else
		{
			std::cerr << aiGetErrorString() << std::endl;
		}

		// cleanup - calling 'aiReleaseImport' is important, as the library 
		// keeps internal resources until the scene is freed again. Not 
		// doing so can cause severe resource leaking.
		aiReleaseImport(scene);
	}

	void Schematic::initMillenniumFalcon()
	{
		const aiScene *scene = NULL;

		if ((scene = aiImportFile("model\\Millennium_Falcon.obj", aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
			//if ((scene = aiImportFile("model\\torchic.blend", aiProcessPreset_TargetRealtime_Fast)) != NULL)
		{
			SchematicNode *schematic = buildSchematic(scene, scene->mRootNode);

			schematicNodeMap.emplace("millennium_falcon", schematic);
		}
		else
		{
			std::cerr << aiGetErrorString() << std::endl;
		}

		// cleanup - calling 'aiReleaseImport' is important, as the library 
		// keeps internal resources until the scene is freed again. Not 
		// doing so can cause severe resource leaking.
		aiReleaseImport(scene);
	}
}
