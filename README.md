# 📦 Uniself

A collection of general-purpose and specialized C++ utilities designed from the ground up using **C++20**. The library is structured into standalone, independent modules covering everything from advanced string formatting to single-threaded neural networks and genetic simulations.

## 🛠️ Tech Stack & Features
* **Language:** C++20
* **Core Language Features:** Concepts, `constexpr` evaluations, Type Traits.
* **Testing Framework:** Google Test (gtest)
* **IDE & Build Tool:** Visual Studio (MSVC)

---

## 🧩 Modules Overview

The toolkit is divided into purpose-built components:

### 1. General-Purpose Utilities
* **concepts:** A set of concepts used in other libraries.
* **renum (reflective enum):** A lightweight single-header utility that allows to create (de)serializable enum-like classes. It enables automatic runtime conversion between enum values and their string representations.
* **strings:** Provides a unified, locale-independent, generic interface for casting various data types into and out of different string formats, including standart string containers (this also includes cross-casting between different standart string containers using hardcoded unicode en(de)coding).
* **math:** Introduces some common-used mathematical functions, such as ::uns::math::equals that is able to compare floating type values using mathematically correct way
* **calendar:** Provides a convenient enum-based way to operate with parts of date and time, including converting to/from ::std::chrono::system_clock::time_point. All enums, are renums to allow convenient way to walk through date parts

### 2. Specialized Components
* **lua_wrapper:** A specialized C++20 wrapper designed for embedding Lua into C++ applications or developing native C++ extensions for Lua.
  * **Bi-directional Reflection:** Provides a comprehensive toolkit for seamless C++ to Lua and Lua to C++ type and function reflection.
  * **Lua-like Data Handling:** Offers intuitive data exchange, manipulation, and error-handling mechanisms that closely mirror native Lua semantics.
  * **Extensible Engine Support:** Architected to be engine-agnostic, with long-term support planned for integrating alternative Lua VM backends and custom engines.
* **population:** A highly flexible, template-driven framework for modeling biological or abstract populations and algorithmic selection.
  * **Diverse Topology:** Supports both flat and hierarchical population structures, with optional population size constraints controlled via the environment or individual specimen attributes.
  * **Custom Reproduction & Breeding:** Offers fully customizable breeding and crossover mechanics, providing a robust foundation for Genetic Algorithms (GA) and evolutionary learning.
  * **Intraspecific Competition & Sustenance:** Features modular feeding and resource-consumption interface, enabling custom rewarding systems, penalty rules, and survival-of-the-fittest intra-species competition.
* **neuronetworks:** A template-based neural network framework designed for building highly customizable network architectures with tailored execution profiles.
  * **Generic Typing & Topology:** Fully parameterized by scalar types and neuron behaviors, allowing seamless switching between data types (e.g., `float`, `double`) and custom activation and collection functions.
  * **Example Single-Threaded CPU Engines:** Includes production-ready base implementations optimized for single-threaded CPU execution, featuring a dedicated training network (via Backpropagation) and a lightweight, forward-only execution network for trained models.
  * **Future GPU Acceleration:** Architecture is prepared for scaling, with upcoming template specializations planned for GPU-accelerated execution using NVIDIA CUDA.

---

## 🧪 Testing

The repository maintains code reliability through a comprehensive suite of unit tests powered by **Google Test**. Standalone testing projects are introduced for each library

---

## 📈 Roadmap

This repository is under active development. The upcoming features and optimizations are structured by module as follows:

### ⚙️ Core & General Utilities
- [ ] **Cross-Platform Support:** Migrate the build system from MSVC `.sln` solution files to **CMake** to enable seamless compilation on Linux and macOS.

### 🔌 Specialized Components

#### 🔸 strings
- [ ] **String Conversion:** Replace standard library routines for floating-to-string and string-to-floating conversions with high-performance, custom-written parsing and formatting algorithms to eliminate runtime overhead.

#### 🔸 lua_wrapper
- [ ] **Alternative Backend Integration:** Implement support for alternative Lua virtual machines, including LuaJIT for performance-critical environments and Luau.
- [ ] **Lua data types refactoring:** Refactor the `uns::lua::value` type into a union-like structure to optimize memory footprint. Refactor `uns::lua::type::table` by removing the redundant pimpl idiom.

#### 🔸 neuronetworks
- [ ] **GPU Acceleration:** Implement GPU-accelerated network classes using **NVIDIA CUDA** to handle parallel batch processing.
- [ ] **SIMD Optimization:** Introduce vectorization (AVX2/AVX-512) for the existing single-threaded CPU engines to boost forward-pass performance.


---

## ⚙️ Requirements & Installation
* **Compiler:** A modern compiler with robust **C++20** support (GCC 10+, Clang 11+, or MSVC 2019+).
* **Lua:** Lua 5.4.4 or higher.
* **Testing:** Google Test (version 1.8.1.8 or compatible).

### Integration
To integrate the library into your project, include the required headers and source files. You must also configure the `UNS_LIB` **system environment variable** on your machine.
> **⚠️ Note on Lua Integration:** The `lua_wrapper` module expects the `LUA_LIB` **environment variable** to be set and requires a **statically linked** build of Lua.
