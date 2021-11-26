#pragma once
namespace cel {
    struct time {
        /**
         * @brief The current deltaTime
         * 
         */
        static double deltaTime;
        /**
         * @brief The current fixedDeltaTime
         * 
         * @note Changing this will change the amount of fixed updates per second
         * 
         */
        static double fixedDeltaTime;
        /**
         * @brief Gets the current running time
         * 
         * @return double the current time
         */
        static double get_current();
    };
}