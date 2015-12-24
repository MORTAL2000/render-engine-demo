#ifndef HEIGHTMAPTERRAIN_H
#define HEIGHTMAPTERRAIN_H

namespace Bagnall
{
	class VertexMesh;

	class HeightMapTerrain
	{
	public:
		static VertexMesh CreateTerrainMeshFromFile(const char *fileName, float scaleZ);

	private:
	};
}

#endif
