/**
 * @page todo_page Features for Lilac
 *
 * LILAC has some additions that it would probably benefit a great deal from
 *
 * \section Improved Input Capabilities
 *
 * As the complexity of possible inputs has grown, the basic lisp has become unwieldy.
 * It is only capable of declaring objects and assigning values to certain types.
 * 
 * A list of things that should be done are:
 *      - Lists/arrays of types would be very useful, especially for something like the C-Elegans input. These lists should be mutable as well.
 *      - General operators. As of now, the program only supports integrators and rhs-type operators. A general operator class would facilitate the creation of complex programs by stringing operators together, instead of having to re-write a system class for special cases.
 *      - Logic in the input section could be very useful for things like the controller, where each type is usually quite specialized.
 *      - Re-work the data/writer system to allow for io streaming or analytics inside the data/writer classes
 *      - Add fft routines for generic dimensions and types
 *
 * \subsection Python Input
 * As the complexity of the program is growing, it may be helpful to improve the input language.
 * This could be easily done by moving the input language to Python,
 * where a Python program would make calls to allocate types of objects and then initiate the simulation with
 * list of the current objects. This would make it very easy to add logic into the system, and would also
 * make extending the language fairly easy since Python handles most of the logic.
 *
 * \subsection Having a Proper Lisp
 * This would also help with moving logic to the input, but may not be as easy to use as a Python implementation. 
 * However, it would be very easy to extend the capabilities of the input language since it is tied directly into
 * the system.
 *
 * A lisp would be more foreign, and would also require that the engine run a full Lisp interpreter. However,
 * It would reduce the number of dependencies that Lilac has.
 *
 * \subsection Generic FFT routines
 *
 * The library currently used for ffts is fftw, which supports ffts in arbitrary dimensions. This means that
 * it is possible to perform ffts on arbitrary data types, as long as they can be mapped to a native array for fftw.
 *
 * fftw also supports real transforms, but it outputsn a strange format and reversing the transform always kills the input
 *
 * The idea that I have currently would follow a pattern similar to the float_traits class.
 * One would have a class designed as such:
 * @code
 * template<class T>
 * struct fft_traits: public float_traits<T>{
 *     bool native_mappable //true if the type is memory-compatible with complex values
 *     //default mapping from a type to a complex
 *     void map_to_comp(const T& inval, comp[dim] outcomp); specialize for each type,
 *     //would be called by the \ref apply::map function to transform into an array
 *     //of complex values for non memory compatible types
 *
 *     void map_from_comp(T& outval, const comp[dim] incomp); specialize for each type,
 *     //maps an array of complex values into a type
 *
 * }
 * @endcode
 * The fft routine could be templated with dimension, probably like
 * @code
 *      template<class T, size_t dim = float_traits<T>::dim>
 *      void fft(....)
 * @endcode
 *
 * would also allow for post/preprocessing lambdas to be passed in place of the standard routines

*
*
 */
