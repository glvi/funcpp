# Sums of types

A sum type `Σ` is an imaginary type that is formed by taking two or
more distinct types, and declaring that `Σ` is either one of those.

A sum type is an _imaginary_ type, because it exists only in the
imagination of the programmer, not in the compiler.

In type notation, given types `A` and `B`, the sum of those types is
denoted as
```
Σ ≡ A + B
```
A value of type `Σ` is then _either_ a value of type `A` _or_ a value
of type `B`.

Check out the definition of a sum type in _Haskell_, as an example for
a pure functional programming language.
```haskell
data Σ a b = A a | B b
```

Check out the definition of a sum type in _Swift_, as an example a
language that is more alike to C++.
```swift
enum Σ {
  case a(A)
  case b(B)
}
```

C++ has no inherent means to denote a sum type. There is a workaround
in the form of `std::variant` in the C++ standard library. But at the
language level itself, it is not possible to define a sum type.
