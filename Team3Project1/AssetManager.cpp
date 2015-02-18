#include "AssetManager.h"
#include "../Framework/OBJMesh.h"
#include "../Framework/MD5Mesh.h"

AssetManager* AssetManager::instance = NULL;

void AssetManager::Destroy()
{
	// TODO Get Context
	for (map<string, LoadedTexture>::iterator i = instance->loadedTextures.begin(); i != instance->loadedTextures.end(); i++)
	{
		delete i->second.texture;
		i = instance->loadedTextures.erase(i);
	}
	for (map<string, LoadedMesh>::iterator i = instance->loadedMeshes.begin(); i != instance->loadedMeshes.end(); i++)
	{
		delete i->second.mesh;
		i = instance->loadedMeshes.erase(i);
	}
	for (map<string, LoadedShader>::iterator i = instance->loadedShaders.begin(); i != instance->loadedShaders.end(); i++)
	{
		delete i->second.shader;
		i = instance->loadedShaders.erase(i);
	}
	for (map<string, LoadedShaderPart>::iterator i = instance->loadedShaderParts.begin(); i != instance->loadedShaderParts.end(); i++)
	{
		delete i->second.shaderPart;
		i = instance->loadedShaderParts.erase(i);
	}
	// TODO Release Context

	if (instance != NULL)
		delete instance;
	instance = NULL;
}

Texture* AssetManager::LoadTexture(void* callerID, string filePath)
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
		// TODO Get Context
		Texture* newTexture = new Texture(filePath);
		// TODO Release Context
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
		// TODO Get Context
		delete loadedTextures[filePath].texture;
		// TODO Release Context
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
		if (filePath.substr(filePath.length() - 4, 3) == "obj")
		{
			// TODO Get Context
			newMesh = new OBJMesh(filePath);
			// TODO Release Context
		}
		else if (filePath.substr(filePath.length() - 4, 3) == "md5")
		{
			// TODO Get Context
			newMesh = new MD5Mesh(MD5FileData(filePath));
			// TODO Release Context
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
		// TODO Get Context
		delete loadedMeshes[filePath].mesh;
		// TODO Release Context
		i = loadedMeshes.erase(i);
	}
}

Shader* AssetManager::LoadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath)
{
	// Check if the shader as a whole is already loaded
	string shaderName = vertexShaderFilePath.append(fragmentShaderFilePath).append(geometryShaderFilePath);
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
		ShaderPart* vertexShader = NULL;
		ShaderPart* fragmentShader = NULL;
		ShaderPart* geometryShader = NULL;
		// TODO Get Context
		Shader* newShader = new Shader();
		// TODO Release Context

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
			// TODO Get Context
			vertexShader = ShaderPart::LoadShaderFile(vertexShaderFilePath, ShaderType::VERTEX);
			// TODO Release Context
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
			// TODO Get Context
			fragmentShader = ShaderPart::LoadShaderFile(fragmentShaderFilePath, ShaderType::FRAGMENT);
			// TODO Release Context
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
				// TODO Get Context
				geometryShader = ShaderPart::LoadShaderFile(geometryShaderFilePath, ShaderType::GEOMETRY);
				// TODO Release Context
				loadedShaderParts.insert(pair<string, LoadedShaderPart>(geometryShaderFilePath, LoadedShaderPart(geometryShader, newShader)));
				
			}
		}

		// Load this Shader
		newShader->SetVertex(vertexShader);
		newShader->SetFragment(fragmentShader);
		
#if WINDOWS_BUILD
		newShader->SetGeometry(geometryShader);
		// TODO Get Context
		newShader->LinkProgram();
		// TODO Release Context
#endif
		return newShader;
	}
}
void AssetManager::UnloadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath)
{
	// Check if this shader is actually loaded...
	string shaderName = vertexShaderFilePath.append(fragmentShaderFilePath).append(geometryShaderFilePath);
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
			// TODO Get Context
			delete loadedShaderParts[vertexShaderFilePath].shaderPart;
			// TODO Release Context
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
			// TODO Get Context
			delete loadedShaderParts[fragmentShaderFilePath].shaderPart;
			// TODO Release Context
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
				// TODO Get Context
				delete loadedShaderParts[geometryShaderFilePath].shaderPart;
				// TODO Release Context
				j = loadedShaderParts.erase(j);
			}
		}

		// Remove the shader.
		// TODO Get Context
		delete loadedShaders[shaderName].shader;
		// TODO Release Context
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
		// TODO Get Context
		Font* newFont = new Font(xCount, yCount);
		newFont->SetTexture(LoadTexture(newFont, filePath));
		// TODO Release Context
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
		// TODO Get Context
		UnloadTexture(loadedFonts[filePath].font, filePath);
		// TODO Release Context
		i = loadedFonts.erase(i);
	}
}