#include "Level.hpp"
#include "nlohmann/json.hpp"
#include <format>
#include <fstream>

using nlohmann::json;
using std::runtime_error;
using std::ifstream;
using std::format;

Level::Level(const string& level_name)
    : _entities{}
    , _backgrounds{} {

    ifstream level_file{"data/" + level_name};
    auto level_data = json::parse(level_file);
    for (const auto& [entity_name, val] : level_data["entities"].items()) {
        for (const auto& position : val) {
            auto constructor = EntityRegistry::GetConstructor(entity_name);
            if (!constructor) {
                throw runtime_error(format("entity: {} is not registered but is included in the level: {}", entity_name, level_name));
            }
            _entities.push_back(constructor({position[0], position[1]}));
        }
    }
    for (const auto& [background_name, val] : level_data["backgrounds"].items()) {
        for (const auto& bounds_with_z : val) {
            ZIndex z = bounds_with_z[2];
            _backgrounds[z][background_name].push_back({
                .from = {bounds_with_z[0][0], bounds_with_z[0][1]},
                .to = {bounds_with_z[1][0], bounds_with_z[1][1]},
            });
        }
    }
}

unordered_map<SDL_Texture*, RenderTexture> Level::RenderTextures(Camera& camera) const {
    unordered_map<SDL_Texture*, RenderTexture> output{};
    for (const auto& [z_index, background_map] : _backgrounds) {
        for (const auto& [background_name, background_vec] : background_map) {
            SDL_Texture* texture = TextureMana
            for (const auto& background : background_vec) {

            }
        }
    }


}
