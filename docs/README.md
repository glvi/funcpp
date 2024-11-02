# Functional C++ – Putting the "fun" back into C++

A project without an application, *Fun C++* is about achieving a
purely functional programming style with C++. There may be the
occasional foray into other areas like type theory, category theory,
or propositional logic. But primarily *Fun C++* is about the joy of
functional programming.

## Requirements

- A C++20 compiler

  Actually, you'll need LLVM clang 19 or later. Because I decided to
  go with C++ modules. The rationale was that I wanted to make certain
  helper symbols unavailable to the user, which I can do with C++
  modules, but cannot do when including C++ header files.

- GNU make

  Because I am an old-fashioned kind of guy.

- Doxygen (opt.)

  … if you want API docs

(end of file)
