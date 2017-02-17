#pragma once

#include <mutex>
#include <map>
#include <functional>

template<class state_type, class event_type>
class moore
{

private:

	class internal_state
	{

	public:

		internal_state(std::function<void(void)> activation_callback)
		: activation_callback(activation_callback)
		{
		}

		internal_state(const internal_state&)				= delete;
		internal_state& operator=(const internal_state&)	= delete;

		internal_state(internal_state&& moved)
		: activation_callback(std::move(moved.activation_callback))
		, auto_edge(moved.auto_edge)
		, event_edges(std::move(moved.event_edges))
		{

		}

		internal_state& operator=(internal_state&& moved)
		{
			activation_callback = std::move(moved.activation_callback);
			auto_edge			= moved.auto_edge;
			event_edges			= std::move(moved.event_edges);
			return *this;
		}
		
		~internal_state(void)
		{
		}

		void make_edge(state_type target_state)
		{

			if(auto_edge.first)
			{
				throw std::exception("Cannot add a second auto edge");
			}
			else if(!event_edges.empty())
			{
				throw std::exception("Cannot insert an auto edge, there are event edges");
			}
			else
			{
				auto_edge.first		= true;
				auto_edge.second	= target_state;
			}

		}

		void make_edge(event_type event, state_type target_state)
		{

			if(auto_edge.first)
			{
				throw std::exception("Cannot add an event edge, there was a auto edge");
			}
			else if(!event_edges.insert(std::make_pair(event, target_state)).second)
			{
				throw std::exception("The event is already associated with an edge");
			}

		}

		state_type process(event_type event)
		{
			
			auto edge(event_edges.find(event));
			if(event_edges.end() == edge)
			{
				throw std::exception("There is no edge for that event");
			}

			return edge->second;

		}

		std::pair<bool, state_type> activate(void)
		{
			activation_callback();
			return auto_edge;
		}

	private:

		std::function<void(void)>			activation_callback;

		std::pair<bool, state_type>			auto_edge;
		std::map<event_type, state_type>	event_edges;

	};

public:

	moore(
		state_type					start_state, 
		std::function<void(void)>	start_state_callback
	)
	: start_state(start_state)
	{
		add_state(start_state, start_state_callback);
		set_current_state(start_state);
	}
	
	~moore(void)
	{
	}
	
	void add_state(state_type new_state, std::function<void(void)> callback)
	{

		std::lock_guard<std::mutex> lock(state_mutex);
		
		if(!states.insert(std::make_pair(new_state, internal_state(callback))).second)
		{
			throw std::exception("Could not add state");
		}

	}

	virtual void add_transition(state_type desired_source_state, state_type desired_target_state)
	{

		std::lock_guard<std::mutex> lock(state_mutex);

		auto found_source_state(states.find(desired_source_state));
		if(states.end() == found_source_state)
		{
			throw std::exception("Source state must be added before the edge");
		}

		auto found_target_state(states.find(desired_target_state));
		if(states.end() == found_target_state)
		{
			throw std::exception("Target state must be added before the edge");
		}

		found_source_state->second.make_edge(desired_target_state);

	}

	virtual void add_transition(state_type desired_source_state, event_type event, state_type desired_target_state)
	{

		std::lock_guard<std::mutex> lock(state_mutex);

		auto found_source_state(states.find(desired_source_state));
		if(states.end() == found_source_state)
		{
			throw std::exception("Source state must be added before the edge");
		}

		auto found_target_state(states.find(desired_target_state));
		if(states.end() == found_target_state)
		{
			throw std::exception("Target state must be added before the edge");
		}

		found_source_state->second.make_edge(event, desired_target_state);

	}

	virtual void handle_event(event_type event)
	{

		std::lock_guard<std::mutex> lock(state_mutex);

		auto found_state(states.find(current_state));
		if(states.end() == found_state)
		{
			throw std::exception("Source state must be added before the edge");
		}
		else
		{
			set_current_state(found_state->second.process(event));
		}

	}

private:

	void set_current_state(state_type desired_state)
	{

		auto desired_state_it(states.find(desired_state));
		if(states.end() == desired_state_it)
		{
			throw std::exception("Internal Error! Attempting to set invalid state");
		}

		current_state = desired_state_it->first;

		auto subsequent(desired_state_it->second.activate());
		if(subsequent.first)
		{
			set_current_state(subsequent.second);
		}
		
	}

private:

	mutable std::mutex						state_mutex;
	std::map<state_type, internal_state>	states;
	state_type								start_state;
	state_type								current_state;

};
