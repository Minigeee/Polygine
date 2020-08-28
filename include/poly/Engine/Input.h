#ifndef POLY_INPUT_H
#define POLY_INPUT_H

namespace poly
{

///////////////////////////////////////////////////////////

enum class Keyboard
{
	Space              = 32,
	Apostrophe         = 39,  /* ' */
	Comma              = 44,  /* , */
	Minus              = 45,  /* - */
	Period             = 46,  /* . */
	Slash              = 47,  /* / */
	Zero               = 48,
	One                = 49,
	Two                = 50,
	Three              = 51,
	Four               = 52,
	Five               = 53,
	Six                = 54,
	Seven              = 55,
	Eight              = 56,
	Nine               = 57,
	Semicolon          = 59,  /* ; */
	Equal              = 61,  /* = */
	A                  = 65,
	B                  = 66,
	C                  = 67,
	D                  = 68,
	E                  = 69,
	F                  = 70,
	G                  = 71,
	H                  = 72,
	I                  = 73,
	J                  = 74,
	K                  = 75,
	L                  = 76,
	M                  = 77,
	N                  = 78,
	O                  = 79,
	P                  = 80,
	Q                  = 81,
	R                  = 82,
	S                  = 83,
	T                  = 84,
	U                  = 85,
	V                  = 86,
	W                  = 87,
	X                  = 88,
	Y                  = 89,
	Z                  = 90,
	LeftBracket        = 91,  /* [ */
	Backslash          = 92,  /* \ */
	RightBracket       = 93,  /* ] */
	GraveAccent        = 96,  /* ` */
	World1             = 161, /* non-us #1 */
	World2             = 162, /* non-us #2 */

	Escape             = 256,
	Enter              = 257,
	Tab                = 258,
	Backspace          = 259,
	Insert             = 260,
	Delete             = 261,
	Right              = 262,
	Left               = 263,
	Down               = 264,
	Up                 = 265,
	PageUp             = 266,
	PageDown           = 267,
	Home               = 268,
	End                = 269,
	CapsLock           = 280,
	ScrollLock         = 281,
	NumLock            = 282,
	PrintScreen        = 283,
	Pause              = 284,
	F1                 = 290,
	F2                 = 291,
	F3                 = 292,
	F4                 = 293,
	F5                 = 294,
	F6                 = 295,
	F7                 = 296,
	F8                 = 297,
	F9                 = 298,
	F10                = 299,
	F11                = 300,
	F12                = 301,
	F13                = 302,
	F14                = 303,
	F15                = 304,
	F16                = 305,
	F17                = 306,
	F18                = 307,
	F19                = 308,
	F20                = 309,
	F21                = 310,
	F22                = 311,
	F23                = 312,
	F24                = 313,
	F25                = 314,
	KP_0               = 320,
	KP_1               = 321,
	KP_2               = 322,
	KP_3               = 323,
	KP_4               = 324,
	KP_5               = 325,
	KP_6               = 326,
	KP_7               = 327,
	KP_8               = 328,
	KP_9               = 329,
	KP_Decimal         = 330,
	KP_Divide          = 331,
	KP_Multiply        = 332,
	KP_Subtract        = 333,
	KP_Add             = 334,
	KP_Enter           = 335,
	KP_Equal           = 336,
	LeftShift          = 340,
	LeftControl        = 341,
	LeftAlt            = 342,
	LeftSuper          = 343,
	RightShift         = 344,
	RightControl       = 345,
	RightAlt           = 346,
	RightSuper         = 347,
	Menu               = 348
};


enum class Mouse
{
	Button1			= 0,
	Button2			= 1,
	Button3			= 2,
	Button4			= 3,
	Button5			= 4,
	Button6			= 5,
	Button7			= 6,
	Button8			= 7,

	Left			= Button1,
	Right			= Button2,
	Middle			= Button3
};


enum class InputAction
{
	Release = 0,
	Press,
	Repeat
};


enum class CursorMode
{
	Normal		= 0x00034001,
	Hidden		= 0x00034002,
	Disabled	= 0x00034003
};

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \brief Keyboard event
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_KeyEvent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_KeyEvent() = default;

	///////////////////////////////////////////////////////////
	/// \brief Construct the event from its properties
	///
	/// \param key The key that was pressed/released/held
	/// \param action The action the occured (pressed/released/held)
	///
	///////////////////////////////////////////////////////////
	E_KeyEvent(Keyboard key, InputAction action);

	Keyboard m_key;			//!< The key that was pressed/released/held
	InputAction m_action;	//!< The action the occured (pressed/released/held)
};

///////////////////////////////////////////////////////////
/// \brief Mouse button event
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_MouseButton
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_MouseButton() = default;

	///////////////////////////////////////////////////////////
	/// \brief Construct the event from its properties
	///
	/// \param button The mouse button that was pressed/released
	/// \param action The action the occured pressed/released
	///
	///////////////////////////////////////////////////////////
	E_MouseButton(Mouse button, InputAction action);

	Mouse m_button;			//!< The mouse button that was pressed/released
	InputAction m_action;	//!< The action the occured pressed/released
};

///////////////////////////////////////////////////////////
/// \brief Mouse move event
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_MouseMove
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_MouseMove() = default;

	///////////////////////////////////////////////////////////
	/// \brief Construct the event from its properties
	///
	/// \param x The x-coordinate of the mouse relative to the top-left of the window
	/// \param y The y-coordinate of the mouse relative to the top-left of the window
	///
	///////////////////////////////////////////////////////////
	E_MouseMove(float x, float y);

	float m_x;	//!< The x-coordinate of the mouse relative to the top-left of the window
	float m_y;	//!< The y-coordinate of the mouse relative to the top-left of the window
};

///////////////////////////////////////////////////////////
/// \brief Mouse scroll event
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_MouseScroll
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_MouseScroll() = default;

	///////////////////////////////////////////////////////////
	/// \brief Construct the event from its properties
	///
	/// \param dx The amount scrolled on the x-axis
	/// \param dy The amount scrolled on the y-axis
	///
	///////////////////////////////////////////////////////////
	E_MouseScroll(float dx, float dy);

	float m_dx;	//!< The amount scrolled on the x-axis
	float m_dy;	//!< The amount scrolled on the y-axis
};

///////////////////////////////////////////////////////////

}

#endif