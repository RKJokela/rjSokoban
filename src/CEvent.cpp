#include "CEvent.h"

CEvent::CEvent() { }
CEvent::~CEvent() { }

void CEvent::OnEvent(SDL_Event* Event) {
	switch (Event->type) {
	case SDL_WINDOWEVENT:
		switch (Event->window.event) {
		case SDL_WINDOWEVENT_ENTER:
			OnMouseFocus(Event->window.windowID); break;
		case SDL_WINDOWEVENT_LEAVE:
			OnMouseBlur(Event->window.windowID); break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			OnInputFocus(Event->window.windowID); break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			OnInputBlur(Event->window.windowID); break;
		case SDL_WINDOWEVENT_MINIMIZED:
			OnMinimize(Event->window.windowID); break;
		case SDL_WINDOWEVENT_RESTORED:
			OnRestore(Event->window.windowID); break;
		case SDL_WINDOWEVENT_EXPOSED:
			OnExpose(Event->window.windowID); break;
		case SDL_WINDOWEVENT_RESIZED:
			OnResize(Event->window.data1, Event->window.data2);
		case SDL_WINDOWEVENT_MOVED:
			OnWindowMoved(Event->window.windowID);
		case SDL_WINDOWEVENT_CLOSE:
			OnWindowClosed(Event->window.windowID);
		default:
			break;
		}
		break;
	case SDL_KEYDOWN:
		OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode, Event->key.repeat != 0);
		break;
	case SDL_KEYUP:
		OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode, Event->key.repeat != 0);
		break;
	case SDL_MOUSEMOTION:
		OnMouseMove(Event->motion.x, Event->motion.y,
			Event->motion.xrel, Event->motion.yrel,
			(Event->motion.state&SDL_BUTTON_LMASK) != 0,
			(Event->motion.state&SDL_BUTTON_RMASK) != 0,
			(Event->motion.state&SDL_BUTTON_MMASK) != 0);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (Event->button.button) {
		case SDL_BUTTON_LEFT:
			OnLButtonDown(Event->button.x, Event->button.y); break;
		case SDL_BUTTON_RIGHT:
			OnLButtonDown(Event->button.x, Event->button.y); break;
		case SDL_BUTTON_MIDDLE:
			OnLButtonDown(Event->button.x, Event->button.y); break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (Event->button.button) {
		case SDL_BUTTON_LEFT:
			OnLButtonUp(Event->button.x, Event->button.y); break;
		case SDL_BUTTON_RIGHT:
			OnRButtonUp(Event->button.x, Event->button.y); break;
		case SDL_BUTTON_MIDDLE:
			OnMButtonUp(Event->button.x, Event->button.y); break;
		}
		break;
	case SDL_MOUSEWHEEL:
		OnMouseWheel(Event->wheel.y > 0, Event->wheel.y < 0);
		break;
	case SDL_QUIT:
		OnExit();
		break;
	case SDL_SYSWMEVENT:
		// IGNORE
		break;
	default:
		OnUser(Event->user.type, Event->user.code, Event->user.data1, Event->user.data2);
	}
}

void CEvent::OnInputFocus(Uint32 windowID) { }
void CEvent::OnInputBlur(Uint32 windowID) { }
void CEvent::OnKeyDown(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat) { }
void CEvent::OnKeyUp(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat) { }
void CEvent::OnMouseFocus(Uint32 windowID) { }
void CEvent::OnMouseBlur(Uint32 windowID) { }
void CEvent::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) { }
void CEvent::OnMouseWheel(bool Up, bool Down) { }
void CEvent::OnLButtonDown(int mX, int mY) { }
void CEvent::OnLButtonUp(int mX, int mY) { }
void CEvent::OnRButtonDown(int mX, int mY) { }
void CEvent::OnRButtonUp(int mX, int mY) { }
void CEvent::OnMButtonDown(int mX, int mY) { }
void CEvent::OnMButtonUp(int mX, int mY) { }
void CEvent::OnMinimize(Uint32 windowID) { }
void CEvent::OnRestore(Uint32 windowID) { }
void CEvent::OnResize(int w, int h) { }
void CEvent::OnExpose(Uint32 windowID) { }
void CEvent::OnExit() { }
void CEvent::OnUser(Uint8 type, int code, void* data1, void* data2) { }
void CEvent::OnWindowMoved(Uint32 windowID) { }
void CEvent::OnWindowClosed(Uint32 windowID) { }
