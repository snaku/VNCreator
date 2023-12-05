#include "VulkanInit.h"

// A SAVOIR: swapchain.run est appelée ici
void VulkanInit::run(/*Window& window*/ GLFWwindow& window)
{
	createInstance();
	createSurface(window);
	pickPhysicalDevice();
	createLogicalDevice();
	swapchain.run(*this);
}

// instance vulkan
void VulkanInit::createInstance()
{
	// info
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
void VulkanInit::createSurface(GLFWwindow& window)
{
	if (glfwCreateWindowSurface(this->instance, &window, nullptr, &this->surface) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!!surface: glfwCreateWindowSurface(instance, window.getWindow(), nullptr, &surface) != VK_SUCCESS");
	}
}

// physical device (GPU)
void VulkanInit::pickPhysicalDevice()
{
	// combien de physical device ?
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(this->instance, &count, nullptr);

	// informations sur les physical devices disponibles
	std::vector<VkPhysicalDevice> devices(count);
	vkEnumeratePhysicalDevices(this->instance, &count, devices.data());
	
	// parcoure chaque phyical device pour en trouver un adapté
	for (const auto& device : devices)
	{
		vkGetPhysicalDeviceProperties(device, &this->deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &this->deviceFeatures);

		if (isSuitable(device))
		{
			this->physicalDevice = device;
			break;
		}
	}

	// vérifie si un physical device adapté a été trouvé
	if (this->physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("!!!!!physicalDevice == VK_NULL_HANDLE");
	}

	vkGetPhysicalDeviceProperties(this->physicalDevice, &this->deviceProperties);
	std::cout << "GPU: " << this->deviceProperties.deviceName << std::endl;
}

bool VulkanInit::isSuitable(VkPhysicalDevice device)
{
	queueFamilyIndices familyIndices = findQueueFamilies(device);

	bool extSupported = checkDeviceExt(device);
	bool swapChainAdequate = false;

	if (extSupported)
	{
		swapChainSupport swapchainSupport = swapchain.querySwapChainSupport(device, *this);
		swapChainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presModes.empty();
	}

	return familyIndices.isComplete() && extSupported && swapChainAdequate;
}

queueFamilyIndices VulkanInit::findQueueFamilies(VkPhysicalDevice device)
{
	queueFamilyIndices familyIndices;

	// obtient le nbr de queueFamilies prises en charge par le physical device
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	// vector pour stocker les prop de chaque queueFamily
	std::vector<VkQueueFamilyProperties> familyProp(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, familyProp.data());

	// parcourt le vector familyProp
	int i = 0;
	for (const auto& prop : familyProp)
	{
		// vérifie si la queueFamily prend en charge les op gfx
		if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			familyIndices.gfxFamily = i;
		}
		
		// gfxFamily.has_value
		if (familyIndices.isComplete())
		{
			break;
		}

		VkBool32 presSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presSupport);

		if (presSupport)
		{
			familyIndices.presFamily = i;
		}

		i++;
	}

	return familyIndices;
}

void VulkanInit::createLogicalDevice()
{
	queueFamilyIndices familyIndices = findQueueFamilies(physicalDevice);
	
	float priorities = 1.0;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVect{ processQueueFamilies(familyIndices) };

	VkDeviceCreateInfo deviceInfo{};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = queueCreateInfoVect.data();
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoVect.size());
	deviceInfo.pEnabledFeatures = &this->deviceFeatures;
	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(this->deviceExt.size());
	deviceInfo.ppEnabledExtensionNames = this->deviceExt.data();

	if (this->enableVL)
	{
		deviceInfo.enabledLayerCount = (uint32_t)this->validationLayers.size();
		deviceInfo.ppEnabledLayerNames = this->validationLayers.data();
	}
	else
	{
		deviceInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!!vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS");
	}

	vkGetDeviceQueue(this->device, familyIndices.gfxFamily.value(), 0, &this->gfxQueue);
	vkGetDeviceQueue(this->device, familyIndices.presFamily.value(), 0, &this->presQueue);
}

std::vector<VkDeviceQueueCreateInfo> VulkanInit::processQueueFamilies(const queueFamilyIndices& indices) const {

	try 
	{
		std::set<uint32_t> queueIndices{ indices.gfxFamily.value(), 
										 indices.presFamily.value() };
	
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVect;

		for (const auto& index : queueIndices) 
		{
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.pQueuePriorities = &priorities;
			createInfo.queueFamilyIndex = index;
			createInfo.queueCount = 1;

			queueCreateInfoVect.push_back(createInfo);
		}

		return queueCreateInfoVect;
	}

	catch (const std::exception& e) 
	{
		throw std::exception("ERROR : queue processing for logical device creation");
	}
}

bool VulkanInit::checkDeviceExt(VkPhysicalDevice device)
{
	// obtenir le nbr d'ext du physical device
	uint32_t extCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);

	std::vector<VkExtensionProperties> availExt(extCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, availExt.data());

	std::set<std::string> reqExt(this->deviceExt.begin(), this->deviceExt.end());

	// parcourir toutes les ext disponibles
	for (const auto& ext : availExt)
	{
		// suppr l'extension de reqExt si elle est présente
		reqExt.erase(ext.extensionName);
	}

	// si les ext requises ont été trouvées, alors l'appareil supporte les ext
	return reqExt.empty();
}

// clean (A SAVOIR: swapchain.clean est appelée ici)
void VulkanInit::clean()
{
	swapchain.clean(*this);
	vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(this->instance, nullptr);
}

// getters
VkInstance VulkanInit::getInstance()
{
	return this->instance;
}
VkSurfaceKHR VulkanInit::getSurface()
{
	return this->surface;
}
VkPhysicalDevice VulkanInit::getPhysicalDevice()
{
	return this->physicalDevice;
}
VkDevice VulkanInit::getDevice()
{
	return this->device;
}