#ifndef		CEVENT_H
#define		CEVENT_H

#include <SDL.h>

class CEvent {
public:
	CEvent();
	virtual ~CEvent();

	virtual void OnEvent(SDL_Event* Event);

	virtual void OnInputFocus(Uint32 windowID);
	virtual void OnInputBlur(Uint32 windowID);
	virtual void OnKeyDown(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat);
	virtual void OnKeyUp(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat);
	virtual void OnMouseFocus(Uint32 windowID);
	virtual void OnMouseBlur(Uint32 windowID);
	virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	virtual void OnMouseWheel(bool Up, bool Down);
	virtual void OnLButtonDown(int mX, int mY);
	virtual void OnLButtonUp(int mX, int mY);
	virtual void OnRButtonDown(int mX, int mY);
	virtual void OnRButtonUp(int mX, int mY);
	virtual void OnMButtonDown(int mX, int mY);
	virtual void OnMButtonUp(int mX, int mY);
	virtual void OnMinimize(Uint32 windowID);
	virtual void OnRestore(Uint32 windowID);
	virtual void OnResize(int w, int h);
	virtual void OnExpose(Uint32 windowID);
	virtual void OnExit();
	virtual void OnUser(Uint8 type, int code, void* data1, void* data2);
	virtual void OnWindowMoved(Uint32 windowID);
	virtual void OnWindowClosed(Uint32 windowID);
};

#endif	//	CEVENT_H
