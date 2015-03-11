#if PS3_BUILD
#pragma once

#include "Texture.h"
Texture::Texture(string filename, unsigned int flags)
{
	filename.append(".gtf");
	std::cout << "Loading GTF File: " << filename << std::endl;
	textureObject = LoadGTF(filename);
}

/*
Drop a DDS file onto the DDS2GTF exe inside the 
C:\usr\local\cell\host-win32\bin folder to make a GTF 
file suitable for loading with this function.
*/
CellGcmTexture* Texture::LoadGTF(std::string filename) {
	CellGcmTexture* texture = new CellGcmTexture();	//A new GCM texture!

	std::ifstream file;

	std::string folder = SYS_APP_HOME;
	folder.append("/").append(filename);
	//Attempt to open the texture file
	std::cout << "Loading GTF from(" << folder << ")" << std::endl;
	file.open(folder.c_str(), std::ios::binary);
	if(!file.is_open())
	{
		//File hasn't loaded! 
		delete texture;
		std::cout << "LoadGTF file error" << std::endl;
		return NULL;
	}

	CellGtfFileHeader header;
	file.read((char*)&header,sizeof(CellGtfFileHeader));

	CellGtfTextureAttribute*attributes = new CellGtfTextureAttribute[header.NumTexture];

	file.read((char*)attributes,header.NumTexture * sizeof(CellGtfTextureAttribute));


	/*
	GTF textures can technically have multiple textures in it, for now
	lets just load 1
	*/
	for(int i = 0; i < minimum(1,header.NumTexture); ++i) {
		memcpy((void*)texture,(void*)&(attributes[i].tex), sizeof(CellGcmTexture));

		file.seekg(attributes[i].OffsetToTex);

		std::cout << "Texture: Getting Memory" << std::endl;
		char*rsxdata = (char*)GCMRenderer::localMemoryAlign(128, attributes[i].TextureSize);
		file.read(rsxdata,attributes[i].TextureSize);

		cellGcmAddressToOffset( rsxdata, &texture->offset );
		std::cout << "Texture: Got Memory" << std::endl;
	}

	//memory = (width * height * 4) / 1024.0f / 1024.0f; // Assume all textures are 32 bit for the moment.
	std::cout << "LoadGTF success!" << std::endl;

	delete[] attributes;

	return texture;
}
Texture::~Texture(){}
void Texture::SetRepeating(bool repeat){}
#endif
