#pragma once
class DrawBuffer
{
	//int left;
	//int bottom;
	//int width;
	//int height;
	unsigned int buffer;
	unsigned int depthBuffer;

		void CreateDepthBufferAttachment(int width,int height);
	void CreateColorBufferAttachment(int width,int height);
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
	DrawBuffer(int width,int height);
	void SetDrawDistination( int num,int mode);
	void resize(int width,int height);
	void Bind();
	void UnBind();
	~DrawBuffer(void);
};

