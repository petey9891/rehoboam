//
// Created by Andrew Peterson on 11/2/23.
//

#include "state/application_state.h"
#include "network/command.h"
#include "applications/shader_application.h"

#include "applications/cube_loading.h"
#include "applications/cube_calibration.h"
#include "applications/lava_lamp.h"
#include "applications/clouds.h"
#include "applications/gyroid.h"
#include "applications/water.h"

using namespace Rehoboam::SocketLibrary;

ApplicationState::ApplicationState()
    : m_state({ true, 1.0f }) {
    this->initializeApplications();
    this->switchToShaderApplication(CalibrationMode);
}

void ApplicationState::applyCommand(const Command& cmd) {
    if (cmd.type == MessageType::CubeDisplayToggle) {
        this->m_state.display_on = !this->m_state.display_on;
    } else if (cmd.type == MessageType::CubeBrightness) {
        this->m_state.brightness = (float) cmd.data[0] / 100.0f;
    } else if (cmd.type == MessageType::CubeDisplayChange) {
        if (!cmd.data.empty()) {
            try {
                Mode mode = ApplicationState::commandToMode(cmd.data[0]);
                printf("Applying command: %d\n", mode);
                this->switchToShaderApplication(mode);
            } catch (const std::out_of_range &e) {
               // Do nothing
               LOG(ERROR, "Error", (std::string) e.what());
            }
        }
    }
}

ShaderApplication* ApplicationState::getActiveShaderApplication() {
    return this->m_activeShaderApp;
}

void ApplicationState::initializeApplications() {
    this->m_shaderApps[LoadingMode] = std::make_unique<CubeLoading>();
    this->m_shaderApps[CalibrationMode] = std::make_unique<CubeCalibration>();
    this->m_shaderApps[LavaLampMode] = std::make_unique<LavaLamp>();
    this->m_shaderApps[CloudsMode] = std::make_unique<Clouds>();
    this->m_shaderApps[GyroidMode] = std::make_unique<Gyroid>();
    this->m_shaderApps[WaterMode] = std::make_unique<Water>();
}

void ApplicationState::switchToShaderApplication(ApplicationState::Mode mode) {
    LOG(INFO, "Loading mode", ApplicationState::modeToString(mode));
    ShaderApplication* newApp = this->m_shaderApps[mode].get();

    if (newApp == nullptr) return;

    this->m_activeShaderApp = newApp;
    this->m_activeShaderApp->activate();
}

ApplicationState::Mode ApplicationState::commandToMode(uint8_t value) {
    switch (value) {
        case CalibrationMode: return CalibrationMode;
        case LoadingMode: return LoadingMode;
        case LavaLampMode: return LavaLampMode;
        case CloudsMode: return CloudsMode;
        case GyroidMode: return GyroidMode;
        case WaterMode: return WaterMode;
        default: throw std::out_of_range("Invalid value for Mode enum");
    }
}

std::string ApplicationState::modeToString(Mode mode) {
    switch (mode) {
        case CalibrationMode: return "CalibrationMode";
        case LoadingMode: return "LoadingMode";
        case LavaLampMode: return "LavaLampMode";
        case CloudsMode: return "CloudsMode";
        case GyroidMode: return "GyroidMode";
        case WaterMode: return "WaterMode";
        default: throw std::out_of_range("Invalid value for Mode enum");
    }
}
