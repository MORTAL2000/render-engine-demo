#ifndef MATERIAL_H
#define MATERIAL_H

#include "Bagnall.h"
#include <vector>

namespace Bagnall
{
	struct Material
	{
		vec4 ambient; // ambient color
		vec4 diffuse; // diffuse color
		vec4 specular; // specular color
		vec4 emission; // emission color
		float shininess; // shininess coefficient

						 /**********************************************************************
						 Function:		Plastic
						 Purpose:		Create a plastic material based on a given color
						 Calls:			const vec4& color - the color of the plastic material
						 **********************************************************************/
		static Material Plastic(const vec4& color);

		/**********************************************************************
		Function:		Rubber
		Purpose:		Create a rubber material based on a given color
		Calls:			const vec4& color - the color of the rubber material
		**********************************************************************/
		static Material Rubber(const vec4& color);

		// BUILT IN MATERIALS
		static Material Pewter();
		static Material Chrome();
		static Material Obsidian();
		static Material Silver();
		static Material Bronze();
		static Material Brass();
		static Material Pearl();
		static Material WhitePlastic();
		static Material BlackPlastic();
		static Material CyanPlastic();
		static Material GreenPlastic();
		static Material RedPlastic();
		static Material YellowPlastic();
		static Material BlackRubber();
		static Material WhiteRubber();
		static Material CyanRubber();
		static Material GreenRubber();
		static Material RedRubber();
		static Material YellowRubber();
	};
}

#endif
