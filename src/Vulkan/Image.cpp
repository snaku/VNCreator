#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Image.h"

void Image::loadImage(const char* imagePath)
{
	int texLargeur, texHauteur, texChannel;
	
	// charge l'image
	stbi_uc* img = stbi_load(imagePath, &texLargeur, &texHauteur, &texChannel, STBI_rgb_alpha);
	const uint64_t imgSize = static_cast<uint64_t>(texLargeur * texHauteur * 4);
	
	if (!img)
	{
		throw std::runtime_error(std::string("erreur lors du chargement de l'image: ")+imagePath);
	}
	else
	{
		std::cout << "L'image a ete chargee avec succes: " << imagePath << std::endl;
		std::cout << "Dimensions de l'image: " << texLargeur << "x" << texHauteur << std::endl;
	}

	/*VkBuffer stagingBuffer = nullptr;
	VkDeviceMemory stagingBufferMemory = nullptr;*/
	
	void* mappedData = nullptr;

	/***************************
	*	pas pour maintenant	   *
	****************************	
	*vkMapMemory(vulkan.getDevice(), stagingBufferMemory, 0, imgSize, 0, &mappedData);
	*memcpy(&mappedData, &img, imgSize);
	*vkUnmapMemory(vulkan.getDevice(), stagingBufferMemory);
	*/

	stbi_image_free(img);
}