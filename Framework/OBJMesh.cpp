#include "OBJMesh.h"
#include "../Team3Project1/GameStateManager.h"
/*
OBJ files look generally something like this:

v xpos ypos zpos
..more vertices

vt xtex ytex
...more texcoords

vn xnorm ynorm znorm
...more normals

f vert index / tex index / norm index  vert index / tex index / norm index  vert index / tex index / norm index
...more faces

(i.e there's a set of float/float/float for each vertex of a face)

OBJ files can also be split up into a number of submeshes, making loading them
in even more annoying. 
*/
bool	OBJMesh::LoadOBJMesh(std::string filename)	{
	std::ifstream f(filename.c_str(),std::ios::in);

	if(!f) {//Oh dear, it can't find the file :(
		return false;
	}

	OBJSubMesh* currentMesh = new OBJSubMesh();
	inputSubMeshes.push_back(currentMesh);	//It's safe to assume our OBJ will have a mesh in it ;)

	string currentMtlLib;
	string currentMtlType;

	while(!f.eof()) {
		std::string currentLine;
		f >> currentLine;

		if(currentLine == OBJCOMMENT) {		//This line is a comment, ignore it
			getline(f, currentLine);
			continue;
		}
		else if(currentLine == OBJSMOOTHSHADE) {
			getline(f, currentLine);
			continue; // TODO - implement
		}
		else if(currentLine == OBJMTLLIB) {
			f >> currentMtlLib;
			getline(f, currentLine); // handle file name with spaces
			currentMtlLib.append(currentLine);
		}
		else if(currentLine == OBJUSEMTL) {
			currentMesh = new OBJSubMesh();
			inputSubMeshes.push_back(currentMesh);

			currentMesh->mtlSrc = currentMtlLib;

			f >> currentMtlType;

			currentMesh->mtlType = currentMtlType;
		}
		else if(currentLine == OBJMESH || currentLine == OBJOBJECT1 || currentLine == OBJOBJECT2) {	//This line is a submesh!
			currentMesh = new OBJSubMesh();
			inputSubMeshes.push_back(currentMesh);

			currentMesh->mtlSrc = currentMtlLib;
			currentMesh->mtlType = currentMtlType;
			getline(f, currentLine); // TODO - maybe save obj name
		}
		else if(currentLine == OBJVERT) {	//This line is a vertex
			T3Vector3 vertex;
			f >> vertex.x; f >> vertex.y; f >> vertex.z;
			inputVertices.push_back(vertex);
		}
		else if(currentLine == OBJNORM) {	//This line is a Normal!
			T3Vector3 normal;
			f >> normal.x; f >> normal.y; f >> normal.z;
			inputNormals.push_back(normal);
		}
		else if(currentLine == OBJTEX) {	//This line is a texture coordinate!
			T3Vector2 texCoord;
			f >> texCoord.x; f >> texCoord.y;
			/*
			TODO! Some OBJ files might have 3D tex coords...
			*/
			inputTexCoords.push_back(texCoord);
		}
		else if(currentLine == OBJFACE) {	//This is an object face!
			if(!currentMesh) {
				inputSubMeshes.push_back(new OBJSubMesh());
				currentMesh = inputSubMeshes[inputSubMeshes.size()-1];
			}

			std::string			faceData;		//Keep the entire line in this!
			std::vector<int>	faceIndices;	//Keep the extracted indices in here!
		
			getline(f,faceData);		//Use a string helper function to read in the entire face line

			/*
			It's possible an OBJ might have normals defined, but not tex coords!
			Such files should then have a face which looks like this:

				f <vertex index>//<normal index>
				instead of <vertex index>/<tex coord>/<normal index>

				you can be some OBJ files will have "/ /" instead of "//" though... :(
			*/
			bool	skipTex = false;
			if(faceData.find("//") != std::string::npos) {	
				skipTex = true;
			}

			/*
			Count the number of slashes, but also convert the slashes to spaces
			so that string streaminimumg of ints doesn't fail on the slash

				"f  0/0/0" becomes "f 0 0 0" etc
			*/
			for(size_t i = 0; i < faceData.length(); ++i) {
				if(faceData[i] == '/') {
					faceData[i] = ' ';
				}
			}

			/*
			Now string stream the indices from the string into a temporary
			vector.
			*/
			int tempIndex;
			std::stringstream	ss(faceData);
			while(ss >> tempIndex) {
				faceIndices.push_back(tempIndex);
			}

			//If the face indices vector is a multiple of 3, we're looking at triangles
			//with some combination of vertices, normals, texCoords
			if(faceIndices.size()%3 == 0) {		//This face is a triangle (probably)!
				if(faceIndices.size() == 3) {	//This face has only vertex information;
					currentMesh->vertIndices.push_back(faceIndices.at(0));
					currentMesh->vertIndices.push_back(faceIndices.at(1));
					currentMesh->vertIndices.push_back(faceIndices.at(2));
				}
				else if(faceIndices.size() == 9) {	//This face has vertex, normal and tex information!
					for(int i = 0; i < 9; i += 3) {
						currentMesh->vertIndices.push_back(faceIndices.at(i));
						currentMesh->texIndices.push_back(faceIndices.at(i+1));
						currentMesh->normIndices.push_back(faceIndices.at(i+2));
					}
				}
				else if(faceIndices.size() == 6) {	//This face has vertex, and one other index...
					for(int i = 0; i < 6; i += 2) {
						currentMesh->vertIndices.push_back(faceIndices.at(i));
						if(!skipTex) {		// a double slash means it's skipping tex info...
							currentMesh->texIndices.push_back(faceIndices.at(i+1));
						}
						else{
							currentMesh->normIndices.push_back(faceIndices.at(i+1));
						}
					}
				}
			}
			else{	
				//Uh oh! Face isn't a triangle. Have fun adding stuff to this ;)
				bool a = true;
			}
		}
		else{
#if DEBUG
			printf ("OBJMesh::LoadOBJMesh Unknown file data:", currentLine, "\n");
#endif
		}
	}

	f.close();
	return true;
}

bool OBJMesh::BuildOBJMesh() {
	/* 
	Stores the loaded in materials
	*/
	map <string, MTLInfo> inputMaterials;

	//We now have all our mesh data loaded in...Now to convert it into OpenGL vertex buffers!
	for(unsigned int i = 0; i < inputSubMeshes.size(); ) {
		OBJSubMesh*sm = inputSubMeshes[i];

		/*
		We're going to be lazy and turn the indices into an absolute list
		of vertex attributes (it makes handling the submesh list easier)
		*/
		if(sm->vertIndices.empty()) {
			delete sm;
			inputSubMeshes.erase(inputSubMeshes.begin() + i);
			continue;
		}
		else	{
			OBJMesh*m;
			
			if(i == 0) {
				m = this;
			}
			else{
				m = new OBJMesh();
			}

			m->numVertices	= sm->vertIndices.size();

			m->AssignVertexMemory();
			for(unsigned int j = 0; j < sm->vertIndices.size(); ++j)
			{
				m->vertices[j] = inputVertices[sm->vertIndices[j] - 1];
			}

			if(!sm->texIndices.empty())	
			{
				for(unsigned int j = 0; j < sm->texIndices.size(); ++j) 
				{
					m->vertices[j].SetTexCoord(inputTexCoords[sm->texIndices[j] - 1]);
				}
			}

			if(sm->normIndices.empty()) {
				m->GenerateNormals();
			}
			else
			{
				for(unsigned int j = 0; j < sm->normIndices.size(); ++j) {
					m->vertices[j].SetNormal(inputNormals[sm->normIndices[j]-1]);
				}
			}

			m->GenerateTangents();

			m->BufferData();

			m->SetTexturesFromMTL(sm->mtlSrc, sm->mtlType, inputMaterials);

			if(i != 0) {
				AddChild(m);
			}	
		}
		delete inputSubMeshes[i];
		++i;
	}
	return true;
}

/*
Draws the current OBJMesh. The handy thing about overloaded virtual functions
is that they can still run the code they have 'overridden', by calling the 
parent class function as you would a static function. So all of the new stuff
you've been building up in the Mesh class as the tutorials go on, will 
automatically be used by this overloaded function. Once 'this' has been drawn,
all of the children of 'this' will be drawn
*/
void OBJMesh::Draw() {
#if WINDOWS_BUILD
	GLint prog = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

	if (texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureName());
		glUniform1i(glGetUniformLocation(prog, "useDiffuseTex"), 1);
	}
	if (bumpTexture) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bumpTexture->GetTextureName());
		glUniform1i(glGetUniformLocation(prog, "useDiffuseTex"), 1);
	}
#endif

#if WINDOWS_BUILD
	Mesh::Draw();
#endif//TODO fix for ps3
	for(unsigned int i = 0; i < children.size(); ++i) {
#if WINDOWS_BUILD
		children.at(i)->Draw();
#endif
	}
};

void	OBJMesh::SetTexturesFromMTL(string &mtlFile, string &mtlType, map<string, MTLInfo>& materials) {
	if(mtlType.empty() || mtlFile.empty()) {
		return;
	}

	if (materials.size() > 0) {
		map <string, MTLInfo>::iterator i = materials.find(mtlType);
		if(i != materials.end()) {
			if(!i->second.diffuse.empty())	{
				texture = i->second.diffuseNum;
				texturePath = string(i->second.diffuse);
			}

			if(!i->second.bump.empty())	{
				bumpTexture = i->second.bumpNum;
				bumpPath = string(i->second.bump);
			}

			return;
		}
	}
	std::ifstream f(string(MESHDIR + mtlFile).c_str(),std::ios::in);

	if(!f) {//Oh dear, it can't find the file :(
		return;
	}

	MTLInfo currentMTL;
	string  currentMTLName;
	
	int mtlCount = 0;

	while(!f.eof()) {
		std::string currentLine;
		f >> currentLine;
		
		if(currentLine == MTLNEW) {
			if(mtlCount > 0) {

#ifdef OBJ_FIX_TEXTURES
				FixTextures(currentMTL);
#endif

				materials.insert(std::make_pair(currentMTLName,currentMTL));
			}
			currentMTL.diffuse = "";
			currentMTL.bump = "";

			f >> currentMTLName;

			mtlCount++;		
		}
		else if(currentLine == MTLDIFFUSEMAP) {
			f >> currentMTL.diffuse;
			currentLine.clear();
			getline(f, currentLine); // handle file name with spaces
			currentMTL.diffuse.append(currentLine);

			if (!currentMTL.diffuse.empty()) {
#if WINDOWS_BUILD
				currentMTL.diffuseNum = GameStateManager::Assets()->LoadTexture(this, string(currentMTL.diffuse), SOIL_FLAG_INVERT_Y);
#endif
#if PS3_BUILD
				currentMTL.diffuseNum = GameStateManager::Assets()->LoadTexture(this, string(currentMTL.diffuse), 0);
#endif
			}
		}
		else if(currentLine == MTLBUMPMAP || currentLine == MTLBUMPMAPALT) {
			f >> currentMTL.bump;
			currentLine.clear();
			getline(f, currentLine); // handle file name with spaces
			currentMTL.diffuse.append(currentLine);

			if (!currentMTL.bump.empty())
			{
#if WINDOWS_BUILD
				currentMTL.bumpNum = GameStateManager::Assets()->LoadTexture(this, string(currentMTL.bump), 0);
#endif
#if PS3_BUILD
				currentMTL.bumpNum = GameStateManager::Assets()->LoadTexture(this, string(currentMTL.bump), 0);
#endif
			}
		}
	}

#ifdef OBJ_FIX_TEXTURES
	FixTextures(currentMTL);
#endif

	materials.insert(std::make_pair(currentMTLName,currentMTL));

	
//	SetTexturesFromMTL(mtlFile,mtlType, materials);
	// Recheck if material has been loaded - if not, does not exist
	if (materials.size() > 0) {
		map <string, MTLInfo>::iterator i = materials.find(mtlType);
		if(i != materials.end()) {
			if(!i->second.diffuse.empty())	
			{
				texture = i->second.diffuseNum;
				texturePath = string(currentMTL.diffuse);
			}

			if(!i->second.bump.empty())	
			{
				bumpTexture = i->second.bumpNum;
				bumpPath = string(currentMTL.bump);
			}

			return;
		}
	}

#if PS3_BUILD
normTex = GameStateManager::Assets()->LoadTexture(this, texturePath,0);
bumpTex = GameStateManager::Assets()->LoadTexture(this, texturePath,0);
#endif
}

/*
The mtl files in that big pack of city buildings haven't been exported correctly...
*/
void	OBJMesh::FixTextures(MTLInfo &info) {
	if(!info.bumpNum) {

		string temp = info.diffuse;

		if(temp[temp.size() - 5] == 'd') {
			temp[temp.size() - 5] = 'n';
		}
		else {
			temp.insert(temp.size() - 4, "_n");
		}

		info.bump = temp;

//		info.bumpNum = SOIL_load_OGL_texture(string(TEXTUREDIR + info.bump).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y |  SOIL_FLAG_TEXTURE_REPEATS);
	}
}
