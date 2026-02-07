#include "evk_vulkan_core.h"
#include "evk.h"
#include "vecmath/vecmath.h"

#define EVK_VULKAN_RENDERPHASE_IMPLEMENTATION
#include "evk_vulkan_renderphase.h"

#define EVK_VULKAN_DRAWABLE_IMPLEMENTATION
#include "evk_vulkan_drawable.h"

#ifdef __cplusplus 
extern "C" {
#endif

/// @brief holds information about an instance
typedef struct evkInstance
{
    VkInstance instance;
	#ifdef EVK_ENABLE_VALIDATIONS
	VkDebugUtilsMessengerEXT debugger;
	#endif
    VkSurfaceKHR surface;
} evkInstance;

/// @brief holds information about a device
typedef struct evkDevice
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalProps;
    VkPhysicalDeviceMemoryProperties physicaMemProps;
    VkPhysicalDeviceFeatures phyiscalFeatures;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    uint32_t graphicsIndex;
    uint32_t presentIndex;
    uint32_t computeIndex;
} evkDevice;

/// @brief usefull information about a given swapchain, used uppon swapchain creation
typedef struct evkSwapchainDetails
{
    VkExtent2D extent;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* surfaceFormats;
    VkPresentModeKHR* presentModes;
    uint32_t surfaceFormatCount;
    uint32_t presentModeCount;
} evkSwapchainDetails;

/// @brief holds information about a swapchain
typedef struct evkSwapchain
{
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;
    uint32_t imageCount;
    VkSwapchainKHR swapchain;
    VkImage* images;
    VkImageView* imageViews;
    uint32_t imageIndex;
} evkSwapchain;

/// @brief holds information about the sync system between CPU and GPU
typedef struct evkSync
{
    uint32_t currentFrame;
    VkSemaphore* imageAvailableSemaphores;
    VkSemaphore* finishedRenderingSemaphores;
    VkFence* framesInFlightFences;
    uint32_t objectCount;
} evkSync;

/// @brief holds all vulkan backend structures needed on runtime
struct evkVulkanBackend
{
    evkMSAA msaa;
    evkInstance evkInstance;
    evkDevice evkDevice;
    evkSwapchain evkSwapchain;
    evkSync evkSync;
    
    evkRenderphaseType currentRenderphase;
    evkMainRenderphase evkMainRenderphase;
    evkPickingRenderphase evkPickingRenderphase;
    evkUIRenderphase evkUIRenderphase;
    evkViewportRenderphase evkViewportRenderphase;

    shashtable* buffers;
    shashtable* pipelines;
};

static evkVulkanBackend* g_EVKBackend = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief if validations are enabled, all vulkan messages will be call this function, wich will log the messages into the terminal
static VKAPI_ATTR VkBool32 VKAPI_CALL ievk_log_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback, void* userData)
{
    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        EVK_LOG(evk_Error, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        EVK_LOG(evk_Warn, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        EVK_LOG(evk_Info, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        EVK_LOG(evk_Trace, "%s\n", callback->pMessage);
        return VK_FALSE;
    }
    return VK_TRUE;
}

/// @brief the instance creation requires the names of all extensions it'll use, this changes depending on: platform, portability and validation requests, this function returns the list correctly
static bool ievk_get_instance_extensions(uint32_t* count, const char** names)
{
    static const char* EXT[] =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,

        #ifdef _WIN32
        "VK_KHR_win32_surface",
        #elif defined(__APPLE__)
        "VK_EXT_metal_surface",
        #elif defined(__ANDROID__)
        "VK_KHR_android_surface",
        #elif defined(__linux__) && !defined(__ANDROID__)
        #ifdef EVK_LINUX_USE_XLIB
        "VK_KHR_xlib_surface",
        #ifdef EVK_LINUX_USE_XCB
        "VK_KHR_xcb_surface",
        #endif
        #else
        "VK_KHR_wayland_surface",
        #endif
        #endif

        #ifdef __APPLE__
        "VK_KHR_portability_enumeration",
        #endif

        #ifdef EVK_ENABLE_VALIDATIONS
        "VK_EXT_debug_utils"
        #endif
    };

    static const uint32_t N = sizeof(EXT) / sizeof(EXT[0]);

    if (!count) return false;
    if (!names) { *count = N; return true; }
    if (*count < N) { *count = N; return false; }

    for (uint32_t i = 0; i < N; i++) {
        names[i] = EXT[i];
    }

    *count = N;

    return true;
}

/// @brief the vulkan instance is the begining of all vulkan stuff, it's like the root object between our code and the gpu, this function creates it correctly
static evkInstance ievk_instance_create(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion)
{
    volkInitialize();

    evkInstance evkInstance = { 0 };

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = engineVersion;
    appInfo.apiVersion = EVK_MAKE_VERSION(0, 1, 1, 0);
    appInfo.pNext = NULL;

    uint32_t count;
    if (!ievk_get_instance_extensions(&count, NULL)) {
        EVK_LOG(evk_Fatal, "Failed to retrieve initial count of required instance extensions");
    }

    const char** extensions = m_malloc(count * sizeof(const char*));
    if (!ievk_get_instance_extensions(&count, extensions)) {
        EVK_LOG(evk_Fatal, "Failed to retrieve further list of required instance extensions");
        m_free(extensions);
    }

    VkInstanceCreateInfo instanceCI = { 0 };
    instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCI.pApplicationInfo = &appInfo;
    instanceCI.enabledExtensionCount = count;
    instanceCI.ppEnabledExtensionNames = extensions;
    instanceCI.flags = 0;
    instanceCI.pNext = NULL;
    instanceCI.enabledLayerCount = 0;
    instanceCI.ppEnabledLayerNames = NULL;
    #ifdef __APPLE__
    instanceCI.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    #ifdef EVK_ENABLE_VALIDATIONS
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsCI = { 0 };
    const char* validationList = "VK_LAYER_KHRONOS_validation";
    instanceCI.enabledLayerCount = 1U;
    instanceCI.ppEnabledLayerNames = &validationList;

    debugUtilsCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsCI.pfnUserCallback = ievk_log_callback;
    debugUtilsCI.pUserData = NULL;
    instanceCI.pNext = &debugUtilsCI;
    #endif

    EVK_ASSERT(vkCreateInstance(&instanceCI, NULL, &evkInstance.instance) == VK_SUCCESS, "Failed to create vulkan instance");
    volkLoadInstance(evkInstance.instance);

    #ifdef EVK_ENABLE_VALIDATIONS
    EVK_ASSERT(vkCreateDebugUtilsMessengerEXT(evkInstance.instance, &debugUtilsCI, NULL, &evkInstance.debugger) == VK_SUCCESS, "Failed to create vulkan debugger");
    #endif

    m_free(extensions);
    return evkInstance;
}

/// @brief this function releases the used resources on instance creation
static void ievk_instance_destroy(evkInstance* evkInstance)
{
    EVK_ASSERT(evkInstance != NULL, "Vulkan backend is NULL");

    vkDestroySurfaceKHR(evkInstance->instance, evkInstance->surface, NULL);
    evkInstance->surface = VK_NULL_HANDLE;

    #ifdef EVK_ENABLE_VALIDATIONS
    if (vkDestroyDebugUtilsMessengerEXT) {
        vkDestroyDebugUtilsMessengerEXT(evkInstance->instance, evkInstance->debugger, NULL);
    }
    evkInstance->debugger = VK_NULL_HANDLE;
    #endif

    vkDestroyInstance(evkInstance->instance, NULL);
    evkInstance->instance = VK_NULL_HANDLE;

    volkFinalize();
}

/// @brief creates the the renderable surface/window and depends on the operating system, pay attention if you're on linux since the auto-detection will default to wayland if not especified
static void ievk_surface_create(VkInstance instance, VkSurfaceKHR* surface, void* rawWindow, void* rawDisplay)
{
    #ifdef _WIN32

    VkWin32SurfaceCreateInfoKHR createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = GetModuleHandle(NULL);
    createInfo.hwnd = (HWND)rawWindow;
    if (vkCreateWin32SurfaceKHR) {
        EVK_ASSERT(vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Win32 surface");
    }

    else {
        EVK_ASSERT(true, "Cannot find vkCreateWin32SurfaceKHR");
    }
    
    #elif defined(__APPLE__)

    VkMetalSurfaceCreateInfoEXT createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    createInfo.pLayer = (CAMetalLayer*)rawWindow;
    if (vkCreateMetalSurfaceEXT) {
        EVK_ASSERT(vkCreateMetalSurfaceEXT(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Metal surface");
    }
    else {
        EVK_ASSERT(true, "Cannot find vkCreateMetalSurfaceEXT");
    }

    #elif defined(__ANDROID__)

    VkAndroidSurfaceCreateInfoKHR createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.window = (ANativeWindow*)rawWindow;
    if (vkCreateAndroidSurfaceKHR) {
        EVK_ASSERT(vkCreateAndroidSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Android surface");
    }
    else {
        EVK_ASSERT(true, "Cannot find vkCreateAndroidSurfaceKHR");
    }

    #elif defined(__linux__) && !defined(__ANDROID__)
        #ifdef EVK_LINUX_USE_XLIB

        VkXlibSurfaceCreateInfoKHR createInfo = { 0 };
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.dpy = (Display*)rawDisplay;
        createInfo.window = (Window)rawWindow;
        if (vkCreateXlibSurfaceKHR) {
            EVK_ASSERT(vkCreateXlibSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Xlib surface");
        }
        else {
            EVK_ASSERT(true, "Cannot find vkCreateXlibSurfaceKHR");
        }

        #elif defined(EVK_LINUX_USE_XCB)

        VkXcbSurfaceCreateInfoKHR createInfo = { 0 };
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.connection = (xcb_connection_t*)rawDisplay;
        createInfo.window = (xcb_window_t)rawWindow;
        if (vkCreateXcbSurfaceKHR) {
            EVK_ASSERT(vkCreateXcbSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the XCB surface");
        }
        else {
            EVK_ASSERT(true, "Cannot find vkCreateXcbSurfaceKHR");
        }

        #else

        VkWaylandSurfaceCreateInfoKHR createInfo = { 0 };
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.display = (struct wl_display*)rawDisplay;
        createInfo.surface = (struct wl_surface*)rawWindow;
        if (vkCreateWaylandSurfaceKHR) {
            EVK_ASSERT(vkCreateWaylandSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Wayland surface");
        }
        else {
            EVK_ASSERT(true, "Cannot find vkCreateWaylandSurfaceKHR");
        }

        #endif
    #endif
}

/// @brief makes a check if all required extensions are present for that particular physical device
static bool ievk_check_device_extension_support(VkPhysicalDevice device, const char** required_extensions, uint32_t extension_count)
{
    uint32_t available_extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &available_extension_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties*)m_malloc(available_extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &available_extension_count, available_extensions);

    for (uint32_t i = 0; i < extension_count; i++) {
        int extension_found = 0;
        for (uint32_t j = 0; j < available_extension_count; j++) {
            if (strcmp(required_extensions[i], available_extensions[j].extensionName) == 0) {
                extension_found = 1;
                break;
            }
        }
        if (!extension_found) {
            m_free(available_extensions);
            return 0;
        }
    }

    m_free(available_extensions);
    return 1;
}

/// @brief since one compute may have multiple physical gpus we must check them all to see which is more fit
static VkPhysicalDevice ievk_device_choose(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t gpus = 0;
    vkEnumeratePhysicalDevices(instance, &gpus, NULL);

    VkPhysicalDevice* devices = (VkPhysicalDevice*)m_malloc(gpus * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &gpus, devices);

    VkPhysicalDevice choosenOne = VK_NULL_HANDLE;
    const char* requiredExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    const uint32_t requiredExtensionsCount = 1;
    VkDeviceSize bestScore = 0;

    for (uint32_t i = 0; i < gpus; i++) {

        VkPhysicalDeviceProperties device_props;
        VkPhysicalDeviceFeatures device_features;
        VkPhysicalDeviceMemoryProperties mem_props;
        vkGetPhysicalDeviceProperties(devices[i], &device_props);
        vkGetPhysicalDeviceFeatures(devices[i], &device_features);
        vkGetPhysicalDeviceMemoryProperties(devices[i], &mem_props);
        evkQueueFamily indices = evk_device_find_queue_families(devices[i], surface);
        if (!indices.graphicsFound || !indices.presentFound || !indices.computeFound) continue;
        if (!ievk_check_device_extension_support(devices[i], requiredExtensions, requiredExtensionsCount)) continue;

        VkDeviceSize currentScore = 0;
        if (device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) currentScore += 1000;  // discrete gpu
        currentScore += device_props.limits.maxImageDimension2D;                                    // max texture size
        for (uint32_t j = 0; j < mem_props.memoryHeapCount; j++) {                                  // prefer devices with dedicated VRAM
            if (mem_props.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                currentScore += mem_props.memoryHeaps[j].size / (VkDeviceSize)(1024 * 1024);        // mb
            }
        }

        if (currentScore > bestScore) {
            bestScore = currentScore;
            choosenOne = devices[i];
        }
    }

    m_free(devices);
    return choosenOne;
}

/// @brief creates the logical device based on choosen physical device and surface, it'll be logical connection to a specific GPU, used for creatin all vulkan objects from now on
static evkDevice ievk_device_create(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice)
{
    evkDevice device = { 0 };
    device.physicalDevice = physicalDevice;

    #ifdef EVK_ENABLE_VALIDATIONS
    const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
    uint32_t validationLayerCount = 1;
    #else
    const char* validationLayers[] = NULL;
    uint32_t validationLayerCount = 0;
    #endif

    evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
    uint32_t queueFamilyIndices[3] = { 0 };
    uint32_t queueCount = 0;
    float queuePriority = 1.0f;

    if (indices.graphics != -1) queueFamilyIndices[queueCount++] = indices.graphics;
    if (indices.present != -1 && indices.present != indices.graphics)  queueFamilyIndices[queueCount++] = indices.present;
    if (indices.compute != -1 && indices.compute != indices.graphics && indices.compute != indices.present) queueFamilyIndices[queueCount++] = indices.compute;

    VkDeviceQueueCreateInfo* queueCreateInfos = (VkDeviceQueueCreateInfo*)m_malloc(sizeof(VkDeviceQueueCreateInfo) * queueCount);
    for (uint32_t i = 0; i < queueCount; i++) {
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].pNext = NULL;
        queueCreateInfos[i].queueFamilyIndex = queueFamilyIndices[i];
        queueCreateInfos[i].queueCount = 1;
        queueCreateInfos[i].pQueuePriorities = &queuePriority;
        queueCreateInfos[i].flags = 0;
    }

    #if defined(__APPLE__)
    const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME };
    uint32_t extensionCount = 2;
    #else
    const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    uint32_t extensionCount = 1;
    #endif

    VkPhysicalDeviceFeatures deviceFeatures = { 0 };
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCI = { 0 };
    deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCI.pNext = NULL;
    deviceCI.flags = 0;
    deviceCI.queueCreateInfoCount = queueCount;
    deviceCI.pQueueCreateInfos = queueCreateInfos;
    deviceCI.enabledExtensionCount = extensionCount;
    deviceCI.ppEnabledExtensionNames = extensions;
    deviceCI.pEnabledFeatures = &deviceFeatures;
    deviceCI.enabledLayerCount = validationLayerCount;
    deviceCI.ppEnabledLayerNames = validationLayers;
    EVK_ASSERT(vkCreateDevice(physicalDevice, &deviceCI, NULL, &device.device) == VK_SUCCESS, "Failed to create vulkan logical device");
    
    volkLoadDevice(device.device);

    vkGetPhysicalDeviceProperties(physicalDevice, &device.physicalProps);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &device.physicaMemProps);
    vkGetPhysicalDeviceFeatures(physicalDevice, &device.phyiscalFeatures);
    vkGetDeviceQueue(device.device, indices.graphics, 0, &device.graphicsQueue);
    vkGetDeviceQueue(device.device, indices.present, 0, &device.presentQueue);
    vkGetDeviceQueue(device.device, indices.compute, 0, &device.computeQueue);

    device.graphicsIndex = indices.graphics;
    device.presentIndex = indices.present;
    device.computeIndex = indices.compute;

    m_free(queueCreateInfos);

    return device;
}

/// @brief releases all resources used uppon device creation
static void ievk_device_destroy(evkDevice* evkDevice)
{
    EVK_ASSERT(evkDevice != NULL, "evkDevice is NULL");

    vkDestroyDevice(g_EVKBackend->evkDevice.device, NULL);
    g_EVKBackend->evkDevice.device = VK_NULL_HANDLE;
}

/// @brief queries information for the swapchain, like available surface formats and present mode.
static evkSwapchainDetails ievk_swapchain_query_details(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    evkSwapchainDetails details = { 0 };
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &details.surfaceFormatCount, NULL);

    if (details.surfaceFormatCount != 0) {
        details.surfaceFormats = (VkSurfaceFormatKHR*)m_malloc(details.surfaceFormatCount * sizeof(VkSurfaceFormatKHR));

        if (details.surfaceFormats) {
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &details.surfaceFormatCount, details.surfaceFormats);
        }
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &details.presentModeCount, NULL);

    if (details.presentModeCount != 0) {
        details.presentModes = (VkPresentModeKHR*)m_malloc(details.presentModeCount * sizeof(VkPresentModeKHR));
        if (details.presentModes) {
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &details.presentModeCount, details.presentModes);
        }
    }

    return details;
}

/// @brief chooses the surface format most appropriate from a list of available formats
static VkSurfaceFormatKHR ievk_swapchain_choose_surface_format(VkSurfaceFormatKHR* formats, uint32_t quantity)
{
    for (uint32_t i = 0; i < quantity; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return formats[i];
    }

    return formats[0];
}

/// @brief chooses the appresentation mode for the swapchain
static VkPresentModeKHR ievk_swapchain_choose_present_mode(VkPresentModeKHR* modes, uint32_t quantity, bool vsync)
{
    if (modes == NULL || quantity == 0 || vsync)  return VK_PRESENT_MODE_FIFO_KHR;

    bool immediateModeAvailable = false;
    for (uint32_t i = 0; i < quantity; i++) {
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)  return VK_PRESENT_MODE_MAILBOX_KHR;
        if (modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) immediateModeAvailable = true;
    }

    if (immediateModeAvailable) return VK_PRESENT_MODE_IMMEDIATE_KHR;
    return VK_PRESENT_MODE_FIFO_KHR;
}

/// @brief adjusts the correct extent for the swapchain
static VkExtent2D ievk_swapchain_adjust_extent(const VkSurfaceCapabilitiesKHR* capabilities, uint32_t width, uint32_t height)
{
    if (capabilities->currentExtent.width != UINT32_MAX) return capabilities->currentExtent;

    VkExtent2D actualExtent = { width, height };
    actualExtent.width = (uint32_t)f_clamp((const float)actualExtent.width, (const float)capabilities->minImageExtent.width, (const float)capabilities->maxImageExtent.width);
    actualExtent.height = (uint32_t)f_clamp((const float)actualExtent.height, (const float)capabilities->minImageExtent.height, (const float)capabilities->maxImageExtent.height);

    return actualExtent;
}

/// @brief creates the swapchain object
static evkSwapchain ievk_swapchain_create(VkSurfaceKHR surface, VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vsync)
{
    evkSwapchain swapchain = { 0 };

    evkSwapchainDetails details = ievk_swapchain_query_details(physicalDevice, surface);
    swapchain.format = ievk_swapchain_choose_surface_format(details.surfaceFormats, details.surfaceFormatCount);
    swapchain.presentMode = ievk_swapchain_choose_present_mode(details.presentModes, details.presentModeCount, vsync);
    swapchain.extent = ievk_swapchain_adjust_extent(&details.capabilities, extent.width, extent.height);

    swapchain.imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && swapchain.imageCount > details.capabilities.maxImageCount) swapchain.imageCount = details.capabilities.maxImageCount;

    evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
    int queueFamilyIndices[] = { indices.graphics, indices.present, indices.compute };
    VkSwapchainCreateInfoKHR swapchainCI = { 0 };
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = NULL;
    swapchainCI.flags = 0;
    swapchainCI.surface = surface;
    swapchainCI.minImageCount = swapchain.imageCount;
    swapchainCI.imageFormat = swapchain.format.format;
    swapchainCI.imageColorSpace = swapchain.format.colorSpace;
    swapchainCI.imageExtent = swapchain.extent;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // allow copying swapchain images
    swapchainCI.preTransform = details.capabilities.currentTransform;
    swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCI.presentMode = swapchain.presentMode;
    swapchainCI.clipped = VK_TRUE;

    if (indices.graphics != indices.present) {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices = (uint32_t*)queueFamilyIndices;
    }

    else {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    EVK_ASSERT(vkCreateSwapchainKHR(device, &swapchainCI, NULL, &swapchain.swapchain) == VK_SUCCESS, "Failed to create swapchain");

    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &swapchain.imageCount, NULL);
    swapchain.images = (VkImage*)m_malloc(swapchain.imageCount * sizeof(VkImage));
    swapchain.imageViews = (VkImageView*)m_malloc(sizeof(VkImageView) * swapchain.imageCount);
    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &swapchain.imageCount, swapchain.images);

    for (uint32_t i = 0; i < swapchain.imageCount; i++) {
        evk_device_create_image_view(device, swapchain.images[i], swapchain.format.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1, VK_IMAGE_VIEW_TYPE_2D, NULL, &swapchain.imageViews[i]);
    }

    // free details
    m_free(details.presentModes);
    m_free(details.surfaceFormats);

    return swapchain;
}

/// @brief releases all resources used on swapchain creation
static void ievk_swapchain_destroy(evkSwapchain* swapchain, VkDevice device)
{
    for (uint32_t i = 0; i < swapchain->imageCount; i++) {
        vkDestroyImageView(device, swapchain->imageViews[i], NULL);
    }

    m_free(swapchain->imageViews);
    m_free(swapchain->images);

    vkDestroySwapchainKHR(device, swapchain->swapchain, NULL);
    swapchain->swapchain = VK_NULL_HANDLE;
}

/// @brief creates all syncronization resources for CPU-GPU communication
static evkSync ievk_sync_create(VkDevice device, uint32_t objectCount)
{
    evkSync sync = { 0 };
    sync.objectCount = objectCount;

    VkSemaphoreCreateInfo semaphoreCI = { 0 };
    semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCI.pNext = NULL;
    semaphoreCI.flags = 0;

    VkFenceCreateInfo fenceCI = { 0 };
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.pNext = NULL;
    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    sync.imageAvailableSemaphores = (VkSemaphore*)m_malloc(sizeof(VkSemaphore) * objectCount);
    sync.finishedRenderingSemaphores = (VkSemaphore*)m_malloc(sizeof(VkSemaphore) * objectCount);
    sync.framesInFlightFences = (VkFence*)m_malloc(sizeof(VkFence) * objectCount);

    for (size_t i = 0; i < objectCount; i++) {
        EVK_ASSERT(vkCreateSemaphore(device, &semaphoreCI, NULL, &sync.imageAvailableSemaphores[i]) == VK_SUCCESS, "Failed to create image available semaphore");
        EVK_ASSERT(vkCreateSemaphore(device, &semaphoreCI, NULL, &sync.finishedRenderingSemaphores[i]) == VK_SUCCESS, "Failed to create rendering finished semaphore");
        EVK_ASSERT(vkCreateFence(device, &fenceCI, NULL, &sync.framesInFlightFences[i]) == VK_SUCCESS, "Failed to create syncronizer fence");
    }

    return sync;
}

/// @brief releases all resources used on sync creation
static void ievk_sync_destroy(evkSync* sync, VkDevice device)
{
    for (uint32_t i = 0; i < sync->objectCount; i++) {
        if (sync->imageAvailableSemaphores[i]) vkDestroySemaphore(device, sync->imageAvailableSemaphores[i], NULL);
        if (sync->finishedRenderingSemaphores[i]) vkDestroySemaphore(device, sync->finishedRenderingSemaphores[i], NULL);
        if (sync->framesInFlightFences[i]) vkDestroyFence(device, sync->framesInFlightFences[i], NULL);
    }
    m_free(sync->imageAvailableSemaphores);
    m_free(sync->finishedRenderingSemaphores);
    m_free(sync->framesInFlightFences);
}

static void ievk_resize(VkExtent2D extent)
{
    vkDeviceWaitIdle(g_EVKBackend->evkDevice.device);

    // if you wish to make a vulkan resize, you must first re-invent the universe
    evk_renderphase_ui_destroy(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device);
    if (evk_using_viewport()) {
        evk_renderphase_viewport_destroy(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device);
    }
    evk_renderphase_picking_destroy(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_main_destroy(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device);
    
    ievk_swapchain_destroy(&g_EVKBackend->evkSwapchain, g_EVKBackend->evkDevice.device);
    g_EVKBackend->evkSwapchain = ievk_swapchain_create(g_EVKBackend->evkInstance.surface, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, extent, evk_using_vsync());

    // renderphases
    g_EVKBackend->evkMainRenderphase = evk_renderphase_main_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa, false); // false because on this setup it'll never be the final phase
    EVK_ASSERT(evk_renderphase_main_create_framebuffers(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create main render phase frame buffers");

    g_EVKBackend->evkPickingRenderphase = evk_renderphase_picking_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->msaa);
    EVK_ASSERT(evk_renderphase_picking_create_framebuffers(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent) == evk_Success, "Failed to create picking render phase framebuffers");

    g_EVKBackend->evkUIRenderphase = evk_renderphase_ui_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, true); // true because on this setup this is the final phase
    EVK_ASSERT(evk_renderphase_ui_create_framebuffers(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create ui render phase framebuffers");

    if (evk_using_viewport()) {
        g_EVKBackend->evkViewportRenderphase = evk_renderphase_viewport_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa);
        EVK_ASSERT(evk_renderphase_viewport_create_framebuffers(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create viewport framebuffers");
    }

    evk_camera_set_aspect_ratio(evk_get_main_camera(), (float)(extent.width / extent.height));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General core
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkResult evk_initialize_backend(const evkCreateInfo* ci)
{
    // general initialization
    if (g_EVKBackend == NULL) {
        g_EVKBackend = (evkVulkanBackend*)m_malloc(sizeof(evkVulkanBackend));
        memset(g_EVKBackend, 0, sizeof(g_EVKBackend));
        if (!g_EVKBackend) {
            EVK_LOG(evk_Fatal, "Failed to allocate memory resources for evkVulkanBackend");
            return evk_Failure;
        }
        
        g_EVKBackend->buffers = shashtable_init();
        g_EVKBackend->pipelines = shashtable_init();
        g_EVKBackend->msaa = ci->MSAA;
    }
    
    // instance
    g_EVKBackend->evkInstance = ievk_instance_create(ci->appName, ci->appVersion, ci->engineName, ci->engineVersion);

    // surface
    #ifdef _WIN32
    ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.window, NULL);
    #elif defined(__APPLE__)
    ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.layer, NULL);
    #elif defined(__ANDROID__)
    ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.window, NULL);
    #elif defined(__linux__) && !defined(__ANDROID__)
		#ifdef EVK_LINUX_USE_XLIB
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.window, ci->window.display);
        #elif defined(EVK_LINUX_USE_XCB)
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.connection);
        #else
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.display);
        #endif
    #endif

    // device
    VkPhysicalDevice physicalDevice = ievk_device_choose(g_EVKBackend->evkInstance.instance, g_EVKBackend->evkInstance.surface);
    g_EVKBackend->evkDevice = ievk_device_create(g_EVKBackend->evkInstance.instance, g_EVKBackend->evkInstance.surface, physicalDevice);

    // swapchain
    g_EVKBackend->evkSwapchain = ievk_swapchain_create(g_EVKBackend->evkInstance.surface, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, (VkExtent2D){ci->width, ci->height}, ci->vsync);
    
    // sync
    g_EVKBackend->evkSync = ievk_sync_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkSwapchain.imageCount);

    // render phases
    g_EVKBackend->evkMainRenderphase = evk_renderphase_main_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa, false); // false because on this setup it'll never be the final phase
    EVK_ASSERT(evk_renderphase_main_create_framebuffers(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create main render phase frame buffers");
    
    g_EVKBackend->evkPickingRenderphase = evk_renderphase_picking_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->msaa);
    EVK_ASSERT(evk_renderphase_picking_create_framebuffers(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent) == evk_Success, "Failed to create picking render phase framebuffers");
    
    g_EVKBackend->evkUIRenderphase = evk_renderphase_ui_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, true); // true because on this setup this is the final phase
    EVK_ASSERT(evk_renderphase_ui_create_framebuffers(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create ui render phase framebuffers");
    
    if (evk_using_viewport()) {
        g_EVKBackend->evkViewportRenderphase = evk_renderphase_viewport_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa);
        EVK_ASSERT(evk_renderphase_viewport_create_framebuffers(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create viewport framebuffers");
    }

    // buffers
    evkBuffer* cameraBuffer = evk_buffer_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, sizeof(evkCameraUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, EVK_CONCURRENTLY_RENDERED_FRAMES);
    EVK_ASSERT(shashtable_insert(g_EVKBackend->buffers, "MainCamera", cameraBuffer) == CTOOLBOX_SUCCESS, "Failed to insert camera buffer into the buffer library");

    // pipelines
    evkRenderpass* renderpass = evk_using_viewport() ? &g_EVKBackend->evkViewportRenderphase.evkRenderpass : &g_EVKBackend->evkMainRenderphase.evkRenderpass;
    EVK_ASSERT(evk_pipeline_sprite_create(g_EVKBackend->pipelines, renderpass, &g_EVKBackend->evkPickingRenderphase.evkRenderpass, g_EVKBackend->evkDevice.device) == evk_Success, "Failed to create quad pipelines");

    return evk_Success;
}

void evk_shutdown_backend()
{
    evk_buffer_destroy(g_EVKBackend->evkDevice.device, (evkBuffer*)shashtable_lookup(g_EVKBackend->buffers, "MainCamera"));
    shashtable_destroy(g_EVKBackend->buffers);

    evk_pipeline_sprite_destroy(g_EVKBackend->pipelines, g_EVKBackend->evkDevice.device);
    shashtable_destroy(g_EVKBackend->pipelines);

    if (evk_using_viewport()) {
        evk_renderphase_viewport_destroy(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device);
    }
    
    evk_renderphase_ui_destroy(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_picking_destroy(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_main_destroy(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device);

    ievk_sync_destroy(&g_EVKBackend->evkSync, g_EVKBackend->evkDevice.device);
    ievk_swapchain_destroy(&g_EVKBackend->evkSwapchain, g_EVKBackend->evkDevice.device);
    ievk_device_destroy(&g_EVKBackend->evkDevice);
    ievk_instance_destroy(&g_EVKBackend->evkInstance);

    m_free(g_EVKBackend);
}

void evk_update_backend(float timestep, bool* mustResize)
{
    // first phase
    evkCamera* mainCamera = evk_get_main_camera();
    evk_camera_update(mainCamera, timestep);

    evkCameraUBO mainCameraData = { 0 };
    mainCameraData.view = evk_camera_get_view(mainCamera);
    mainCameraData.viewInverse = evk_camera_get_view_inverse(mainCamera);
    mainCameraData.proj = evk_camera_get_perspective(mainCamera);
    evkBuffer* buffer = (evkBuffer*)shashtable_lookup(evk_get_buffers_library(), "MainCamera");
    evk_buffer_copy(buffer, evk_get_current_frame(), &mainCameraData, sizeof(evkCameraUBO), 0);

    // second phase
    vkWaitForFences(g_EVKBackend->evkDevice.device, 1, & g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame], VK_TRUE, UINT64_MAX);
    VkResult res = vkAcquireNextImageKHR(g_EVKBackend->evkDevice.device, g_EVKBackend->evkSwapchain.swapchain, UINT64_MAX, g_EVKBackend->evkSync.imageAvailableSemaphores[g_EVKBackend->evkSync.currentFrame], VK_NULL_HANDLE, &g_EVKBackend->evkSwapchain.imageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        float2 framebufferSize = evk_get_framebuffer_size();
        ievk_resize((VkExtent2D) { (uint32_t)framebufferSize.xy.x, (uint32_t)framebufferSize.xy.y });
        *mustResize = false;

        g_EVKBackend->evkSync.currentFrame = (g_EVKBackend->evkSync.currentFrame + 1) % EVK_CONCURRENTLY_RENDERED_FRAMES;
        return;
    }

    EVK_ASSERT(res == VK_SUCCESS || res == VK_SUBOPTIMAL_KHR, "Renderer update was not able to aquire an image from the swapchain");
    vkResetFences(g_EVKBackend->evkDevice.device, 1, &g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);

    // render phases
    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Main;
    evk_renderphase_main_update(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    
    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Picking;
    evk_renderphase_picking_update(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    
    if (evk_using_viewport()) {
        g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Picking;
        evk_renderphase_viewport_update(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    }

    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_UI;
    evk_renderphase_ui_update(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_get_renderui_callback());

    // submit command buffers
    VkSwapchainKHR swapChains[] = { g_EVKBackend->evkSwapchain.swapchain };
    VkSemaphore waitSemaphores[] = { g_EVKBackend->evkSync.imageAvailableSemaphores[g_EVKBackend->evkSync.currentFrame] };
    VkSemaphore signalSemaphores[] = { g_EVKBackend->evkSync.finishedRenderingSemaphores[g_EVKBackend->evkSwapchain.imageIndex] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo = { 0 };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (evk_using_viewport()) {
        VkCommandBuffer commandBuffers[4] = { 0 };
        commandBuffers[0] = g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[1] = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[2] = g_EVKBackend->evkViewportRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[3] = g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];

        submitInfo.commandBufferCount = EVK_STATIC_ARRAY_SIZE(commandBuffers);
        submitInfo.pCommandBuffers = commandBuffers;

        VkResult queueSubmit = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submitInfo, g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);
        if (queueSubmit != VK_SUCCESS) {
            EVK_ASSERT(1, "Renderer update was not able to submit frame to graphics queue");
        }
    }

    else {
        VkCommandBuffer commandBuffers[3] = { 0 };
        commandBuffers[0] = g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[1] = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[2] = g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];

        submitInfo.commandBufferCount = EVK_STATIC_ARRAY_SIZE(commandBuffers);
        submitInfo.pCommandBuffers = commandBuffers;

        VkResult queueSubmit = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submitInfo, g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);
        if (queueSubmit != VK_SUCCESS) {
            EVK_ASSERT(1, "Renderer update was not able to submit frame to graphics queue");
        }
    }

    // present the image
    VkPresentInfoKHR presentInfo = { 0 };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &g_EVKBackend->evkSwapchain.imageIndex;

    res = vkQueuePresentKHR(g_EVKBackend->evkDevice.graphicsQueue, &presentInfo);

    // failed to present the image, must recreate
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || *mustResize == true ) {
        float2 framebufferSize = evk_get_framebuffer_size();
        ievk_resize((VkExtent2D) { (uint32_t)framebufferSize.xy.x, (uint32_t)framebufferSize.xy.y });
        *mustResize = false;
    }

    else if (res != VK_SUCCESS) {
        EVK_ASSERT(1, "Renderer update was not able to properly present the graphics queue frame");
    }

    // advance to the next frame for the next render call
    g_EVKBackend->evkSync.currentFrame = (g_EVKBackend->evkSync.currentFrame + 1) % EVK_CONCURRENTLY_RENDERED_FRAMES;
}

uint32_t evk_pick_object_backend(float2 xy)
{
    VkResult res;
    uint32_t pixelValue = 0;

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingMemory = VK_NULL_HANDLE;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;

    VkBufferCreateInfo bufferCI = { 0 };
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.size = sizeof(uint32_t);
    bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    res = vkCreateBuffer(g_EVKBackend->evkDevice.device, &bufferCI, NULL, &stagingBuffer);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create staging buffer for picking");
        return 0;
    }

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(g_EVKBackend->evkDevice.device, stagingBuffer, &memReq);

    VkDeviceSize alignedSize = (memReq.size + 3) & ~3;

    uint32_t memType = evk_device_find_suitable_memory_type(g_EVKBackend->evkDevice.physicalDevice, memReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (memType == UINT32_MAX) {
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "No suitable memory type for picking");
        return 0;
    }

    VkMemoryAllocateInfo allocInfo = { 0 };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = alignedSize;
    allocInfo.memoryTypeIndex = memType;

    res = vkAllocateMemory(g_EVKBackend->evkDevice.device, &allocInfo, NULL, &stagingMemory);
    if (res != VK_SUCCESS) {
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to allocate memory for picking");
        return 0;
    }

    res = vkBindBufferMemory(g_EVKBackend->evkDevice.device, stagingBuffer, stagingMemory, 0);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to bind buffer memory for picking");
        return 0;
    }

    VkCommandBufferAllocateInfo cmdAlloc = { 0 };
    cmdAlloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAlloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAlloc.commandPool = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool;
    cmdAlloc.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(g_EVKBackend->evkDevice.device, &cmdAlloc, &cmdBuffer);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to allocate command buffer for picking");
        return 0;
    }

    VkCommandBufferBeginInfo beginInfo = { 0 };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    res = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to begin command buffer for picking");
        return 0;
    }

    uint32_t pickingImageIndex = g_EVKBackend->evkSwapchain.imageIndex;

    VkImageMemoryBarrier barrier = { 0 };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = g_EVKBackend->evkPickingRenderphase.colorImage;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

    // adjust screen coordinates
    float2 winSize = { (float)g_EVKBackend->evkSwapchain.extent.width, (float)g_EVKBackend->evkSwapchain.extent.height };
    if (evk_using_viewport()) {
        winSize = evk_get_viewport_size();
    }
    uint32_t fbX = (uint32_t)(xy.xy.x * g_EVKBackend->evkSwapchain.extent.width / winSize.xy.x);
    uint32_t fbY = (uint32_t)(xy.xy.y * g_EVKBackend->evkSwapchain.extent.height / winSize.xy.y);

    VkBufferImageCopy region = { 0 };
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = (VkOffset3D){ (int32_t)fbX, (int32_t)fbY, 0 };
    region.imageExtent = (VkExtent3D){ g_EVKBackend->evkSwapchain.extent.width, g_EVKBackend->evkSwapchain.extent.height, 1 }; // 1, 1, 1
    vkCmdCopyImageToBuffer(cmdBuffer, g_EVKBackend->evkPickingRenderphase.colorImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingBuffer, 1, &region);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

    res = vkEndCommandBuffer(cmdBuffer);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, & cmdBuffer);
        EVK_LOG(evk_Error, "Failed to end command buffer for picking");
        return 0;
    }

    VkFenceCreateInfo fenceCI = { 0 };
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    res = vkCreateFence(g_EVKBackend->evkDevice.device, &fenceCI, NULL, &fence);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to create fence for picking");
        return 0;
    }

    VkSubmitInfo submit = { 0 };
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmdBuffer;

    res = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submit, fence);
    if (res != VK_SUCCESS) {
        vkDestroyFence(g_EVKBackend->evkDevice.device, fence, NULL);
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to submit picking command buffer");
        return 0;
    }

    res = vkWaitForFences(g_EVKBackend->evkDevice.device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(g_EVKBackend->evkDevice.device, fence, NULL);

    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to wait for picking fence");
        return 0;
    }

    // read the pixel value
    void* data = NULL;
    res = vkMapMemory(g_EVKBackend->evkDevice.device, stagingMemory, 0, sizeof(uint32_t), 0, &data);

    if (res == VK_SUCCESS && data != NULL) {
        pixelValue = *(uint32_t*)data;
        vkUnmapMemory(g_EVKBackend->evkDevice.device, stagingMemory);

        if (pixelValue != 0) {
            EVK_LOG(evk_Error, "Picked entity ID: %u", pixelValue);
        }
    }
    else {
        EVK_LOG(evk_Error, "Failed to map memory for picking read");
    }

    vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
    vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
    vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);

    return pixelValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter/Setter
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VkInstance evk_get_instance()
{
    return g_EVKBackend->evkInstance.instance;
}

VkPhysicalDevice evk_get_physical_device()
{
    return g_EVKBackend->evkDevice.physicalDevice;
}

VkPhysicalDeviceProperties evk_get_physical_device_properties()
{
    return g_EVKBackend->evkDevice.physicalProps;
}

VkPhysicalDeviceFeatures evk_get_physical_device_features()
{
    return g_EVKBackend->evkDevice.phyiscalFeatures;
}

VkPhysicalDeviceMemoryProperties evk_get_physical_device_memory_properties()
{
    return g_EVKBackend->evkDevice.physicaMemProps;
}

VkDevice evk_get_device()
{
    return g_EVKBackend->evkDevice.device;
}

VkQueue evk_get_graphics_queue()
{
    return g_EVKBackend->evkDevice.graphicsQueue;
}

VkRenderPass evk_get_renderpass(evkRenderphaseType type)
{
    switch (type)
    {
        case evk_Renderphase_Type_Main: return g_EVKBackend->evkMainRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_Picking: return g_EVKBackend->evkPickingRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_UI: return g_EVKBackend->evkUIRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_Viewport:
        {
            if (!evk_using_viewport()) {
                EVK_LOG(evk_Error, "Requesting viewport renderphase but viewport was not enabled");
                return VK_NULL_HANDLE;
            }

            return g_EVKBackend->evkViewportRenderphase.evkRenderpass.renderpass;
        }
    }
    return VK_NULL_HANDLE;
}

VkCommandPool evk_get_command_pool(evkRenderphaseType type)
{
    switch (type)
    {
        case evk_Renderphase_Type_Main: return g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_Picking: return g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_UI: return g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_Viewport:
        {
            if (!evk_using_viewport()) {
                EVK_LOG(evk_Error, "Requesting viewport command pool but viewport was not enabled");
                return VK_NULL_HANDLE;
            }

            return g_EVKBackend->evkViewportRenderphase.evkRenderpass.cmdPool;
        }
    }
    return VK_NULL_HANDLE;
}

VkDescriptorPool evk_get_ui_descriptor_pool()
{
    return g_EVKBackend->evkUIRenderphase.descriptorPool;
}

VkDescriptorSetLayout evk_get_ui_descriptor_set_layout()
{
    return g_EVKBackend->evkUIRenderphase.descriptorSetLayout;
}

void* evk_get_renderphase(evkRenderphaseType type)
{
    switch (type)
    {
        case evk_Renderphase_Type_Main: return &g_EVKBackend->evkMainRenderphase;
        case evk_Renderphase_Type_Picking: return &g_EVKBackend->evkPickingRenderphase;
        case evk_Renderphase_Type_UI: return &g_EVKBackend->evkUIRenderphase;
        case evk_Renderphase_Type_Viewport: return &g_EVKBackend->evkViewportRenderphase;
    }
    EVK_LOG(evk_Fatal, "This renderphase (%d) is undefined", type);
    return NULL;
}

shashtable* evk_get_pipelines_library()
{
    return g_EVKBackend->pipelines;
}

shashtable* evk_get_buffers_library()
{
    return g_EVKBackend->buffers;
}

uint32_t evk_get_current_frame()
{
    return g_EVKBackend->evkSync.currentFrame;
}

evkRenderphaseType evk_get_current_renderphase_type()
{
    return g_EVKBackend->currentRenderphase;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Device
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkQueueFamily evk_device_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    evkQueueFamily indices = { 0 };
    indices.graphics = UINT32_MAX;
    indices.present = UINT32_MAX;
    indices.compute = UINT32_MAX;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties*)m_malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
            indices.graphicsFound = 1;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.compute = i;
            indices.computeFound = 1;
        }

        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support) {
            indices.present = i;
            indices.presentFound = 1;
        }

        if (indices.graphicsFound && indices.presentFound && indices.computeFound) break;
    }

    m_free(queue_families);
    return indices;
}

evkResult evk_device_create_image(VkExtent2D size, uint32_t mipLevels, uint32_t arrayLayers, VkDevice device, VkPhysicalDevice physicalDevice, VkImage* image, VkDeviceMemory* memory, VkFormat format, evkMSAA samples, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkImageCreateFlags flags)
{
    VkImageCreateInfo imageCI = { 0 };
    imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCI.pNext = NULL;
    imageCI.flags = flags;
    imageCI.imageType = VK_IMAGE_TYPE_2D;
    imageCI.extent.width = size.width;
    imageCI.extent.height = size.height;
    imageCI.extent.depth = 1;
    imageCI.mipLevels = mipLevels;
    imageCI.arrayLayers = arrayLayers;
    imageCI.format = format;
    imageCI.tiling = tiling;
    imageCI.usage = usage;
    imageCI.samples = (VkSampleCountFlagBits)samples;
    imageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageCI, NULL, image) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, *image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = { 0 };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

    if (vkAllocateMemory(device, &allocInfo, NULL, memory) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate memory for the device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    if (vkBindImageMemory(device, *image, *memory, 0) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to bind memory with device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    return evk_Success;
}

evkResult evk_device_create_image_view(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, uint32_t mipLevels, uint32_t layerCount, VkImageViewType viewType, const VkComponentMapping* swizzle, VkImageView* outView)
{
    if (mipLevels == 0 || layerCount == 0) {
        EVK_LOG(evk_Error, "Invalid mipLevels or layerCount (must be >= 1)");
        return evk_Failure;
    }

    VkImageViewCreateInfo imageViewCI = { 0 };
    imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCI.image = image;
    imageViewCI.viewType = viewType;
    imageViewCI.format = format;
    imageViewCI.subresourceRange.aspectMask = aspect;
    imageViewCI.subresourceRange.baseMipLevel = 0;
    imageViewCI.subresourceRange.levelCount = mipLevels;
    imageViewCI.subresourceRange.baseArrayLayer = 0;
    imageViewCI.subresourceRange.layerCount = layerCount;
    imageViewCI.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    // using a different swizzling
    if (swizzle) {
        imageViewCI.components = *swizzle;
    }

    if (vkCreateImageView(device, &imageViewCI, NULL, outView) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create image view");
        return evk_Failure;
    }

    return evk_Success;
}

evkResult evk_device_create_image_sampler(VkDevice device, VkPhysicalDevice physicalDevice, VkFilter min, VkFilter mag, VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w, float mipLevels, VkSampler* outSampler)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physicalDevice, &props);

    VkSamplerCreateInfo samplerCI = { 0 };
    samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.magFilter = mag;
    samplerCI.minFilter = min;
    samplerCI.addressModeU = u;
    samplerCI.addressModeV = v;
    samplerCI.addressModeW = w;
    samplerCI.anisotropyEnable = VK_TRUE;
    samplerCI.maxAnisotropy = props.limits.maxSamplerAnisotropy;
    samplerCI.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCI.unnormalizedCoordinates = VK_FALSE;
    samplerCI.compareEnable = VK_FALSE;
    samplerCI.maxLod = mipLevels;
    samplerCI.minLod = 0.0f;
    samplerCI.mipLodBias = 0.0f;
    samplerCI.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(device, &samplerCI, NULL, outSampler) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create image sampler");
        return evk_Failure;
    }

    return evk_Success;
}

evkResult evk_device_create_image_descriptor_set(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkSampler sampler, VkImageView view, VkDescriptorSet* outDescriptor)
{
    VkDescriptorSetAllocateInfo allocInfo = { 0 };
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    if (vkAllocateDescriptorSets(device, &allocInfo, outDescriptor) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate descriptor set");
        return evk_Failure;
    }

    // update descriptor set
    VkDescriptorImageInfo descImage[1] = { 0 };
    descImage[0].sampler = sampler;
    descImage[0].imageView = view;
    descImage[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet writeDesc[1] = { 0 };
    writeDesc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDesc[0].dstSet = *outDescriptor;
    writeDesc[0].descriptorCount = 1;
    writeDesc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDesc[0].pImageInfo = descImage;
    vkUpdateDescriptorSets(device, 1, writeDesc, 0, NULL);

    return evk_Success;
}

void evk_device_create_image_mipmaps(VkDevice device, VkQueue queue, VkCommandBuffer cmdBuffer, int32_t width, int32_t height, int32_t mipLevels, VkImage image)
{
    if (mipLevels <= 1) return;

    VkImageMemoryBarrier barrier = { 0 };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = width;
    int32_t mipHeight = height;

    for (int32_t i = 1; i < mipLevels; i++) {

        // transition previous mip level to transfer source
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        // blit from previous level to current level
        VkImageBlit blit = { 0 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.srcOffsets[1].x = mipWidth;
        blit.srcOffsets[1].y = mipHeight;
        blit.srcOffsets[1].z = 1;
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;
        blit.dstOffsets[1].x = mipWidth > 1 ? mipWidth / 2 : 1;
        blit.dstOffsets[1].y = mipHeight > 1 ? mipHeight / 2 : 1;
        blit.dstOffsets[1].z = 1;
        vkCmdBlitImage(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        // transition previous level to shader read
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    // transition last mip level
    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
}

void evk_device_create_image_memory_barrier(VkCommandBuffer cmdBuffer, VkImage image, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
{
    VkImageMemoryBarrier imageMemoryBarrier = { 0 };
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = 0;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.srcAccessMask = srcAccessFlags;
    imageMemoryBarrier.dstAccessMask = dstAccessFlags;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
}

uint32_t evk_device_find_suitable_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    EVK_LOG(evk_Error, "Failed to find suitable memory type");
    return UINT32_MAX;
}

VkFormat evk_device_find_suitable_format(VkPhysicalDevice physicalDevice, const VkFormat* candidates, uint32_t candidatesCount, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    VkFormat resFormat = VK_FORMAT_UNDEFINED;
    for (uint32_t i = 0; i < candidatesCount; i++) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, candidates[i], &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) resFormat = candidates[i];
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) resFormat = candidates[i];
    }

    EVK_ASSERT(resFormat != VK_FORMAT_UNDEFINED, "Failed to find suitable VkFormat");
    return resFormat;
}

VkFormat evk_device_find_depth_format(VkPhysicalDevice physicalDevice)
{
    const VkFormat candidates[] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT };
    VkFormat format = evk_device_find_suitable_format(physicalDevice, candidates, 3, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    return format;
}

evkResult evk_device_create_buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data)
{
    VkBufferCreateInfo bufferCI = { 0 };
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.size = size;
    bufferCI.usage = usage;
    bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult res = vkCreateBuffer(device, &bufferCI, NULL, buffer);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create buffer on GPU");
        return evk_Failure;
    }

    VkMemoryRequirements memRequirements = { 0 };
    vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);

    // allocate memory for the buffer and bind it
    VkMemoryAllocateInfo allocInfo = { 0 };
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, properties);

    res = vkAllocateMemory(device, &allocInfo, NULL, memory);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate memory for GPU buffer");
        vkDestroyBuffer(device, *buffer, NULL);
        return evk_Failure;
    }

    res = vkBindBufferMemory(device, *buffer, *memory, 0);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to bind GPU memory with buffer");
        vkDestroyBuffer(device, *buffer, NULL);
        vkFreeMemory(device, *memory, NULL);
        return evk_Failure;
    }

    // map data if passing it
    if (data != NULL) {
        void* mapped;
        res = vkMapMemory(device, *memory, 0, size, 0, &mapped);
        if (res == VK_SUCCESS) {
            memcpy(mapped, data, size);
            vkUnmapMemory(device, *memory);
        }

        else {
            EVK_LOG(evk_Error, "Failed to map memory for data upload (VkResult: %d)", res);
        }
    }
    return evk_Success;
}

VkCommandBuffer evk_device_begin_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool)
{
    VkCommandBufferAllocateInfo cmdBufferAllocInfo = { 0 };
    cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocInfo.commandPool = cmdPool;
    cmdBufferAllocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, &commandBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate command buffers for single time use");
    }

    VkCommandBufferBeginInfo cmdBufferBeginInfo = { 0 };
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &cmdBufferBeginInfo)) {
        EVK_LOG(evk_Error, "Failed to begin single time command buffer");
    }

    return commandBuffer;
}

evkResult evk_device_end_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool, VkCommandBuffer cmdBuffer, VkQueue queue)
{
    if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to end command buffer recording");
        return evk_Failure;
    }

    VkSubmitInfo submitInfo = { 0 };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;

    if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to submit command buffer to queue");
        return evk_Failure;
    }

    if (vkQueueWaitIdle(queue) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to await queue response from sent command buffer");
        return evk_Failure;
    }

    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
    return evk_Success;
}

int32_t evk_device_calculate_image_mipmap(uint32_t width, uint32_t height, bool uiImage)
{
    if (uiImage || evk_get_msaa() != evk_Msaa_Off) return 1; // UI textures or MSAA textures cannot have mipmaps
    return i_floor(f_log2(f_max((float)width, (float)height))) + 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkBuffer* evk_buffer_create(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, uint32_t frameCount)
{
    if (size == 0 || frameCount == 0) {
        EVK_LOG(evk_Error, "Invalid buffer size or frame count");
        return NULL;
    }

    evkBuffer* buffer = (evkBuffer*)m_malloc(sizeof(evkBuffer));
    if (!buffer) {
        EVK_LOG(evk_Error, "Failed to allocate buffer structure");
        return NULL;
    }

    buffer->size = size;
    buffer->usage = usage;
    buffer->memoryProperties = memoryProperties;
    buffer->frameCount = frameCount;

    // allocate arrays for per-frame resources
    buffer->buffers = (VkBuffer*)m_malloc(sizeof(VkBuffer) * frameCount);
    buffer->memories = (VkDeviceMemory*)m_malloc(sizeof(VkDeviceMemory) * frameCount);
    buffer->mappedPointers = (void**)m_malloc(sizeof(void*) * frameCount);
    buffer->isMapped = (bool*)m_malloc(sizeof(bool) * frameCount);

    if (!buffer->buffers || !buffer->memories || !buffer->mappedPointers || !buffer->isMapped) {
        EVK_LOG(evk_Error, "Failed to allocate buffer arrays");
        evk_buffer_destroy(device, buffer);
        return NULL;
    }

    memset(buffer->mappedPointers, 0, sizeof(void*) * frameCount);
    memset(buffer->isMapped, 0, sizeof(bool) * frameCount);

    // create each buffer
    for (uint32_t i = 0; i < frameCount; i++)
    {
        VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkResult result = vkCreateBuffer(device, &bufferInfo, NULL, &buffer->buffers[i]);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to create buffer %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer->buffers[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo = { 0 };
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

        result = vkAllocateMemory(device, &allocInfo, NULL, &buffer->memories[i]);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to allocate buffer memory %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        result = vkBindBufferMemory(device, buffer->buffers[i], buffer->memories[i], 0);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to bind buffer memory %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        // auto-map if host visible
        if (memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            evkResult evkResult = evk_buffer_map(device, buffer, i);
            if (evkResult != evk_Success) {
                EVK_LOG(evk_Warn, "Failed to auto-map buffer %u: %d", i, result);
            }
        }
    }

    return buffer;
}

void evk_buffer_destroy(VkDevice device, evkBuffer* buffer)
{
    if (!buffer) return;

    if (buffer->buffers) {
        for (uint32_t i = 0; i < buffer->frameCount; i++) {
            if (buffer->buffers[i] != VK_NULL_HANDLE) {
                // unmap if mapped
                if (buffer->isMapped[i]) {
                    evk_buffer_unmap(device, buffer, i);
                }

                vkDestroyBuffer(device, buffer->buffers[i], NULL);
                buffer->buffers[i] = VK_NULL_HANDLE;
            }
        }
        m_free(buffer->buffers);
    }

    if (buffer->memories) {
        for (uint32_t i = 0; i < buffer->frameCount; i++) {
            if (buffer->memories[i] != VK_NULL_HANDLE) {
                vkFreeMemory(device, buffer->memories[i], NULL);
                buffer->memories[i] = VK_NULL_HANDLE;
            }
        }
        m_free(buffer->memories);
    }

    if (buffer->mappedPointers) m_free(buffer->mappedPointers);
    if (buffer->isMapped) m_free(buffer->isMapped);

    m_free(buffer);
}

evkResult evk_buffer_map(VkDevice device, evkBuffer* buffer, uint32_t frameIndex)
{
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;
    if (buffer->isMapped[frameIndex]) return evk_Success; // already mapped

    if (!(buffer->memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
        EVK_LOG(evk_Error, "Cannot map non-host-visible buffer");
        return evk_Failure;
    }

    VkResult result = vkMapMemory(device, buffer->memories[frameIndex], 0, buffer->size, 0, &buffer->mappedPointers[frameIndex]);
    if (result == VK_SUCCESS) buffer->isMapped[frameIndex] = true;
    else EVK_LOG(evk_Error, "Failed to map buffer: %d", result);

    return evk_Success;
}

evkResult evk_buffer_unmap(VkDevice device, evkBuffer* buffer, uint32_t frameIndex)
{
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;
    if (!buffer->isMapped[frameIndex]) return evk_Success; // not mapped

    vkUnmapMemory(device, buffer->memories[frameIndex]);
    buffer->mappedPointers[frameIndex] = NULL;
    buffer->isMapped[frameIndex] = false;

    return evk_Success;
}

evkResult evk_buffer_copy(evkBuffer* buffer, uint32_t frameIndex, const void* data, VkDeviceSize size, VkDeviceSize offset)
{
    if (!buffer || !data || size == 0) return evk_Failure;

    if (frameIndex >= buffer->frameCount) {
        EVK_LOG(evk_Error, "Frame index %u out of bounds", frameIndex);
        return evk_Failure;
    }

    if (offset + size > buffer->size) {
        EVK_LOG(evk_Error, "Copy exceeds buffer size");
        return evk_Failure;
    }

    if (!buffer->isMapped[frameIndex]) {
        EVK_LOG(evk_Error, "Buffer not mapped at frame %u", frameIndex);
        return evk_Failure;
    }

    memcpy((char*)buffer->mappedPointers[frameIndex] + offset, data, size);
    return evk_Success;
}

evkResult evk_buffer_flush(VkDevice device, evkBuffer* buffer, uint32_t frameIndex, VkDeviceSize size, VkDeviceSize nonCoherentAtomSize, VkDeviceSize offset)
{
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;

    if (!(buffer->memoryProperties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
    {
        VkMappedMemoryRange memoryRange = { VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
        memoryRange.memory = buffer->memories[frameIndex];

        VkDeviceSize atomSize = nonCoherentAtomSize;
        VkDeviceSize alignedOffset = offset & ~(atomSize - 1);
        VkDeviceSize end = offset + size;
        VkDeviceSize alignedEnd = (end + atomSize - 1) & ~(atomSize - 1);
        VkDeviceSize alignedSize = alignedEnd - alignedOffset;

        // clamp to buffer size
        if (alignedOffset + alignedSize > buffer->size) {
            alignedSize = buffer->size - alignedOffset;
        }

        memoryRange.offset = alignedOffset;
        memoryRange.size = alignedSize;

        if (vkFlushMappedMemoryRanges(device, 1, &memoryRange) != VK_SUCCESS) {
            return evk_Failure;
        }
    }

    return evk_Success;
}

evkResult evk_buffer_command_copy(VkCommandBuffer commandBuffer, evkBuffer* srcBuffer, uint32_t srcFrameIndex, evkBuffer* dstBuffer, uint32_t dstFrameIndex, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    if (!srcBuffer || !dstBuffer || srcFrameIndex >= srcBuffer->frameCount || dstFrameIndex >= dstBuffer->frameCount) return evk_Failure;

    VkBufferCopy copyRegion = { 0 };
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    copyRegion.size = (size == VK_WHOLE_SIZE) ? srcBuffer->size - srcOffset : size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer->buffers[srcFrameIndex], dstBuffer->buffers[dstFrameIndex], 1, &copyRegion);

    return evk_Success;
}

#ifdef __cplusplus 
}
#endif
