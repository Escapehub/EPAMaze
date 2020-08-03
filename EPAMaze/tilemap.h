#ifndef TILEMAP_H
#define TILEMAP_H
#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string&, sf::Vector2u, const int*, unsigned int, unsigned int);
private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

#endif 