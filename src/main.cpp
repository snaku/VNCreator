#include "VNCreator.h"
#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		VNCreator::loadImage("C:\\Users\\Desktop\\Test img vulkan\\btmc.png");
		VNCreator::init();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}