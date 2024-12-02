#ifndef EXITCOMPONENT_H
#define EXITCOMPONENT_H

#include "../applicationManager.h"
#include "../Storage/localStorage.h"
#include "../uiRenderer.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

class ExitComponent {
public:
    ExitComponent() {};
    ftxui::Component exitComponent(uiRenderer& UI);
};

#endif // EXITCOMPONENT_H