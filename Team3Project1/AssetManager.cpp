#include "AssetManager.h"
#include "../Framework/OBJMesh.h"
#include "../Framework/MD5Mesh.h"
#include "GameStateManager.h"

AssetManager* AssetManager::instance = NULL;


bool AssetManager::Initialize(AssetManager*& out) 
{ 
	if (instance == NULL)
		instance = new AssetManager();

	instance->triangle = NULL;
	instance->quad = NULL;
	instance->quadAlt = NULL;
	instance->quadCentral = NULL;
	instance->quadTexCoordCol = NULL;
	instance->circle = NULL;
	instance->cone = NULL;
	instance->cylinder = NULL;

	out = instance;
	return true; 
}
void AssetManager::Destroy()
{
	GameStateManager::Graphics()->GetRenderContext();
	while (!instance->loadedTextures.empty())
	{
		map<string, LoadedTexture>::iterator i = instance->loadedTextures.begin();
		delete (*i).second.texture;
		instance->loadedTextures.erase(i);
	}
	while (!instance->loadedMeshes.empty()) 
	{
		map<string, LoadedMesh>::iterator i = instance->loadedMeshes.begin();
		delete (*i).second.mesh;
		instance->loadedMeshes.erase(i);
	}
	while (!instance->loadedShaders.empty())
	{
		map<string, LoadedShader>::iterator i = instance->loadedShaders.begin();
		delete (*i).second.shader;
		instance->loadedShaders.erase(i);
	}
	while (!instance->loadedShaderParts.empty())
	{
		map<string, LoadedShaderPart>::iterator i = instance->loadedShaderParts.begin();
		delete (*i).second.shaderPart;
		instance->loadedShaderParts.erase(i);
	}
	GameStateManager::Graphics()->DropRenderContext();

	if (instance != NULL)
		delete instance;
	instance = NULL;
}

Texture* AssetManager::LoadTexture(void* callerID, string filePath, unsigned int flags)
{
	// Check if this texture is already loaded
	map<string, LoadedTexture>::iterator i = loadedTextures.find(filePath);
	if (i != loadedTextures.end())
	{
		// Check if this caller already has this texture loaded
		for (unsigned int j = 0; j < loadedTextures[filePath].callerIDs.size(); j++)
		{
			if (loadedTextures[filePath].callerIDs[j] == callerID)
				return loadedTextures[filePath].texture; // It has, simply return
		}
		loadedTextures[filePath].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return loadedTextures[filePath].texture;
	}
	else
	{
		// Load this texture in...
		GameStateManager::Graphics()->GetRenderContext();
		Texture* newTexture = new Texture(filePath, flags);
		newTexture->SetRepeating(true);
		GameStateManager::Graphics()->DropRenderContext();
		loadedTextures.insert(pair<string, LoadedTexture>(filePath, LoadedTexture(newTexture, callerID)));
		return newTexture;
	}
}

void AssetManager::UnloadTexture(void* callerID, string filePath)
{
	// Check if this texture is actually loaded...
	map<string, LoadedTexture>::iterator i = loadedTextures.find(filePath);
	if (i == loadedTextures.end())
		return; // This texture does not exist, ignore.

	// This texture does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedTextures[filePath].callerIDs.begin(); i != loadedTextures[filePath].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedTextures[filePath].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedTextures[filePath].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete loadedTextures[filePath].texture;
		GameStateManager::Graphics()->DropRenderContext();
		i = loadedTextures.erase(i);
	}
}

Mesh* AssetManager::LoadMesh(void* callerID, string filePath)
{
	// Check if this mesh is already loaded
	map<string, LoadedMesh>::iterator i = loadedMeshes.find(filePath);
	if (i != loadedMeshes.end())
	{
		// Check if this caller already has this mesh loaded
		for (unsigned int j = 0; j < loadedMeshes[filePath].callerIDs.size(); j++)
		{
			if (loadedMeshes[filePath].callerIDs[j] == callerID)
				return loadedMeshes[filePath].mesh; // It has, simply return
		}
		loadedMeshes[filePath].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return loadedMeshes[filePath].mesh;
	}
	else
	{
		// Load this mesh in...
		Mesh* newMesh;
		if (filePath.substr(filePath.length() - 3, 3) == "obj")
		{
			GameStateManager::Graphics()->GetRenderContext();
			newMesh = new OBJMesh(filePath);
			GameStateManager::Graphics()->DropRenderContext();
		}
		else if (filePath.substr(filePath.length() - 3, 3) == "md5")
		{
			GameStateManager::Graphics()->GetRenderContext();
#if WINDOWS_BUILD
			newMesh = new MD5Mesh(MD5FileData(filePath));
#endif
			GameStateManager::Graphics()->DropRenderContext();
		}
		else
			return NULL; // Unrecognised fileType. OOOPS!!

		loadedMeshes.insert(pair<string, LoadedMesh>(filePath, LoadedMesh(newMesh, callerID)));
		return newMesh;
	}
}

void AssetManager::UnloadMesh(void* callerID, string filePath)
{
	// Check if this mesh is actually loaded...
	map<string, LoadedMesh>::iterator i = loadedMeshes.find(filePath);
	if (i == loadedMeshes.end())
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedMeshes[filePath].callerIDs.begin(); i != loadedMeshes[filePath].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedMeshes[filePath].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedMeshes[filePath].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete loadedMeshes[filePath].mesh;
		GameStateManager::Graphics()->DropRenderContext();
		i = loadedMeshes.erase(i);
	}
}

//Generates a single triangle, with RGB colours
Mesh* AssetManager::LoadTriangle(void* callerID)
{
	// Check if this mesh is already loaded...
	if (triangle)
	{
		for (unsigned int i = 0; i < triangleUsers.size(); i++)
		{
			if (triangleUsers[i] == callerID)
				return triangle; // It has, simply return
		}
		triangleUsers.push_back(callerID); // It hasn't, add this caller, then return.
		return triangle;
	}
	else
	{
		GameStateManager::Graphics()->GetRenderContext();
		triangle = Mesh::GenerateTriangle();
		GameStateManager::Graphics()->DropRenderContext();
		triangleUsers.push_back(callerID);
		return triangle;
	}
}
//Generates a single white quad, going from -1 to 1 on the x and y axis.
Mesh* AssetManager::LoadQuad(void* callerID)
{
	// Check if this mesh is already loaded...
	if (quad)
	{
		for (unsigned int i = 0; i < quadUsers.size(); i++)
		{
			if (quadUsers[i] == callerID)
				return quad; // It has, simply return
		}
		quadUsers.push_back(callerID); // It hasn't, add this caller, then return.
		return quad;
	}
	else
	{
		GameStateManager::Graphics()->GetRenderContext();
		quad = Mesh::GenerateQuad();
		GameStateManager::Graphics()->DropRenderContext();
		quadUsers.push_back(callerID);
		return quad;
	}
}
//Generates a single white quad, going from 0 to 1 on the x and y axis.
Mesh* AssetManager::LoadQuadAlt(void* callerID)
{
	// Check if this mesh is already loaded...
	if (quadAlt)
	{
		for (unsigned int i = 0; i < quadAltUsers.size(); i++)
		{
			if (quadAltUsers[i] == callerID)
				return quadAlt; // It has, simply return
		}
		quadAltUsers.push_back(callerID); // It hasn't, add this caller, then return.
		return quadAlt;
	}
	else
	{
		GameStateManager::Graphics()->GetRenderContext();
		quadAlt = Mesh::GenerateQuadAlt();
		GameStateManager::Graphics()->DropRenderContext();
		quadAltUsers.push_back(callerID);
		return quadAlt;
	}
}
//Generates a single white quad, going from -0.5 to 0.5 on the x and y axis.
Mesh* AssetManager::LoadQuadCentral(void* callerID)
{
	// Check if this mesh is already loaded...
	if (quadCentral)
	{
		for (unsigned int i = 0; i < quadCentralUsers.size(); i++)
		{
			if (quadCentralUsers[i] == callerID)
				return quadCentral; // It has, simply return
		}
		quadCentralUsers.push_back(callerID); // It hasn't, add this caller, then return.
		return quadCentral;
	}
	else
	{
		GameStateManager::Graphics()->GetRenderContext();
		quadCentral = Mesh::GenerateQuadCentral();
		GameStateManager::Graphics()->DropRenderContext();
		quadCentralUsers.push_back(callerID);
		return quadCentral;
	}
}
//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
Mesh* AssetManager::LoadQuadTexCoordCol(void* callerID, T3Vector2 scale, T3Vector2 texCoord, T3Vector4 colour)
{
	// Check if this mesh is already loaded...
	if (quadTexCoordCol)
	{
		for (unsigned int i = 0; i < quadTexCoordColUsers.size(); i++)
		{
			if (quadTexCoordColUsers[i] == callerID)
				return quadTexCoordCol; // It has, simply return
		}
		quadTexCoordColUsers.push_back(callerID); // It hasn't, add this caller, then return.
		return quadTexCoordCol;
	}
	else
	{
		GameStateManager::Graphics()->GetRenderContext();
		quadTexCoordCol = Mesh::GenerateQuad();
		GameStateManager::Graphics()->DropRenderContext();
		quadTexCoordColUsers.push_back(callerID);
		return quadTexCoordCol;
	}
}
// Generates Circle/Cone/Cylinder meshes with subdivs segments to approximate curvature
Mesh* AssetManager::LoadCircle(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is already loaded
	map<unsigned int, LoadedMesh>::iterator i = circleUsers.find(subdivs);
	if (i != circleUsers.end())
	{
		// Check if this caller already has this mesh loaded
		for (unsigned int j = 0; j < circleUsers[subdivs].callerIDs.size(); j++)
		{
			if (circleUsers[subdivs].callerIDs[j] == callerID)
				return circleUsers[subdivs].mesh; // It has, simply return
		}
		circleUsers[subdivs].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return circleUsers[subdivs].mesh;
	}
	else
	{
		// Load this mesh in...
		GameStateManager::Graphics()->GetRenderContext();
		circleUsers.insert(pair<unsigned int, LoadedMesh>(subdivs, LoadedMesh(Mesh::GenerateCircle(subdivs), callerID)));
		GameStateManager::Graphics()->DropRenderContext();
		return circleUsers[subdivs].mesh;
	}
}
Mesh* AssetManager::LoadCone(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is already loaded
	map<unsigned int, LoadedMesh>::iterator i = coneUsers.find(subdivs);
	if (i != coneUsers.end())
	{
		// Check if this caller already has this mesh loaded
		for (unsigned int j = 0; j < coneUsers[subdivs].callerIDs.size(); j++)
		{
			if (coneUsers[subdivs].callerIDs[j] == callerID)
				return coneUsers[subdivs].mesh; // It has, simply return
		}
		coneUsers[subdivs].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return coneUsers[subdivs].mesh;
	}
	else
	{
		// Load this mesh in...
		GameStateManager::Graphics()->GetRenderContext();
		coneUsers.insert(pair<unsigned int, LoadedMesh>(subdivs, LoadedMesh(Mesh::GenerateCone(subdivs), callerID)));
		GameStateManager::Graphics()->DropRenderContext();
		return coneUsers[subdivs].mesh;
	}
}
Mesh* AssetManager::LoadCylinder(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is already loaded
	map<unsigned int, LoadedMesh>::iterator i = cylinderUsers.find(subdivs);
	if (i != cylinderUsers.end())
	{
		// Check if this caller already has this mesh loaded
		for (unsigned int j = 0; j < cylinderUsers[subdivs].callerIDs.size(); j++)
		{
			if (cylinderUsers[subdivs].callerIDs[j] == callerID)
				return cylinderUsers[subdivs].mesh; // It has, simply return
		}
		cylinderUsers[subdivs].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return cylinderUsers[subdivs].mesh;
	}
	else
	{
		// Load this mesh in...
		GameStateManager::Graphics()->GetRenderContext();
		cylinderUsers.insert(pair<unsigned int, LoadedMesh>(subdivs, LoadedMesh(Mesh::GenerateCylinder(subdivs), callerID)));
		GameStateManager::Graphics()->DropRenderContext();
		return cylinderUsers[subdivs].mesh;
	}
}
//Generates a single triangle, with RGB colours
void AssetManager::UnloadTriangle(void* callerID)
{
	// Check if this mesh is actually loaded...
	if (!triangle)
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = triangleUsers.begin(); i != triangleUsers.end(); i++)
	{
		if (*i == callerID)
		{
			i = triangleUsers.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (triangleUsers.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete triangle;
		triangle = NULL;
		GameStateManager::Graphics()->DropRenderContext();
	}
}
//Generates a single white quad, going from -1 to 1 on the x and y axis.
void AssetManager::UnloadQuad(void* callerID)
{
	// Check if this mesh is actually loaded...
	if (!quad)
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = quadUsers.begin(); i != quadUsers.end(); i++)
	{
		if (*i == callerID)
		{
			i = quadUsers.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (quadUsers.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete quad;
		quad = NULL;
		GameStateManager::Graphics()->DropRenderContext();
	}
}
//Generates a single white quad, going from 0 to 1 on the x and y axis.
void AssetManager::UnloadQuadAlt(void* callerID)
{
	// Check if this mesh is actually loaded...
	if (!quadAlt)
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = quadAltUsers.begin(); i != quadAltUsers.end(); i++)
	{
		if (*i == callerID)
		{
			i = quadAltUsers.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (quadAltUsers.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete quadAlt;
		quadAlt = NULL;
		GameStateManager::Graphics()->DropRenderContext();
	}
}
//Generates a single white quad, going from -0.5 to 0.5 on the x and y axis.
void AssetManager::UnloadQuadCentral(void* callerID)
{
	// Check if this mesh is actually loaded...
	if (!quadCentral)
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = quadCentralUsers.begin(); i != quadCentralUsers.end(); i++)
	{
		if (*i == callerID)
		{
			i = quadCentralUsers.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (quadCentralUsers.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete quadCentral;
		quadCentral = NULL;
		GameStateManager::Graphics()->DropRenderContext();
	}
}
//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
void AssetManager::UnloadQuadTexCoordCol(void* callerID, T3Vector2 scale, T3Vector2 texCoord, T3Vector4 colour)
{
	// Check if this mesh is actually loaded...
	if (!quadTexCoordCol)
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = quadTexCoordColUsers.begin(); i != quadTexCoordColUsers.end(); i++)
	{
		if (*i == callerID)
		{
			i = quadTexCoordColUsers.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (quadTexCoordColUsers.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete quadTexCoordCol;
		quadTexCoordCol = NULL;
		GameStateManager::Graphics()->DropRenderContext();
	}
}
// Generates Circle/Cone/Cylinder meshes with subdivs segments to approximate curvature
void AssetManager::UnloadCircle(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is actually loaded...
	map<unsigned int, LoadedMesh>::iterator i = circleUsers.find(subdivs);
	if (i == circleUsers.end())
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = circleUsers[subdivs].callerIDs.begin(); i != circleUsers[subdivs].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = circleUsers[subdivs].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (circleUsers[subdivs].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete circleUsers[subdivs].mesh;
		GameStateManager::Graphics()->DropRenderContext();
		i = circleUsers.erase(i);
	}
}
void AssetManager::UnloadCone(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is actually loaded...
	map<unsigned int, LoadedMesh>::iterator i = coneUsers.find(subdivs);
	if (i == coneUsers.end())
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = coneUsers[subdivs].callerIDs.begin(); i != coneUsers[subdivs].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = coneUsers[subdivs].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (coneUsers[subdivs].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete coneUsers[subdivs].mesh;
		GameStateManager::Graphics()->DropRenderContext();
		i = coneUsers.erase(i);
	}
}
void AssetManager::UnloadCylinder(void* callerID, unsigned int subdivs)
{
	// Check if this mesh is actually loaded...
	map<unsigned int, LoadedMesh>::iterator i = cylinderUsers.find(subdivs);
	if (i == cylinderUsers.end())
		return; // This texture does not exist, ignore.

	// This mesh does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = cylinderUsers[subdivs].callerIDs.begin(); i != cylinderUsers[subdivs].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = cylinderUsers[subdivs].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (cylinderUsers[subdivs].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		delete cylinderUsers[subdivs].mesh;
		GameStateManager::Graphics()->DropRenderContext();
		i = cylinderUsers.erase(i);
	}
}

Shader* AssetManager::LoadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath)
{
	// Check if the shader as a whole is already loaded
	string shaderName = vertexShaderFilePath;
	shaderName.append(fragmentShaderFilePath).append(geometryShaderFilePath);
	map<string, LoadedShader>::iterator i = loadedShaders.find(shaderName);
	if (i != loadedShaders.end())
	{
		// Check if this caller already has this shader loaded
		for (unsigned int j = 0; j < loadedShaders[shaderName].callerIDs.size(); j++)
		{
			if (loadedShaders[shaderName].callerIDs[j] == callerID)
				return loadedShaders[shaderName].shader; // It has, simply return
		}
		loadedShaders[shaderName].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return loadedShaders[shaderName].shader;
	}
	else
	{
		// Check if any of these shader parts are already loaded
		ShaderPart* vertexShader;
		ShaderPart* fragmentShader;
		ShaderPart* geometryShader;
		GameStateManager::Graphics()->GetRenderContext();
		Shader* newShader = new Shader();
		GameStateManager::Graphics()->DropRenderContext();

		map<string, LoadedShaderPart>::iterator j = loadedShaderParts.find(vertexShaderFilePath);
		if (j != loadedShaderParts.end())
		{
			// Add this shader to the caller list.
			loadedShaderParts[vertexShaderFilePath].callerIDs.push_back(newShader);
			vertexShader = loadedShaderParts[vertexShaderFilePath].shaderPart;
		}
		else
		{
			// Load this vertex shader
			string raw = ShaderPart::LoadShaderFile(vertexShaderFilePath);
			GameStateManager::Graphics()->GetRenderContext();
			vertexShader = new ShaderPart(raw, ShaderType::VERTEX);
			GameStateManager::Graphics()->DropRenderContext();
			loadedShaderParts.insert(pair<string, LoadedShaderPart>(vertexShaderFilePath, LoadedShaderPart(vertexShader, newShader)));
		}
		j = loadedShaderParts.find(fragmentShaderFilePath);
		if (j != loadedShaderParts.end())
		{
			// Add this shader to the caller list.
			loadedShaderParts[fragmentShaderFilePath].callerIDs.push_back(newShader);
			fragmentShader = loadedShaderParts[fragmentShaderFilePath].shaderPart;
		}
		else
		{
			// Load this fragment shader
			string raw = ShaderPart::LoadShaderFile(fragmentShaderFilePath);
			GameStateManager::Graphics()->GetRenderContext();
			fragmentShader = new ShaderPart(raw, ShaderType::FRAGMENT);
			GameStateManager::Graphics()->DropRenderContext();
			loadedShaderParts.insert(pair<string, LoadedShaderPart>(fragmentShaderFilePath, LoadedShaderPart(fragmentShader, newShader)));
			
		}

		if (geometryShaderFilePath != "")
		{
			j = loadedShaderParts.find(geometryShaderFilePath);
			if (j != loadedShaderParts.end())
			{
				// Add this shader to the caller list.
				loadedShaderParts[geometryShaderFilePath].callerIDs.push_back(newShader);
				geometryShader = loadedShaderParts[geometryShaderFilePath].shaderPart;
			}
			else
			{
				// Load this geometry shader
				string raw = ShaderPart::LoadShaderFile(geometryShaderFilePath);
				GameStateManager::Graphics()->GetRenderContext();
				geometryShader = new ShaderPart(raw, ShaderType::GEOMETRY);
				GameStateManager::Graphics()->DropRenderContext();
				loadedShaderParts.insert(pair<string, LoadedShaderPart>(geometryShaderFilePath, LoadedShaderPart(geometryShader, newShader)));
				
			}
		}
		else
			geometryShader = NULL;

		// Load this Shader
		newShader->SetVertex(vertexShader);
		newShader->SetFragment(fragmentShader);
		
#if WINDOWS_BUILD
		newShader->SetGeometry(geometryShader);
		GameStateManager::Graphics()->GetRenderContext();
		bool loadOk = newShader->LinkProgram();
		GameStateManager::Graphics()->DropRenderContext();
		if (!loadOk) return NULL; // Check link status
#endif
		loadedShaders.insert(pair<string, LoadedShader>(shaderName, LoadedShader(newShader, callerID)));
		return newShader;
	}
}
void AssetManager::UnloadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath)
{
	// Check if this shader is actually loaded...
	string shaderName = vertexShaderFilePath;
	shaderName.append(fragmentShaderFilePath);
	shaderName.append(geometryShaderFilePath);
	map<string, LoadedShader>::iterator i = loadedShaders.find(shaderName);
	if (i == loadedShaders.end())
		return; // This shader does not exist, ignore.

	// This shader does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedShaders[shaderName].callerIDs.begin(); i != loadedShaders[shaderName].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedShaders[shaderName].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedShaders[shaderName].callerIDs.size() == 0)
	{
		// Check each shader part & remove the shader from the callers list, removing the shader part where no users are left.
		map<string, LoadedShaderPart>::iterator j = loadedShaderParts.find(vertexShaderFilePath);
		if (j != loadedShaderParts.end()) // Strictly speaking, if it was we somehow have a shader that's missing this part :/ but it's easier for the next checks if we have an iterator to it.
		{
			for (vector<void*>::iterator k = j->second.callerIDs.begin(); k != j->second.callerIDs.end(); k++)
			{
				if (*k == callerID)
				{
					k = j->second.callerIDs.erase(k);
					break;
				}
			}
		}
		if (j->second.callerIDs.size() == 0)
		{
			GameStateManager::Graphics()->GetRenderContext();
			delete loadedShaderParts[vertexShaderFilePath].shaderPart;
			GameStateManager::Graphics()->DropRenderContext();
			j = loadedShaderParts.erase(j);
		}
		j = loadedShaderParts.find(fragmentShaderFilePath);
		if (j != loadedShaderParts.end()) // Strictly speaking, if it was we somehow have a shader that's missing this part :/ but it's easier for the next checks if we have an iterator to it.
		{
			for (vector<void*>::iterator k = j->second.callerIDs.begin(); k != j->second.callerIDs.end(); k++)
			{
				if (*k == callerID)
				{
					k = j->second.callerIDs.erase(k);
					break;
				}
			}
		}
		if (j->second.callerIDs.size() == 0)
		{
			GameStateManager::Graphics()->GetRenderContext();
			delete loadedShaderParts[fragmentShaderFilePath].shaderPart;
			GameStateManager::Graphics()->DropRenderContext();
			j = loadedShaderParts.erase(j);
		}
		if (geometryShaderFilePath != "")
		{
			j = loadedShaderParts.find(geometryShaderFilePath);
			if (j != loadedShaderParts.end()) // Strictly speaking, if it was we somehow have a shader that's missing this part :/ but it's easier for the next checks if we have an iterator to it.
			{
				for (vector<void*>::iterator k = j->second.callerIDs.begin(); k != j->second.callerIDs.end(); k++)
				{
					if (*k == callerID)
					{
						k = j->second.callerIDs.erase(k);
						break;
					}
				}
			}
			if (j->second.callerIDs.size() == 0)
			{
				GameStateManager::Graphics()->GetRenderContext();
				delete loadedShaderParts[geometryShaderFilePath].shaderPart;
				GameStateManager::Graphics()->DropRenderContext();
				j = loadedShaderParts.erase(j);
			}
		}

		// Remove the shader.
		GameStateManager::Graphics()->GetRenderContext();
		delete loadedShaders[shaderName].shader;
		GameStateManager::Graphics()->DropRenderContext();
		i = loadedShaders.erase(i);
	}
}

Font* AssetManager::LoadFont(void* callerID, string filePath, unsigned int xCount, unsigned int yCount)
{
	// Check if this font is already loaded
	map<string, LoadedFont>::iterator i = loadedFonts.find(filePath);
	if (i != loadedFonts.end())
	{
		// Check if this caller already has this font loaded
		for (unsigned int j = 0; j < loadedFonts[filePath].callerIDs.size(); j++)
		{
			if (loadedFonts[filePath].callerIDs[j] == callerID)
				return loadedFonts[filePath].font; // It has, simply return
		}
		loadedFonts[filePath].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
		return loadedFonts[filePath].font;
	}
	else
	{
		// Load this font in...
		GameStateManager::Graphics()->GetRenderContext();
		Font* newFont = new Font(xCount, yCount);
		newFont->SetTexture(LoadTexture(newFont, filePath, SOIL_FLAG_COMPRESS_TO_DXT));
		GameStateManager::Graphics()->DropRenderContext();
		loadedFonts.insert(pair<string, LoadedFont>(filePath, LoadedFont(newFont, callerID)));
		return newFont;
	}
}

void AssetManager::UnloadFont(void* callerID, string filePath)
{
	// Check if this font is actually loaded...
	map<string, LoadedFont>::iterator i = loadedFonts.find(filePath);
	if (i == loadedFonts.end())
		return; // This texture does not exist, ignore.

	// This font does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedFonts[filePath].callerIDs.begin(); i != loadedFonts[filePath].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedFonts[filePath].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedFonts[filePath].callerIDs.size() == 0)
	{
		GameStateManager::Graphics()->GetRenderContext();
		UnloadTexture(loadedFonts[filePath].font, filePath);
		GameStateManager::Graphics()->DropRenderContext();
		i = loadedFonts.erase(i);
	}
}