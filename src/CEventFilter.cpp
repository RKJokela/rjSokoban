#include "CEventFilter.h"

namespace CEventFilter {

	namespace {

		EventPriorityList _defaultPriorities = {
			PRI_OTHER,		// CAT_QUIT
			PRI_OTHER,		// CAT_WINDOW
			PRI_GAME_INPUT,	// CAT_KEYBOARD
			PRI_GAME_INPUT,	// CAT_MOUSE
			PRI_IGNORE,		// CAT_JOYSTICK
			PRI_IGNORE,		// CAT_CONTROLLER
			PRI_IGNORE,		// CAT_TOUCH
			PRI_IGNORE,		// CAT_GESTURE
			PRI_OTHER,		// CAT_CLIPBOARD
			PRI_OTHER,		// CAT_DRAGDROP
			PRI_IGNORE,		// CAT_RENDER
			PRI_GAME_INPUT,	// CAT_USER
		};
		EventCategory _cats[NUM_EVENT_CATEGORIES] = {
			CAT_QUIT,
			CAT_WINDOW,
			CAT_KEYBOARD,
			CAT_MOUSE,
			CAT_JOYSTICK,
			CAT_CONTROLLER,
			CAT_TOUCH,
			CAT_GESTURE,
			CAT_CLIPBOARD,
			CAT_DRAGDROP,
			CAT_RENDER,
			CAT_USER
		};

		std::map<EventCategory, EventPriority> _priorities;
	
		int _defaultFilter(void* bGameInputFlag, SDL_Event* e) {
			bool* _input_happened = (bool*)bGameInputFlag;
			int passesFilter = 0;
			EventCategory cat = getCategoryFromType(e->type);
			EventPriority pri = _priorities.at(cat);
			switch (pri) {
			case PRI_GAME_INPUT:
				*_input_happened = true;
			case PRI_OTHER:
				passesFilter = 1;
			case PRI_IGNORE:
				break;
			}

			return passesFilter;
		}

		SDL_EventFilter _filter = _defaultFilter;

		SDL_EventFilter _oldFilter = NULL;
		void* _oldData = NULL;
	} // anonymous namespace

	EventCategory getCategoryFromType(Uint32 type) {
		if (type >= CAT_USER) return CAT_USER;
		type &= 0x0000fff0;
		if (type == CAT_CONTROLLER) return CAT_CONTROLLER;
		return (EventCategory)(type &= 0xff00);
	}
	
	// default filter function and priorities
	void SetDefaults() {
		SetPriorities(_defaultPriorities);
		_filter = _defaultFilter;
	}

	// if you use this, SetPriority and GetPriority will not do anything meaningful
	void SetCustomFilter(SDL_EventFilter filterFunc) {
		_filter = filterFunc;
	}

	// set the priority for the given event category
	void SetPriority(EventCategory cat, EventPriority priority) {
		_priorities[cat] = priority;
	}

	// set all priorities from an array ordered by category number
	void SetPriorities(EventPriorityList priorities) {
		for (int i = 0; i < NUM_EVENT_CATEGORIES; i++)
			SetPriority(_cats[i], priorities[i]);
	}

	// set all priorities to the same value
	// (use to ignore or pass all events, or as a starting point before enabling/disabling desired events)
	void SetAllPrioritiesTo(EventPriority priority) {
		for (int i = 0; i < NUM_EVENT_CATEGORIES; i++)
			SetPriority(_cats[i], priority);
	}

	// check current setting for a given category
	EventPriority GetPriority(EventCategory cat) { return _priorities[cat]; }

	// must call this to actually start filtering events (after setting desired priorities)
	void TurnFilterOn(void* bGameInputFlag) {
		SDL_GetEventFilter(&_oldFilter, &_oldData);
		SDL_SetEventFilter(_filter, bGameInputFlag);
	}

	// stop filtering events
	void TurnFilterOff() {
		if (_oldFilter != NULL) {
			SDL_SetEventFilter(_oldFilter, _oldData);
		}
	}

} // CEventFilter
