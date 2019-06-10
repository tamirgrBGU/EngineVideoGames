#pragma once

struct Viewport
	{
	public:
		Viewport()
		{
			this->left = 0;
			this->bottom = 0;
			this->width = 100;
			this->height = 100;
		}

		Viewport(int left,int bottom,int width,int height)
		{
			this->left = left;
			this->bottom = bottom;
			this->width = width;
			this->height = height;
		}

		Viewport(const Viewport &vp)
		{
			this->left = vp.left;
			this->bottom = vp.bottom;
			this->width = vp.width;
			this->height = vp.height;
		}

		int GetLeft() { return left; }
		int GetBottom() { return bottom; }
		int GetWidth() { return width; }
		int GetHeight() {return height;}
		void GetViewport(int *viewport)
		{
			viewport[0] = left;
			viewport[1] = bottom;
			viewport[2] = width;
			viewport[3] = height;
		}

		void SetViewport(int _left, int _bottom, int _width,int _height)
		{
			left = _left;
			bottom = _bottom;
			width = _width;
			height = _height;		
		}

	   float GetWHRelation()
	   {
		    return (float) width / (float)height;
	   }
	private:
		int left, bottom, width, height;
	};

