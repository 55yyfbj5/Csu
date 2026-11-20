#include "Core/Core_handle.hpp"
//#include "Resource_management/Id.hpp"
#include "Resource_management/Resource_error.hpp"
#include "Resource_management/Resource_states.hpp"
#include <algorithm>
#include <cstdint>
#include <expected>
#include <stack>
#include <system_error>
#include <vector>

namespace Resource {
    template<typename RESOURCE, typename HANDLE_TAG>
    class Pool {
    public:
    using Resource_handle = Core::Handle<HANDLE_TAG>;
        Pool(int max_slot_amount) : _max_slot_amount(std::max(1, max_slot_amount)) {
            _slots.resize(_max_slot_amount);
            for(int slot_number = 0; slot_number < _max_slot_amount; ++slot_number) {
                _free_slots.push(slot_number);
            }
        };

        std::expected<int, std::error_code> get_number_of_available_slots() {
            return _free_slots.size();
        };

        // Error free_id(const Resource::Id& id) {

        // };

        std::expected<Resource_handle, std::error_code> allocate_id() {
            if(_slots.size() == 0) {
                return std::unexpected(Resource::Error::NO_FREE_SLOTS_LEFT);
            }
            uint16_t slot_index =_free_slots.top();
            uint32_t unique_stamp = _created_ids_counter++;

            Resource_handle id = Resource_handle(slot_index, unique_stamp);

            _free_slots.pop();

            return id;
        };

        std::expected<RESOURCE*, std::error_code> get_resource_pointer(const Resource_handle& resource_handle) {
            // if (!resource_handle.is_valid) {
            //      return std::unexpected(Resource::Error::INVALID_ID);
            // }
            auto& slot = _slots[resource_handle.slot_index];
            slot.state = Resource::State::USED;
            // _free_slots.push(resource_handle.slot_index);
            return &slot;
        };

        std::expected<void, std::error_code> free_resource (const Resource_handle& id) {
            if (!id.is_valid) {
                return std::unexpected(make_error_code(Resource::Error::INVALID_ID));
            }
            auto& slot = _slots[id.slot_index];
            slot.state = Resource::State::FREE;
            return {};

        };

    private:
        std::vector<RESOURCE> _slots {};
        std::stack<int> _free_slots {};
        int _max_slot_amount;
        inline static int _created_ids_counter{0};
    };
};
