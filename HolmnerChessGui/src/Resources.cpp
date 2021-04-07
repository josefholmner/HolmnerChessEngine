#include "Resources.h"

#include "Images/MenuBackgroundImg.cpp"

const ImageData Resources::getMenuBackgroundImg()
{
	return ImageData(menuBackgroundImg, sizeof(menuBackgroundImg));
}
