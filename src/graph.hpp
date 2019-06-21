/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cassert>
#include <cmath>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <ext/hash_map>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>


using namespace std;
using namespace log4cplus;
using __gnu_cxx::hash_map;

// globals


/// Cost function.
/// \todo Respect time dependency.
typedef float Cost_Function;

/// Edge label.
/// \todo Respect vertex labels.
typedef int Label;

/// Maximum edge label ID.
int MAX_LABEL = 0;

// ----------------------------------------------------------------------------

template<typename EDGE_TYPE> class Vertex;
class Network_Vertex;
class NFA_Vertex;
class Product_Vertex;



// ----------------------------------------------------------------------------

// edges


/// Labeled unweighted edge.
/// The template parameter declares the type of end-vertices.
template<typename VERTEX_TYPE>
class Edge
{
protected:
  /// Tail vertex.
  VERTEX_TYPE* const tail_vertex;

  /// Head vertex.
  VERTEX_TYPE* const head_vertex;

  /// Edge label.
  Label edge_label;


public:
  /// Constructor.
  Edge(VERTEX_TYPE* const t, VERTEX_TYPE* const h, const Label& l):
    tail_vertex(t), head_vertex(h), edge_label(l) {}

  /// Destructor.
  /// \todo To be implemented.
  ~Edge() {}

  /// Assignment operator.
  Edge& operator=(const Edge& edge)
  {
    // HSM - is this correct?
    tail_vertex = edge.tail();
    head_vertex = edge.head();
    edge_label = edge.label();
    // tail = edge.tail();
    // head = edge.head();
    // label = edge.label();
    return *this;
  }

  /// Get tail vertex.
  VERTEX_TYPE* const tail() const { return tail_vertex; }

  /// Get head vertex.
  VERTEX_TYPE* const head() const { return head_vertex; }

  /// Get edge label.
  Label& label() { return edge_label; }

  /// Return info string.
  string info()
  {
    return tail_vertex->info() + "--" + head_vertex->info()
      + " (" + itos(edge_label) + ")";
  }
};


/// Output operator.
template<typename VERTEX_TYPE>
ostream& operator<<(ostream& out, Edge<VERTEX_TYPE>& edge)
{
  out << edge.info();
  return out;
}



/// Weighted edge.
class Network_Edge: public Edge<Network_Vertex>
{
protected:
  /// Edge cost.
  Cost_Function edge_cost;


public:
  /// Constructor.
  Network_Edge(Network_Vertex* const t, Network_Vertex* const h,
	       const Label& l, const Cost_Function& c):
    Edge<Network_Vertex>(t, h, l), edge_cost(c) {}

  /// Get edge cost.
  Cost_Function cost() const { return edge_cost; }
};



/// NFA edge (transition).
typedef Edge<NFA_Vertex> NFA_Edge;



// ----------------------------------------------------------------------------

// vertices


/// Vertex with list of adjacent vertices.
template<typename EDGE_TYPE>
class Vertex
{
public:
  /// Constant edge iterator.
  typedef typename list<EDGE_TYPE*>::const_iterator Edge_It;


protected:
  /// Vertex ID.
  const long vertex_id;

  /// Outgoing edges.
  /// Edges are sorted by label.
  list<EDGE_TYPE*> out_edges;

  /// To speed up iteration over edges with given labels,
  /// keep a pointer to the first edge with each label.
  /// The first edge with label l is pointed to by edge_pointers[l]
  /// and the one-edge-past-the-last-one with label l, by edge_pointers[l+1].
  /// (If there are no edges with label l,
  /// then we have edge_pointers[l] == edge_pointers[l+1].)
  vector<Edge_It> edge_pointers;


public:
  /// Constructor.
  Vertex(const long i):
    vertex_id(i), out_edges(), edge_pointers() {}

  /// Destructor.
  /// \todo To be implemented.
  ~Vertex() {}

  /// Get vertex ID.
  const long id() const { return vertex_id; }

  /// Get first outgoing edge with given label.
  Edge_It out_edge_begin(const Label& label = 0) const
  {
    return edge_pointers[label];
  }

  /// Get edge past last outgoing edge with given label.
  Edge_It out_edge_end(const Label& label = MAX_LABEL) const
  {
    return edge_pointers[label+1];
  }

  /// Add outgoing edge.
  /// \todo Vector instead of list and better sorting method.
  void add_edge(EDGE_TYPE*);

  /// Set edge pointers.
  void set_edge_pointers();

  /// Check edges.
  void check_edges()
  {
    for (Edge_It it=out_edges.begin(); it!=out_edges.end(); ++it)
      cout << "check_edges " << **it << endl;
  }

  /// Check pointers.
  void check_pointers() const
  {
    for (Label i=0; i<=MAX_LABEL+1; i++)
      if (out_edge_begin(i) == out_edges.end())
	cout << "label " << i << " e" << endl;
      else cout << "label " << i << " " << **out_edge_begin(i) << endl;
  }
};


template<typename EDGE_TYPE>
void Vertex<EDGE_TYPE>::add_edge(EDGE_TYPE* edge)
{
  typename list<EDGE_TYPE*>::iterator it = out_edges.begin();
  while (it != out_edges.end() && (*it)->label() < edge->label())
    ++it;
  out_edges.insert(it, edge);  // insert edge before it
}


template<typename EDGE_TYPE>
void Vertex<EDGE_TYPE>::set_edge_pointers()
{
  edge_pointers.resize(MAX_LABEL+2);
  Edge_It it = out_edges.begin();
  Label curr_label = 0;
  while (it != out_edges.end())
  {
    // set pointers for labels before first label used
    for (Label same_label=curr_label; same_label<=(*it)->label(); ++same_label)
      edge_pointers[same_label] = it;
    curr_label = (*it++)->label() + 1;
  }

  // set pointers for labels after last label used
  for (Label same_label=curr_label; same_label<=MAX_LABEL+1; ++same_label)
    edge_pointers[same_label] = it;
}



/// Vertex with external ID and coordinates.
class Network_Vertex: public Vertex<Network_Edge>
{
protected:
  /// External vertex ID.
  const long external_vertex_id;

  /// x-coordinate.
  float x_coordinate;

  /// y-coordinate.
  float y_coordinate;


public:
  /// Constructor.
  Network_Vertex(const long i, const long e, float x = 0, float y = 0):
    Vertex<Network_Edge>(i), external_vertex_id(e),
    x_coordinate(x), y_coordinate(y) {}

  /// Get external ID.
  const long external_id() const { return external_vertex_id; }

  /// Get x-coordinate.
  float x_coord() const { return x_coordinate; }

  /// Get y-coordinate.
  float y_coord() const { return y_coordinate; }

  /// Get x-coordinate.
  void set_x_coord(float x) { x_coordinate = x; }

  /// Get y-coordinate.
  void set_y_coord(float y) { y_coordinate = y; }

  /// Return info string.
  string info() { return itos(external_vertex_id); }
};


/// Output operator.
ostream& operator<<(ostream& out, Network_Vertex& vertex)
{
  out << vertex.info();
  return out;
}


// helper function
/// Euclidean distance between two network vertices.
float euclid_dist(Network_Vertex* vertex1, Network_Vertex* vertex2)
{
  return euclid_dist(vertex1->x_coord(), vertex1->y_coord(),
		     vertex2->x_coord(), vertex2->y_coord());
}



/// NFA vertex (state).
class NFA_Vertex: public Vertex<NFA_Edge>
{
protected:
  /// Marker for start state.
  bool start_state;

  /// Marker for accepting state.
  bool accepting_state;


public:
  /// Constructor.
  NFA_Vertex(long i, bool s, bool a):
    Vertex<NFA_Edge>(i), start_state(s), accepting_state(a) {}

  /// Is start state?
  bool start() const { return start_state; }

  /// Is accepting state?
  bool accepting() const { return accepting_state; }

  /// Return info string.
  string info() { return itos(vertex_id); }
};


/// Output operator.
ostream& operator<<(ostream& out, NFA_Vertex& vertex)
{
  out << vertex.info();
  return out;
}



/// Product vertex.
class Product_Vertex
{
protected:
  /// Network vertex.
  Network_Vertex* network_vertex;

  /// NFA vertex.
  NFA_Vertex* nfa_vertex;


public:
  /// Constructor.
  Product_Vertex(Network_Vertex* const net,
		 NFA_Vertex* const nfa):
    network_vertex(net), nfa_vertex(nfa) {}

  /// Copy-constructor.
  Product_Vertex(const Product_Vertex& vertex):
    network_vertex(vertex.network()), nfa_vertex(vertex.nfa()) {}

  /// Assignment operator.
  Product_Vertex& operator=(const Product_Vertex& vertex)
  {
    network_vertex = vertex.network();
    nfa_vertex = vertex.nfa();
    return *this;
  }

  /// Destructor.
  /// \todo To be implemented.
  ~Product_Vertex() {}

  /// Get network vertex.
  Network_Vertex* const network() const { return network_vertex; }

  /// Get NFA vertex.
  NFA_Vertex* const nfa() const { return nfa_vertex; }

  /// Is there neighbor with given label?
  bool label_shared(const Label& label) const
  {
    return
      network_vertex->out_edge_begin(label) !=
      network_vertex->out_edge_begin(label+1) &&
      nfa_vertex->out_edge_begin(label) != nfa_vertex->out_edge_begin(label+1);
  }

  /// Return info string.
  string info()
  {
    string info_text = "";
    if (network_vertex == NULL)
      info_text += "NULL";
    else info_text += network_vertex->info();
    info_text += "/";
    if (nfa_vertex == NULL)
      info_text += "NULL";
    else info_text += nfa_vertex->info();
    return info_text;
  }
};


/// Null product vertex.
const Product_Vertex NULL_PRODUCT_VERTEX = Product_Vertex(NULL, NULL);


/// Equal-operator for Product_Vertex.
inline bool operator==(const Product_Vertex& one, const Product_Vertex& other)
{
  return one.network() == other.network() &&
    one.nfa() == other.nfa();
}


/// Not-equal operator for Product_Vertex.
inline bool operator!=(const Product_Vertex& one, const Product_Vertex& other)
{
  return !(one == other);
}


/// Output operator.
ostream& operator<<(ostream& out, Product_Vertex& vertex)
{
  out << vertex.info();
  return out;
}



// ----------------------------------------------------------------------------

/// Product neighbor iterator.
class Product_Neighbor_Iterator
{
protected:
  /// Tail product vertex.
  Product_Vertex tail_vertex;

  /// Head product vertex.
  Product_Vertex head_vertex;

  /// Current edge label.
  Label curr_label;

  /// Network edge iterator.
  Network_Vertex::Edge_It network_it;

  /// NFA edge iterator.
  NFA_Vertex::Edge_It nfa_it;


public:
  /// Constructor.
  Product_Neighbor_Iterator(Product_Vertex const t):
    tail_vertex(t), head_vertex(NULL_PRODUCT_VERTEX), curr_label()
  {
    while (valid() && !tail_vertex.label_shared(curr_label))
      ++curr_label;
    network_it = tail_vertex.network()->out_edge_begin(curr_label);
    nfa_it = tail_vertex.nfa()->out_edge_begin(curr_label);
    if (valid())
      generate_head();
  }

  /// Is iterator valid?
  bool valid() const
  {
    return curr_label <= MAX_LABEL;
  }

  /// Increment-operator.
  void operator++()
  {
    const Network_Vertex* network_vertex = tail_vertex.network();
    const NFA_Vertex* nfa_vertex = tail_vertex.nfa();

    ++network_it;
    if (network_it == network_vertex->out_edge_end(curr_label))
    {
      ++nfa_it;
      if (nfa_it == nfa_vertex->out_edge_end(curr_label))
      {
	do
	  ++curr_label;
	while (valid() && !tail_vertex.label_shared(curr_label));
	if (valid())
	{
	  network_it = network_vertex->out_edge_begin(curr_label);
	  nfa_it = nfa_vertex->out_edge_begin(curr_label);
	}
      }
      else
	network_it = network_vertex->out_edge_begin(curr_label);
    }
    if (valid())
      generate_head();
  }

  /// Get network edge.
  Network_Edge* network_edge() const { return *network_it; }

  /// Get NFA edge.
  NFA_Edge* nfa_edge() const { return *nfa_it; }

  /// Get tail product vertex.
  Product_Vertex tail() const { return tail_vertex; }

  /// Get head product vertex.
  void generate_head()
  {
    head_vertex = Product_Vertex((Network_Vertex*)(*network_it)->head(),
				 (NFA_Vertex*)(*nfa_it)->head());
  }

  /// Get head product vertex.
  Product_Vertex head() const { return head_vertex; }

  /// Get edge label.
  const Label& label() const { return curr_label; }

  /// Get network edge cost.
  Cost_Function cost() const
  {
    return ((Network_Edge*)*network_it)->cost();
  }

  /// Return info string.
  string info()
  {
    return (*network_it)->info() + " | " + (*nfa_it)->info();
  }
};


/// Output operator.
ostream& operator<<(ostream& out, Product_Neighbor_Iterator& it)
{
  out << it.info();
  return out;
}



// ----------------------------------------------------------------------------

// graphs


/// Basic graph.
template<typename VERTEX_TYPE>
class Graph
{
protected:
  /// Vertices.
  /// CAVE: vertices start from index 1.
  vector<VERTEX_TYPE*> vertices;

  /// Logger.
  Logger graph_logger;


public:
  /// Constructor.
  Graph(): vertices(), graph_logger(Logger::getInstance("Graph"))
  {
    graph_logger.addAppender(myConsoleAppender);
    graph_logger.setLogLevel(INFO_LOG_LEVEL);
  }

  /// Destructor.
  /// \todo To be implemented.
  virtual ~Graph() {}

  /// Get number of vertices.
  size_t size() const { return vertices.size(); }

  /// Vertex access.
  VERTEX_TYPE* operator[](unsigned int const index) const
  {
    return vertices[index];
  }

  /// Check graph.
  void check_graph()
  {
    for (const_iterator it=vertices.begin(); it!=end(); ++it)
    {
      cout << "vertex " << **it << endl;
      (*it)->check_edges();
      (*it)->check_pointers();
    }
  }

  /// Add edge.
  Edge<VERTEX_TYPE>* add_edge(VERTEX_TYPE* const tail_vertex,
			      VERTEX_TYPE* const head_vertex,
			      const Label& label);

  /// Vertex iterator.
  typedef typename vector<VERTEX_TYPE*>::iterator iterator;

  /// Constant vertex iterator.
  typedef typename vector<VERTEX_TYPE*>::const_iterator const_iterator;

  /// Begin-iterator for vertices.
  virtual typename vector<VERTEX_TYPE*>::const_iterator begin() const
  {
    return vertices.begin();
  }

  /// End-iterator for vertices.
  typename vector<VERTEX_TYPE*>::const_iterator end() const
  {
    return vertices.end();
  }

  /// Set edge pointers for all vertices.
  void set_edge_pointers();
};


template<typename VERTEX_TYPE>
Edge<VERTEX_TYPE>* Graph<VERTEX_TYPE>::add_edge(VERTEX_TYPE* const tail_vertex,
						VERTEX_TYPE* const head_vertex,
						const Label& label)
{
  Edge<VERTEX_TYPE>* new_edge =
    new Edge<VERTEX_TYPE>(tail_vertex, head_vertex, (Label)label);
  tail_vertex->add_edge(new_edge);
  return new_edge;
}


template<typename VERTEX_TYPE>
void Graph<VERTEX_TYPE>::set_edge_pointers()
{
  for (const_iterator it=begin(); it!=end(); ++it)
  {
    // cout << **it << endl;
    (*it)->set_edge_pointers();
  }
}



/// Network graph.
class Network_Graph: public Graph<Network_Vertex>
{
protected:
  /// Mapping from external to internal vertex ID's.
  hash_map<long, long> internal_vertex_id;


public:
  /// Standard constructor.
  Network_Graph(): Graph<Network_Vertex>(), internal_vertex_id() {}

  /// Constructor.
  Network_Graph(const string network_filename, const string coords_filename)
  {
    read_graph(network_filename);
    read_coordinates(coords_filename);
    set_edge_pointers();
  }

  /// Destructor.
  ~Network_Graph() {}

  /// Begin-iterator for vertices (skip 0-th vertex).
  // TODO
//   virtual vector<Network_Vertex*>::const_iterator begin() const
//   {
//     return ++vertices.begin();
//   }

  /// Get internal ID.
  const long internal_id(long ext) const
  {
    return internal_vertex_id.find(ext)->second;
  }

  /// Add vertex if none such exists yet.
  /// Internal vertex ID 0 (default value of hash_map) indicates "no entry".
  Network_Vertex* add_vertex(long ext);

  Network_Edge* add_edge(Network_Vertex* const tail_vertex,
			 Network_Vertex* const head_vertex,
			 const Label& label,
			 const Cost_Function& cost);

  /// Read graph.
  /// File format: list of edges:
  /// tail_id head_id tmp1 tmp2 tmp3 tmp4 cost label
  void read_graph(string network_filename);

  /// Read vertex coordinates.
  void read_coordinates(string coords_filename);

  /// Construct backward graph.
  void construct_back_graph(Network_Graph& back_graph,
			    map<Network_Vertex*, Network_Vertex*>& back_vertex,
			    map<Network_Vertex*, Network_Vertex*>& orig_vertex);
};


Network_Vertex* Network_Graph::add_vertex(long ext)
{
  if (internal_vertex_id[ext] == 0)
  {
    internal_vertex_id[ext] = vertices.size();
    Network_Vertex* new_vertex =
      new Network_Vertex(internal_vertex_id[ext], ext);
    vertices.push_back(new_vertex);
    return new_vertex;
  }
  else
    return vertices[internal_vertex_id[ext]];
}


Network_Edge* Network_Graph::add_edge(Network_Vertex* const tail_vertex,
				      Network_Vertex* const head_vertex,
				      const Label& label,
				      const Cost_Function& cost)
{
  Network_Edge* new_edge =
    new Network_Edge(tail_vertex, head_vertex, label, cost);
  tail_vertex->add_edge(new_edge);
  return new_edge;
}


void Network_Graph::read_graph(const string network_filename)
{
  ifstream network_file(network_filename.c_str());
  string header;
  getline(network_file, header);
  add_vertex(-1);  // dummy vertex

  long from_id, to_id;
  int tmp1, tmp2, tmp3, tmp4;
  Cost_Function cost;
  Label label;
  while (true)
  {
    network_file >> from_id >> to_id >> tmp1 >> tmp2 >> tmp3 >> tmp4
		 >> cost >> label;
    if (label >= MAX_LABEL)
      MAX_LABEL = label;
    if (network_file.good())
    {
      Network_Vertex* from = add_vertex(from_id);
      Network_Vertex* to = add_vertex(to_id);
      add_edge(from, to, label, cost);
    }
    else break;
  }
  LOG4CPLUS_INFO(graph_logger, "Network has " + itos(size()-1) + " vertices.");  // subtract dummy vertex
}


void Network_Graph::read_coordinates(const string coords_filename)
{
  ifstream coords_file(coords_filename.c_str());
  string header;
  getline(coords_file, header);
  long ext;
  float x_coord, y_coord;

  //HSM: This does not look safe - Or is it? New version below.
//   while (coords_file)
//   {
//     coords_file >> ext >> x_coord >> y_coord;
//     vertices[internal_vertex_id[ext]]->set_x_coord(x_coord);
//     vertices[internal_vertex_id[ext]]->set_y_coord(y_coord);
//   }

  while (coords_file)
  {
    coords_file >> ext >> x_coord >> y_coord;
    if( coords_file.good() ){
      vertices[internal_vertex_id[ext]]->set_x_coord(x_coord);
      vertices[internal_vertex_id[ext]]->set_y_coord(y_coord);
    }
    else
      break;
  }
}


void Network_Graph::construct_back_graph(Network_Graph& back_graph,
					 map<Network_Vertex*, Network_Vertex*>& back_vertex,
					 map<Network_Vertex*, Network_Vertex*>& orig_vertex)
{
  for (const_iterator vertex_it=begin(); vertex_it!=end(); ++vertex_it)
  {
    Network_Vertex* new_vertex =
      back_graph.add_vertex((*vertex_it)->external_id());
    back_vertex[*vertex_it] = new_vertex;
    orig_vertex[new_vertex] = *vertex_it;
    new_vertex->set_x_coord((*vertex_it)->x_coord());
    new_vertex->set_y_coord((*vertex_it)->y_coord());
  }

  for (const_iterator vertex_it=begin(); vertex_it!=end(); ++vertex_it)
  {
    for (Network_Vertex::Edge_It edge_it=(*vertex_it)->out_edge_begin();
	 edge_it!=(*vertex_it)->out_edge_end(); ++edge_it)
    {
      Network_Vertex* from = back_vertex[(*edge_it)->head()];
      Network_Vertex* to = back_vertex[(*edge_it)->tail()];
      back_graph.add_edge(from, to, (*edge_it)->label(), (*edge_it)->cost());

    }
  }
  back_graph.set_edge_pointers();
}



/// NFA graph.
class NFA_Graph: public Graph<NFA_Vertex>
{
protected:
  /// Start state.
  /// Allow several start states to handle reverse graph uniformly.
  list<NFA_Vertex*> start_states;

  /// Accepting states.
  list<NFA_Vertex*> accepting_states;


public:
  /// Standard constructor.
  NFA_Graph(): Graph<NFA_Vertex>(), start_states(), accepting_states() {}

  /// Constructor.
  NFA_Graph(const string nfa_filename, Network_Graph& network)
  {
    read_graph(nfa_filename);
    set_edge_pointers();
  }

  /// Get start state.
  list<NFA_Vertex*> start() const { return start_states; }

  /// Get accepting states.
  list<NFA_Vertex*> accepting() const { return accepting_states; }

  /// Add vertex.
  NFA_Vertex* add_vertex(const long id, bool s, bool a);

  /// Add start state.
  void add_start(NFA_Vertex* vertex)
  {
    start_states.push_back(vertex);
  }

  /// Add accepting state.
  void add_accepting(NFA_Vertex* vertex)
  {
    accepting_states.push_back(vertex);
  }

  /// Read automaton.
  /// File format:
  ///   nmb_states
  ///   id is_start is_accepting
  ///   ...
  ///   tail_id head_id label
  ///   ...
  /// CAVE: ID's must be 0..nmb_states-1.
  void read_graph(string nfa_filename);

  /// Construct backward graph.
  void construct_back_graph(NFA_Graph& back_graph,
			    map<NFA_Vertex*, NFA_Vertex*>& back_vertex,
			    map<NFA_Vertex*, NFA_Vertex*>& orig_vertex);
};


NFA_Vertex* NFA_Graph::add_vertex(const long id, bool s, bool a)
{
  NFA_Vertex* new_vertex = new NFA_Vertex(id, s, a);
  vertices.push_back(new_vertex);
  return new_vertex;
}


void NFA_Graph::read_graph(const string nfa_filename)
{
  ifstream nfa_file(nfa_filename.c_str());
  if( !nfa_file )
    LOG4CPLUS_ERROR(graph_logger, "NFA_Graph: unable to open file " + nfa_filename + ". Have you adapted the main NFA file to the current machine?");

  string header;
  int num_states;
  long id, from_id, to_id;
  bool start, accepting;
  Label label;

  // read states
  nfa_file >> num_states;
  LOG4CPLUS_INFO(graph_logger, "NFA has " + itos(num_states) + " states.");
  getline(nfa_file, header);
  getline(nfa_file, header);
  vertices = vector<NFA_Vertex*>(num_states);
  for (int i=0; i<num_states; ++i)
  {
    nfa_file >> id >> start >> accepting;
    assert (i == id);
    // TODO
    // NFA_Vertex* new_vertex = add_vertex(id, start, accepting);
    NFA_Vertex* new_vertex = new NFA_Vertex(id, start, accepting);
    vertices[i] = new_vertex;
    if (start)
      start_states.push_back(new_vertex);
    if (accepting)
      accepting_states.push_back(new_vertex);
  }
  // TODO
  // vertices[num_states] = new NFA_Vertex(num_states, false, false);

  // read transitions
  getline(nfa_file, header);
  getline(nfa_file, header);
  while (true)
  {
    nfa_file >> from_id >> to_id >> label;
    if (label >= MAX_LABEL)
      MAX_LABEL = label;
    if (nfa_file.good())
    {
      NFA_Vertex* from = vertices[from_id];
      NFA_Vertex* to = vertices[to_id];
      add_edge(from, to, label);
    }
    else break;
  }
}


void NFA_Graph::construct_back_graph(NFA_Graph& back_graph,
				     map<NFA_Vertex*, NFA_Vertex*>& back_vertex,
				     map<NFA_Vertex*, NFA_Vertex*>& orig_vertex)
{
  for (const_iterator vertex_it=begin(); vertex_it!=end(); ++vertex_it)
  {
    bool is_start = (*vertex_it)->start();
    bool is_accepting = (*vertex_it)->accepting();
    NFA_Vertex* new_vertex =
      back_graph.add_vertex((*vertex_it)->id(), is_accepting, is_start);
    back_vertex[*vertex_it] = new_vertex;
    orig_vertex[new_vertex] = *vertex_it;
    if (is_start)
      back_graph.add_accepting(new_vertex);
    if (is_accepting)
      back_graph.add_start(new_vertex);
  }

  for (const_iterator vertex_it=begin(); vertex_it!=end(); ++vertex_it)
  {
    for (NFA_Vertex::Edge_It edge_it=(*vertex_it)->out_edge_begin();
	 edge_it!=(*vertex_it)->out_edge_end(); ++edge_it)
    {
      NFA_Vertex* from = back_vertex[(*edge_it)->head()];
      NFA_Vertex* to = back_vertex[(*edge_it)->tail()];
      back_graph.add_edge(from, to, (*edge_it)->label());
    }
  }
  back_graph.set_edge_pointers();
}


#endif
