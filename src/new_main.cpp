/*
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    Re-Router Threaded
    June 12 
    
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

#include <cassert>
#include <iostream>
#include <fstream>
#include <thread>

#include "dijkstra.hpp"
#include "graph.hpp"
#include "timer.hpp"
#include "tools.hpp"

#include <log4cplus/layout.h>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

using namespace std;
using namespace log4cplus;

Event_Handler event_handler;

/* enum Request_Mode{
  SINGLE,
  ALL_PAIRS,
  ON_LINE,
  RANDOM,
  FILE_PAIRS
};

/// Shortest-path algorithms.
enum Algorithm{
  STD = 0,
};
*/

/// Logger.
Logger main_logger = Logger::getInstance("Router");

istream &operator>>(istream &in, Trip_Request &request)
{
  in >> request.source >> request.destination >> request.start_time;
  return in;
}

/// Plan output operator.
ostream &operator<<(ostream &out, Plan &plan)
{
  const unsigned int plan_size = plan.path.size();
  out << plan_size << '\t';

  if (plan_size == 0) // No plan
    return out;
  else
  {
    for (Plan_Iterator it = plan.path.begin(); it != plan.path.end(); ++it)
    {
      out << it->external_id << '\t' << it->time << "\t";

      // link ID travelled added.
      if (it != plan.path.begin())
        out << it->edge_label << '\t';
    }
  }
  return out;
}
/* 
class Request_Handler{
protected:
  /// Request mode.
  Request_Mode request_mode;

  /// Is there request yet?
  bool requests_finished;

  /// Next request.
  Trip_Request trip_request;

  /// Network graph.
  const Network_Graph *network;

  /// Stream for reading requests from file.
  char *stream_filename;
  fstream trip_stream;

public:
  /// Constructor.
  Request_Handler() : request_mode(ON_LINE),requests_finished(false), trip_request(), stream_filename(0) {}

  /// Set request mode.
  void set_mode(const Request_Mode &m) { request_mode = m; }

  /// Set single request.
  void set_request(const Trip_Request &r){
    trip_request.source = r.source;
    trip_request.destination = r.destination;
    trip_request.start_time = r.start_time;
    trip_request.id = r.id;
  }

  /// Set network graph.
  void set_network(const Network_Graph *n) { network = n; }

  /// Set stream for trips.

  void set_stream(const char *stream_filename){
    this->stream_filename = const_cast<char *>(stream_filename);
  }

  /// Get mode.
  //Request_Mode mode() const { return request_mode; }

  /// Get trip request.
  Trip_Request request() const { return trip_request; }

  /// Set first request.
  void init(){
    trip_request.nfaID = 0; // HSM - default value.

    if(request_mode == FILE_PAIRS){

      if (!stream_filename){
        cout << "No filename for pairs given. Bye!" << endl;
        exit(-1);
      }

      trip_stream.open(stream_filename);

      if (!trip_stream){
        cout << "There was an error opening the file with source destination pairs." << endl;
        cout << "Filename given: " << stream_filename << endl;
        exit(-1);
      }

      // Remove header if any. -- currently there is no header.

      long int t_id, src, dest, nfaID;
      double t0;
      trip_stream >> t_id >> src >> dest >> t0 >> nfaID;

      if (trip_stream.eof() || trip_stream.bad()){
        cout << "Okay, the trip file seems to be empty. Bye!" << endl;
        requests_finished = true;
        //exit(-1);
      }

      trip_request.id = t_id;
      trip_request.source = src;
      trip_request.destination = dest;
      trip_request.start_time = t0;
      trip_request.nfaID = nfaID;

    }
  }

  /// Is there request yet?
  bool finished() const { return requests_finished; }

  /// Set next request.
  void next_request(){

    if(request_mode == FILE_PAIRS){

      long int t_id, src, dest, nfaID;
      double t0;
      trip_stream >> t_id >> src >> dest >> t0 >> nfaID;

      if (trip_stream.eof() || trip_stream.bad()){
        requests_finished = true;
        return;
      }

      trip_request.id = t_id;
      trip_request.source = src;
      trip_request.destination = dest;
      trip_request.start_time = t0;
      trip_request.nfaID = nfaID;
    }
  }

};

/// Router.
class Router{
  protected:
    /// Network graph.
    Network_Graph &network;

    /// NFA graph.
    vector<NFA_Graph *> &nfaVector;

    /// Routing engines.
    vector<vector<Shortest_Path *>> dijkstra;

  public:
    /// Constructor.
    Router(Network_Graph &n1, vector<NFA_Graph *> &nfaVec) : network(n1), nfaVector(nfaVec){
      const unsigned int nNFA = nfaVec.size();

      dijkstra = vector<vector<Shortest_Path *>>(nNFA);

      for (unsigned int i = 0; i < nNFA; ++i){
        dijkstra[i].resize(4);

        dijkstra[i][STD] = new Shortest_Path(network, *nfaVector[i]);
      }
    }

    /// Compute route with specified routing algorithm.
    void find_path(Algorithm algorithm, Trip_Request trip, Plan &plan,
                  double &time_elapsed, unsigned int nfaChoice = 0){
      dijkstra[nfaChoice][algorithm]->init(trip);
      Timer timer;
      dijkstra[nfaChoice][algorithm]->dijkstra();
      time_elapsed = timer.elapsed();
      dijkstra[nfaChoice][algorithm]->reconstruct_path(plan);
    }
};
*/
//Condensed
void print_usage()
{
  cout << "Usage:" << endl
       << " Router [-g <graph>] [-c <coords>] [-N <NFAFILE>]" << endl
       << "        ([-t <time>] | [-f <pairs>] )" << endl
       << "        " << endl
       << " -c <coords>  coordinates (vertex) file" << endl
       << " -f <pairs>   pairs from file" << endl
       << " -g <graph>   graph (edge) file" << endl
       << " -N <NFAFile> file specifying nfa collection" << endl
       << " -t <time>    time of departure" << endl;
}
//TODO this has to modify the init method?
//Possibly use this in the main thread and pass vectors to child threads

//Threaded trip request processing
void thread_method(Request_Handler &request_handler, Plan plan, Network_Graph &network, unsigned int algorithm, ostream &out_file, int singleNFA, string nfa_filename, const char *nfa_collection_filename)
{
  vector<NFA_Graph *> nfaVector;

  if (singleNFA == 1)
  {
    NFA_Graph *nfa = new NFA_Graph(nfa_filename, network);
    nfaVector.push_back(nfa);
  }
  else
  {
    ifstream file(nfa_collection_filename);

    if (!file)
    {
      cout << "There was an error opening the file specifying the collection of NFAs." << endl;
      cout << "Filename given: " << nfa_collection_filename << endl;
      exit(-1);
    }

    char buffer[5000];
    file.getline(buffer, 5000, '\n'); // Skip the header.

    int nNFAs;

    file >> nNFAs;
    cout << "Found " << nNFAs << " NFAs" << endl;
    file.get();

    for (unsigned int i = 0; (int)i < nNFAs; ++i)
    {
      file.getline(buffer, 5000, '\n');
      cout << "NFA: " << i << "\t" << buffer << endl;
      NFA_Graph *nfa = new NFA_Graph(buffer, network);
      nfaVector.push_back(nfa);
    }
  }

  cout << "Status." << endl;

  LOG4CPLUS_DEBUG(main_logger, "Building router...");
  Router router(network, nfaVector);
  LOG4CPLUS_DEBUG(main_logger, "Router built.");

  cout << "Status.." << endl;

  event_handler.set_graph(network);

  cout << "Status..." << endl;

  // initialize request handler

  request_handler.set_network(&network);

  request_handler.init();

  cout << "Status...." << endl;

  while (!request_handler.finished())
  {
    Trip_Request trip_request = request_handler.request();

    float distance = 0.0;

    plan.path.clear();
    event_handler.clear();
    double time_elapsed;

    router.find_path((Algorithm)algorithm, request_handler.request(), plan,
                     time_elapsed, trip_request.nfaID);

    out_file << trip_request.id << '\t'
             << trip_request.source << '\t'
             << trip_request.destination << '\t';

    out_file << plan << endl;

    bool error = false;
    string error_message = "Differing distances:  request " + itos(trip_request.source) + "--" + itos(trip_request.destination) + "  distances";

    error_message += " " + ftos(distance);
    error_message += "  differences";

    if (error)
      LOG4CPLUS_ERROR(main_logger, error_message);

    request_handler.next_request();
  }
}

int main(int argc, char *argv[])
{
  // initialize logger
  myConsoleAppender->setLayout(myLayout);
  main_logger.addAppender(myConsoleAppender);
  main_logger.setLogLevel(INFO_LOG_LEVEL);

  if (argc == 1)
  {
    print_usage();
    return 0;
  }
  const char *network_filename = "";
  const char *coords_filename = "";
  const char *nfa_filename = "";
  const char *nfa_collection_filename = "";
  const char *pairs_filename = "";
  const char *out_filename = "plans.txt";

  // parse command-line arguments
  int c;
  Trip_Request request;
  request.start_time = 0;
  Request_Handler request_handler;
  Plan plan;
  ifstream pairs_file;
  ofstream out_file;
  unsigned int algorithm = STD;

  unsigned singleNFA = 1;

  cout << "*** Standard Dijkstra" << endl;
  event_handler.set_filename_affix("D");
  algorithm = STD;

  //cleaned up parsing
  while ((c = getopt(argc, argv, "a:c:d:f:g:ilnN:o:p:r:s:t:v:z")) != -1)
  {

    switch (c)
    {
    case 'c':
      coords_filename = optarg;
      break;
    case 'f':
      request_handler.set_mode(FILE_PAIRS);
      pairs_filename = optarg;
      request_handler.set_stream(pairs_filename);
      break;
    case 'g':
      network_filename = optarg;
      break;
    case 'N':
      nfa_collection_filename = optarg;
      singleNFA = 0;
      break;

    case 't':
      request.start_time = atoi(optarg);
      request_handler.set_mode(SINGLE);
      break;
    }
  }

  // control output
  cout << "Data:" << endl
       << " network: " << network_filename << endl
       << " coords:  " << coords_filename << endl
       << " NFA:     " << nfa_filename << endl
       << " NFAColl: " << nfa_collection_filename << endl;

  // build network
  LOG4CPLUS_DEBUG(main_logger, "Building network...");
  Network_Graph network(network_filename, coords_filename);

  LOG4CPLUS_DEBUG(main_logger, "Building NFA...");

  // process queries
  out_file.open(out_filename);
  if (!out_file)
  {
    cout << "Sorry, could not open file " << out_filename << ". Bye!" << endl;
    exit(-1);
  }
  std::thread test(thread_method, std::ref(request_handler), plan, std::ref(network), algorithm, std::ref(out_file), singleNFA, nfa_filename, nfa_collection_filename);
  test.join();

  out_file.close();

  return 0;
}