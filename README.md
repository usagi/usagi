# USAGI

This is the library for modern C++ projects presented by Usagi Ito<usagi@WonderRabbitProject.net> since 2015.

## Note

The library is under developing for ever, maybe. It will be integrate that have been implemented in the old libraries by the author.

## Concept

- Include useful helper free functions and types.
- Subdivide a feature unit.
- Documentation by the Doxygen.
- Support header only usage as much as possible.

## features

### usagi::chrono

- to_string_iso8601_gmt: convert from a `time_point` to a ISO8601-`string` in GMT
- to_string_iso8601_jst: convert from a `time_point` to a ISO8601-`string` in JST
- from_iso8601: convert from a ISO8601-`string` to a `time_point`
- time_zone_difference: get a time zone difference

### usagi::container

- circular_tile: a container type as a 2d-circular buffer

### usagi::math

- circular_modulus: modulus for get an unified sequence in all negative or positive values

### usagi::memory

#### Macros

- ARRAY_ELEMENTSOF: get a count of elements in an array type.
- ELEMENTSOF: get a count of elements in a type.
- OFFSETOF: get a memory address offset of element in a type.

### usagi/environment_special_support.hxx

Have an easy development for several strange environment if include it.

#### For the Windows Environment

- usagi/environment_special_support/winsock.hxx
- usagi/environment_special_support/windows.hxx
- usagi/environment_special_support/ciso646.hxx
- usagi/environment_special_support/use_math_defines.hxx

#### For the Emscripten Environment

- usagi/environment_special_support/emscripten.hxx

## License

- [MIT](LICENSE)

## Author

- Usagi Ito<usagi@WonderRabbitProject.net>

