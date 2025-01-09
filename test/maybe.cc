import funcpp;

using namespace funcpp;

template<typename A>
class maybe_var {
  using n_type = typename nothing::type;
  using s_type = typename some<A>::type;
  enum {is_n, is_s} d = is_n;
  union {n_type n; s_type s;};

public:
  maybe_var() : d{is_n} {
  }

  maybe_var(A a) : d{is_s}, s{a} {
  }

  ~maybe_var() {
    if (d == is_s) s.~s_type();
  }

  operator bool() {
    return d == is_s;
  }

  friend bool operator==(maybe_var m, maybe_var n) {
    if (m.d == is_n) return n.d == is_n;
    return n.d == is_s and m.s == n.s;
  }

  friend bool operator==(maybe_var m, A a) {
    return m.d == is_s and m.s == a;
  }

  friend bool operator==(A a, maybe_var m) {
    return m.d == is_s and a == m.s;
  }

  template<template<typename> typename F, typename... Args>
  friend auto invoke(F<A> f, maybe_var m, Args&&...args) {
    return m ? f(m.s, args...) : f(m.n, args...);
  }
};

template<typename A>
struct maybe_is7 {
  bool operator()(nothing::type) { return false; }
  bool operator()(some<A>::type s) { return s == 7; }
};

template<typename A>
bool is7(maybe_var<A> m) {
  return invoke(maybe_is7<A>{}, m);
}

int main() {
  maybe_var<int> none;
  maybe_var<int> some = 0;
  int err = 0;
  ++err; if (none) return err;
  ++err; if (!some) return err;
  ++err; if (some != 0) return err;
  some = 7;
  ++err; if (some != 7) return err;
  ++err; if (!is7(some)) return err;
  ++err; if (some == none) return err;
  some = {};
  ++err; if (some != none) return err;
  return 0;
}
