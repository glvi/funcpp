# Sums of types

A sum type `Σ` is an imaginary type that is formed by taking two or
more distinct types, and declaring that `Σ` is either one of those.

It is called an _imaginary_ type, as opposed to a _real_ type, because
it exists only in the imagination of the programmer, it is not
accessible to the compiler.

C++ has no inherent means to denote a sum type. Technically, there is `union` as in
```c++
union Σ {
  A a;
  B b;
};
```
However, from a given value of type `Σ` the information if it holds `A` or `B` can
not be recovered. Let's call this a _forgetful_ union.

A forgetful union would require to externally keep track of which
case of the type `Σ` is actually in use.
```c++
union Σ {
  A a;
  B b;
};
Σ σ = /* … */;
switch (/*what?*/) {
  case /*what?*/: return σ.a;
  case /*what?*/: return σ.b;
}
```

Compare this to the definition of a sum type in _Swift_, which
automatically keeps track under the hood which case is active.
```swift
enum Σ {
  case a(A)
  case b(B)
}
let σ : Σ = // …
switch σ {
  case .a(let a): return a
  case .b(let b): return b
}
```

Since the C++ union is forgetful, and additional code would be
required to make it an actual sum type, it is not being considered a
proper implementation of a sum type. The additional code is available
in the C++ standard library as `std::variant`. But just at the
language level, sum types are imaginery.

## Type theory

In type notation, given types `A` and `B`, the sum of those types is
denoted as
```
Σ ≡ A + B
```
A value of type `Σ` is then _either_ a value of type `A` _or_ a value
of type `B`.

Note that this is a disjoint union. There may be no `a:A` that is also
in `B`, and there may be no `b:B` that is also in `A`.

Especially, there is no `Σ ≡ A + A` because `A + A ≡ A`, and `Σ ≡ A` is
not a sum but a type alias.

Even the programming languages that provide sum types don't get this right, but on purpose. A compiler would have to check if types `A` and `B` have common values, and then reject their sum. But the compiler may not know the actual values of either type, so couldn't check it. In general, programming languages will forsake this feature, and allow constructs like the following
```haskell
data Σ a = A a | B a
```
```swift
enum Σ {
  case a(A)
  case b(A)
}
```
Those are called _improper_ sum types, as opposed to _proper_ sum
types where `A` and `B` are disjoint.
