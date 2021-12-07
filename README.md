# cel
Cel is an open source game engine, developed primarily for linux.



# Setup

Linux:                                                 

## Dependencies
cel requires a couple of libraries, namely GLFW, glm and bullet. These can be installed via a package manager.

Ubuntu:
    ```
    sudo apt-get install libglfw3 libglm-dev libbullet-dev
    ```

Arch:
    ```
    sudo pacman -S glm glfw-x11 bullet
    ```

Clone the repository with `git clone https://github.com/Jacgar8/cel.git`                                            

`cd` into the directory and run `make` to build cel                                                                 
 *      All output files can be found in the `build` directory.
# Using Cel
To use cel, create a class derived from `cel::project` and set up `cel::project_builder`                            
The project is now ready. To compile it, you must link with libcel.so (Linux), which can be found in the `build` directory. 
An example project is included with cel, in `src/celtest`

 # Naming
Function names are written in snake case 
E.g. `void do_stuff(int param)`          
                                         
Variable names are written in snake case 
E.g. `int variable_name`                 
                                          
Functions/Classes beginning with two underscores are intended for internal use, but exposed in the event that they are needed. It is not recommended to use these.
