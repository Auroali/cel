#pragma once
namespace cel {
    struct time {
        static double deltaTime;
        static double fixedDeltaTime;
        static double get_current();
    };
}