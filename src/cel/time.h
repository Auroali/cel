#pragma once
namespace cel {
    struct time {
        /**
         * @brief The current delta time
         * 
         * @warning Modifying this may cause unintended behaviour
         */
        static double delta_time;
        /**
         * @brief The current fixed delta time
         * 
         * @note Changing this will change the amount of fixed updates per second
         * 
         */
        static double fixed_delta_time;
        /**
         * @brief Gets the current running time
         * 
         * @return double the current time
         */
        static double get_current();
    };
}