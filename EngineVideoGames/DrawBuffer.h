#pragma once
class DrawBuffer
{
	//int left;
	//int bottom;
	//int width;
	//int height;
	unsigned int buffer;
public:
	enum
	{
		COLOR,
		DEPTH,
		STENCIL,
		BACK,
		FRONT,
		ACCUM,
		NONE,
	};
	DrawBuffer(void);
	void SetDrawDistination( int num,int mode);
//	DrawBuffer(int _left, int _bottom, int _width,int _height, int _buffer);
	//void GetViewport(int *viewport);
//	void SetViewport(int left, int bottom, int width,int height);
	void Bind();
	void UnBind();
	~DrawBuffer(void);
};

