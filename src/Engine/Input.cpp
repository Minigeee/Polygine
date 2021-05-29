#include <poly/Engine/Input.h>

namespace poly
{


///////////////////////////////////////////////////////////
E_KeyEvent::E_KeyEvent(Keyboard key, InputAction action) :
	m_key		(key),
	m_action	(action)
{ }


///////////////////////////////////////////////////////////
E_MouseButton::E_MouseButton(Mouse button, InputAction action) :
	m_button	(button),
	m_action	(action)
{ }


///////////////////////////////////////////////////////////
E_MouseMove::E_MouseMove(float x, float y) :
	m_x		(x),
	m_y		(y)
{ }


///////////////////////////////////////////////////////////
E_MouseScroll::E_MouseScroll(float dx, float dy) :
	m_dx		(dx),
	m_dy		(dy)
{ }


///////////////////////////////////////////////////////////
E_TextInput::E_TextInput(Uint32 c) :
	m_char	(c)
{ }


}