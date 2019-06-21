/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <ext/hash_map>

#include "basics.hpp"
#include "events.hpp"
#include "graph.hpp"

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

using namespace std;
using namespace log4cplus;

// ----------------------------------------------------------------------------

// globals


/// Event handler.
extern Event_Handler event_handler;



// ----------------------------------------------------------------------------


/// \todo Switch from map to hash_map.
namespace __gnu_cxx
{
  /// Hash for Product_Vertex.
  template <>
  class hash<Product_Vertex>
  {
  public:
    /// () operator.
    size_t operator()(const Product_Vertex& vertex) const
    {
      return vertex.network()->external_id() * vertex.nfa()->id();
    }
  };
}



/// Class encapsulating less-operator for Product_Vertex.
struct Product_Vertex_Lt
{
  bool operator()(const Product_Vertex& v1, const Product_Vertex& v2) const
  {
    // HSM: Apr 2019 - This expression gets ambiguity messages. C++
    //    precedence rules states that it will be parsed
    //    left-to-right. This may not be as intended (seems like ||
    //    splits the expression, but we are not using Products for the
    //    reg exp router, so leave it as it.

    return (   (v1.network() < v2.network()) ||
	        v1.network() == v2.network() && v1.nfa() < v2.nfa());
  }
};



// ----------------------------------------------------------------------------

// queue-related classes


/// Infinity.
const Cost_Function INF = 1e100;



/// Product_Vertex inserted in queue plus additional information.
class Touched_Vertex
{
protected:
  /// Touched vertex.
  Product_Vertex touched_vertex;

  /// Distance.
  Cost_Function distance;

  /// Parent vertex.
  Product_Vertex parent_vertex;

  /// Is touched vertex valid?
  bool is_valid;

  // edge_label
  Label edge_label;


public:
  /// Standard constructor.
  // edge_label
  Touched_Vertex():
    touched_vertex(NULL_PRODUCT_VERTEX), distance(INF),
    parent_vertex(NULL_PRODUCT_VERTEX), is_valid(true), edge_label() {}

  /// updated Constructor with edge_label
  Touched_Vertex(Product_Vertex v, Cost_Function d,
		 Product_Vertex p, Label e):
    touched_vertex(v), distance(d), parent_vertex(p), is_valid(true),
    edge_label(e) {}

  // return edge_label
  const Label& label() const { return edge_label; }

  /// updated Assignment operator with edge_label.
  Touched_Vertex& operator=(const Touched_Vertex& vertex)
  {
    touched_vertex = vertex.vertex();
    distance = vertex.dist();
    parent_vertex = vertex.parent();
    is_valid = vertex.valid();
    edge_label = vertex.edge_label;
    return *this;
  }

  /// Set distance.
  void set_dist(Cost_Function new_dist)
  {
    distance = new_dist;
  }

  /// Set parent.
  void set_parent(Product_Vertex new_parent)
  {
    parent_vertex = new_parent;
  }

  /// Set valid.
  void set_valid(bool new_valid)
  {
    is_valid = new_valid;
  }

  /// Get touched vertex.
  Product_Vertex vertex() const { return touched_vertex; }

  /// Get distance.
  Cost_Function dist() const { return distance; }

  /// Get parent vertex.
  Product_Vertex parent() const { return parent_vertex; }

  /// Get valid.
  bool valid() const { return is_valid; }

  /// Less-operator.
  bool operator<(const Touched_Vertex& vertex) const
  {
    return distance > vertex.dist();
  }

  /// Return info string.
  // edge_label: pending
  string info()
  {
    string info_text = "";
    if (touched_vertex == NULL_PRODUCT_VERTEX)
      info_text += "NULL";
    else info_text += touched_vertex.info();
    info_text += " <";
    if (parent_vertex == NULL_PRODUCT_VERTEX)
      info_text += "NULL";
    else info_text += parent_vertex.info();
    info_text += " (" + ftos(distance) + ")";
    return info_text;
  }
};


/// Output operator.
ostream& operator<<(ostream& out, Touched_Vertex& vertex)
{
  out << vertex.info();
  return out;
}


/// Class encapsulating less-operator for Touched_Vertex.
struct Touched_Vertex_Lt
{
  bool operator()(const Touched_Vertex* v1, const Touched_Vertex* v2) const
  {
    return (v1->dist() > v2->dist());
  }
};



/// Queue keeping Touched_Vertex*.
typedef priority_queue<Touched_Vertex*, vector<Touched_Vertex*>,
		       Touched_Vertex_Lt> Touched_Vertex_Queue;


/// Priority queue.
class Priority_Queue: public Touched_Vertex_Queue
{
public:
  /// Check queue.
  void check_queue()
  {
    Touched_Vertex_Queue q = (Touched_Vertex_Queue)*this;
    while (!q.empty())
    {
      cout << "check_queue " << *q.top() << endl;
      q.pop();
    }
  }

  /// Empty function.
  bool empty()
  {
    while (!Touched_Vertex_Queue::empty() &&
	   !Touched_Vertex_Queue::top()->valid())
      pop();
    return Touched_Vertex_Queue::empty();
  }

  /// Pop function.
  Touched_Vertex* pop()
  {
    Touched_Vertex* popped_vertex = Touched_Vertex_Queue::top();
    Touched_Vertex_Queue::pop();
    return popped_vertex;
  }

  /// Decrease key.
  void decrease(Touched_Vertex*& vertex, Cost_Function new_dist,
		Product_Vertex new_parent)
  {
    vertex->set_valid(false);
    // change to accomodate edge_label
    vertex = new Touched_Vertex(vertex->vertex(), new_dist, new_parent,
    vertex->label());
    push(vertex);
  }
};




// ----------------------------------------------------------------------------


/// Trip request.
struct Trip_Request
{
  /// Source vertex.
  long int source;

  /// Destination vertex.
  long int destination;

  /// Start time.
  float start_time;

  // HSM
  // id of trip
  long unsigned int id;
  float nfaID;
};


/// Output operator.
ostream& operator<<(ostream& out, Trip_Request& request)
{
  out << "Trip request:"  << endl
      << " source:      " << request.source << endl
      << " destination: " << request.destination << endl
      << " start time:  " << request.start_time << endl
      << " id : "         << request.id << endl
      << " nfaID : "      << request.nfaID << endl;
  return out;
}



// ----------------------------------------------------------------------------


/// Location.
struct Location
{
  /// External ID.
  long external_id;

  /// Time.
  float time;

  // putting travelled mode here
  Label edge_label;

  /// new Constructor.
  // updated constructor to accomodate edge_label
  Location(long int e, float t, Label l):
    external_id(e), time(t), edge_label(l) {}
};



// ----------------------------------------------------------------------------


/// Plan.
struct Plan
{
  /// Path description.
  list<Location> path;

  /// Reverse path.
  void reverse() { path.reverse(); }
};



/// Plan iterator.
typedef list<Location>::iterator Plan_Iterator;



// ----------------------------------------------------------------------------


/// Dijkstra's algorithm.
class Shortest_Path
{
protected:
  /// Network graph.
  Network_Graph& network;

  /// NFA graph.
  NFA_Graph& nfa;

  /// Source vertex.
  Network_Vertex* source;

  /// Destination vertex.
  Network_Vertex* destination;

  /// Start time.
  float start_time;

  /// Priority queue.
  Priority_Queue queue;

  /// Touched-vertex info.
  map<Product_Vertex, Touched_Vertex*, Product_Vertex_Lt> vertex_info;

  /// Current product vertex.
  Touched_Vertex* curr_touched;

  /// Logger.
  Logger dijkstra_logger;


public:
  /// Constructor.
  Shortest_Path(Network_Graph& n1, NFA_Graph& n2):
    network(n1), nfa(n2), source(NULL), destination(NULL), start_time(0),
    queue(), vertex_info(), curr_touched(NULL),
    dijkstra_logger(Logger::getInstance("Shortest_Path"))
  {
    dijkstra_logger.addAppender(myConsoleAppender);
    dijkstra_logger.setLogLevel(INFO_LOG_LEVEL);
  }

  /// Destructor.
  /// \todo To be implemented.
  virtual ~Shortest_Path() {}

  /// Initialization.
  virtual void init(const Trip_Request& trip);

  /// Get edge cost.
  virtual Cost_Function cost(Network_Edge* edge) const
  {
    return edge->cost();
  }

  /// Insert vertex in queue.
  virtual void push(Touched_Vertex* touched_vertex)
  {
    queue.push(touched_vertex);

    // event handling
    string info(itos(touched_vertex->vertex().network()->external_id())
		+ " [" + itos(touched_vertex->vertex().nfa()->id()) + "]");
    Vertex_Event vertex_event(TN, info, touched_vertex->vertex());
    event_handler.handle_event(vertex_event);
  }

  /// Get next vertex from queue.
  virtual void pop()
  {
    curr_touched = queue.top();
    queue.pop();

    // event handling
    string info(itos(curr_touched->vertex().network()->external_id())
		+ " [" + itos(curr_touched->vertex().nfa()->id()) + "]");
    Vertex_Event vertex_event(VN, info, curr_touched->vertex());
    event_handler.handle_event(vertex_event);
  }

  /// Scan edge.
  virtual void scan(Product_Neighbor_Iterator& neighbor_it)
  {
    // event handling
    string info = itos(neighbor_it.tail().network()->external_id()) + "--" +
      itos(neighbor_it.head().network()->external_id());
    Edge_Event edge_event(TE, info, neighbor_it.tail(), neighbor_it.head());
    event_handler.handle_event(edge_event);
  }

  /// Does edge have to be considered?
  virtual bool relevant(Product_Neighbor_Iterator& neighbor_it);

  /// Relax edge.
  virtual void relax(Product_Neighbor_Iterator& neighbor_it);

  /// Has destination vertex been reached?
  virtual bool finished()
  {
    return (curr_touched->vertex().network() == destination &&
	    curr_touched->vertex().nfa()->accepting());
  }

  /// Is queue empty?
  virtual bool queue_empty() { return queue.empty(); }

  /// Run Dijkstra's algorithm.
  virtual void dijkstra();

  /// Reconstruct path.
  virtual void reconstruct_path(Plan& plan);
};


void Shortest_Path::init(const Trip_Request& trip)
{
  LOG4CPLUS_DEBUG(dijkstra_logger, "Initializing...");
  // identify trip request data
  source = network[network.internal_id(trip.source)];
  destination = network[network.internal_id(trip.destination)];
  start_time = trip.start_time;

  vertex_info.clear();
  queue = Priority_Queue();

  // process source vertex
  Product_Vertex source_product(source, nfa.start().front());
  // currently made edge_label=-1 for the first node
  Touched_Vertex* touched_vertex =
    new Touched_Vertex(source_product, start_time, NULL_PRODUCT_VERTEX, -1);
  push(touched_vertex);
  vertex_info[source_product] = touched_vertex;
  LOG4CPLUS_DEBUG(dijkstra_logger, "Initialized.");
}


bool Shortest_Path::relevant(Product_Neighbor_Iterator& neighbor_it)
{
  if (vertex_info[neighbor_it.head()] == NULL)
  {
    // a new touched vertex for empty queue: added edge_label
    Touched_Vertex* touched_vertex =
      new Touched_Vertex(neighbor_it.head(), INF, neighbor_it.tail(),neighbor_it.label());
    vertex_info[neighbor_it.head()] = touched_vertex;
    push(touched_vertex);
  }
  return (curr_touched->dist() + cost(neighbor_it.network_edge()) <
	  vertex_info[neighbor_it.head()]->dist());
}


void Shortest_Path::relax(Product_Neighbor_Iterator& neighbor_it)
{
  Cost_Function new_dist = vertex_info[neighbor_it.tail()]->dist() +
    cost(neighbor_it.network_edge());
  queue.decrease(vertex_info[neighbor_it.head()], new_dist,
		 neighbor_it.tail());

  // event handling
  string info(itos(neighbor_it.tail().network()->external_id()) + "--" +
	      itos(neighbor_it.head().network()->external_id()) +
	      " (" + ftos(new_dist) + ")");
  Edge_Event edge_event(VE, info, neighbor_it.tail(), neighbor_it.head());
  event_handler.handle_event(edge_event);
}


void Shortest_Path::dijkstra()
{
  LOG4CPLUS_DEBUG(dijkstra_logger, "Running search...");
  do
  {
    LOG4CPLUS_DEBUG(dijkstra_logger, "Popping item...");
    pop();
    LOG4CPLUS_DEBUG(dijkstra_logger, "Popped item: " + curr_touched->info());
    LOG4CPLUS_DEBUG(dijkstra_logger, "For all incident edges...");
    for (Product_Neighbor_Iterator neighbor_it(curr_touched->vertex());
	 neighbor_it.valid(); ++neighbor_it)
    {
      LOG4CPLUS_DEBUG(dijkstra_logger, "Scanning edge " + neighbor_it.info() + " ...");
      scan(neighbor_it);
      LOG4CPLUS_DEBUG(dijkstra_logger, "Testing relevance...");
      if (relevant(neighbor_it))
      {
	LOG4CPLUS_DEBUG(dijkstra_logger, "Relaxing edge...");
	relax(neighbor_it);
      }
      LOG4CPLUS_DEBUG(dijkstra_logger, "Edge finished...");
    }
  } while (!finished() && !queue_empty());
  LOG4CPLUS_DEBUG(dijkstra_logger, "Search finished.");
}


void Shortest_Path::reconstruct_path(Plan& plan)
{
  LOG4CPLUS_DEBUG(dijkstra_logger, "Reconstructing path...");
  if (finished())
  {
    while (curr_touched->parent()!=NULL_PRODUCT_VERTEX)
    {
      // updated for edge_label
      plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				    curr_touched->dist(),curr_touched->label()));
      curr_touched = vertex_info[curr_touched->parent()];
    }
      // updated for edge_label
    plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				  curr_touched->dist(),curr_touched->label()));
  }
  LOG4CPLUS_DEBUG(dijkstra_logger, "Path reconstructed.");
}



// ----------------------------------------------------------------------------


/// Goal-directed Dijkstra.
class Goal_Dijkstra: public Shortest_Path
{
protected:
  /// Maximum speed.
  float max_speed;


public:
  /// Constructor.
  Goal_Dijkstra(Network_Graph& network, NFA_Graph& nfa):
    Shortest_Path(network, nfa)
  {
    max_speed = 0;
    for (Network_Graph::const_iterator vertex_it=network.begin();
	 vertex_it!=network.end(); vertex_it++)
      for (Network_Vertex::Edge_It edge_it=(*vertex_it)->out_edge_begin();
	   edge_it!=(*vertex_it)->out_edge_end(); edge_it++)
      {
	double curr_speed =
	  euclid_dist((*edge_it)->head(), (*edge_it)->tail()) / (*edge_it)->cost();
	if (curr_speed > max_speed)
	  max_speed = curr_speed;
      }
  }

  /// Get edge cost.
  /// \todo Retranslate edge costs in reconstruct_path.
  virtual Cost_Function cost(Network_Edge* edge) const
  {
    Cost_Function new_cost = edge->cost()
      + euclid_dist(edge->head(), destination) / max_speed
      - euclid_dist(edge->tail(), destination) / max_speed;
    assert(new_cost >= -0.001);
    return new_cost;
  }

  /// Reconstruct path.
  virtual void reconstruct_path(Plan& plan);
};


void Goal_Dijkstra::reconstruct_path(Plan& plan)
{
  LOG4CPLUS_DEBUG(dijkstra_logger, "Reconstructing path...");
  if (finished())
  {
    while (curr_touched->parent()!=NULL_PRODUCT_VERTEX)
    {
      // updated for edge_label
      plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				    curr_touched->dist()
				    - euclid_dist(curr_touched->vertex().network(),
						  destination) / max_speed
				    + euclid_dist(source, destination) / max_speed,
                curr_touched->label()));
      curr_touched = vertex_info[curr_touched->parent()];
    }
      // updated for edge_label
    plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				  curr_touched->dist(),curr_touched->label()));
  }
  LOG4CPLUS_DEBUG(dijkstra_logger, "Path reconstructed.");
}



// ----------------------------------------------------------------------------


/// Bidirectional Dijkstra.
class Bi_Dijkstra: public Shortest_Path
{
protected:
  /// Network for backward search.
  Network_Graph network_back;

  /// Mapping from original to backward network.
  map<Network_Vertex*, Network_Vertex*> network_back_vertex;

  /// Mapping from backward to original network.
  map<Network_Vertex*, Network_Vertex*> network_orig_vertex;

  /// Mapping from original to backward NFA.
  map<NFA_Vertex*, NFA_Vertex*> nfa_back_vertex;

  /// Mapping from backward to original NFA.
  map<NFA_Vertex*, NFA_Vertex*> nfa_orig_vertex;

  /// NFA for backward search.
  NFA_Graph nfa_back;

  /// Queue for backward search.
  Priority_Queue queue_back;

  /// Switch for queue to be considered next.
  /// 1 == queue, -1 == queue_back.
  int queue_number;

  /// Has vertex been removed from queue (scanned)?
  map<Product_Vertex, bool, Product_Vertex_Lt> visited;

  /// Shortest distance so far.
  double shortest_distance;

  /// Last vertex from forward search on shortest path.
  Product_Vertex link_vertex_for;

  /// Last vertex from backward search on shortest path.
  Product_Vertex link_vertex_back;

  /// Logger.
  Logger bi_logger;


public:
  /// Constructor.
  Bi_Dijkstra(Network_Graph& network, NFA_Graph& nfa):
    Shortest_Path(network, nfa), network_back_vertex(), network_orig_vertex(),
    nfa_back_vertex(), nfa_orig_vertex(), queue_back(),
    queue_number(1), visited(), shortest_distance(INF),
    link_vertex_for(NULL_PRODUCT_VERTEX), link_vertex_back(NULL_PRODUCT_VERTEX),
    bi_logger(Logger::getInstance("Bi_Dijkstra"))
  {
    network.construct_back_graph(network_back, network_back_vertex,
				 network_orig_vertex);
    nfa.construct_back_graph(nfa_back, nfa_back_vertex,
			     nfa_orig_vertex);
    bi_logger.addAppender(myConsoleAppender);
    bi_logger.setLogLevel(INFO_LOG_LEVEL);
  }

  /// Initialization.
  virtual void init(const Trip_Request& trip);

  /// Insert vertex in queue.
  virtual void push(Touched_Vertex* touched_vertex)
  {
    if (queue_number == 1)
      queue.push(touched_vertex);
    else queue_back.push(touched_vertex);

    // event handling
    string info(itos(touched_vertex->vertex().network()->external_id())
		+ " [" + itos(touched_vertex->vertex().nfa()->id()) + "]");
    Vertex_Event vertex_event(queue_number==1 ? TN : TNB, info, touched_vertex->vertex());
    event_handler.handle_event(vertex_event);
  }

  /// Get next vertex from queue.
  virtual void pop()
  {
    queue_number *= -1;
    LOG4CPLUS_TRACE(bi_logger, "Queue number: " + itos(queue_number));
    LOG4CPLUS_TRACE(bi_logger, "Forward queue empty: " + itos(queue.empty()));
    LOG4CPLUS_TRACE(bi_logger, "Backward queue empty: " + itos(queue_back.empty()));

    // HSM: Prior expression:
    // if (queue_number == 1 && !queue.empty() ||
    // 	queue_number == -1 && queue_back.empty())
    if ( (queue_number == 1 && !queue.empty()      ) ||
	 (queue_number == -1 && queue_back.empty() )    )
    {
      queue_number = 1;
      LOG4CPLUS_TRACE(bi_logger, "Popping from forward queue...");
      curr_touched = queue.top();
      queue.pop();
    }
    // HSM: Prior expression:
    // else if (queue_number == -1 && !queue_back.empty() ||
    // 	     queue_number == 1 && queue.empty())
    // HSM: New expression:
    else if (  (queue_number == -1 && !queue_back.empty()) ||
	       (queue_number == 1  && queue.empty()      ) )
    {
      queue_number = -1;
      LOG4CPLUS_TRACE(bi_logger, "Popping from backward queue...");
      curr_touched = queue_back.top();
      queue_back.pop();
    }
    visited[curr_touched->vertex()] = true;
    LOG4CPLUS_TRACE(bi_logger, "Visited vertex: " + curr_touched->vertex().info());

    // event handling
    string info(itos(curr_touched->vertex().network()->external_id())
		+ " [" + itos(curr_touched->vertex().nfa()->id()) + "]");
    Vertex_Event vertex_event(queue_number==1 ? VN : VNB, info, curr_touched->vertex());
    event_handler.handle_event(vertex_event);
  }

  /// Scan edge.
  virtual void scan(Product_Neighbor_Iterator& neighbor_it)
  {
    if (queue_number == 1)
    {
      Product_Vertex head_product_back(network_back_vertex[neighbor_it.head().network()], nfa_back_vertex[neighbor_it.head().nfa()]);
      if (vertex_info[head_product_back] != NULL &&
	  vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_back]->dist() < shortest_distance)
      {
	shortest_distance = vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_back]->dist();
	link_vertex_for = neighbor_it.head();
	link_vertex_back = head_product_back;
	LOG4CPLUS_TRACE(bi_logger, "Link vertices: " + link_vertex_for.info() + "  "
			+ link_vertex_back.info());
      }
    }
    else
    {
      Product_Vertex head_product_orig(network_orig_vertex[neighbor_it.head().network()], nfa_orig_vertex[neighbor_it.head().nfa()]);
      if (vertex_info[head_product_orig] != NULL &&
	  vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_orig]->dist() < shortest_distance)
      {
	shortest_distance = vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_orig]->dist();
	link_vertex_for = Product_Vertex(network_orig_vertex[neighbor_it.head().network()], nfa_orig_vertex[neighbor_it.head().nfa()]);
	link_vertex_back = neighbor_it.head();
	LOG4CPLUS_TRACE(bi_logger, "Link vertices: " + link_vertex_for.info() + "  "
			+ link_vertex_back.info());
      }
    }

    // event handling
    string info = itos(neighbor_it.tail().network()->external_id()) + "--" +
      itos(neighbor_it.head().network()->external_id());
    Edge_Event edge_event(queue_number==1 ? TE : TEB, info, neighbor_it.tail(),
			  neighbor_it.head());
    event_handler.handle_event(edge_event);
  }

  /// Relax edge.
  virtual void relax(Product_Neighbor_Iterator& neighbor_it);

  /// Has destination vertex been reached?
  virtual bool finished()
  {
    bool return_value = false;
    if (queue_number == 1)
    {
      Product_Vertex product_vertex(network_back_vertex[curr_touched->vertex().network()],
				    nfa_back_vertex[curr_touched->vertex().nfa()]);
      LOG4CPLUS_TRACE(bi_logger, "Current backward vertex: " + product_vertex.info()
		      + "  visited: " + itos(visited[product_vertex]));
      if (visited[product_vertex])
	return_value = true;
    }
    else if (queue_number == -1)
    {
      Product_Vertex product_vertex(network_orig_vertex[curr_touched->vertex().network()],
				    nfa_orig_vertex[curr_touched->vertex().nfa()]);
      LOG4CPLUS_TRACE(bi_logger, "Current original vertex: " + product_vertex.info()
		      + "  visited: " + itos(visited[product_vertex]));
      if (visited[product_vertex])
	return_value = true;
    }
    LOG4CPLUS_TRACE(bi_logger, "Shortest distance: " + ftos(shortest_distance));
    return return_value;
  }

  /// Are queues empty?
  virtual bool queue_empty() { return queue.empty() && queue_back.empty(); }

  /// Reconstruct path.
  virtual void reconstruct_path(Plan& plan);
};


void Bi_Dijkstra::init(const Trip_Request& trip)
{
  queue_number = 1;
  Shortest_Path::init(trip);
  queue_back = Priority_Queue();
  visited.clear();
  link_vertex_for = NULL_PRODUCT_VERTEX;
  link_vertex_back = NULL_PRODUCT_VERTEX;
  shortest_distance = INF;

  // process source vertices for backward graph
  queue_number = -1;
  list<NFA_Vertex*> start_states = nfa_back.start();
  while (!start_states.empty())
  {
    Product_Vertex source_product(network_back_vertex[destination],
				  start_states.front());
    start_states.pop_front();
  // currently made edge_label=-1
    Touched_Vertex* touched_vertex =
      new Touched_Vertex(source_product, 0, NULL_PRODUCT_VERTEX,-1);
    push(touched_vertex);
    vertex_info[source_product] = touched_vertex;
  }
}


void Bi_Dijkstra::relax(Product_Neighbor_Iterator& neighbor_it)
{
  Cost_Function new_dist = vertex_info[neighbor_it.tail()]->dist() +
    cost(neighbor_it.network_edge());
  if (queue_number == 1)
    queue.decrease(vertex_info[neighbor_it.head()], new_dist,
		   neighbor_it.tail());
  else queue_back.decrease(vertex_info[neighbor_it.head()], new_dist,
			   neighbor_it.tail());

  // event handling
  string info(itos(neighbor_it.tail().network()->external_id()) + "--" +
	      itos(neighbor_it.head().network()->external_id()) +
	      " (" + ftos(new_dist) + ")");
  Edge_Event edge_event(queue_number==1 ? VE : VEB, info, neighbor_it.tail(),
			neighbor_it.head());
  event_handler.handle_event(edge_event);
}


void Bi_Dijkstra::reconstruct_path(Plan& plan)
{
  // edge_label of the previous touched vertex
  Label prev_edge_label;

  LOG4CPLUS_DEBUG(bi_logger, "Reconstructing path...");
  if (finished())
  {
    LOG4CPLUS_TRACE(bi_logger, "Link vertex forward: " + link_vertex_for.info());
    curr_touched = vertex_info[link_vertex_for];
    LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
    Cost_Function link_dist_for;
    if (curr_touched->parent() != NULL_PRODUCT_VERTEX)
    {
      link_dist_for = curr_touched->dist();
      do
      {
      // updated for edge_label
	plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				      curr_touched->dist(),curr_touched->label()));
	curr_touched = vertex_info[curr_touched->parent()];
	LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
      } while (curr_touched->parent() != NULL_PRODUCT_VERTEX);
    }
      // updated for edge_label
    plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				  curr_touched->dist(),curr_touched->label()));
    LOG4CPLUS_TRACE(bi_logger, "Link vertex backward: " + link_vertex_back.info());
    curr_touched = vertex_info[link_vertex_back];
    LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
    Cost_Function link_dist_back;
    if (curr_touched->parent() != NULL_PRODUCT_VERTEX)
    {
      link_dist_back = curr_touched->dist();
      while (curr_touched->parent() != NULL_PRODUCT_VERTEX)
      {
         // edge_label of the previous touched vertex
         prev_edge_label = curr_touched->label();
	curr_touched = vertex_info[curr_touched->parent()];
	LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
      // updated for edge_label
	plan.path.push_back(Location(curr_touched->vertex().network()->external_id(),
				     link_dist_for + link_dist_back -
                 curr_touched->dist(),prev_edge_label));
      }
    }
  }
  LOG4CPLUS_DEBUG(bi_logger, "Path reconstructed.");
}



// ----------------------------------------------------------------------------


/// Bidirectional goal-directed Dijkstra.
class Bi_Goal_Dijkstra: public Bi_Dijkstra
{
protected:
  /// Maximum speed.
  float max_speed;


public:
  /// Constructor.
  Bi_Goal_Dijkstra(Network_Graph& network, NFA_Graph& nfa):
    Bi_Dijkstra(network, nfa)
  {
    max_speed = 0;
    for (Network_Graph::const_iterator vertex_it=network.begin();
	 vertex_it!=network.end(); vertex_it++)
      for (Network_Vertex::Edge_It edge_it=(*vertex_it)->out_edge_begin();
	   edge_it!=(*vertex_it)->out_edge_end(); edge_it++)
      {
	double curr_speed = euclid_dist((*edge_it)->head(), (*edge_it)->tail());
	if (curr_speed > max_speed)
	  max_speed = curr_speed;
      }
  }

  /// Get edge cost.
  virtual Cost_Function cost(Network_Edge* edge) const
  {
    Cost_Function new_cost = edge->cost()
      + euclid_dist(edge->head(), destination) / max_speed
      - euclid_dist(edge->tail(), destination) / max_speed;
    assert(new_cost >= -0.001);
    return new_cost;
  }

  /// Get backward edge cost.
  virtual Cost_Function cost_back(Network_Edge* edge) const
  {
    Cost_Function new_cost = edge->cost()
      - euclid_dist(edge->head(), destination) / max_speed
      + euclid_dist(edge->tail(), destination) / max_speed;
    assert(new_cost >= -0.001);
    return new_cost;
  }

  /// Scan edge.
  virtual void scan(Product_Neighbor_Iterator& neighbor_it)
  {
    if (queue_number == 1)
    {
      Product_Vertex head_product_back(network_back_vertex[neighbor_it.head().network()], nfa_back_vertex[neighbor_it.head().nfa()]);
      if (vertex_info[head_product_back] != NULL &&
	  vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_back]->dist() < shortest_distance)
      {
	shortest_distance =  vertex_info[neighbor_it.tail()]->dist() +
	  cost(neighbor_it.network_edge()) +
	  vertex_info[head_product_back]->dist();
	link_vertex_for = neighbor_it.head();
	link_vertex_back = head_product_back;
      }
    }
    else
    {
      Product_Vertex head_product_orig(network_orig_vertex[neighbor_it.head().network()], nfa_orig_vertex[neighbor_it.head().nfa()]);
      if (vertex_info[head_product_orig] != NULL &&
	  vertex_info[neighbor_it.tail()]->dist() +
	  cost_back(neighbor_it.network_edge()) +
	  vertex_info[head_product_orig]->dist() < shortest_distance)
      {
	shortest_distance =  vertex_info[neighbor_it.tail()]->dist() +
	  cost_back(neighbor_it.network_edge()) +
	  vertex_info[head_product_orig]->dist();
	link_vertex_for = Product_Vertex(network_orig_vertex[neighbor_it.head().network()], nfa_orig_vertex[neighbor_it.head().nfa()]);
	link_vertex_back = neighbor_it.head();
      }
    }

    // event handling
    string info = itos(neighbor_it.tail().network()->external_id()) + "--" +
      itos(neighbor_it.head().network()->external_id());
    Edge_Event edge_event(queue_number==1 ? TE : TEB, info, neighbor_it.tail(),
			      neighbor_it.head());
    event_handler.handle_event(edge_event);
  }

  /// Relax edge.
  void relax(Product_Neighbor_Iterator& neighbor_it);

  /// Reconstruct path.
  virtual void reconstruct_path(Plan& plan);
};


void Bi_Goal_Dijkstra::relax(Product_Neighbor_Iterator& neighbor_it)
{
  Cost_Function new_dist;
  new_dist = vertex_info[neighbor_it.tail()]->dist() +
    cost(neighbor_it.network_edge());
  if (queue_number == 1)
  {
    new_dist = vertex_info[neighbor_it.tail()]->dist() +
      cost(neighbor_it.network_edge());
    queue.decrease(vertex_info[neighbor_it.head()], new_dist,
		   neighbor_it.tail());
  }
  else
  {
    new_dist = vertex_info[neighbor_it.tail()]->dist() +
      cost_back(neighbor_it.network_edge());
    queue_back.decrease(vertex_info[neighbor_it.head()], new_dist,
			   neighbor_it.tail());
  }

  // event handling
  string info(itos(neighbor_it.tail().network()->external_id()) + "--" +
	      itos(neighbor_it.head().network()->external_id()) +
	      " (" + ftos(new_dist) + ")");
  Edge_Event edge_event(queue_number==1 ? VE : VEB, info, neighbor_it.tail(),
			    neighbor_it.head());
  event_handler.handle_event(edge_event);
}


void Bi_Goal_Dijkstra::reconstruct_path(Plan& plan)
{
  // edge_label of the previous touched vertex
  Label prev_edge_label;

  LOG4CPLUS_DEBUG(bi_logger, "Reconstructing path...");
  if (finished())
  {
    LOG4CPLUS_TRACE(bi_logger, "Link vertex forward: " + link_vertex_for.info());
    curr_touched = vertex_info[link_vertex_for];
    LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
    Cost_Function link_dist_for;
    if (curr_touched->parent() != NULL_PRODUCT_VERTEX)
    {
      link_dist_for = curr_touched->dist()
	- euclid_dist(curr_touched->vertex().network(), destination) / max_speed
	+ euclid_dist(source, destination)  / max_speed;
      LOG4CPLUS_TRACE(bi_logger, "Link vertex distance forward: " + ftos(link_dist_for));
      do
      {
      // updated for edge_label
	plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				      curr_touched->dist()
				      - euclid_dist(curr_touched->vertex().network(),
						    destination) / max_speed
				      + euclid_dist(source, destination) /
                  max_speed,curr_touched->label()));
	curr_touched = vertex_info[curr_touched->parent()];
	LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
      } while (curr_touched->parent() != NULL_PRODUCT_VERTEX);
    }
      // updated for edge_label
    plan.path.push_front(Location(curr_touched->vertex().network()->external_id(),
				  curr_touched->dist(),curr_touched->label()));
    LOG4CPLUS_TRACE(bi_logger, "Link vertex backward: " + link_vertex_back.info());
    curr_touched = vertex_info[link_vertex_back];
    LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
    Cost_Function link_dist_back;
    if (curr_touched->parent() != NULL_PRODUCT_VERTEX)
    {
      link_dist_back = curr_touched->dist()
	+ euclid_dist(curr_touched->vertex().network(), destination) / max_speed;
      LOG4CPLUS_TRACE(bi_logger, "Link vertex distance backward: " + ftos(link_dist_back));
      while (curr_touched->parent() != NULL_PRODUCT_VERTEX)
      {
         // edge_label of the previous touched vertex
         prev_edge_label = curr_touched->label();
	curr_touched = vertex_info[curr_touched->parent()];
	LOG4CPLUS_TRACE(bi_logger, "Current vertex: " + curr_touched->info());
      // updated for edge_label
	plan.path.push_back(Location(curr_touched->vertex().network()->external_id(),
				     link_dist_for + link_dist_back - curr_touched->dist()
				     - euclid_dist(curr_touched->vertex().network(),
                   destination) / max_speed,prev_edge_label));
      }
    }
  }
  LOG4CPLUS_DEBUG(bi_logger, "Path reconstructed.");
}


#endif
