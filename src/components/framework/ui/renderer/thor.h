#ifndef _THORVG_H_
#define _THORVG_H_

#include <cstdint>
#include <functional>
#include <list>
#include <cstdarg>

#include "src/framework/thread/currentthread.h"
#include "src/qor/reference/newref.h"
#include "src/components/framework/ui/opengles/opengles.h"

#ifdef TVG_API
    #undef TVG_API
#endif

#ifndef TVG_STATIC
    #ifdef _WIN32
        #if QOR_RENDERER
            #define TVG_API __declspec(dllexport)
        #else
            #define TVG_API __declspec(dllimport)
        #endif
    #elif (defined(__SUNPRO_C)  || defined(__SUNPRO_CC))
        #define TVG_API __global
    #else
        #if (defined(__GNUC__) && __GNUC__ >= 4) || defined(__INTEL_COMPILER)
            #define TVG_API __attribute__ ((visibility("default")))
        #else
            #define TVG_API
        #endif
    #endif
#else
    #define TVG_API
#endif

#ifdef TVG_DEPRECATED
    #undef TVG_DEPRECATED
#endif

#ifdef _WIN32
    #define TVG_DEPRECATED __declspec(deprecated)
#elif __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
    #define TVG_DEPRECATED __attribute__ ((__deprecated__))
#else
    #define TVG_DEPRECATED
#endif

#define _TVG_DECLARE_PRIVATE(A) \
protected: \
    A(const A&) = delete; \
    const A& operator=(const A&) = delete; \
    A()

#define _TVG_DECLARE_PRIVATE_BASE(A) \
    _TVG_DECLARE_PRIVATE(A); \
public: \
    struct Impl; \
    Impl* pImpl

#define _TVG_DECLARE_PRIVATE_DERIVE(A) \
    _TVG_DECLARE_PRIVATE(A); \
protected: \
    ~A() {}

#define _TVG_DISABLE_CTOR(A) \
    A() = delete; \
    ~A() = delete

#define _TVG_DECLARE_ACCESSOR(A) \
    friend A

namespace qor{ namespace components{ namespace ui{ namespace renderer{

struct RenderMethod;
struct Animation;
struct Shape;

/**
 * @brief Enumeration specifying the result from the APIs.
 *
 * All ThorVG APIs could potentially return one of the values in the list.
 * Please note that some APIs may additionally specify the reasons that trigger their return values.
 *
 */
enum struct Result
{
    Success = 0,           ///< The value returned in case of a correct request execution.
    InvalidArguments,      ///< The value returned in the event of a problem with the arguments given to the API - e.g. empty paths or null pointers.
    InsufficientCondition, ///< The value returned in case the request cannot be processed - e.g. asking for properties of an object, which does not exist.
    FailedAllocation,      ///< The value returned in case of unsuccessful memory allocation.
    MemoryCorruption,      ///< The value returned in the event of bad memory handling - e.g. failing in pointer releasing or casting
    NonSupport,            ///< The value returned in case of choosing unsupported engine features(options).
    Unknown = 255          ///< The value returned in all other cases.
};


/**
 * @brief Enumeration specifying the methods of combining the 8-bit color channels into 32-bit color.
 */
enum struct ColorSpace : uint8_t
{
    ABGR8888 = 0,      ///< The channels are joined in the order: alpha, blue, green, red. Colors are alpha-premultiplied.
    ARGB8888,          ///< The channels are joined in the order: alpha, red, green, blue. Colors are alpha-premultiplied.
    ABGR8888S,         ///< The channels are joined in the order: alpha, blue, green, red. Colors are un-alpha-premultiplied. @since 0.12
    ARGB8888S,         ///< The channels are joined in the order: alpha, red, green, blue. Colors are un-alpha-premultiplied. @since 0.12
    Grayscale8,        ///< One single channel data.
    Unknown = 255      ///< Unknown channel data. This is reserved for an initial ColorSpace value. @since 1.0
};


/**
 * @brief Enumeration to specify rendering engine behavior.
 *
 * @note The availability or behavior of @c SmartRender may vary depending on platform or backend support.
 *       It attempts to optimize rendering performance by updating only the regions  of the canvas that have
 *       changed between frames (partial redraw). This can be highly effective in scenarios  where most of the
 *       canvas remains static and only small portions are updated—such as simple animations or GUI interactions.
 *       However, in complex scenes where a large portion of the canvas changes frequently (e.g., full-screen animations
 *       or heavy object movements), the overhead of tracking changes and managing update regions may outweigh the benefits,
 *       resulting in decreased performance compared to the default rendering mode. Thus, it is recommended to benchmark
 *       both modes in your specific use case to determine the optimal setting.
 *
 * @note Experimental API
 */
enum struct EngineOption : uint8_t
{
    None = 0,                   /**< No engine options are enabled. This may be used to explicitly disable all optional behaviors. */
    Default = 1 << 0,           /**< Uses the default rendering mode. */
    SmartRender = 1 << 1        /**< Enables automatic partial (smart) rendering optimizations. */
};


enum struct PathCommand : uint8_t
{
    Close = 0, ///< Ends the current sub-path and connects it with its initial point. This command doesn't expect any points.
    MoveTo,    ///< Sets a new initial point of the sub-path and a new current point. This command expects 1 point: the starting position.
    LineTo,    ///< Draws a line from the current point to the given point and sets a new value of the current point. This command expects 1 point: the end-position of the line.
    CubicTo    ///< Draws a cubic Bezier curve from the current point to the given point using two given control points and sets a new value of the current point. This command expects 3 points: the 1st control-point, the 2nd control-point, the end-point of the curve.
};


enum struct StrokeCap : uint8_t
{
    Butt = 0, ///< The stroke ends exactly at each of the two end-points of a sub-path. For zero length sub-paths no stroke is rendered.
    Round,    ///< The stroke is extended in both end-points of a sub-path by a half circle, with a radius equal to the half of a stroke width. For zero length sub-paths a full circle is rendered.
    Square    ///< The stroke is extended in both end-points of a sub-path by a rectangle, with the width equal to the stroke width and the length equal to the half of the stroke width. For zero length sub-paths the square is rendered with the size of the stroke width.
};


enum struct StrokeJoin : uint8_t
{
    Miter = 0, ///< The outer corner of the joined path segments is spiked. The spike is created by extension beyond the join point of the outer edges of the stroke until they intersect. In case the extension goes beyond the limit, the join style is converted to the Bevel style.
    Round,     ///< The outer corner of the joined path segments is rounded. The circular region is centered at the join point.
    Bevel      ///< The outer corner of the joined path segments is bevelled at the join point. The triangular region of the corner is enclosed by a straight line between the outer corners of each stroke.
};


enum struct FillSpread : uint8_t
{
    Pad = 0, ///< The remaining area is filled with the closest stop color.
    Reflect, ///< The gradient pattern is reflected outside the gradient area until the expected region is filled.
    Repeat   ///< The gradient pattern is repeated continuously beyond the gradient area until the expected region is filled.
};


enum struct FillRule : uint8_t
{
    NonZero = 0, ///< A line from the point to a location outside the shape is drawn. The intersections of the line with the path segment of the shape are counted. Starting from zero, if the path segment of the shape crosses the line clockwise, one is added, otherwise one is subtracted. If the resulting sum is non zero, the point is inside the shape.
    EvenOdd      ///< A line from the point to a location outside the shape is drawn and its intersections with the path segments of the shape are counted. If the number of intersections is an odd number, the point is inside the shape.
};


//Notation: S(Source), T(Target), SA(Source Alpha), TA(Target Alpha)
enum struct MaskMethod : uint8_t
{
    None = 0,       ///< No Masking is applied.
    Alpha,          ///< Alpha Masking using the masking target's pixels as an alpha value.
    InvAlpha,       ///< Alpha Masking using the complement to the masking target's pixels as an alpha value.
    Luma,           ///< Alpha Masking using the grayscale (0.2126R + 0.7152G + 0.0722*B) of the masking target's pixels. @since 0.9
    InvLuma,        ///< Alpha Masking using the grayscale (0.2126R + 0.7152G + 0.0722*B) of the complement to the masking target's pixels. @since 0.11
    Add,            ///< Combines the target and source objects pixels using target alpha. (T * TA) + (S * (255 - TA)) (Experimental API)
    Subtract,       ///< Subtracts the source color from the target color while considering their respective target alpha. (T * TA) - (S * (255 - TA)) (Experimental API)
    Intersect,      ///< Computes the result by taking the minimum value between the target alpha and the source alpha and multiplies it with the target color. (T * min(TA, SA)) (Experimental API)
    Difference,     ///< Calculates the absolute difference between the target color and the source color multiplied by the complement of the target alpha. abs(T - S * (255 - TA)) (Experimental API)
    Lighten,        ///< Where multiple masks intersect, the highest transparency value is used. (Experimental API)
    Darken          ///< Where multiple masks intersect, the lowest transparency value is used. (Experimental API)
};

//Notation: S(source paint as the top layer), D(destination as the bottom layer), Sa(source paint alpha), Da(destination alpha)
enum struct BlendMethod : uint8_t
{
    Normal = 0,        ///< Perform the alpha blending(default). S if (Sa == 255), otherwise (Sa * S) + (255 - Sa) * D
    Multiply,          ///< Takes the RGB channel values from 0 to 255 of each pixel in the top layer and multiples them with the values for the corresponding pixel from the bottom layer. (S * D)
    Screen,            ///< The values of the pixels in the two layers are inverted, multiplied, and then inverted again. (S + D) - (S * D)
    Overlay,           ///< Combines Multiply and Screen blend modes. (2 * S * D) if (D < 128), otherwise 255 - 2 * (255 - S) * (255 - D)
    Darken,            ///< Creates a pixel that retains the smallest components of the top and bottom layer pixels. min(S, D)
    Lighten,           ///< Only has the opposite action of Darken Only. max(S, D)
    ColorDodge,        ///< Divides the bottom layer by the inverted top layer. D / (255 - S)
    ColorBurn,         ///< Divides the inverted bottom layer by the top layer, and then inverts the result. 255 - (255 - D) / S
    HardLight,         ///< The same as Overlay but with the color roles reversed. (2 * S * D) if (S < 128), otherwise 255 - 2 * (255 - S) * (255 - D)
    SoftLight,         ///< The same as Overlay but with applying pure black or white does not result in pure black or white. (255 - 2 * S) * (D * D) + (2 * S * D)
    Difference,        ///< Subtracts the bottom layer from the top layer or the other way around, to always get a non-negative value. (S - D) if (S > D), otherwise (D - S)
    Exclusion,         ///< The result is twice the product of the top and bottom layers, subtracted from their sum. S + D - (2 * S * D)
    Hue,               ///< Combine with HSL(Sh + Ds + Dl) then convert it to RGB. @since 1.0
    Saturation,        ///< Combine with HSL(Dh + Ss + Dl) then convert it to RGB. @since 1.0
    Color,             ///< Combine with HSL(Sh + Ss + Dl) then convert it to RGB. @since 1.0
    Luminosity,        ///< Combine with HSL(Dh + Ds + Sl) then convert it to RGB. @since 1.0
    Add,               ///< Simply adds pixel values of one layer with the other. (S + D)
    Composition = 255  ///< For intermediate composition layers; suitable for use with Scene or Picture. @since 1.0
};


enum struct SceneEffect : uint8_t
{
    ClearAll = 0,      ///< Reset all previously applied scene effects, restoring the scene to its original state.
    GaussianBlur,      ///< Apply a blur effect with a Gaussian filter. Param(4) = {sigma(double)[> 0], direction(int)[both: 0 / horizontal: 1 / vertical: 2], border(int)[duplicate: 0 / wrap: 1], quality(int)[0 - 100]}
    DropShadow,        ///< Apply a drop shadow effect with a Gaussian Blur filter. Param(8) = {color_R(int)[0 - 255], color_G(int)[0 - 255], color_B(int)[0 - 255], opacity(int)[0 - 255], angle(double)[0 - 360], distance(double), blur_sigma(double)[> 0], quality(int)[0 - 100]}
    Fill,              ///< Override the scene content color with a given fill information. Param(4) = {color_R(int)[0 - 255], color_G(int)[0 - 255], color_B(int)[0 - 255], opacity(int)[0 - 255]}
    Tint,              ///< Tinting the current scene color with a given black, white color parameters. Param(7) = {black_R(int)[0 - 255], black_G(int)[0 - 255], black_B(int)[0 - 255], white_R(int)[0 - 255], white_G(int)[0 - 255], white_B(int)[0 - 255], intensity(double)[0 - 100]}
    Tritone            ///< Apply a tritone color effect to the scene using three color parameters for shadows, midtones, and highlights. A blending factor determines the mix between the original color and the tritone colors. Param(9) = {Shadow_R(int)[0 - 255], Shadow_G(int)[0 - 255], Shadow_B(int)[0 - 255], Midtone_R(int)[0 - 255], Midtone_G(int)[0 - 255], Midtone_B(int)[0 - 255], Highlight_R(int)[0 - 255], Highlight_G(int)[0 - 255], Highlight_B(int)[0 - 255], Blend(int)[0 - 255]}
};

enum struct TextWrap : uint8_t
{
    None = 0,      ///< Do not wrap text. Text is rendered on a single line and may overflow the bounding area.
    Character,     ///< Wrap at the character level. If a word cannot fit, it is broken into individual characters to fit the line.
    Word,          ///< Wrap at the word level. Words that do not fit are moved to the next line.
    Smart,         ///< Smart choose wrapping method: word wrap first, falling back to character wrap if a word does not fit.
    Ellipsis,      ///< Truncate overflowing text and append an ellipsis ("...") at the end. Typically used for single-line labels.
    Hyphenation    ///< Reserved. No Support.
};


enum struct Type : uint8_t
{
    Undefined = 0,         ///< Unkown class
    Shape,                 ///< Shape class
    Scene,                 ///< Scene class
    Picture,               ///< Picture class
    Text,                  ///< Text class
    LinearGradient = 10,   ///< LinearGradient class
    RadialGradient         ///< RadialGradient class
};

struct Point
{
    float x;  ///< The x-coordinate of the point.
    float y;  ///< The y-coordinate of the point.
};

//The elements e11, e12, e21 and e22 represent the rotation matrix, including the scaling factor.
//The elements e13 and e23 determine the translation of the object along the x and y-axis, respectively.
//The elements e31 and e32 are set to 0, e33 is set to 1.
struct Matrix
{
    float e11, e12, e13;
    float e21, e22, e23;
    float e31, e32, e33;
};


struct TVG_API Paint
{
    const Paint* parent() const noexcept;
    Result visible(bool on) noexcept;
    Result rotate(float degree) noexcept;
    Result scale(float factor) noexcept;
    Result translate(float x, float y) noexcept;
    Result transform(const Matrix& m) noexcept;
    Matrix& transform() noexcept;
    Result opacity(uint8_t o) noexcept;
    Result mask(Paint* target, MaskMethod method) noexcept;
    Result clip(Shape* clipper) noexcept;
    Result blend(BlendMethod method) noexcept;
    Result bounds(Point* pt4) noexcept;
    Result bounds(float* x, float* y, float* w, float* h) noexcept;
    bool intersects(int32_t x, int32_t y, int32_t w = 1, int32_t h = 1) noexcept;
    Paint* duplicate() const noexcept;
    uint8_t opacity() const noexcept;
    MaskMethod mask(const Paint** target) const noexcept;
    Shape* clip() const noexcept;
    bool visible() const noexcept;

    uint16_t ref() noexcept;
    uint16_t unref(bool free = true) noexcept;
    uint16_t refCnt() const noexcept;
    virtual Type type() const noexcept = 0;
    uint32_t id = 0;
    static void rel(Paint* paint) noexcept;

protected:

    virtual ~Paint();

    _TVG_DECLARE_PRIVATE_BASE(Paint);
};

struct TVG_API Fill
{
    struct ColorStop
    {
        float offset; /**< The relative position of the color. */
        uint8_t r;    /**< The red color channel value in the range [0 ~ 255]. */
        uint8_t g;    /**< The green color channel value in the range [0 ~ 255]. */
        uint8_t b;    /**< The blue color channel value in the range [0 ~ 255]. */
        uint8_t a;    /**< The alpha channel value in the range [0 ~ 255], where 0 is completely transparent and 255 is opaque. */
    };

    virtual ~Fill();

    Result colorStops(const ColorStop* colorStops, uint32_t cnt) noexcept;
    Result spread(FillSpread s) noexcept;
    Result transform(const Matrix& m) noexcept;
    uint32_t colorStops(const ColorStop** colorStops) const noexcept;
    FillSpread spread() const noexcept;
    Matrix& transform() const noexcept;
    Fill* duplicate() const noexcept;
    virtual Type type() const noexcept = 0;

    _TVG_DECLARE_PRIVATE_BASE(Fill);
};

struct TVG_API Canvas
{
    virtual ~Canvas();

    const std::list<Paint*>& paints() const noexcept;
    Result push(Paint* target, Paint* at = nullptr) noexcept;
    Result remove(Paint* paint = nullptr) noexcept;
    Result update() noexcept;
    Result draw(bool clear = false) noexcept;
    Result viewport(int32_t x, int32_t y, int32_t w, int32_t h) noexcept;
    Result sync() noexcept;

    _TVG_DECLARE_PRIVATE_BASE(Canvas);
};

struct TVG_API LinearGradient : Fill
{
    Result linear(float x1, float y1, float x2, float y2) noexcept;
    Result linear(float* x1, float* y1, float* x2, float* y2) const noexcept;
    static LinearGradient* gen() noexcept;
    Type type() const noexcept override;

    _TVG_DECLARE_PRIVATE(LinearGradient);
};


struct TVG_API RadialGradient : Fill
{
    Result radial(float cx, float cy, float r, float fx, float fy, float fr) noexcept;
    Result radial(float* cx, float* cy, float* r, float* fx = nullptr, float* fy = nullptr, float* fr = nullptr) const noexcept;
    static RadialGradient* gen() noexcept;
    Type type() const noexcept override;

    _TVG_DECLARE_PRIVATE(RadialGradient);
};

struct TVG_API Shape : Paint
{
    Result reset() noexcept;
    Result moveTo(float x, float y) noexcept;
    Result lineTo(float x, float y) noexcept;
    Result cubicTo(float cx1, float cy1, float cx2, float cy2, float x, float y) noexcept;
    Result close() noexcept;
    Result appendRect(float x, float y, float w, float h, float rx = 0, float ry = 0, bool cw = true) noexcept;
    Result appendCircle(float cx, float cy, float rx, float ry, bool cw = true) noexcept;
    Result appendPath(const PathCommand* cmds, uint32_t cmdCnt, const Point* pts, uint32_t ptsCnt) noexcept;
    Result strokeWidth(float width) noexcept;
    Result strokeFill(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept;
    Result strokeFill(Fill* f) noexcept;
    Result strokeDash(const float* dashPattern, uint32_t cnt, float offset = 0.0f) noexcept;
    Result strokeCap(StrokeCap cap) noexcept;
    Result strokeJoin(StrokeJoin join) noexcept;
    Result strokeMiterlimit(float miterlimit) noexcept;
    Result trimpath(float begin, float end, bool simultaneous = true) noexcept;
    Result fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept;
    Result fill(Fill* f) noexcept;
    Result fillRule(FillRule r) noexcept;
    Result order(bool strokeFirst) noexcept;
    Result path(const PathCommand** cmds, uint32_t* cmdsCnt, const Point** pts, uint32_t* ptsCnt) const noexcept;
    const Fill* fill() const noexcept;
    Result fill(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a = nullptr) const noexcept;
    FillRule fillRule() const noexcept;
    float strokeWidth() const noexcept;
    Result strokeFill(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a = nullptr) const noexcept;
    const Fill* strokeFill() const noexcept;
    uint32_t strokeDash(const float** dashPattern, float* offset = nullptr) const noexcept;
    StrokeCap strokeCap() const noexcept;
    StrokeJoin strokeJoin() const noexcept;
    float strokeMiterlimit() const noexcept;
    static Shape* gen() noexcept;
    Type type() const noexcept override;

    _TVG_DECLARE_PRIVATE_DERIVE(Shape);
};

struct TVG_API Picture : Paint
{
    Result load(const char* filename) noexcept;
    Result load(const char* data, uint32_t size, const char* mimeType, const char* rpath = nullptr, bool copy = false) noexcept;
    Result size(float w, float h) noexcept;
    Result size(float* w, float* h) const noexcept;
    Result origin(float x, float y) noexcept;
    Result origin(float* x, float* y) const noexcept;
    Result load(const uint32_t* data, uint32_t w, uint32_t h, ColorSpace cs, bool copy = false) noexcept;
    Result resolver(std::function<bool(Paint* paint, const char* src, void* data)> func, void* data) noexcept;
    const Paint* paint(uint32_t id) noexcept;
    static Picture* gen() noexcept;
    Type type() const noexcept override;

    _TVG_DECLARE_ACCESSOR(Animation);
    _TVG_DECLARE_PRIVATE_DERIVE(Picture);
};

struct TVG_API Scene : Paint
{
    Result push(Paint* target, Paint* at = nullptr) noexcept;

    const std::list<Paint*>& paints() const noexcept;
    Result remove(Paint* paint = nullptr) noexcept;
    Result push(SceneEffect effect, ...) noexcept;

    static Scene* gen() noexcept;
    Type type() const noexcept override;

    _TVG_DECLARE_PRIVATE_DERIVE(Scene);
};

struct TVG_API Text : Paint
{
    Result font(const char* name) noexcept;
    Result size(float size) noexcept;
    Result text(const char* text) noexcept;
    Result align(float x, float y) noexcept;
    Result layout(float w, float h) noexcept;
    Result wrap(TextWrap mode) noexcept;
    Result italic(float shear = 0.18f) noexcept;
    Result outline(float width, uint8_t r, uint8_t g, uint8_t b) noexcept;
    Result fill(uint8_t r, uint8_t g, uint8_t b) noexcept;
    Result fill(Fill* f) noexcept;
    static Result load(const char* filename) noexcept;
    static Result load(const char* name, const char* data, uint32_t size, const char* mimeType = "ttf", bool copy = false) noexcept;
    static Result unload(const char* filename) noexcept;
    static Text* gen() noexcept;

    Type type() const noexcept override;

    _TVG_DECLARE_PRIVATE_DERIVE(Text);
};

struct TVG_API SwCanvas final : Canvas
{
    ~SwCanvas() override;

    Result target(uint32_t* buffer, uint32_t stride, uint32_t w, uint32_t h, ColorSpace cs) noexcept;
    static SwCanvas* gen(EngineOption op = EngineOption::Default) noexcept;

    _TVG_DECLARE_PRIVATE(SwCanvas);
};


struct TVG_API GlCanvas final : Canvas
{
    ~GlCanvas() override;

    Result target(void* context, int32_t id, uint32_t w, uint32_t h, ColorSpace cs) noexcept;
    static GlCanvas* gen(qor::ref_of<qor::components::OpenGLESFeature>::type openGLES) noexcept;

    _TVG_DECLARE_PRIVATE(GlCanvas);
};


struct TVG_API WgCanvas final : Canvas
{
    ~WgCanvas() override;

    Result target(void* device, void* instance, void* target, uint32_t w, uint32_t h, ColorSpace cs, int type = 0) noexcept;
    static WgCanvas* gen() noexcept;

    _TVG_DECLARE_PRIVATE(WgCanvas);
};


struct TVG_API Initializer final
{
    static Result init(uint32_t threads = 0) noexcept;
    static Result term() noexcept;
    static const char* version(uint32_t* major, uint32_t* minor, uint32_t* micro) noexcept;

    _TVG_DISABLE_CTOR(Initializer);
};


struct TVG_API Animation
{
    virtual ~Animation();

    Result frame(float no) noexcept;
    Picture* picture() const noexcept;

    float curFrame() const noexcept;
    float totalFrame() const noexcept;
    float duration() const noexcept;
    Result segment(float begin, float end) noexcept;
    Result segment(float* begin, float* end = nullptr) noexcept;
    static Animation* gen() noexcept;

    _TVG_DECLARE_PRIVATE_BASE(Animation);
};


struct TVG_API Saver final
{
    ~Saver();

    Result background(Paint* paint) noexcept;
    Result save(Paint* paint, const char* filename, uint32_t quality = 100) noexcept;
    Result save(Animation* animation, const char* filename, uint32_t quality = 100, uint32_t fps = 0) noexcept;
    Result sync() noexcept;
    static Saver* gen() noexcept;

    _TVG_DECLARE_PRIVATE_BASE(Saver);
};


struct TVG_API Accessor final
{
    ~Accessor();

    Result set(Paint* paint, std::function<bool(const Paint* paint, void* data)> func, void* data) noexcept;
    static uint32_t id(const char* name) noexcept;
    static Accessor* gen() noexcept;

    _TVG_DECLARE_PRIVATE_BASE(Accessor);
};

}}}}//qor::components::ui::renderer

#endif //_THORVG_H_
