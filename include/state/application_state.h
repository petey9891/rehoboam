//
// Created by Andrew Peterson on 11/2/23.
//

#include <cstdint>
#include <unordered_map>
#include <string>
#include <memory>

// Forward declare
class ShaderApplication;
struct Command;

class ApplicationState {
public:
    enum Mode {
        LoadingMode,
        CalibrationMode,
        LavaLampMode,
        CloudsMode,
        GyroidMode,
        WaterMode
    };

    struct State {
        bool display_on;
        float_t brightness;
    };

    ApplicationState();
    ~ApplicationState() = default;

    ShaderApplication* getActiveShaderApplication();
    const State& getState() const {
        return m_state;
    }

    void applyCommand(const Command& cmd);

private:
    void initializeApplications();
    void switchToShaderApplication(Mode mode);

    static Mode commandToMode(uint8_t value);
    static std::string modeToString(Mode mode);

    State m_state;

    std::unordered_map<Mode, std::unique_ptr<ShaderApplication>> m_shaderApps;
    ShaderApplication* m_activeShaderApp = nullptr;
};