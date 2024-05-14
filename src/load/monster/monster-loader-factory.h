#pragma once

#include <memory>

enum class MonsterLoaderVersionType;
class PlayerType;
class MonsterEntity;
class MonsterLoaderBase;
class MonsterLoaderFactory {
public:
    static std::shared_ptr<MonsterLoaderBase> create_loader();

private:
    MonsterLoaderFactory() = delete;
    static MonsterLoaderVersionType get_version();
};
