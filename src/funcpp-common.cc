export module funcpp:common;

///@defgroup funcppCommon Concepts, types, and constants common to all partitions

export namespace funcpp
{
  ///@addtogroup funcppCommon
  ///@{

  /**
   * @brief The constant value `a : A`.
   *
   * @tparam A any type
   *
   * @tparam a any value of type `A`
   *
   * Reproduces the template arguments `A` and `a` in the static
   * members `value_type` and `value` respectively.
   */
  template<typename A, A a>
  struct constant {
    /// @brief Reproduces the template argument `A`
    using value_type = A;
    /// @brief Reproduces the template argument `a`
    static constexpr value_type const value = a;
  };

  /**
   * @brief The constant value `a : bool`.
   *
   * @tparam a any value of type `bool`
   */
  template<bool a>
  struct bool_constant : constant<bool, a> {};

  /**
   * @brief The constant value `false : bool`.
   *
   * **Example:**
   *
   *     static_assert(not funcpp::false_type::value);
   */
  struct false_type : bool_constant<false> {};

  /**
   * @brief The constant value `true : bool`.
   *
   * **Example:**
   *
   *     static_assert(funcpp::true_type::value);
   */
  struct true_type : bool_constant<true> {};

  /**
   * @brief Checks if the type `A` is constant `0`.
   *
   * @tparam A any type
   */
  template<typename A>
  struct is_zero : constant<bool, A::value == 0> {};

  /**
   * @brief The constant value `0 : A`.
   *
   * @tparam A any type
   *
   * **Example:**
   *
   *     static_assert(funcpp::zero::value == 0);
   */
  template<typename A>
  struct zero_type : constant<A, 0> {};

  /// @cond show_tmpl_spec
  /**
   * @brief Always true.
   *
   * Specialisation for `zero<A>`
   */
  template<typename A>
  struct is_zero<zero_type<A>> : constant<bool, true> {};
  /// @endcond

  /**
   * @brief The constant value `1 : A`.
   *
   * @tparam A any type
   *
   * **Example:**
   *
   *     static_assert(funcpp::one::value == 1);
   */
  template<typename A>
  struct one_type : constant<A, 1> {};

  /**
   * @brief Checks it two types are the same.
   *
   * @tparam A any type
   *
   * @tparam B any type
   *
   * Binary type predicate that is `false` in general, but `true` when
   * both arguments are the same type.
   *
   * **Example:**
   *
   *     static_assert(funcpp::refl<int, int>::value);
   *     static_assert(not funcpp::refl<int, long>::value);
   */
  template<typename A, typename B>
  struct refl;

  /// @cond show_tmpl_spec
  template<typename A, typename B>
  struct refl : false_type {};
  /// @endcond

  /// @cond show_tmpl_spec
  template<typename A>
  struct refl<A, A> : true_type {};
  /// @endcond

  /**
   * @brief Maps a function `F` over a type `A`.
   *
   * @tparam F any unary type transformer
   *
   * @tparam A any type
   *
   * In general, this is just `F<A>::type`.
   *
   * May be specialised for certain types, for example collections.
   */
  template<template<typename> typename F, typename A>
  struct map {
    /// @brief `F` applied to `A`.
    using type = typename F<A>::type;
  };

  /**
   * @brief Reduces a type `A` given some function `F`.
   *
   * @tparam F any binary type transformer
   *
   * @tparam I any type
   *
   * @tparam A any type
   *
   * In general, this is just `F<I,A>::type`.
   *
   * May be specialised for certain types, for example collections.
   */
  template<template<typename, typename> typename F, typename I, typename A>
  struct fold {
    /// @brief `F` applied to `I` and `A`.
    using type = typename F<I,A>::type;
  };

  /**
   * @brief Counts a type `A`.
   *
   * @tparam A any type
   *
   * In general, this is undefined.
   *
   * May be specialised for certain types, for example collections.
   *
   * Specializations shall provide a static member `value : unsigned long long`.
   */
  template<typename A>
  struct count;

  /**
   * @brief Removes one level of nesting from a nested type.
   *
   * @tparam A any type
   *
   * In general, this is undefined.
   *
   * May be specialised for certain types, for example collections.
   */
  template<typename>
  struct join1;

///@}
} // namespace funcpp
// End of file
