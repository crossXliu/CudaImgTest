#include <iostream>
#include <type_traits>
#include <vector>
#include <tuple>

using std::cout;
using std::endl;

template <typename T> class CBase {
public:
  explicit CBase() {}
  virtual ~CBase() {}
  void print() { std::cout << "hello world" << std::endl; }

  template <T N> void print() { std::cout << "print val " << N << std::endl; }
};

enum Fmt {
    YUV = 0,
    RGB = 1,
};

template <Fmt ftype>
class FmtFactor {
public:
    constexpr static float val = 0;
};

template<>
class FmtFactor<YUV> {
public:
    constexpr static float val = 3.0F / 2.0F;
};

template<>
class FmtFactor<RGB> {
public:
    constexpr static float val = 3.0F;
};

constexpr Fmt g_ftype[]{YUV, YUV, RGB};

template <size_t idx = 0, std::enable_if_t<idx == sizeof(g_ftype) / sizeof(Fmt),
                                       void *> = nullptr>
void runCases(std::vector<float> &res) {}

template <size_t idx = 0, std::enable_if_t<idx < sizeof(g_ftype) / sizeof(Fmt), void*> = nullptr>
void runCases(std::vector<float>& res)
{
    res.push_back(FmtFactor<g_ftype[idx]>::val);
    runCases<idx + 1>(res);
}

int main(int argc, char *argv[]) {
  CBase<int> obj;
  obj.template print<200>();

  std::vector<float> res;
  runCases(res);

  for (auto val : res) {
    cout << val << endl;
  }
  return 0;
}
