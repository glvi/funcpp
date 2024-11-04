# Sums of types

## Type theory

In type theory, a sum of types `S ≡ A+B` is a type that is formed from
two or more given types. Values in `S` are then introduced by taking
them from _either_ `A` _or_ `B`. Type theory usually adds some record
keeping (`ina`, `inb`) to remember which alternative was used.

```
S ≡ A + B               // formation
a:A ⇒ (ina a):S         // introduction 1/2
b:B ⇒ (inb b):S         // introduction 2/2
s:S ∧ s = (ina a) ⇒ a:A // elimination 1/2
s:S ∧ s = (inb a) ⇒ b:B // elimination 2/2
```

Sums of types may involve more than just two alternatives. In general,
a sum is denoted as
```
Σn Ai ≡ A1 + A2 + … + An
```
with
```
Σ2 ≡ A + B // by convention
Σ1 ≡ A     // by convention
Σ0 ≡ void  // type without values
```

Recursive formation is allowed; as long as at least one of the
alternatives is free from recursion.
```
S ≡ A + B + S // recursive formation
```

In order to use `S` as the domain of a function `f`, said function
must be defined for the domains of both `A` _and_ `B`. Function
application is delegated to the appropriate domain. The co-domains of
`f(A)` and `f(B)` may differ.
```
let  f : A → X
and  f : B → Y
then f : A + B → X + Y
         ina a ↦ f(a)
         inb b ↦ f(b)
```

Recursive function definitions are allowed; as long as at least
one of the alternatives is free from recursion.
```
f : A + B + S → X + Y
    ina a ↦ f(a)
    inb b ↦ f(b)
    ins s ↦ f(s) // recursion
```

## C++

In C++ a sum type is an _imaginary_ type, as opposed to a _real_ type,
in the sense that it exists only in the imagination of the programmer.
It is not accessible to the compiler.

In principle, there is `union` as in
```c++
union S {
  A a;
  B b;
};
```

But `union` is _forgetful_ in the sense that it does not remember whether
it holds `A` or `B`. The information is contained in the union; but
without outside help, it cannot be recovered.
```c++
enum class D {_a, _b} d;
union S {A a; B b;};
S s = /* an A or a B */; // these two assignments
D d = /*   _a or  _b */; // should be atomic
switch (d) {
  case D::_a: return s.a;
  case D::_b: return s.b;
}
```

Since the C++ union is forgetful, and additional code would be
required to make it an actual sum type, it is not being considered a
proper implementation of a sum type. The C++ standard library has an
implementation of a sum type in `std::variant`.  But at the language
level, sum types are imaginery, not real.

## Practical use

Without proper representation of sum types, it falls to the programmer
to manage sums of types. Usually the alternatives of a sum types are
declared and defined together, and very probably refer to each
other. Compare maybes, lists, or trees:

```c++
// maybe = _nothing | _some<A>
struct _nothing {
};

template <typename A>
struct _some {
  A value;
};
```

```c++
// list = _nil | _cons<A, list>
struct _nil {
};

template <typename A, typename B>
struct _cons {
  A a;
  B b;
};
```

```c++
// tree = _leaf | _node<A, tree, tree>
struct _leaf {
};

template <typename A, typename B, typename C>
struct _node {
    A a;
    B b;
    C c;
};
```

Even though C++ has no explicit means of declaring sum types, sums
types occur implicitly in C++ in the form of function overloading. See
the example of a counting function below.

```c++
unsigned count(_nothing&&) {
  return 0;
}

template <typename A>
unsigned count(_some<A>&&) {
  return 1;
}
```

```c++
unsigned count(_nil&&) {
  return 0;
}

template <typename A, typename B>
unsigned count(_cons<A, B>&& cons)
{
  return 1 + count(cons.b);
}
```

```c++
unsigned count(_leaf&&)
{
  return 0;
}

template <typename A, typename B, typename C>
unsigned count(_node<A,B,C>&& node)
{
  return 1 + count(node.b) + count(node.c);
}
```

Classifier types and concepts can be used to improve declarations.

```c++
template <typename>
struct is_list {
  static constexpr bool const value = false;
};

template <>
struct is_list<_nil> {
  static constexpr bool const value = true;
};

template <typename A>
struct is_list<_cons<A, B>> {
  static constexpr bool const value = is_list<B>::value;
};

template <typename A>
concept list_type = is_list<A>::value;

template <typename A, list_type B>
unsigned count(_cons<A, B>&& cons)
{
  return 1 + count(cons.b);
}
```

```c++
template <typename>
struct is_tree {
  static constexpr bool const value = false;
};

template <>
struct is_tree<_leaf> {
  static constexpr bool const value = true;
};

template <typename A, typename B, typename C>
struct is_tree<_node<A, B, C>> {
  static constexpr bool const value = is_tree<B>::value && is_tree<C>::value;
};

template <typename A>
concept tree_type = is_tree<A>::value;

template <typename A, tree_type B, tree_type C>
unsigned count(_node<A, B, C>&& node)
{
  return 1 + count(node.b) + count(node.c);
}
```

But there can never be concise declarations like these
```c++
unsigned count(maybe&&);
unsigned count(list&&);
unsigned count(tree&&);
```
because those types are imaginary, and not known to the compiler.
