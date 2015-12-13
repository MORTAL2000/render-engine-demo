/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Interface file for the LightSource class. There is a pool of
light sources that can be acquired/released by objects in the
game.

Date:				December 9, 2015

*************************************************************************************/

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#define NUM_LIGHT_SOURCES 1

#include "Bagnall.h"

namespace Bagnall
{
	class LightSource
	{
	public:
		static void Init();

		static LightSource* ObtainLightSource();

		static void ReleaseLightSource(LightSource *l);

		static void UpdateLightSourceMatricesInShaders();

		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 position;

		void UpdateMatrix();

	private:
		static LightSource *lightSources[NUM_LIGHT_SOURCES];
		static GLfloat lightSourceMatrices[NUM_LIGHT_SOURCES * 16];

		int index;
		bool used;

		LightSource(int i);
	};
}

#endif
