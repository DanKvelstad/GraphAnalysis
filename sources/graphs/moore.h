#pragma once

#include <mutex>
#include <map>
#include <functional>

template<class StateT, class EventT>
class moore
{

public:

	typedef EventT EventType;
	typedef StateT StateType;
	
public:

	moore(
		StateT						StartState, 
		std::function<void(void)>	StartStateCallback
	)
	: StartState(StartState)
	{
		add_state(StartState, StartStateCallback);
		set_current_state(StartState);
	}
	
	~moore(void)
	{
	}

	void reset()
	{

		std::lock_guard<std::mutex> StateLock(StateMutex);

		set_current_state(StartState);

	}

	void add_state(StateT State, std::function<void(void)> Callback)
	{

		std::lock_guard<std::mutex> StateLock(StateMutex);
		
		auto AddedState(
			States.insert(
				std::make_pair(
					State, 
					Callback
				)
			).second
		);

		if(!AddedState)
		{
			throw std::exception("Could not add state");
		}

	}

	virtual void add_edge(StateT SourceState, StateT TargetState)
	{

		std::lock_guard<std::mutex> StateLock(StateMutex);

		if(0 == States.count(SourceState))
		{
			throw std::exception("Source state must be added before the edge");
		}

		if(0 == States.count(TargetState))
		{
			throw std::exception("Target state must be added before the edge");
		}

		auto InsertionResult(
			AutoEdges.insert(
				std::make_pair(
					SourceState, 
					TargetState
				)
			).second
		);

		if(!InsertionResult)
		{
			throw std::exception("Cannot add edge, it has already been added");
		}

	}

	virtual void add_edge(StateT SourceState, EventT Event, StateT TargetState)
	{

		std::lock_guard<std::mutex> StateLock(StateMutex);

		if(0 == States.count(SourceState))
		{
			throw std::exception("Source state must be added before the edge");
		}

		if(0 == States.count(TargetState))
		{
			throw std::exception("Target state must be added before the edge");
		}

		auto& EdgesForEvent(
			EventEdges.insert(
				std::make_pair(
					Event, 
					decltype(EventEdges)::mapped_type()
				)
			).first->second
		);

		auto EdgeWasAdded(
			EdgesForEvent.insert(
				std::make_pair(
					SourceState, 
					TargetState
				)
			).second
		);
		
		if(!EdgeWasAdded)
		{
			throw std::exception("Could not add edge, it has already been added");
		}

	}

	virtual void handle_event(EventT Event)
	{

		std::lock_guard<std::mutex> StateLock(StateMutex);

		auto EdgesForEvent(EventEdges.find(Event));
		if(std::end(EventEdges) == EdgesForEvent)
		{
			throw std::exception("There are no event-edges for the event");
		}
	
		auto SelectedEdge(EdgesForEvent->second.find(CurrentState));
		if(std::end(EdgesForEvent->second) == SelectedEdge)
		{
			throw std::exception("There are no edge for this event from the current state");
		}

		set_current_state(SelectedEdge->second);

	}

private:

	void set_current_state(StateT State)
	{

		auto StateEntry(States.find(State));

		if(States.end() == StateEntry)
		{
			throw std::exception("Internal Error! Attempting to set invalid state");
		}

		CurrentState = StateEntry->first;

		StateEntry->second();

		auto AutoEdgeEntry(AutoEdges.find(CurrentState));
		if(std::end(AutoEdges) != AutoEdgeEntry)
		{
			set_current_state(AutoEdgeEntry->second);
		}

	}

private:

	mutable std::mutex	StateMutex;

	std::map<StateT, std::function<void(void)>>		States;
	std::map<StateT, StateT>						AutoEdges;
	std::map<EventT, std::map<StateT, StateT>>		EventEdges;
	
	StateT StartState;
	StateT CurrentState;

};
