#ifndef SKYBOX_H
#define SKYBOX_H

#include "DrawableObject.h"

namespace Bagnall
{
	class Skybox : public DrawableObject
	{
	public:
		static void Init();

		Skybox(Object *par);

		void Draw() const;

	private:
		static int indexOffset;
		static int indexCount;

		static void createPrototypeVertices();
	};
}

#endif
