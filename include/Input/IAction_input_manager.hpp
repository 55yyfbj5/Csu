#include <expected>
#include <system_error>

enum class Key_state {
    PRESSED,
    HELD,

};

enum class Key {
    S, D, K, L
};

struct Key_event {
    Key_state state;
    Key key;
};

class IAction_input_manager {
public:
    virtual ~IAction_input_manager() = default;
    [[nodiscard]]virtual Key_event poll_keyboard_input() = 0;

};
