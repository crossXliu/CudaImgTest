#include <iostream>
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
  constexpr CudaCase(Fmt inFmt, int inputWidth, int inputHeight,
                     const char *inputPath, Fmt outFmt, int outputWidth,
                     int outputHeight, const char *outputPath)
      : inputFmt(inFmt), inputWidth(inputWidth), inputHeight(inputHeight), inputPath(inputPath),
      outputFmt(outFmt), outputWidth(outputWidth), outputHeight(outputHeight), outputPath(outputPath) {}
  enum Fmt inputFmt;
  int inputWidth;
  int inputHeight;
  enum Fmt outputFmt;
  int outputWidth;
  int outputHeight;
  const char *inputPath;
  const char *outputPath;
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
    return std::make_tuple(t);
}

template <typename TupleHead, typename... Tuple>
constexpr auto describeCases(TupleHead h, Tuple... t)
{
    return std::tuple_cat(std::make_tuple(h), describeCases<Tuple...>(t...));
}

template <typename TCaseContainer, size_t idx = 0, std::enable_if_t<idx == std::tuple_size<decltype(TCaseContainer::describeInstance())>::value,
                                       void *> = nullptr>
void runCases(std::vector<float> &res) {}

template <typename TCaseContainer, size_t idx = 0, std::enable_if_t<idx < std::tuple_size<decltype(TCaseContainer::describeInstance())>::value, void*> = nullptr>
void runCases(std::vector<float>& res)
{
    auto fmtFactorIn = FmtFactor<std::get<idx>(TCaseContainer::describeInstance()).inputFmt>::val;
    auto inputPath = std::get<idx>(TCaseContainer::describeInstance()).inputPath;
    auto inputWidth = std::get<idx>(TCaseContainer::describeInstance()).inputWidth;
    auto inputHeight = std::get<idx>(TCaseContainer::describeInstance()).inputHeight;

    auto fmtFactorOut = FmtFactor<std::get<idx>(TCaseContainer::describeInstance()).outputFmt>::val;
    auto outputPath = std::get<idx>(TCaseContainer::describeInstance()).outputPath;
    auto outputWidth = std::get<idx>(TCaseContainer::describeInstance()).outputWidth;
    auto outputHeight = std::get<idx>(TCaseContainer::describeInstance()).outputHeight;

    // the main function is here
    res.push_back(fmtFactorIn);

    // run next case recursively
    runCases<TCaseContainer, idx + 1>(res);
}

class CasesContainer {
public:
    constexpr static auto describeInstance() {
        return describeCases(CudaCase(YUV, 1920, 1080, "/home/moa/.cfg", YUV, 1920, 1080, ""));
    }
};

class CasesContainerLDC {
public:
  constexpr static auto describeInstance() {
    return describeCases(
        CudaCase(YUV, 1920, 1080, "/home/moa/.cfg", YUV, 1920, 1080, ""),
        CudaCase(RGB, 1920, 1080, "/home/moa/.cfg", YUV, 1920, 1080, ""),
        CudaCase(YUV, 1920, 1080, "/home/moa/.cfg", RGB, 1920, 1080, ""));
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
