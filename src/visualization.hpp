/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include <map>

#include "basics.hpp"
#include "graph.hpp"

using namespace std;

// globals


/// Colors.
enum Color { BLACK=0, RED=1, BLUE=2, ORANGE=3, BLUEVIOLET=4 };


/// Shapes.
enum Shape { POINT=0, CIRCLE=1, BOX=2, DIAMOND=3 };


/// Styles.
enum Style { SOLID=0, BOLD=1, DASHED=2, DOTTED=3 };



// ----------------------------------------------------------------------------


/// Visualization.
class Visualization
{
  /// Graph.
  Network_Graph graph;

  /// Vertex color.
  map<const Network_Vertex*, Color> vertex_color;

  /// Vertex label.
  map<const Network_Vertex*, string> vertex_label;

  /// Vertex shape.
  map<const Network_Vertex*, Shape> vertex_shape;

  /// Vertex style.
  map<const Network_Vertex*, Style> vertex_style;

  /// Edge color.
  map<pair<const Network_Vertex*, const Network_Vertex*>, Color> edge_color;

  /// Edge label.
  map<pair<const Network_Vertex*, const Network_Vertex*>, string> edge_label;

  /// Edge style.
  map<pair<const Network_Vertex*, const Network_Vertex*>, Style> edge_style;

  /// Output filename.
  string output_filename;

  /// Output filename affix.
  string filename_affix;

  /// Detailed visualization.
  bool detailed;


public:
  /// Standard constructor.
  Visualization() {}

  /// Constructor.
  Visualization(const Network_Graph& g, string o, bool d=true):
    graph(g), output_filename(o), detailed(d) {}

  /// Clear.
  void clear()
  {
    vertex_color = map<const Network_Vertex*, Color>();
    vertex_label = map<const Network_Vertex*, string>();
    vertex_shape = map<const Network_Vertex*, Shape>();
    vertex_style = map<const Network_Vertex*, Style>();
    edge_color =
      map<pair<const Network_Vertex*, const Network_Vertex*>, Color>();
    edge_label =
      map<pair<const Network_Vertex*, const Network_Vertex*>, string>();
    edge_style =
      map<pair<const Network_Vertex*, const Network_Vertex*>, Style>();
    filename_affix = "";
    init();
  }

  /// Init data structures.
  void init();

  /// Set graph.
  void set_graph(Network_Graph& gr) { graph = gr; }

  /// Set output filename.
  void set_outfile_name(string out) { output_filename = out; }

  /// Set output filename affix (e.g., to distinguish between algorithms).
  void set_filename_affix(string affix) { filename_affix = affix; }

  /// Set detailed visualization.
  void set_detailed_vis(bool det) { detailed = det; }

  /// Set vertex color.
  void set_vertex_color(const Network_Vertex* vertex, Color color)
  {
    vertex_color[vertex] = color;
  }

  /// Set vertex label.
  void set_vertex_label(const Network_Vertex* vertex, string s)
  {
    vertex_label[vertex] = s;
  }

  /// Set vertex shape.
  void set_vertex_shape(const Network_Vertex* vertex, Shape s)
  {
    if (detailed)
      vertex_shape[vertex] = s;
    else vertex_shape[vertex] = POINT;
  }

  /// Set vertex style.
  void set_vertex_style(const Network_Vertex* vertex, Style s)
  {
    if (detailed)
      vertex_style[vertex] = s;
    else vertex_style[vertex] = SOLID;
  }

  /// Set edge color.
  void set_edge_color(const Network_Vertex* vertex1,
		      const Network_Vertex* vertex2, Color color)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    edge_color[p] = color;
  }

  /// Set edge label.
  void set_edge_label(const Network_Vertex* vertex1,
		      const Network_Vertex* vertex2, string s)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    edge_label[p] = s;
  }

  /// Set edge style.
  void set_edge_style(const Network_Vertex* vertex1,
		      const Network_Vertex* vertex2, Style s)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    edge_style[p] = s;
  }

  /// Get vertex color.
  string vertex_color_string(const Network_Vertex* vertex)
  {
    switch (vertex_color[vertex])
    {
    case BLACK: return "black"; break;
    case RED: return "red"; break;
    case BLUE: return "blue"; break;
    case ORANGE: return "orange"; break;
    case BLUEVIOLET: return "blueviolet"; break;
    default: return "";
    }
  }

  /// Get vertex label.
  string vertex_label_string(const Network_Vertex* vertex)
  {
    return vertex_label[vertex];
  }

  /// Get vertex shape.
  string vertex_shape_string(const Network_Vertex* vertex)
  {
    switch (vertex_shape[vertex])
    {
    case CIRCLE: return "circle"; break;
    case POINT: return "point"; break;
    case BOX: return "box"; break;
    case DIAMOND: return "diamond"; break;
    default: return "";
    }
  }

  /// Get vertex style.
  string vertex_style_string(const Network_Vertex* vertex)
  {
    switch (vertex_style[vertex])
    {
    case SOLID: return "solid"; break;
    case BOLD: return "bold"; break;
    case DASHED: return "dashed"; break;
    case DOTTED: return "dotted"; break;
    default: return "";
    }
  }

  /// Get edge color.
  string edge_color_string(const Network_Vertex* vertex1,
			   const Network_Vertex* vertex2)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    switch (edge_color[p])
    {
    case BLACK: return "black"; break;
    case RED: return "red"; break;
    case BLUE: return "blue"; break;
    case ORANGE: return "orange"; break;
    case BLUEVIOLET: return "blueviolet"; break;
    default: return "";
    }
  }

  /// Get edge label.
  string edge_label_string(const Network_Vertex* vertex1,
			   const Network_Vertex* vertex2)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    return edge_label[p];
  }

  /// Get edge.
  string edge_style_string(const Network_Vertex* vertex1,
			   const Network_Vertex* vertex2)
  {
    pair<const Network_Vertex*, const Network_Vertex*> p(vertex1, vertex2);
    switch (edge_style[p])
    {
    case SOLID: return "solid"; break;
    case BOLD: return "bold"; break;
    case DASHED: return "dashed"; break;
    case DOTTED: return "dotted"; break;
    default: return "";
    }
  }

  /// Write graph to file (DOT format).
  void write(int time_stamp);
};


void Visualization::init()
{
  for (Network_Graph::const_iterator vertex_it = graph.begin();
       vertex_it != graph.end(); vertex_it++)
  {
    if (detailed)
      set_vertex_shape(*vertex_it, CIRCLE);
    for (Network_Vertex::Edge_It edge_it = (*vertex_it)->out_edge_begin();
	 edge_it != (*vertex_it)->out_edge_end(); edge_it++)
      set_edge_label(*vertex_it, (*edge_it)->head(),
		     ftos((*edge_it)->cost()) + " | "
		     + itos((*edge_it)->label()));
  }
}


void Visualization::write(int time_stamp)
{
  string curr_filename = output_filename + filename_affix;
  int counter = time_stamp;
  while (counter < 1000)
  {
    curr_filename += "0";
    counter *= 10;
  }
  curr_filename += itos(time_stamp) + ".dot";
  ofstream outfile(curr_filename.c_str());
  outfile << "digraph {" << endl
	  << "edge [fontsize=\"10\", fontcolor=\"dimgray\"];" << endl;

  // determine expansion
  float x_min(1e+99), x_max(0), y_min(1e+99), y_max(0);
  for (Network_Graph::const_iterator vertex_it = graph.begin();
       vertex_it != graph.end(); vertex_it++)
  {
    if ((*vertex_it)->external_id() != -1)
    {
      if ((*vertex_it)->x_coord() < x_min)
	x_min = (*vertex_it)->x_coord();
      if ((*vertex_it)->x_coord() > x_max)
	x_max = (*vertex_it)->x_coord();
      if ((*vertex_it)->y_coord() < y_min)
	y_min = (*vertex_it)->y_coord();
      if ((*vertex_it)->y_coord() > y_max)
	y_max = (*vertex_it)->y_coord();
    }
  }
  float x_fac = (float)6/(x_max - x_min);
  float y_fac = (float)10/(y_max - y_min);

  // write vertices and edges
  for (Network_Graph::const_iterator vertex_it = graph.begin();
       vertex_it != graph.end(); vertex_it++)
  {
    if ((*vertex_it)->external_id() != -1)
    {
      // write vertices
      outfile << (*vertex_it)->external_id() << " [pos=\""
	      << ((*vertex_it)->x_coord()-x_min)*x_fac << ","
	      << ((*vertex_it)->y_coord()-y_min)*y_fac
	      << "\", color=\"" << vertex_color_string(*vertex_it)
	      << "\", shape=\"" << vertex_shape_string(*vertex_it)
	      << "\", style=\"" << vertex_style_string(*vertex_it) << "\"];"
	      << endl;

      // write adjacent edges
      for (Network_Vertex::Edge_It edge_it = (*vertex_it)->out_edge_begin();
	   edge_it != (*vertex_it)->out_edge_end(); edge_it++)
      {
	outfile << (*vertex_it)->external_id() << " -> "
		<< (*edge_it)->head()->external_id()
		<< " [color=\""
		<< edge_color_string(*vertex_it, (*edge_it)->head());
	if (detailed)
	  outfile << "\", labelfloat=\"true\", label=\""
		  << edge_label_string(*vertex_it, (*edge_it)->head());
	outfile << "\", style=\""
		<< edge_style_string(*vertex_it, (*edge_it)->head())
		<< "\"];" << endl;
      }
    }
  }

  outfile << "}" << endl;
}


#endif
