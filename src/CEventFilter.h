#ifndef		CEVENT_FILTER_H
#define		CEVENT_FILTER_H

#include <SDL.h>
#include <map>

#define NUM_EVENT_CATEGORIES 12

// Can copy-paste from here to define your own event priorities
//CEventFilter::EventPriorityList myPriorities = {
//	CEventFilter::PRI_OTHER,		// CAT_QUIT
//	CEventFilter::PRI_OTHER,		// CAT_WINDOW
//	CEventFilter::PRI_GAME_INPUT,	// CAT_KEYBOARD
//	CEventFilter::PRI_GAME_INPUT,	// CAT_MOUSE
//	CEventFilter::PRI_IGNORE,		// CAT_JOYSTICK
//	CEventFilter::PRI_IGNORE,		// CAT_CONTROLLER
//	CEventFilter::PRI_IGNORE,		// CAT_TOUCH
//	CEventFilter::PRI_IGNORE,		// CAT_GESTURE
//	CEventFilter::PRI_OTHER,		// CAT_CLIPBOARD
//	CEventFilter::PRI_OTHER,		// CAT_DRAGDROP
//	CEventFilter::PRI_IGNORE,		// CAT_RENDER
//	CEventFilter::PRI_IGNORE,		// CAT_USER
//};

// This class lets you choose which events will be processed by the game
//  and which will be ignored.
namespace CEventFilter {

	enum EventCategory {
		CAT_QUIT		= 0x0100,
		CAT_WINDOW		= 0x0200,
		CAT_KEYBOARD	= 0x0300, // not to be confused with KEYBOARD_CAT
		CAT_MOUSE		= 0x0400, // see definition of TOM_JERRY
		CAT_JOYSTICK	= 0x0600,
		CAT_CONTROLLER	= 0x0650,
		CAT_TOUCH		= 0x0700,
		CAT_GESTURE		= 0x0800,
		CAT_CLIPBOARD	= 0x0900,
		CAT_DRAGDROP	= 0x1000,
		CAT_RENDER		= 0x2000,
		CAT_USER		= 0x8000
	};
	enum EventPriority {
		// These events will be filtered out completely - program will never see them!
		PRI_IGNORE = 0,
		// These will be flagged to easily link with game-specific logic
		PRI_GAME_INPUT,
		// Mostly window events here (things that need handling but not directly related to the game)
		PRI_OTHER
	};
	typedef EventPriority EventPriorityList[NUM_EVENT_CATEGORIES];

	EventCategory getCategoryFromType(Uint32 type);

	// default filter function and priorities
	void SetDefaults();

	// if you use this, SetPriority and GetPriority will not do anything meaningful
	void SetCustomFilter(SDL_EventFilter filterFunc); 

	// set the priority for the given event category
	void SetPriority(EventCategory cat, EventPriority priority);

	// set all priorities from an array ordered by category number
	void SetPriorities(EventPriorityList priorities);

	// set all priorities to the same value
	// (use to ignore or pass all events, or as a starting point before enabling/disabling desired events)
	void SetAllPrioritiesTo(EventPriority priority);

	// check current setting for a given category
	EventPriority GetPriority(EventCategory cat);

	// must call this to actually start filtering events (after setting desired priorities)
	void TurnFilterOn(void* bGameInputFlag);

	// stop filtering events
	void TurnFilterOff();

} // CEventFilter

#endif	//	CEVENT_FILTER_H
