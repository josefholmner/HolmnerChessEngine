#include "Resources.h"

#include "Images/MenuBackgroundImg.cpp"
#include "Images/MenuButtonHardImg.cpp"
#include "Images/MenuButtonMediumImg.cpp"
#include "Images/MenuButtonEasyImg.cpp"
#include "Images/MenuButtonSillyImg.cpp"
#include "Images/MenuButtonSideWhiteImg.cpp"
#include "Images/MenuButtonSideBlackImg.cpp"
#include "Images/BoardImg.cpp"

const ImageData Resources::getMenuBackgroundImg()
{
	return ImageData(menuBackgroundImg, sizeof(menuBackgroundImg));
}

const ImageData Resources::getMenuButtonHardImg()
{
	return ImageData(menuButtonHardImg, sizeof(menuButtonHardImg));
}

const ImageData Resources::getMenuButtonMediumImg()
{
	return ImageData(menuButtonMediumImg, sizeof(menuButtonMediumImg));
}

const ImageData Resources::getMenuButtonEasyImg()
{
	return ImageData(menuButtonEasyImg, sizeof(menuButtonEasyImg));
}

const ImageData Resources::getMenuButtonSillyImg()
{
	return ImageData(menuButtonSillyImg, sizeof(menuButtonSillyImg));
}

const ImageData Resources::getMenuButtonSideWhiteImg()
{
	return ImageData(menuButtonSideWhiteImg, sizeof(menuButtonSideWhiteImg));
}

const ImageData Resources::getMenuButtonSideBlackImg()
{
	return ImageData(menuButtonSideBlackImg, sizeof(menuButtonSideBlackImg));
}

const ImageData Resources::getBoardImg()
{
	return ImageData(boardImg, sizeof(boardImg));
}
