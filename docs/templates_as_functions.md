# Templates are just functions

There is much ado about templates. They are allegedly difficult to
understand, admittedly difficult to debug, and the public opinion is
split whether mucking about with templates is proper programming, or
if it serves no practical purpose.

The template syntax of C++ is awkward to begin with. And the fact,
that C++ templates are always taught as an afterthought, makes them
feel grafted on to the language.

Programming with templates is usually called _metaprogramming_ as in
_what comes after programming?_

But I argue, that programming with C++ templates is not _meta_ but
just plain old functional programming. In fact, is is pure functional
programming in the sense that there are no side effects to function
application.

A template definition is just an awkward function definition, and
template specialisation is just awkward function application.

So, yes, templates are just awkward functions. But they are also
awesome. Because they add to the language the capability to apply
functions to types, and to have function that convert between types
and values.

Let's look at a concrete example
```
template <typename A> struct make_signed;
template <> struct make_signed<unsigned char>      { using type = signed char; };
template <> struct make_signed<unsigned short>     { using type = signed short; };
template <> struct make_signed<unsigned int>       { using type = signed int; };
template <> struct make_signed<unsigned long>      { using type = signed long; };
template <> struct make_signed<unsigned long long> { using type = signed long long; };
```

In general, the template is declared but not defined. So the compiler
would throw an error if the template were applied to, for example,
`float`

In special cases, that is when applying the template to an unsigned
integer type, the template is defined, and <q>returns</q> the
corresponding signed integer type.

If we cheat a little, and introduce a macro, then applying the
template just looks like applying a function.
```
#define MAKE_SIGNED(A) typename make_signed<A>::type
using signed_int = MAKE_SIGNED(unsigned int);
```

In plain old C++ we have functions that map values to value. With
templates, the language obtains the capability to map types to types,
values to types, and types to values.
```
template <typename A> struct type2type   { using type = /*…*/; };
template <     int a> struct value2type  { using type = /*…*/; };
template <typename A> struct type2value  { static constexpr auto const value = /*…*/; };
template <     int a> struct value2value { static constexpr auto const value = /*…*/; };
```
Compare to conventional functions
```
B    value2value(A a) { return /*…*/; }
auto value2value(A a) -> { return /*…*/; }
auto value2value = [](A a) -> { return /*…*/; };
```

Let's take a little detour into Type Theory.

In Type Theory, a value from a type is usally denoted like this
```
a : A
```
meaning `a` is a _member_ (or _inhabitant_) of type `A`. `A` can be
anything: `int`, `nat`, `real`, etc.

When talking about types, we are usually not interested in the members
of a type, but the type itself, and how it relates to other types. In
Type Theory, types are members (or inhabitants) of a (type) universe,
denoted as
```
A : U
```
where the `U` is usually represented in curly or italic typescript.

Note the analogy: a concrete value is to its type, what the type is to
its universe.
