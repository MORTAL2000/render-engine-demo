#include "Schematic.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "Material.h"
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

		initTorchic();

		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
		aiDetachAllLogStreams();
	}

	// PRIVATE
	std::unordered_map<const char*, SchematicNode*> Schematic::schematicNodeMap;

	SchematicNode* Schematic::buildSchematic(const aiScene *scene, aiNode *node)
	{
		//if (node->mNumMeshes == 0)
		//	return NULL;

		SchematicNode *schematicNode = new SchematicNode();

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
			for (int j = 0; j < mesh->mNumVertices; ++j)
				vertices.push_back(vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f));

			std::vector<vec4> normals;
			if (mesh->HasNormals())
			{
				for (int j = 0; j < mesh->mNumVertices; ++j)
					normals.push_back(vec4(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 1.0f));
			}
			else
				normals = std::vector<vec4>(vertices);

			std::vector<vec4> tangents;
			std::vector<vec4> binormals;
			if (mesh->HasTangentsAndBitangents())
			{
				for (int j = 0; j < mesh->mNumVertices; ++j)
				{
					tangents.push_back(vec4(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z, 1.0f));
					binormals.push_back(vec4(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z, 1.0f));
				}
			}
			else
			{
				tangents = std::vector<vec4>(vertices);
				binormals = std::vector<vec4>(vertices);
			}

			std::vector<vec2> textureCoordinates;
			for (int j = 0; j < mesh->mNumVertices; ++j)
				textureCoordinates.push_back(vec2(0.0f, 0.0f));

			int vOffset = Shader::Vertices.size();
			int vCount = vertices.size();

			Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());
			Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
			Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
			Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
			Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoordinates.begin(), textureCoordinates.end());

			auto aiMat = scene->mMaterials[mesh->mMaterialIndex];

			schematicNode->vertexMeshes.push_back(VertexMesh(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 0.0f)), vOffset, vCount, false));
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
}
