#pragma once

#include <QVulkanWindow>
#include <QVulkanFunctions>
#include <QVulkanWindowRenderer>

class VulkanRenderer : public QVulkanWindowRenderer {
public:
    explicit VulkanRenderer(QVulkanWindow *w);
    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;
    void startNextFrame() override;

private:
    QVulkanWindow *m_window;
    QVulkanDeviceFunctions *m_devFuncs = nullptr;

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
};
