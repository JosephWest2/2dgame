#pragma once
#include <memory>
#include <unordered_set>
#include <vector>

using std::enable_shared_from_this;
using std::unordered_set;
using std::weak_ptr;
using std::vector;

class Tickable;

struct WeakPtrHash {
    size_t operator()(weak_ptr<Tickable>& wp) {
        if (auto sp = wp.lock()) {
            return std::hash<Tickable*>{}(sp.get());
        }
        return size_t(0);
    }
};
struct WeakPtrEqual {
    bool operator()(const weak_ptr<Tickable>& a, const weak_ptr<Tickable>& b) {
        return !a.owner_before(b) && !b.owner_before(a);
    }
};

class Tickable : enable_shared_from_this<Tickable> {
public:
    Tickable() {
        _registry.insert(shared_from_this());
    };
    virtual ~Tickable() = default;
    virtual void OnTick() = 0;

    static void TickAll() {
        vector<weak_ptr<Tickable>> removals{};
        for (auto& wp : _registry) {
            if (auto sp = wp.lock()) {
                sp.get()->OnTick();
            } else {
                removals.push_back(wp);
            }
        }
        for (auto& wp : removals) {
            _registry.erase(wp);
        }
    }
private:
    static unordered_set<weak_ptr<Tickable>, WeakPtrHash, WeakPtrEqual> _registry;
};
