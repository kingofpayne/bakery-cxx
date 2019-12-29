[![Documentation Status](https://readthedocs.org/projects/libbakery/badge/?version=latest)](https://libbakery.readthedocs.io/en/latest/?badge=latest)

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

## Requirements

This library relies on boost (mainly boost::spirit) and gmpxx.

## Building

Building can be achieved using SCons. There is a very simple and poor sconstruct
script which may be improved in the future.
