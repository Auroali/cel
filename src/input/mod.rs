use std::borrow::Borrow;
use std::cell::RefCell;
use lazy_static::lazy_static;
use glfw::Context;
use glfw::{Action, Key};
use std::collections::HashMap;
use std::sync::Mutex;

// P A I N
lazy_static! {
    static ref BINDINGS: Mutex<HashMap<Key, Vec<fn(Action)>>> = Mutex::new(HashMap::new());
}

thread_local! {
    static WIN: RefCell<Option<glfw::Window>> = RefCell::new(None);
}

pub fn init(window: glfw::Window) {
    WIN.with(|w| w.replace(Option::from(window)) );
}

/// Exposes the window instance
pub fn get_window_size() -> (i32, i32) {
    return WIN.with(|n| n.borrow().as_ref().unwrap().get_size());
}

/// Returns whether or not the specified key is being pressed
///
/// This must be called from the main thread
pub fn key_down(key: glfw::Key) -> bool {
    return WIN.with(|w| w.borrow().as_ref().expect("Unable to get reference to window!").get_key(key) == Action::Press);
}

/// Returns whether or not the main window is closed
/// This is handled via input, as it is the only place with
/// an easy way to reference the window
pub(in super) fn should_exit() -> bool {
    return WIN.with(|w| w.borrow().as_ref().expect("Unable to get reference to window!").should_close());
}

/// Swaps the buffers for the window
/// This is handled via input, as it is the only place with
/// an easy way to reference the window
pub(in super) fn swap_buffers() {
    WIN.with(|w| {
        w.borrow_mut().as_mut().expect("Unable to get reference to window!").swap_buffers();
    });
}