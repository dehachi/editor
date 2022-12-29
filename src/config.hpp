#pragma once
#include "screen.hpp"

class Config {
public:
    void read(Screen &screen); // ./config.txtから設定を読み込む
};