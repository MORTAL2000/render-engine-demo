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
		static Material None();
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

		bool operator==(const Material& other) const;
	};
}

#include <glm/gtx/hash.hpp>

// hash function for materials
namespace std {

	template <>
	struct hash<Bagnall::Material>
	{
		std::size_t operator()(const Bagnall::Material& m) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<vec4>()(m.ambient)
				^ (hash<vec4>()(m.diffuse) << 1)) >> 1)
				^ (hash<vec4>()(m.specular) << 1)
				^ (hash<float>()(m.shininess));
		}
	};

}

#endif
