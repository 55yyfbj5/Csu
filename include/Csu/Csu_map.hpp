#pragma once

#include "Core/Core_math.hpp"
#include <cstdint>
#include <expected>
#include <string>
#include <system_error>
#include <vector>

namespace Csu {

struct Difficulty {
    int HPDrainRate;
    int CircleSize;
    int OverallDifficulty;
    int ApproachRate;
    float SliderMultiplier;  //TODO; CONSIDER SWITHCING TO AN INT
    int SliderTickRate;
};

enum class Sample_set { //PLACEHOLDER: GIVE THIS ENUM A BETTER NAME
    NORMAL,
    SOFT,
    DRUM
};

struct General {
    std::string AudioFilename;
    int AudioLeadIn;
    int PreviewTime;
    int Countdown;
    Sample_set SampleSet;
    float StackLeniency; // consider switching to int
    int Mode;
    int LetterboxInBreaks;     // --
    int SpecialStyle;          //  \- i think this this options can only be true or false. consider changing their type to bool
    int WidescreenStoryboard;  // --
};

struct Editor { //TODO: CHECK IF STRUCT IS CORRECT
    std::vector<int> Bookmarks;
    int DistanceSpacing;
    int BeatDivisor;
    int GridSize;
    float TimelineZoom;
};

struct Event { //TODO: COMPLETE

};

struct Metadata {
    std::string Title;
    std::string TitleUnicode;
    std::string Artist;
    std::string ArtistUnicode;
    std::string Creator;
    std::string Version;
    std::string Source;
    std::vector<std::string> Tags;
    int BeatmapID;
    int BeatmapSetID;
};



struct Hit_sample {
    int normal_set;
    int additional_set;
    int custom_index;
    int volume;
    std::string file_name;
};

struct Hit_object {
    int x;
    int y;
    int timing;
    int type;
    int hit_sound;
    Hit_sample hit_sample;
};


class Map {
public:
    static std::expected<Map, std::error_code> create_map_object(const std::string& file_path);
    // Core::Math::Vec2<int> get_logical_note_position(int target_time, double time_scale_factor);
    // Core::Math::Vec2<int> get_render_note_position(const Core::Math::Vec2<int>& surface_extent, int target_time, double time_scale_factor);
    [[nodiscard]] Core::Math::Vec3<int> get_note(int target_time, double time_scale_factor = 1);
    Map() = default;
private:
    std::expected<void, std::error_code> load_map(std::string file_path);
    Metadata _metadata{};
    std::vector<Hit_object> _notes;
};


}


