#include "Input/IAction_input_manager.hpp"


namespace Input {
    class Raylib_action_input_manager : IAction_input_manager{
    public:
        Key_event poll_keyboard_input() override;
    private:
        
        void _update();

};
}

