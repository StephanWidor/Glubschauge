#pragma once

namespace cv {

// swdebug: Maybe we shouldn't use strong typed enum class, so we wouldn't need all those operators...
enum class FaceDistortionType : unsigned
{
    None = 0,
    Eyes = 1,
    Nose = 2,
    Mouth = 4,
    UpperHead = 8,
    LowerHead = 16
};
constexpr unsigned operator~(FaceDistortionType type)
{
    return ~static_cast<unsigned>(type);
}
constexpr unsigned operator&=(unsigned &typeAsUnsigned, FaceDistortionType type)
{
    return typeAsUnsigned &= static_cast<unsigned>(type);
}
constexpr unsigned operator&(unsigned oredTypes, FaceDistortionType type)
{
    return oredTypes & static_cast<unsigned>(type);
}
constexpr unsigned operator&(FaceDistortionType type, unsigned oredTypes)
{
    return oredTypes & type;
}
constexpr unsigned operator|=(unsigned &typeAsUnsigned, FaceDistortionType type)
{
    return typeAsUnsigned |= static_cast<unsigned>(type);
}
constexpr unsigned operator|(unsigned oredTypes, FaceDistortionType type)
{
    return oredTypes | static_cast<unsigned>(type);
}
constexpr unsigned operator|(FaceDistortionType type, unsigned oredTypes)
{
    return oredTypes | type;
}
constexpr unsigned operator|(FaceDistortionType type0, FaceDistortionType type1)
{
    return static_cast<unsigned>(type0) | type1;
}
constexpr bool operator==(unsigned typeAsUnsigned, FaceDistortionType type)
{
    return typeAsUnsigned == static_cast<unsigned>(type);
}
constexpr bool operator==(FaceDistortionType type, unsigned typeAsUnsigned)
{
    return typeAsUnsigned == type;
}
constexpr bool operator!=(unsigned typeAsUnsigned, FaceDistortionType type)
{
    return !(typeAsUnsigned == type);
}
constexpr bool operator!=(FaceDistortionType type, unsigned typeAsUnsigned)
{
    return !(typeAsUnsigned == type);
}

}    // namespace cv
