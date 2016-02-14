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
		static int indexOffset;
		static int indexCount;

		static void createPrototypeVertices();
	};
}

#endif
