#pragma once
class DrawBuffer
{
	//int left;
	//int bottom;
	//int width;
	//int height;
	unsigned int buffer;
public:

	DrawBuffer(void);
	void SetDrawDistination( int num);
//	DrawBuffer(int _left, int _bottom, int _width,int _height, int _buffer);
	//void GetViewport(int *viewport);
//	void SetViewport(int left, int bottom, int width,int height);
	void Bind();
	~DrawBuffer(void);
};

