#ifndef POLY_POLY_ENDIAN_H
#define POLY_POLY_ENDIAN_H

namespace poly
{


////////////////////////////////////////////////////////////
/// \brief Check if the machine uses little or big endian format
/// \ingroup Core
///
////////////////////////////////////////////////////////////
struct Endian
{
    enum Type
    {
        Little,     //!< Little endian format
        Big         //!< Big endian format
    };

    ////////////////////////////////////////////////////////////
    /// \brief Get the machine's endian format type
    ///
    /// \return The endian format type
    ///
    ////////////////////////////////////////////////////////////
    static Type getType();
};


}

#endif