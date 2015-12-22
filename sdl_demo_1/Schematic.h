#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "Material.h"
#include "VertexMesh.h"
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>
#include <vector>
#include <unordered_map>

namespace Bagnall
{
	struct SchematicNode
	{
		mat4 transform;
		std::vector<VertexMesh> vertexMeshes;

		std::vector<SchematicNode*> children;
	};

	class Schematic
	{
	public:
		static void Init();

		static SchematicNode* GetSchematicByName(const char *name);

	private:
		static std::unordered_map<const char*, SchematicNode*> schematicNodeMap;

		static SchematicNode* buildSchematic(const aiScene *scene, aiNode *node);

		static void initTorchic();

		static void initMillenniumFalcon();

		static void initStarDestroyer();

		static void initR2D2();

		static void initC3PO();
	};
}

#endif
