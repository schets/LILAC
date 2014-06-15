/**
 * @page input_page Input Tutorial
 * \section Introduction
 * The input language is a small subset of LISP, allowing users to create objects and variables
 * The basic syntax for declaring an object is (type name arguments).
 * \section Types
 * The LILAC input system allows for users to declare strongly typed input values
 * \subsection nat Native Types
 * To declare a variable of type double with a value of 2, one would write
 * @code
 * (double val 2)
 * @endcode
 * This would create an item with the label val, and type double. Inside the engine, a function
 * could call retrieve on this item with a double reference and get the value two in return
 *
 * The native types currently supported are @ref integer, \ref _float "float", @ref _double "double",
 * @ref _unsigned "unsigned", @ref  variable "var", and @ref string.
 * Note that internally, the \ref _float "float", @ref _double "double",
 * @ref _unsigned "unsigned", and @ref  variable "var" types have slightly different names
 *
 * \subsection obj Objects
 * In addition to native types, one can create objects that are interior to the system.
 * The possible objects are those registered with the
 * @ref type_register class.
 * The syntax is very similar to the native types, with the object being declared as (type name arguments).
 * 
 * Declaring a fourth order Runge-Kutta integrator object would be 
 * @code
 * (rk4 integrator)
 * @endcode
 *
 * It is also possible to pass arguments to created objects, although it is somewhat different than for native values.
 * A custom object may have a variable that conflicts with another object. For example, the desired rhs and integrator classes both take the parameter relerr.
 * However, each one requires a different value. This can be solved by creating a relerr object that is local to one of the classes.
 * This would be done by creating the object in the argument list, like so:
 * @code
 * (rk45 integrator)
 * (double relerr 1e-4)
 * (rhs_special rhs (double relerr 1e-2))
 * @endcode
 * This creates a global relerr equal to 1e-4, which is the variable that most objects will see when they call retrieve with an item called
 * "relerr". However, the rhs object will see the relerr object with a value of 1e-2.
 *
 * In addition, you can map one object to be equal to another. Say that for some reason, you wanted your variable abserr to always be equal to the variable relerr.
 * You can use the @map function to do so. The map function goes like (@map variable name1 name 2 ...). It can take an arbitrary amount of arguments.
 * @code
 * (class_1 blah1 (@map relerr abserr some_var))
 * (double some_var 0.1)
 * (double relerr 1e-4)
 * @endcode
 * Here, blah1 would see abserr and some_var equal to 1e-4.
 *
 * Support for global mapping may come soon, although that may be deprecated if the input structure is moved to a python program that calls into the C++
 *
 * One can find some examples in the folder src/input_files
 * \section comments Comments and Commands
 *
 * \subsection Comments
 * One can insert comments into a file with the ! character. Anything after this character on a line will be ignored.
 * Another use of this character is naming values that are only internal to the engine, since names containing !
 * will never be valid input and won't clash with input parameters
 *
 * \subsection Commands
 * One can pass some commands to the engine. These are:
 *      -# #include <filename>: This works like the C include in that it directly includes the file <filename>
 *      -# #cd <directory>: This works like the linux command cd. It changes the directory in which the engine looks for include files. This command starts relative to the running directory of the binary, and doesn't affect other IO.
 *      -# #setdir <directory>: This sets the directory relative to the running directory. Like cd but doesn't have to be a directory accessible from the current one
 */

