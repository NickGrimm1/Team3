#if PS3_BUILD
#pragma once

#include "Texture.h"
Texture::Texture(string filename, unsigned int flags)
{
	// TODO: Sort if this is TGA or GTF and load accordingly.
	if (filename.substr(filename.length() - 3, 3) == "tga")
	{
		std::cout << "Loading TGA File: " << filename << std::endl;
		textureObject = LoadTGA(filename);
	}
	else if (filename.substr(filename.length() - 3, 3) == "gtf")
	{
		std::cout << "Loading GTF File: " << filename << std::endl;
		textureObject = LoadGTF(filename);
	}
	else
	{
		std::cout << "Filetype not recognised: " << filename << std::endl;
		textureObject = NULL;
	}
}

/*
Loads a TGA file in. Unlike in the Graphics for Games module, in which we relied
on an external library (SOIL) to load in our textures, this time we're going to
have a go at doing it ourselves! Unlike OpenGL, which represents textures just 
with a 'name', in GCM they have a structure type, CellGcmTexture.

Pretty much the easiest type of texture to load in is the TARGA file. Sadly, 
I've lost the tutorial that explained how it's so easy, so I'll have to do
it here, instead. Basically, a targa file has an 18 byte header that defines
the size and bit depth of the texture, and then there's a big chunk of data.
If it's uncompressed, that data is simply the texture, ready for using, but
if it's compressed you'll have to do more with it (I've not yet written this
bit, but it should be fairly straightforward, TGA uses simple 'run length
encoding' for its compression technique. 

*/
CellGcmTexture* Texture::LoadTGA(std::string filename)	{
	CellGcmTexture* texture = new CellGcmTexture();	//A new GCM texture!

	std::ifstream file;

	std::string folder = SYS_APP_HOME + filename;
	//Attempt to open the texture file
	std::cout << "Loading TGA from(" << folder << ")" << std::endl;
	file.open(folder.c_str(), std::ios::binary);
	if(!file.is_open()){
		//File hasn't loaded! 
		delete texture;
		std::cout << "LoadTGATexture file error" << std::endl;
		return NULL;
	}

	//Read in the first 18 bytes of the file, containing the header
	unsigned char TGAheader[18];
	file.read((char*)TGAheader,sizeof(TGAheader));

	//Width and height take up 2 bytes each, so we need to do a bit of
	//bit shifting to make up the final value
	texture->width	= (TGAheader[12] + (TGAheader[13] << 8));
	texture->height = (TGAheader[14] + (TGAheader[15] << 8));
	texture->pitch  = texture->width*(TGAheader[16] / 8);
	texture->depth  = 1;

	std::cout << filename << " has width " << texture->width << std::endl;
	std::cout << filename << " has height " << texture->height << std::endl;
	std::cout << filename << " has bitdepth " << (unsigned int)TGAheader[16] << std::endl;

	//We're going to assume it's uncompressed data, and just read in a load of data.
	int size = texture->width*texture->height*(TGAheader[16] / 8);

	std::cout << filename << " has size "	 << (unsigned int)size		<< std::endl;

	char*rsxdata = (char*)GCMRenderer::localMemoryAlign(128, size);	//A texture in graphics memory needs aligning...

	//Read in the texture straight into graphics memory. Challenge! How fast will this data
	//transfer be? Why? How could it be sped up?
	file.read(rsxdata,size);	
	file.close();				//Close the file


	unsigned int offset;

	//As we're dealing with graphics memory, we need an offset
	if(cellGcmAddressToOffset( rsxdata, &offset ) != CELL_OK) {
		std::cout << "offset failure (texture)!" << std::endl;		
	};

	texture->location = CELL_GCM_LOCATION_LOCAL;	//We want the texture in graphics memory
	texture->offset   = offset;						//at he following offset

	std::cout << filename << " has RSX memory location " << (unsigned int)rsxdata	<< std::endl;
	std::cout << filename << " has RSX memory offset "   << (unsigned int)offset	<< std::endl;

	texture->format  = CELL_GCM_TEXTURE_A8R8G8B8;	//ARGB format - there doesn't seem to be a RGB format!
	texture->format |= CELL_GCM_TEXTURE_LN;		//Data is a 'linear' array of values

	/*
	Remap allows us to swap around values on the texture when it is sampled.
	For example we could swap around red and blue (normally, we think of textures
	as being in 'red green blue alpha' format, whereas TGA saves its data in
	'blue green red alpha' format, so you might think we need a remapping to handle
	that, but no! GCM uses 'b g r a' format natively, so the data can stay as-is
	*/

	texture->remap = 
		CELL_GCM_TEXTURE_REMAP_REMAP << 14 |
		CELL_GCM_TEXTURE_REMAP_REMAP << 12 |
		CELL_GCM_TEXTURE_REMAP_REMAP << 10 |
		CELL_GCM_TEXTURE_REMAP_REMAP << 8 |
		CELL_GCM_TEXTURE_REMAP_FROM_A << 6 |
		CELL_GCM_TEXTURE_REMAP_FROM_R << 4 |
		CELL_GCM_TEXTURE_REMAP_FROM_G << 2 |
		CELL_GCM_TEXTURE_REMAP_FROM_B;

	texture->mipmap		= 1;		//How many mipmap levels are there (1 == 'largest level mipmap')
	texture->cubemap	= CELL_GCM_FALSE;	//No...it's not a cubemap
	texture->dimension	= CELL_GCM_TEXTURE_DIMENSION_2;	//It's a 2D Texture...

	//memory = (width * height * 4) / 1024.0f / 1024.0f; // Assume all textures are 32 bit for the moment.
	std::cout << "Finished loading texture!" << std::endl;

	return texture;
}

/*
Drop a DDS file onto the DDS2GTF exe inside the 
C:\usr\local\cell\host-win32\bin folder to make a GTF 
file suitable for loading with this function.
*/
CellGcmTexture* Texture::LoadGTF(std::string filename) {
	CellGcmTexture* texture = new CellGcmTexture();	//A new GCM texture!

	std::ifstream file;

	std::string folder = SYS_APP_HOME + filename;
	//Attempt to open the texture file
	std::cout << "Loading GTF from(" << folder << ")" << std::endl;
	file.open(folder.c_str(), std::ios::binary);
	if(!file.is_open()){
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

		char*rsxdata = (char*)GCMRenderer::localMemoryAlign(128, attributes[i].TextureSize);
		file.read(rsxdata,attributes[i].TextureSize);

		cellGcmAddressToOffset( rsxdata, &texture->offset );
	}

	//memory = (width * height * 4) / 1024.0f / 1024.0f; // Assume all textures are 32 bit for the moment.
	std::cout << "LoadGTF success!" << std::endl;

	delete[] attributes;

	return texture;
}
Texture::~Texture(){}
void Texture::SetRepeating(bool repeat){}
#endif
