#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder() {
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename) {
	//get reference to m_Textures using m_s_Instance
	//auto is same as map<string,Texture>
	map<string, Texture>& m = m_s_Instance->m_Textures;


	//Create iterator to hold kvp
	//search for pair using filename
	//auto is same as map<string, Texture>::iterator
	auto KeyValuePair = m.find(filename);

	if (KeyValuePair != m.end()) {

		//if found texture return second part of the pair
		//(the texture)
		return KeyValuePair->second;
	}
	else {
		//filename not found
		//create new pair using filename
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;

	}


}