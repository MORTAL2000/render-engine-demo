#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "DrawableObject.h"

namespace Bagnall
{
	class Rectangle : public DrawableObject
	{
	public:
		static void Init();

		Rectangle(Object *par);

		void Draw() const;

	private:
		static int globalVertexOffset;
		static int vertexCount;

		static void createPrototypeVertices();
	};
}

#endif
