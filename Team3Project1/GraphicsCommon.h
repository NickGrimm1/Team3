#pragma once

// Definitions used within the Graphics Engine

// Reserved Texture Units
#define MESH_OBJECT_COLOUR_TEXTURE_UNIT			0
#define MESH_OBJECT_NORMAL_TEXTURE_UNIT			1
#define DEFERRED_LIGHTS_EMISSIVE_TEXTURE_UNIT	2
#define DEFERRED_LIGHTS_SPECULAR_TEXTURE_UNIT	3
#define GBUFFER_COLOUR_TEXTURE_UNIT				4
#define GBUFFER_NORMALS_TEXTURE_UNIT			5
#define GBUFFER_DEPTH_TEXTURE_UNIT				6
#define SKYBOX_TEXTURE_UNIT						7
#define SHADOW_TEXTURE_UNIT						10

#define DEFAULT_AMBIENT_LIGHT_COLOUR Vector4(0.5, 0.5, 0.5, 1.0);