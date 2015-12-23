#ifndef CUBE_H
#define CUBE_H

#include "DrawableObject.h"

namespace Bagnall
{
	class Cube : public DrawableObject
	{
	public:
		static void Init();

		Cube(Object *par);

		bool ContainsPoint(const vec4& p) const;

	private:
		static int globalVertexOffset;
		static int vertexCount;

		static void createPrototypeVertices();
	};
}

#endif
