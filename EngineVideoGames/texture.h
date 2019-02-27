#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const std::string& fileName);

	void Bind();

	 ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	unsigned int m_texture;
};

#endif
