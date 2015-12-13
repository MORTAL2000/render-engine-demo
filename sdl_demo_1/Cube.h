#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

namespace Bagnall
{
	class Cube : public Object
	{
	public:
		/**********************************************************************
		Function:		Static initializer for the Cube class
		Purpose:		Do static initialization of the Cube class.
		Calls:			createPrototypeVertices
		**********************************************************************/
		static void Init();

		Cube(Object *par) : Object(par) {}

		/**********************************************************************
		Function:		Draw
		Purpose:		Draw the cube
		**********************************************************************/
		void Draw() const;

		/*void DrawRaw() const;

		void DrawEmissive() const;

		void DrawWithTexture(GLuint t) const;

		void DrawWithTextureAndBumpMap(GLuint t, GLuint bmap) const;*/

		bool ContainsPoint(const vec4& p) const;

	private:
		static int globalVertexOffset; // location of cube vertices in global array
		static int vertexCount; // # of cube vertices

								/**********************************************************************
								Function:		createPrototypeVertices
								Purpose:		Create the prototype vertices for cubes and put them in
								the global vertex array
								**********************************************************************/
		static void createPrototypeVertices();
	};
}

#endif
