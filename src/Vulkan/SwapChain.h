#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <limits>
#include <vector>

class VulkanInit;

struct swapChainSupport
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presModes;
};

class Swapchain
{
public:
	
	void createSwapChain(VulkanInit& vulkan);
	void createImageViews(VulkanInit& vulkan);
	void run(VulkanInit& vulkan);
	void clean(VulkanInit& vulkan);

	VkSurfaceFormatKHR swapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availFormat);
	VkPresentModeKHR swapPresMode(const std::vector<VkPresentModeKHR>& availPresMode);
	VkExtent2D swapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	swapChainSupport querySwapChainSupport(VkPhysicalDevice device, VulkanInit& vulkan);
	
	VkExtent2D getSwapchainExtent();

	uint32_t requestedMode;
private:
	const uint32_t LARGEUR = 800;
	const uint32_t HAUTEUR = 600;

	uint32_t choosedPresentMode = requestedMode;

	VkSwapchainKHR swapChain;
	std::vector<VkImageView> swapChainImgView;
	std::vector<VkImage> swapChainImg;
	VkFormat swapChainFormat;
	VkExtent2D swapChainExtent;
};