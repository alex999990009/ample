#include <sstream>

#include "ample/Debug.h"

#include "TextureManager.h"

namespace ample::gui
{
void TextureManager::drawInterface()
{
    ImGui::Begin("Texture manager");
    for (size_t i = 0; i < _textures.size(); ++i)
    {
        if (ImGui::TreeNode(_textures[i]->name().c_str()))
        {
            ImGui::Text("path: %s", _textures[i]->path().c_str());
            ImGui::TreePop();
        }
    }
    if (ImGui::Button("Upload texture"))
    {
        ImGui::OpenPopup("Upload new texture");
    }
    if (ImGui::BeginPopupModal("Upload new texture", NULL, ImGuiWindowFlags_MenuBar))
    {
        ImGui::InputText("Path", rawTexture.buffer, 255);
        ImGui::InputText("Name", rawTexture.name, 255);
        ImGui::InputScalar("Width", ImGuiDataType_U32, &rawTexture.eachSize.x, &rawTexture.eachSizeStep);
        ImGui::InputScalar("Height", ImGuiDataType_U32, &rawTexture.eachSize.y, &rawTexture.eachSizeStep);
        ImGui::InputScalar("X", ImGuiDataType_U32, &rawTexture.startPosition.x, &rawTexture.startPositionStep);
        ImGui::InputScalar("Y", ImGuiDataType_U32, &rawTexture.startPosition.y, &rawTexture.startPositionStep);
        ImGui::InputScalar("Frames X", ImGuiDataType_U32, &rawTexture.framesCount.x, &rawTexture.framesCountStep);
        ImGui::InputScalar("Frames Y", ImGuiDataType_U32, &rawTexture.framesCount.y, &rawTexture.framesCountStep);
        ImGui::Combo("Pixel format", &rawTexture.curFormat, rawTexture.format, IM_ARRAYSIZE(rawTexture.format));
        ImGui::Combo("Playback", &rawTexture.curPlayback, rawTexture.playback, IM_ARRAYSIZE(rawTexture.playback));
        ImGui::Combo("Origin X", &rawTexture.curOriginX, rawTexture.textureOrigin, IM_ARRAYSIZE(rawTexture.textureOrigin));
        ImGui::Combo("Origin Y", &rawTexture.curOriginY, rawTexture.textureOrigin, IM_ARRAYSIZE(rawTexture.textureOrigin));

        if (ImGui::Button("Upload"))
        {
            graphics::channelMode pixelFormat = graphics::channelMode::RGB;
            graphics::texturePlayback playback = graphics::texturePlayback::NORMAL;
            graphics::Vector2d<graphics::textureOrigin> origin = {graphics::textureOrigin::NORMAL, graphics::textureOrigin::NORMAL};
            if (rawTexture.curFormat == 1)
            {
                pixelFormat = graphics::channelMode::RGBA;
            }
            if (rawTexture.curPlayback == 1)
            {
                playback = graphics::texturePlayback::REVERSED;
            }
            if (rawTexture.curPlayback == 2)
            {
                playback = graphics::texturePlayback::BOOMERANG;
            }
            if (rawTexture.curOriginX == 1)
            {
                origin.x = graphics::textureOrigin::REVERSED;
            }
            if (rawTexture.curOriginY == 1)
            {
                origin.y = graphics::textureOrigin::REVERSED;
            }
            std::string path(rawTexture.buffer);
            std::string name(rawTexture.name);
            try
            {
                auto texture = std::make_shared<graphics::Texture>(graphics::TextureRaw{path,
                                                                                        name,
                                                                                        rawTexture.eachSize,
                                                                                        rawTexture.startPosition,
                                                                                        rawTexture.framesCount,
                                                                                        pixelFormat,
                                                                                        playback,
                                                                                        rawTexture.total,
                                                                                        origin});
                _textures.push_back(texture);
                rawTexture.success = true;
            }
            catch (const std::exception &e)
            {
                ImGui::OpenPopup("Unable to load texture");
                rawTexture.errorString = e.what();
            }
        }
        if (ImGui::BeginPopupModal("Unable to load texture", NULL))
        {
            ImGui::Text("%s", rawTexture.errorString.c_str());
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close") || rawTexture.success)
        {
            rawTexture.success = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

std::vector<std::shared_ptr<ample::graphics::Texture>> TextureManager::textures() noexcept
{
    return _textures;
}
} // namespace ample::gui