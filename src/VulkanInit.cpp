#include "VulkanInit.h"
#include "Window.h"

// constructeur
VulkanInit::VulkanInit(Window& window)
{
	run(window);
}

//destructeur
VulkanInit::~VulkanInit()
{
	clean();
}

void VulkanInit::run(Window& window)
{
	createInstance();
	createSurface(window);
}

// instance vulkan
void VulkanInit::createInstance()
{
	VkApplicationInfo info{};
	info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	info.pApplicationName = "VN Creator";
	info.pEngineName = "engine";
	info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	info.apiVersion = VK_API_VERSION_1_0;

	// les extensions vulkan
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extProperties(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extProperties.data());

	// les extensions glfw
	uint32_t glfwExtCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);

	// informations de l'instance
	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &info;
	instanceInfo.enabledExtensionCount = glfwExtCount;
	instanceInfo.ppEnabledExtensionNames = glfwExtensions;

	// validationLayers
	if (this->enableVL)
	{
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
		instanceInfo.ppEnabledLayerNames = this->validationLayers.data();
	}
	else
	{
		instanceInfo.enabledLayerCount = 0;
	}

	// création de l'instance
	if (vkCreateInstance(&instanceInfo, nullptr, &this->instance) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!INSTANCE: vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS");
	}
}

// surface
void VulkanInit::createSurface(Window& window)
{
	if (glfwCreateWindowSurface(this->instance, window.getWindow(), nullptr, &this->surface) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!!surface: glfwCreateWindowSurface(instance, window.getWindow(), nullptr, &surface) != VK_SUCCESS");
	}
}

void VulkanInit::clean()
{
	vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
	vkDestroyInstance(this->instance, nullptr);
}