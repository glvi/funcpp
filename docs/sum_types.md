# Sums of types

## Type theory

In type theory, given two types `A` and `B`, a _sum type_ `S` is
formed as `S ≡ A+B`.  Values in `S` are introduced by taking them from
_either_ `A` _or_ `B`. Some record keeping (`ina`, `inb`) is added to
remember which alternative was used.

```
S ≡ A + B               // formation
a:A ⇒ (ina a):S         // introduction 1/2
b:B ⇒ (inb b):S         // introduction 2/2
s:S ∧ s = (ina a) ⇒ a:A // elimination 1/2
s:S ∧ s = (inb a) ⇒ b:B // elimination 2/2
```

Sum types may involve more than just two alternatives. In general, a
sum type is denoted as follows.

```
Σn Ai ≡ A1 + A2 + … + An
```

With special cases denoted as follows.

```
Σ1 A ≡ A     // type A itself
Σ0   ≡ void  // type without values
```

## C++

In C++ sum types occur _naturally_ in function overloading. Let's look
at a function from the standard C++ library: `abs`.

`abs` has multiple overloaded declarations (just listing two here).

```c++
int  abs (int);
long abs (long);
```

All `abs` functions perform the same operation: return the absolute
value of the argument. `abs` takes as argument either an `int` or a
`long`. Therefore, the type of the argument of the function `abs` is
effectively, the sum type `int + long`.

```c++
// obviously not real C++
// @return the absolute value of the argument
auto abs(<int + long> arg) -> <int + long>;
```

## Type theory

Sum types are called _sum_ types, because of how their cardinality
adds up. If `A` has two values (say `false` and `true`), and `B` has three values
(say `red`, `green`, and `blue`) then `A+B` has five values: (`false`,
`true`, `red`, `green`, and `blue`).

Compare this to the product type `A×B` which has six (2×3) values:
(`(false, red)`, `(false, green)`, `(false, blue)`, `(true, red)`,
`(true, green)`, `(true, blue)`).

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

Even though imaginary, C++ handles sum types just fine with one small caveat.
In C++ variables have a type, one type, not two, nor more.
Therefore, to define a variable of a sum type, sum special treatment is needed.

The following code snippet is not valid C++, becauce variables can't be defined having a concept as type.
```c++
maybe m; // not valid
list  l; // not valid
tree  t; // not valid
```

In the following code snippet, variable definitions are valid, but also determined.
Once `m` is defined as `_nothing`; it can never be a `_cons`; and vice versa.
Similar holds for `l` and `t`.
```c++
_nothing m;
_nil     l;
_leaf    t;
```

We have to resort to C++ unions.
```c++
template<typename A>
struct maybe_var {
  enum { is_nothing, is_some } _d;
  union { _nothing _n; _some<A> _s; };
  maybe_var() : _d{is_nothing} {}
  maybe_var(A a) : _d{is_some}, _s{a} {}
  operator bool() { return _d == is_some; }
};
maybe_var<int> m0;
maybe_var<int> m1 = 7;
```

And functions that shall accept such a C++ union as argument, require some consideration.
```c++
template<typename R, typename A>
struct FunctionOnMaybe {
  R operator()(_nothing) { /* … */ }
  R operator()(_some<A> s) { /* … */ }
};
template<typename R, typename A>
R someFunction(maybe_var<A> m) {
  FunctionOnMaybe<R,A> f;
  return m ? f(m._s) : f(m._n);
}
```
