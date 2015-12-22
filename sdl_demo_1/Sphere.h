#ifndef SPHERE_H
#define SPHERE_H

#include "DrawableObject.h"

namespace Bagnall
{
	class Sphere : public DrawableObject
	{
	public:
		static void Init();

		Sphere(Object *par);

		void Draw() const;

	private:
		static int globalVertexOffset; // location of sphere vertices in global array
		static int vertexCount; // # of cube vertices

		static void createPrototypeVertices();
	};
}

#endif
