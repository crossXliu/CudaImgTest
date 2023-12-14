#include <iostream>
#include <string_view>
#include <type_traits>
#include <vector>
#include <tuple>

using std::cout;
using std::endl;

enum Fmt {
    YUV = 0,
    RGB = 1,
};

struct CudaCase {
    constexpr CudaCase(Fmt iFmt, int w, int h, std::string_view inSv) : inputFmt(iFmt), w(w), h(h), inputPath(inSv) {}
    enum Fmt inputFmt;
    int w;
    int h;
    std::string_view inputPath;
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

constexpr auto genCases(Fmt f) {
    return std::make_tuple(f);
}

template <typename Tuple>
constexpr auto describeCases(Tuple t)
{
    return t;
}

template <typename TupleHead, typename... Tuple>
constexpr auto describeCases(TupleHead h, Tuple... t)
{
    return std::tuple_cat(std::make_tuple(h), std::make_tuple(describeCases<Tuple...>(t...)));
}

template <typename TCaseContainer, size_t idx = 0, std::enable_if_t<idx == std::tuple_size_v<decltype(TCaseContainer::describeInstance())>,
                                       void *> = nullptr>
void runCases(std::vector<float> &res) {}

template <typename TCaseContainer, size_t idx = 0, std::enable_if_t<idx < std::tuple_size_v<decltype(TCaseContainer::describeInstance())>, void*> = nullptr>
void runCases(std::vector<float>& res)
{
    auto fmtFactorIn = FmtFactor<std::get<idx>(TCaseContainer::describeInstance()).inputFmt>::val;
    auto inputPath = std::get<idx>(TCaseContainer::describeInstance()).inputPath;
    auto w = std::get<idx>(TCaseContainer::describeInstance()).w;
    auto h = std::get<idx>(TCaseContainer::describeInstance()).h;

    // the main function is here
    res.push_back(fmtFactorIn);

    // run next case recursively
    runCases<TCaseContainer, idx + 1>(res);
}

class CasesContainer {
public:
    constexpr static auto describeInstance() {
        return describeCases(CudaCase(YUV, 1920, 1080, "/home/moa/.cfg"),
                CudaCase(RGB, 1920, 1080, "/home/moa/.cfg"));
    }
};

class CasesContainerLDC {
public:
    constexpr static auto describeInstance() {
        return describeCases(CudaCase(RGB, 1920, 1080, "/home/moa/.cfg"),
                CudaCase(RGB, 1920, 1080, "/home/moa/.cfg"));
    }
};

int main(int argc, char *argv[]) {
  std::vector<float> res;
  runCases<CasesContainer>(res);
  runCases<CasesContainerLDC>(res);

  for (auto val : res) {
    cout << val << endl;
  }
  return 0;
}
