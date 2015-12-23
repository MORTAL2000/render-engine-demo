#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Object.h"
#include "VertexMesh.h"

namespace Bagnall
{
	class VertexMesh;
	struct SchematicNode;

	class DrawableObject : public Object
	{
	public:
		DrawableObject(Object *par);

		DrawableObject(Object *par, SchematicNode *schematic);

		virtual void Draw() const;

		void SendTransformToGPU();

		void AddVertexMesh(const VertexMesh& vertexMesh);

		void SetEmissive(bool e);

		virtual void SetEmissionColor(const vec4& color);

		//void SetBlend(bool b);

		virtual void SetMaterial(const Material& m);

		void SetTexture(GLuint tex);

		void SetCubeMap (GLuint cm);

		void EnableRender();

		void DisableRender();

		void Cull();

		void UnCull();

		bool GetRenderEnabled() const;

		void SetBumpMapEnabled(bool b);

		void SetReflectiveCubeMap(bool b);

	private:
		bool renderEnabled;

	protected:
		std::vector<VertexMesh> vertexMeshes;

		void init();

		void updateRenderNodes();
	};
}

#endif
