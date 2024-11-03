export module funcpp:maybe;

///@defgroup funcppMaybe Optional values

import :common;

namespace funcpp {

  ///@addtogroup funcppMaybe
  ///@{

  /// @cond show_internal
  /**
   * @brief Optional type that holds nothing.
   */
  struct _nothing {};
  /// @endcond

  /// @cond show_internal
  /**
   * @brief Optional type that holds something
   *
   * @tparam A any type
   */
  template <typename A>
  struct _some {
    /// @brief Reproduces `A`
    using type = A;
  };
  /// @endcond

  ///@}
} // namespace funcpp

export namespace funcpp {

  ///@addtogroup funcppMaybe
  ///@{

  /**
   * @brief Checks if a type is an optional type.
   *
   * @tparam A any type
   *
   * If `A` is a optional type, provides the member constant `value`
   * equal to `true`.  In all other case, `value` is false.
   */
  template<typename A>
  struct is_maybe : false_type {};

  /// @cond show_tmpl_spec
  template<>
  struct is_maybe<_nothing> : true_type {};
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A>
  struct is_maybe<_some<A>> : true_type {};
  /// @endcond

  /**
   * @brief Specifies that a type is an optional type
   *
   * @tparam A any type
   *
   * The concept `maybe_type<A>` is satisfied,
   * - if `A` is `_nothing`, or
   * - if `A` is `_some<B>` for any `B`.
   */
  template <typename A>
  concept maybe_type = is_maybe<A>::value;

  /**
   * @brief Optional type that holds nothing.
   *
   * Provides a member `type` that holds nothing
   */
  struct nothing {
    /// @brief Holds nothing
    using type = _nothing;
  };

  /**
   * @brief Optional type that holds something
   *
   * @tparam A any type
   *
   * Provides a member `type` that holds something
   */
  template <typename A>
  struct some {
    /// @brief Holds `A`
    using type = _some<A>;
  };

  /**
   * @brief Maps a function over an optional type
   *
   * @tparam F any type transformer
   *
   * Specialization of @ref map for nothing.
   *
   * Provides a member `type` that holds nothing.
   */
  template<template<typename> typename F>
  struct map<F, _nothing> {
    /// Nothing
    using type = _nothing;
    static_assert(maybe_type<type>);
  };

  /**
   * @brief Maps a function over an optional type
   *
   * @tparam F any type transformer
   *
   * @tparam A any type
   *
   * Specialization of @ref map for something.
   *
   * Provides a member `type` that holds something.
   *
   * The relationship of `type` to `F`, `A` is as follows.
   *
   *     using thing = typename F<A>::type;
   *     using type  = _some<thing>;
   *
   * where `thing` is introduced here for documentation purposes. It
   * is not a member of @ref map<F,_some<A>>.
   */
  template<template<typename> typename F, typename A>
  struct map<F, _some<A>> {
    /// A non-empty list
    using type = _some<typename F<A>::type>;
    static_assert(maybe_type<type>);
  };

  /**
   * @brief Reduces an optional type given some function `F`
   *
   * Provides a member `type` that reproduces `Acc`.
   */
  template<template<typename, typename> typename F, typename Acc>
  struct fold<F, Acc, _nothing> {
    /// Reproduces the template argument `Acc`
    using type = Acc;
  };

  /**
   * @brief Reduces an optional type given some function `F`
   *
   * Provides a member `type` that reproduces the result of the
   * reduction.
   *
   * The relationship of `type` to `F`, `Acc`, `A` is as follows.
   *
   *     using type = typename F<Acc,A>::type;
   */
  template<template<typename, typename> typename F, typename Acc, typename A>
  struct fold<F, Acc, _some<A>> {
    /// @brief The result of reducing with `F` over `A`
    using type = typename F<Acc, A>::type;
  };

  /**
   * @brief Counts an optional type
   *
   * @tparam A any type
   *
   * Counts `0` or `1`.
   */
  template<maybe_type A>
  struct count<A>;

  /**
   * @brief Counts an optional type
   *
   * Counts `0`.
   */
  template<>
  struct count<_nothing> : zero_type<unsigned long long> {};

  /**
   * @brief Counts an optional type
   *
   * @tparam A any type
   *
   * Counts `1`.
   */
  template<typename A>
  struct count<_some<A>> : one_type<unsigned long long> {};

  /**
   * @brief Flattens an optional type holding some optional type
   *
   * @tparam A any optional type
   *
   * Removes one level of nesting from an optional holding some optional.
   *
   * For example, joining the optional `some<some<A>>` will result in
   * the flattened optional `some<A>`.
   */
  template<maybe_type A>
  struct join1<A>;

  /// @cond show_tmpl_spec
  template<>
  struct join1<_nothing> {
    using type = _nothing;
    static_assert(maybe_type<type>);
  };
  /// @endcond

  /// @cond show_tmpl_spec
  template<>
  struct join1<_some<_nothing>> {
    using type = _nothing;
    static_assert(maybe_type<type>);
  };
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A>
  struct join1<_some<_some<A>>> {
    using type = _some<A>;
    static_assert(maybe_type<type>);
  };
  /// @endcond

  /**
   * @brief Inserts a type into a monad
   */
  template<typename A>
  struct pure {
    /// `A` inserted into the monad
    using type = typename some<A>::type;
    static_assert(maybe_type<type>);
  };

  /**
   * @brief Binds monadic operation to a monad
   */
  template<maybe_type MA, template<typename> typename F>
  struct bind;

  /**
   * @brief Binds monadic operation to a monad
   */
  template<template<typename> typename F>
  struct bind<_nothing, F> {
    /// `F` bound to `_nothing`
    using type = _nothing;
    static_assert(maybe_type<type>);
  };

  /**
   * @brief Binds monadic operation to a monad
   */
  template<typename A, template<typename> typename F>
  struct bind<_some<A>, F> {
    /// `F` bound to `_some<A>`
    using type = typename F<A>::type;
    static_assert(maybe_type<type>);
  };

  ///@}
} // namespace funcpp

// End of file
