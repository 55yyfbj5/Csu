#include <string>
#include <unordered_map>
#include <vector>

class Application {
public:
    bool initialize();
    void shutdown();
    bool run();
private:
    std::vector<const char*> current_program_{};
    bool should_close_ = false;
    int stdsrc_width_ {};
    int stdsrc_height_ {};
};
