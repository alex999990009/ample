#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>

#include "EventHandler.h"
#include "EventManager.h"
#include "Vector2d.h"
#include "Window.h"

namespace ample::control
{
using Event = SDL_Event;

class KeyboardManager final : public EventHandler
{
public:
    void addKeyHandler(const keysym key, std::shared_ptr<KeyHandler> handler);
    void clearKey(const keysym key);
    void handleEvent(const SDL_Event &event) override;
    bool isKeyPressed(keysym key);
    bool isKeyReleased(keysym key);
    bool isKeyDown(keysym key);
    void clear();

private:
    std::unordered_map<int32_t, std::vector<std::shared_ptr<KeyHandler>>> _handlers;
    std::unordered_map<int32_t, int32_t> _keymapWasUp;
    std::unordered_map<int32_t, int32_t> _keymapWasDown;
    std::unordered_map<int32_t, int32_t> _keymapPressed;
};

class EventManager final
{
public:
    EventManager(window::Window &window);
    EventManager(const EventManager &other) = delete;

    EventManager &operator=(const EventManager &other) = delete;

    void update();

    void addKeyHandler(const keysym key, std::shared_ptr<KeyHandler> handler);
    void addEventHandler(const int eventType, std::shared_ptr<EventHandler> handler);
    void clearType(const int &eventType);

    ~EventManager() = default;

    std::shared_ptr<KeyboardManager> keyboard() const;
    std::shared_ptr<MouseHandler> mouse() const;

private:
    std::shared_ptr<KeyboardManager> _keyboard;
    std::shared_ptr<MouseHandler> _mouse;
    SDL_Event ev;
    std::unordered_map<int, std::vector<std::shared_ptr<EventHandler>>> _handlerByType;
    window::Window &_window;
};
} // namespace ample::control
