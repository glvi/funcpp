# Maybe - Optional values

`maybe` may be the simplest monadic type (pardon the pun). In C++
there is `std::optional` which serves this role. At the level of pure
functional programming, there is no equivalent.

Let's introduce the `maybe` <q>type</q>.

```
// maybe<A> ::= _nothing | _some<A>

struct _nothing {};

template <typename A>
struct _some { using type = A; };
```

In order to support easy use of the `maybe` type, let's provide some
classification.

```
template <typename>
struct is_maybe : false_type {};

template <>
struct is_maybe<_nothing> : true_type {};

template <typename A>
struct is_maybe<_some<A>> : true_type {};

template <typename A>
concept maybe_type = is_maybe<A>::value;
```

The classifier `is_maybe` is generally `false`; but for the
specialisations `is_maybe<_nothing>` and `is_maybe<_some<A>>` it is
`true`.

(end of file)
