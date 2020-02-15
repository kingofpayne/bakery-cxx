[![Documentation Status](https://readthedocs.org/projects/libbakery/badge/?version=latest)](https://libbakery.readthedocs.io/en/latest/?badge=latest)
[![Build Status](https://travis-ci.com/kingofpayne/bakery.svg?branch=master)](https://travis-ci.com/kingofpayne/bakery)

# Bakery

Bakery is a C++ library for building and loading complex binary data files. It
allows creating any kind of data file with a meta-language looking like C++,
with very fast loading time thanks to binary just-in-time compilation.

In the first place, this library has been designed for video games, where
loading assets must be efficient. The meta-language makes editing the assets
with a simple text editor possible.

This library is still in development, but it has been tested a lot in a few of
our projects. There might be bugs or missing features, and any contribution is
welcome!

## Documentation

Documentation is available at https://libbakery.readthedocs.io/.

## Requirements

This library depends on boost (mainly boost::spirit), gmpxx and catch2 (for
testing only). A recent gcc compiler supporting C++17 features is required.

## Build and install

Build and installation can be achieved using CMake:

```
mkdir build && cd build
cmake ..
make
make install
```

Both gcc and clang compilers are supported.
Build is regularly tested on Linux Ubuntu Bionic with Continuous Integration.
