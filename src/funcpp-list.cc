export module funcpp:list;

///@defgroup funcppList Lists

import :common;

namespace funcpp {

  ///@addtogroup funcppList
  ///@{

  /**
   * @brief Primitive for a unit type.
   *
   * A @ref _nil represents tautological information. It doesn't
   * represent the absence of information, but rather it's telling you
   * nothing that you don't already know.
   */
  struct _nil {};

  /**
   * @brief Primitive for a pair type
   *
   * @tparam A any type
   *
   * @tparam B any type
   *
   * A @ref _cons binds together two objects (types). The name _cons_
   * derives from _to construct_.
   *
   * See https://en.wikipedia.org/wiki/Cons
   */
  template <typename A, typename B> struct _cons {
    /**
     * @brief The first element of the `_cons`
     *
     * Traditionally called _car_ because it used to be the _contents_
     * of the _address_ part of some _register_ on the IBM 704.
     *
     * See https://en.wikipedia.org/wiki/CAR_and_CDR
     */
    using car = A;
    /**
     * @brief The second element of the `_cons`
     *
     * Traditionally called _cdr_ because it used to be the _contents_
     * of the _decrement_ part of some _register_ on the IBM 704.
     *
     * See https://en.wikipedia.org/wiki/CAR_and_CDR
     */
    using cdr = B;
  };

  ///@}
} // namespace funcpp

export namespace funcpp
{
  ///@addtogroup funcppList
  ///@{

  /**
   * @brief Checks if a type is a list
   *
   * @tparam A any type
   *
   * If `A` is a list, provides the member constant `value` equal to
   * `true`.  In all other case, `value` is false.
   */
  template<typename A>
  struct is_list : false_type {};

  /// @cond show_tmpl_spec
  template<>
  struct is_list<_nil> : true_type {};
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A, typename B>
  struct is_list<_cons<A,B>> : is_list<B> {};
  /// @endcond

  /**
   * @brief Specifies that a type is a list
   *
   * @tparam A any type
   *
   * The concept `list_type<A>` is satisfied,
   * - if `A` is `_nil`, or
   * - if `A` is `_cons<B,C>` and `C` is a list.
   */
  template <typename A>
  concept list_type = is_list<A>::value;

  /**
   * @brief Constructs an empty list
   *
   * Provides a member `type` that is an empty list.
   */
  struct nil {
    /// @brief An empty list
    using type = _nil;
  };

  /**
   * @brief Constructs a new list
   *
   * @tparam A any type
   *
   * @tparam B any list type
   *
   * Constructs a new list that comprises `A` followed by `B`.
   *
   * Provides a member `type` that holds the new list.
   */
  template<typename A, list_type B>
  struct cons {
    /// @brief The new list
    using type = _cons<A,B>;
  };

  /**
   * @brief Constructs a list from a parameter pack
   *
   * @tparam As any number of types
   *
   * **Example:**
   *
   *     using L = typename funcpp::make_list<char, short, int, long>::type;
   */
  template<typename...As>
  struct make_list;

  /// @cond show_tmpl_spec
  template<>
  struct make_list<> {
    using type = _nil;
    static_assert(list_type<type>);
  };
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A, typename...As>
  struct make_list<A, As...> {
    using type = _cons<A, typename make_list<As...>::type>;
    static_assert(list_type<type>);
  };
  /// @endcond

  /**
   * @brief Maps a function over an empty list
   *
   * @tparam F any type transformer
   *
   * Specialization of @ref map for empty lists.
   *
   * Provides a member `type` that is an empty list.
   */
  template<template<typename> typename F>
  struct map<F, _nil> {
    /// An empty list
    using type = _nil;
    static_assert(list_type<type>);
  };

  /**
   * @brief Maps a function over a non-empty list
   *
   * @tparam F any type transformer
   *
   * @tparam A any type
   *
   * @tparam B any list type
   *
   * Specialization of @ref map for non-empty lists.
   *
   * Provides a member `type` that is a non-empty list.
   *
   * The relationship of `type` to `F`, `A`, `B` is as follows.
   *
   *     using head = typename F<A>::type;
   *     using tail = typename map<F, B>::type;
   *     using type = _cons<head, tail>;
   *
   * where `head` and `tail` are introduced here for documentation
   * purposes. They are not members of @ref map<F,_cons<A,B>>.
   */
  template<template<typename> typename F, typename A, list_type B>
  struct map<F, _cons<A, B>> {
    /// A non-empty list
    using type = _cons<typename F<A>::type, typename map<F, B>::type>;
    static_assert(list_type<type>);
  };

  /**
   * @brief Reduces an empty list given some function `F`
   *
   * Provides a member `type` that reproduces `Acc`.
   */
  template<template<typename, typename> typename F, typename Acc>
  struct fold<F, Acc, _nil> {
    /// Reproduces the template argument `Acc`
    using type = Acc;
  };

  /**
   * @brief Reduces a non-empty list given some function `F`
   *
   * Provides a member `type` that reproduces the result of the
   * reduction.
   *
   * The relationship of `type` to `F`, `Acc`, `A`, `B` is as follows.
   *
   *     using acc_ = typename F<Acc,A>::type;
   *     using type = typename fold<F, acc2, B>::type
   *
   * where `acc_` is introduced here for documentation purposes. It is
   * not a member of @ref fold<F,Acc,_cons<A,B>>.
   */
  template<template<typename, typename> typename F, typename Acc, typename A, list_type B>
  struct fold<F, Acc, _cons<A, B>> {
    /// @brief The result of reducing with `F` over `A`
    using type = typename fold<F, typename F<Acc, A>::type, B>::type;
  };

  ///@}
} // namespace funcpp

namespace funcpp {
  ///@addtogroup funcppList
  ///@{

  /// @internal
  template<typename A, typename>
  struct _plus1 {
    /// @brief `1` added to `A`
    using type = constant<typename A::value_type, 1 + A::value>;
  };

  ///@}
}

export namespace funcpp
{
  ///@addtogroup funcppList
  ///@{
  /**
   * @brief Counts the elements of a list
   *
   * @tparam A any list type
   */
  template<list_type A>
  struct count<A> {
    /// @brief Number of elements in `A`
    static constexpr auto const value =
      fold<_plus1, zero_type<unsigned long long>, A>::type::value;
  };

  /**
   * @brief Flattens a list of list
   *
   * @tparam A any list type
   *
   * Removes one level of nesting from a list of lists.
   *
   * For example, joining the list `<<A,B>,<C,D>>` will result in the
   * flattened list `<A,B,C,D>`.
   */
  template<list_type A>
  struct join1<A>;

  /// @cond show_tmpl_spec
  template<>
  struct join1<_nil> {
    using type = _nil;
    static_assert(list_type<type>);
  };
  /// @endcond

  /// @cond show_tmpl_spec
  template<list_type B>
  struct join1<_cons<_nil, B>> {
    using type = typename join1<B>::type;
    static_assert(list_type<type>);
  };
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A, list_type B, list_type C>
  struct join1<_cons<_cons<A, B>, C>> {
    using type = _cons<A, typename join1<_cons<B, C>>::type>;
    static_assert(list_type<type>);
  };
  /// @endcond

  /**
   * @brief Inserts a type into a monad
   */
  template<typename A>
  struct pure {
    /// `A` inserted into the monad
    using type = typename cons<A, _nil>::type;
    static_assert(list_type<type>);
  };

  /**
   * @brief Binds monadic operation to a monad
   */
  template<list_type MA, template<typename> typename F>
  struct bind {
    /// `F` bound to `MA`
    using type = typename join1<typename map<F, MA>::type>::type;
    static_assert(list_type<type>);
  };
  ///@}
} // namespace funcpp
// End of file
