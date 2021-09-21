#pragma once
#include<SFML/Graphics/Texture.hpp>
#include<array>

enum class TEXTURE {PLAYER,ENEMY1,ENEMY2,TERR1,TERR2,TERR3,SWORD};

class TextureManager
{
public:
	void loadInitTextures();
	std::array<sf::Texture, 12> getTextureArray() { return alltextures_; }
private:
	std::array<sf::Texture, 12> alltextures_;
};

inline void TextureManager::loadInitTextures()
{
	sf::Texture temp;
	temp.loadFromFile(R"(player.png)");
	alltextures_[(int)TEXTURE::PLAYER] = temp;
	temp.loadFromFile(R"(enemy.png)");
	alltextures_[(int)TEXTURE::ENEMY1] = temp;
	temp.loadFromFile(R"(one.png)");
	alltextures_[(int)TEXTURE::TERR1] = temp;
	temp.loadFromFile(R"(two.png)");
	alltextures_[(int)TEXTURE::TERR2] = temp;
}

