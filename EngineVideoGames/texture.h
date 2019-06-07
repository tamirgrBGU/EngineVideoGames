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
	void Bind(int slot);
	void bindTex2Buffer( int num, int mode);
	inline int GetSlot(){return m_texture;}

	 ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	unsigned int m_texture;
};

#endif
