import funcpp;

#define NIL       typename nil::type
#define CONS(A,B) typename cons<A,B>::type

namespace test_basic_list_construction {
  using namespace funcpp;
  // Testing basic list construction and classification
  using L0 = NIL;
  using L1 = CONS(short, L0);
  using L2 = CONS(char, L1);
  static_assert(list_type<L0>);
  static_assert(list_type<L1>);
  static_assert(list_type<L2>);
  static_assert(not list_type<void>);
  static_assert(not list_type<int>);
  static_assert(not list_type<int[6]>);
} // namespace test_basic_list_construction

namespace test_list_construction {
  using namespace funcpp;
  using L4 = make_list<char, short, int, long>::type;
  static_assert(list_type<L4>);
  using Lx = CONS(char, CONS(short, CONS(int, CONS(long, NIL))));
  static_assert(list_type<Lx>);
  static_assert(refl<L4, Lx>::value);
} // namespace test_list_construction

namespace test_list_map {
  using namespace funcpp;
  template<typename> struct make_unsigned;
  template<> struct make_unsigned<char> { using type = unsigned char; };
  template<> struct make_unsigned<short> { using type = unsigned short; };
  template<> struct make_unsigned<int> { using type = unsigned int; };
  template<> struct make_unsigned<long> { using type = unsigned long; };
  template<> struct make_unsigned<long long> { using type = unsigned long long; };
  using Ls = make_list<
    char,
    short,
    int,
    long,
    long long>::type;
  static_assert(list_type<Ls>);
  using Lx = make_list<
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long>::type;
  static_assert(list_type<Lx>);
  using Lu = map<make_unsigned, Ls>::type;
  static_assert(list_type<Lu>);
  static_assert(refl<Lx, Lu>::value);
}

namespace test_list_count {
  using namespace funcpp;
  using L = make_list<char, short, int, long>::type;
  static_assert(count<L>::value == 4);
}

namespace test_list_join {
  using namespace funcpp;
  using L1 = make_list<char, short, int, long>::type;
  using L2 = make_list<float, double>::type;
  using L3 = make_list<void, void *>::type;
  using L = join1<make_list<L1, L2, L3>::type>::type;
  using Lx = make_list<char, short, int, long, float, double, void, void *>::type;
  static_assert(refl<Lx, L>::value);
}

int main() {
}
