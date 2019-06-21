/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <map>
#include <string>

#include "graph.hpp"
#include "measurements.hpp"
#include "visualization.hpp"


using namespace std;

// events


/// Event.
struct Event
{
  /// Event type.
  Event_Type type;

  /// Event info.
  string info;

  /// Standard constructor.
  Event(): type(TN), info("") {}

  /// Constructor.
  Event(Event_Type e, string i): type(e), info(i) {}
};



/// Vertex event.
struct Vertex_Event: public Event
{
  /// Product vertex.
  Product_Vertex vertex;

  /// Standard constructor.
  Vertex_Event(): Event(), vertex(NULL_PRODUCT_VERTEX) {}

  /// Constructor.
  Vertex_Event(Event_Type e, string i, Product_Vertex v):
    Event(e, i), vertex(v) {}

  /// Assignment operator.
  Vertex_Event& operator=(Vertex_Event const& event)
  {
    type = event.type;
    info = event.info;
    vertex = event.vertex;
    return *this;
  }
};

/// Edge event.
struct Edge_Event: public Event
{
  /// Tail product vertex.
  Product_Vertex tail;

  /// Head product vertex.
  Product_Vertex head;

  /// Standard constructor.
  Edge_Event(): Event(), tail(NULL_PRODUCT_VERTEX), head(NULL_PRODUCT_VERTEX)
  {}

  /// Constructor.
  Edge_Event(Event_Type e, string i, Product_Vertex t, Product_Vertex h):
    Event(e, i), tail(t), head(h) {}

  /// Assignment operator.
  Edge_Event& operator=(Edge_Event const& event)
  {
    type = event.type;
    info = event.info;
    tail = event.tail;
    head = event.head;
    return *this;
  }
};



// ----------------------------------------------------------------------------


/// Event handler.
class Event_Handler
{
  /// Event ID.
  int time_stamp;

  /// Measurements.
  Measurements measurements;

  /// History.
  History history;

  /// Visualization.
  Visualization visualization;

  /// Record history?
  bool record;

  /// Write visualization?
  bool visualize;


public:
  /// Constructor.
  Event_Handler(): time_stamp(1), measurements(), history(), visualization(),
		   record(false), visualize(false)
  {
    visualization.init();
  }

  /// Clear.
  void clear()
  {
    time_stamp = 1;
    measurements.clear();
    history.clear();
    visualization.clear();
  }

  /// Set record.
  void set_record(bool rec) { record = rec; }

  /// Set visualize.
  void set_visualize(bool vis) { visualize = vis; }

  /// Set graph.
  void set_graph(Network_Graph& gr)
  {
    visualization.set_graph(gr);
  }

  /// Set output filename.
  void set_outfile_name(string out)
  {
    visualization.set_outfile_name(out);
  }

  /// Set output filename affix.
  void set_filename_affix(string affix)
  {
    visualization.set_filename_affix(affix);
  }

  /// Set trace mode.
  void set_trace_mode(bool tr)
  {
    history.set_trace_mode(tr);
  }

  /// Set detailed visualization.
  void set_detailed_vis(bool det)
  {
    visualization.set_detailed_vis(det);
  }

  /// Handle vertex event.
  void handle_event(Vertex_Event& event);

  /// Handle edge event.
  void handle_event(Edge_Event& event);

  /// Print measurements.
  void print_measurements() { measurements.print(); }
};


void Event_Handler::handle_event(Vertex_Event& event)
{
  Network_Vertex* vertex = event.vertex.network();
  switch (event.type)
  {
  case TN:
    measurements.vertex_touched(vertex);
    if (visualize)
      visualization.set_vertex_color(vertex, BLUE);
    break;
  case TNB:
    measurements.vertex_touched(vertex);
    if (visualize)
      visualization.set_vertex_color(vertex, BLUEVIOLET);
    break;
  case VN:
    measurements.vertex_visited(vertex);
    if (visualize)
    {
      visualization.set_vertex_color(vertex, RED);
      visualization.set_vertex_style(vertex, BOLD);
    }
    break;
  case VNB:
    measurements.vertex_visited(vertex);
    if (visualize)
    {
      visualization.set_vertex_color(vertex, ORANGE);
      visualization.set_vertex_style(vertex, BOLD);
    }
    break;
  case TE:
    break;
  case TEB:
    break;
  case VE:
    break;
  case VEB:
    break;
  }

  if (record)
  {
    History_Info info(time_stamp, event.type, event.info);
    history.add_event(info);
  }
  if (visualize)
    visualization.write(time_stamp);
  time_stamp++;
}


void Event_Handler::handle_event(Edge_Event& event)
{
  Network_Vertex* tail = event.tail.network();
  Network_Vertex* head = event.head.network();
  switch (event.type)
  {
  case TN:
    break;
  case TNB:
    break;
  case VN:
    break;
  case VNB:
    break;
  case TE:
    measurements.edge_touched(tail, head);
    if (visualize)
    {
      visualization.set_edge_color(tail, head, BLUE);
      visualization.set_edge_style(tail, head, DASHED);
      visualization.write(time_stamp);
      visualization.set_edge_style(tail, head, SOLID);
    }
    break;
  case TEB:
    measurements.edge_touched(tail, head);
    if (visualize)
    {
      visualization.set_edge_color(tail, head, BLUEVIOLET);
      visualization.set_edge_style(tail, head, DASHED);
      visualization.write(time_stamp);
      visualization.set_edge_style(tail, head, SOLID);
    }
    break;
  case VE:
    measurements.edge_visited(tail, head);
    if (visualize)
    {
      visualization.set_edge_color(tail, head, RED);
      visualization.set_edge_style(tail, head, DASHED);
      visualization.write(time_stamp);
      visualization.set_edge_style(tail, head, SOLID);
    }
    break;
  case VEB:
    measurements.edge_visited(tail, head);
    if (visualize)
    {
      visualization.set_edge_color(tail, head, ORANGE);
      visualization.set_edge_style(tail, head, DASHED);
      visualization.write(time_stamp);
      visualization.set_edge_style(tail, head, SOLID);
    }
    break;
  }

  if (record)
  {
    History_Info info(time_stamp, event.type, event.info);
    history.add_event(info);
  }
  time_stamp++;
}


#endif
