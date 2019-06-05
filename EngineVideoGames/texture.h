#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	enum
	{
		COLOR,
		DEPTH,
		STENCIL,
		ACCUM,
	};
	Texture(const std::string& fileName);
	Texture(int width, int height,int mode);
	void Bind();
	void bindTex2Buffer( int num, int mode);

	 ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	unsigned int m_texture;
};

#endif
