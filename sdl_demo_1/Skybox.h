#ifndef SKYBOX_H
#define SKYBOX_H

#include "DrawableObject.h"

namespace Bagnall
{
	class Skybox : public DrawableObject
	{
	public:
		static void Init();

		Skybox(Object *par) : DrawableObject(par) {}

		void Draw() const;

	private:
		static int globalVertexOffset;
		static int vertexCount;

		static void createPrototypeVertices();
	};
}

#endif
