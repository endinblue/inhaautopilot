#pragma once

#include "cv.h"
#include "highgui.h"

namespace thin
{
	public ref class Thinning  : public System::IDisposable
	{
	public:
		Thinning(void);
	public:
		~Thinning(void);
	public:	
		void	DeleteA(char* img, char* timg,int cx,int cy, int widthStep) ;
		int		nays(char* img, int i, int j, int widthStep) ;
		int		Connect(char* img, int i, int j, int widthStep);
		IplImage*	Thin(IplImage* InImage) ;
		
	};

}

