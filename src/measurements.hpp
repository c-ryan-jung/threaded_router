/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef MEASUREMENTS_HPP
#define MEASUREMENTS_HPP

#include <list>
#include <string>

#include "basics.hpp"
#include "graph.hpp"


using namespace std;

// ----------------------------------------------------------------------------

// event history


/// History info.
class History_Info
{
  /// Time stamp.
  int time_stamp;

  /// Event type.
  Event_Type type;

  /// Description.
  string information;


public:
  /// Constructor.
  History_Info(int t, Event_Type e, string i):
    time_stamp(t), type(e), information(i) {}

  /// Get time stamp as string.
  string time_stamp_string() const
  {
    string s(itos(time_stamp));
    return s;
  }

  /// Get event type as string.
  string event_string() const
  {
    string s;
    switch (type)
    {
    case TN: s = "TN"; break;
    case TNB: s = "TNB"; break;
    case VN: s = "VN"; break;
    case VNB: s = "VNB"; break;
    case TE: s = "TE"; break;
    case TEB: s = "TEB"; break;
    case VE: s = "VE"; break;
    case VEB: s = "VEB"; break;
    }
    return s;
  }

  /// Get info.
  string info() const { return information; }
};



/// History.
class History
{
  /// Record.
  list<History_Info> record;

  /// Trace mode.
  bool trace;


public:
  /// Constructor.
  History(): trace(false) {}

  /// Clear.
  void clear()
  {
    record.clear();
  }

  /// Set trace mode.
  void set_trace_mode(bool tr) { trace = tr; }

  /// Add event.
  void add_event(History_Info h)
  {
    record.push_back(h);
    if (trace)
      cout << h.time_stamp_string() << ": "
	   << h.event_string() << " "
	   << h.info() << endl;
  }

  /// Print history.
  void print()
  {
    cout << "History: " << endl;
    for (list<History_Info>::iterator it = record.begin();
	 it != record.end(); ++it)
      cout << it->time_stamp_string() << ": "
	   << it->event_string() << " "
	   << it->info() << endl;
  }
};



// ----------------------------------------------------------------------------


/// Measurements.
class Measurements
{
  /// Vertices inserted in queue.
  int touched_vertices;

  /// Vertices extracted from queue (scanned vertices).
  int visited_vertices;

  /// Scanned edges.
  int touched_edges;

  /// Relaxed edges.
  int visited_edges;


public:
  /// Constructor.
  Measurements(): touched_vertices(0), visited_vertices(0),
		  touched_edges(0), visited_edges(0) {}

  /// Clear.
  void clear()
  {
    touched_vertices = 0;
    visited_vertices = 0;
    touched_edges = 0;
    visited_edges = 0;
  }

  /// Register touched vertex.
  void vertex_touched(const Network_Vertex* v)
  {
    touched_vertices++;
  }

  /// Register visited vertex.
  void vertex_visited(const Network_Vertex* v)
  {
    visited_vertices++;
  }

  /// Register touched edge.
  void edge_touched(const Network_Vertex* v1, const Network_Vertex* v2)
  {
    touched_edges++;
  }

  /// Register visited edge.
  void edge_visited(const Network_Vertex* v1, const Network_Vertex* v2)
  {
    visited_edges++;
  }

  /// Print measurements.
  void print()
  {
    cout << "Measurements:" << endl;
    cout << "#tn: " << touched_vertices << endl
	 << "#vn: " << visited_vertices << endl
         << "#te: " << touched_edges << endl
         << "#ve: " << visited_edges << endl;
  }
};


#endif
