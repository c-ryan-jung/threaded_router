/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Framework: Reg-Exp Router

  Date: 16 Apr 2019

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <cassert>
#include <iostream>
#include <fstream>

#include "dijkstra.hpp"
#include "graph.hpp"
#include "timer.hpp"

#include <log4cplus/layout.h>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

using namespace std;
using namespace log4cplus;

// globals

/// Request mode.
enum Request_Mode
{
  SINGLE,
  ALL_PAIRS,
  ON_LINE,
  RANDOM,
  FILE_PAIRS
};

/// Shortest-path algorithms.
enum Algorithm
{
  STD = 0,
  GO = 1,
  BI = 2,
  GOBI = 3
};

/// Event handler.
Event_Handler event_handler;

/// Logger.
Logger main_logger = Logger::getInstance("Router");

/// Trip_Request input operator.
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

/// Request handler.
class Request_Handler
{
protected:
  /// Request mode.
  Request_Mode request_mode;

  /// Number of (random) queries to be processed yet.
  int nmb_queries;

  /// Random seed.
  int random_seed;

  /// Is there request yet?
  bool requests_finished;

  /// Next request.
  Trip_Request trip_request;

  /// Network graph.
  Network_Graph *network;

  /// Source vertex iterator.
  Network_Graph::const_iterator it1;

  /// Destination vertex iterator.
  Network_Graph::const_iterator it2;

  /// Stream for reading requests from file.
  char *stream_filename;
  fstream trip_stream;

public:
  /// Constructor.
  Request_Handler() : request_mode(ON_LINE), nmb_queries(0), random_seed(0),
                      requests_finished(false), trip_request(), stream_filename(0) {}

  /// Set request mode.
  void set_mode(const Request_Mode &m) { request_mode = m; }

  /// Set number of queries.
  void set_nmb_queries(int n) { nmb_queries = n; }

  /// Set random seed.
  void set_random_seed(int s) { random_seed = s; }

  /// Set single request.
  void set_request(const Trip_Request &r)
  {
    trip_request.source = r.source;
    trip_request.destination = r.destination;
    trip_request.start_time = r.start_time;
    trip_request.id = r.id;
  }

  /// Set network graph.
  void set_network(Network_Graph *n) { network = n; }

  /// Set stream for trips.

  void set_stream(const char *stream_filename)
  {
    this->stream_filename = const_cast<char *>(stream_filename);
  }

  /// Get mode.
  Request_Mode mode() const { return request_mode; }

  /// Get trip request.
  Trip_Request request() const { return trip_request; }

  /// Set first request.
  void init()
  {
    trip_request.nfaID = 0; // HSM - default value.

    switch (request_mode)
    {
    case FILE_PAIRS:

      if (!stream_filename)
      {
        cout << "No filename for pairs given. Bye!" << endl;
        exit(-1);
      }

      trip_stream.open(stream_filename);

      if (!trip_stream)
      {
        cout << "There was an error opening the file with source destination pairs." << endl;
        cout << "Filename given: " << stream_filename << endl;
        exit(-1);
      }

      // Remove header if any. -- currently there is no header.

      long int t_id, src, dest, nfaID;
      double t0;
      trip_stream >> t_id >> src >> dest >> t0 >> nfaID;

      if (trip_stream.eof() || trip_stream.bad())
      {
        cout << "Okay, the trip file seems to be empty. Bye!" << endl;
        requests_finished = true;
        //exit(-1);
      }

      trip_request.id = t_id;
      trip_request.source = src;
      trip_request.destination = dest;
      trip_request.start_time = t0;
      trip_request.nfaID = nfaID;

      break;

    case SINGLE:
      break;
    case ALL_PAIRS:
      it1 = network->begin();
      ++it1; // skip -1
      it2 = network->begin();
      ++it2; // skip -1
      ++it2; // skip source==destination
      trip_request.source = (*it1)->external_id();
      trip_request.destination = (*it2)->external_id();
      break;
    case ON_LINE:
      cout << "Request a trip: source destination start_time: ";
      cin >> trip_request;
      break;
    case RANDOM:
      if (random_seed != 0)
        srand(random_seed);
      trip_request.source = (*network)[(int)round((double)rand() / RAND_MAX * (network->size() - 1))]->external_id();
      trip_request.destination = (*network)[(int)round((double)rand() / RAND_MAX * (network->size() - 1))]->external_id();
      --nmb_queries;
      break;
    }
  }

  /// Is there request yet?
  bool finished() const { return requests_finished; }

  /// Set next request.
  void next_request()
  {

    switch (request_mode)
    {

    case FILE_PAIRS:

      long int t_id, src, dest, nfaID;
      double t0;
      trip_stream >> t_id >> src >> dest >> t0 >> nfaID;

      if (trip_stream.eof() || trip_stream.bad())
      {
        requests_finished = true;
        break;
      }

      trip_request.id = t_id;
      trip_request.source = src;
      trip_request.destination = dest;
      trip_request.start_time = t0;
      trip_request.nfaID = nfaID;

      break;

    case SINGLE:
      requests_finished = true;
      break;
    case ALL_PAIRS:
      do
      {
        ++it2;
        if (it2 == network->end())
        {
          ++it1;
          it2 = network->begin();
          ++it2;
        }
        if (it1 == network->end())
        {
          requests_finished = true;
          break;
        }
      } while (requests_finished == false && *it1 == *it2);
      trip_request.source = (*it1)->external_id();
      trip_request.destination = (*it2)->external_id();
      break;
    case ON_LINE:
      cout << "Request a trip (source destination start_time): ";
      cin >> trip_request;
      break;
    case RANDOM:
      if (nmb_queries > 0)
      {
        trip_request.source = (*network)[(int)round((double)rand() / RAND_MAX * (network->size() - 1))]->external_id();
        trip_request.destination = (*network)[(int)round((double)rand() / RAND_MAX * (network->size() - 1))]->external_id();
        --nmb_queries;
      }
      else
        requests_finished = true;
      break;
    }
  }
};

// ----------------------------------------------------------------------------

/// Router.
class Router
{
protected:
  /// Network graph.
  Network_Graph &network;

  /// NFA graph.
  vector<NFA_Graph *> &nfaVector;

  /// Routing engines.
  vector<vector<Shortest_Path *>> dijkstra;

public:
  /// Constructor.
  Router(Network_Graph &n1, vector<NFA_Graph *> &nfaVec) : network(n1), nfaVector(nfaVec)
  {
    const unsigned int nNFA = nfaVec.size();

    dijkstra = vector<vector<Shortest_Path *>>(nNFA);

    for (unsigned int i = 0; i < nNFA; ++i)
    {
      dijkstra[i].resize(4);

      dijkstra[i][STD] = new Shortest_Path(network, *nfaVector[i]);
      dijkstra[i][GO] = new Goal_Dijkstra(network, *nfaVector[i]);
      dijkstra[i][BI] = new Bi_Dijkstra(network, *nfaVector[i]);
      dijkstra[i][GOBI] = new Bi_Goal_Dijkstra(network, *nfaVector[i]);

      // dijkstra[STD] = new Shortest_Path(network, nfa);
      // dijkstra[GO] = new Goal_Dijkstra(network, nfa);
      // dijkstra[BI] = new Bi_Dijkstra(network, nfa);
      // dijkstra[GOBI] = new Bi_Goal_Dijkstra(network, nfa);
    }
  }

  /// Compute route with specified routing algorithm.
  void find_path(Algorithm algorithm, Trip_Request trip, Plan &plan,
                 double &time_elapsed, unsigned int nfaChoice = 0)
  {
    dijkstra[nfaChoice][algorithm]->init(trip);
    Timer timer;
    dijkstra[nfaChoice][algorithm]->dijkstra();
    time_elapsed = timer.elapsed();
    dijkstra[nfaChoice][algorithm]->reconstruct_path(plan);
  }
};

// ----------------------------------------------------------------------------

/// Print usage.
void print_usage()
{
  cout << "Usage:" << endl
       << " Router [-g <graph>] [-c <coords>] [-n <NFA> | -N <NFAFILE>]" << endl
       << "        (([-s <source>] [-d <dest>] [-t <time>]) | [-f <pairs>] )" << endl
       << "        [-a <algorithm>] " << endl
       << "        [-o <results filename>] " << endl
       << "        [-v <viz>] [-i] [-l]" << endl
       << " -a <algorithm> shortest path algorithm (0=Dijkstra)|1=Goal)|2=Bi|3=G+B))" << endl
       << " -c <coords>  coordinates (vertex) file" << endl
       << " -d <dest>    destination vertex" << endl
       << " -f <pairs>   pairs from file" << endl
       << " -g <graph>   graph (edge) file" << endl
       << " -i           information output" << endl
       << " -l           labeling in visualization" << endl
       << " -n <NFA>     NFA file" << endl
       << " -N <NFAFile> file specifying nfa collection" << endl
       << " -o <results  filename> filename for results - will not overwrite unless -z given"
       << " -p <seed>    pseudo-random seed" << endl
       << " -r <nmb>     random queries" << endl
       << " -s <source>  source vertex" << endl
       << " -t <time>    time of departure" << endl
       << " -v <viz>     visualization output" << endl
       << " -z           zap existing results file if it already exists" << endl;
}

/// Main function.
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

  // set defaults

  const char *network_filename = "../networks/simfra-test.txt";
  const char *coords_filename = "../networks/simfra-test-coords.txt";
  const char *nfa_filename = "../nfas/simfra-nfa.txt";
  const char *nfa_collection_filename = "../nfas/trans_nfa_file.txt";
  const char *viz_filename = "";
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
  unsigned int zap_file = 0;

  unsigned singleNFA = 1;

  while ((c = getopt(argc, argv, "a:c:d:f:g:ilnN:o:p:r:s:t:v:z")) != -1)
  {
    int algo = 0;

    switch (c)
    {

    case 'a':
      algo = atoi(optarg);

      switch (algo)
      {
      case STD:
        cout << "*** Standard Dijkstra" << endl;
        event_handler.set_filename_affix("D");
        algorithm = STD;
        break;
      case GO:
        cout << "*** Goal-directed Dijkstra" << endl;
        event_handler.set_filename_affix("G");
        algorithm = GO;
        break;
      case BI:
        cout << "*** Bidirectional Dijkstra" << endl;
        event_handler.set_filename_affix("B");
        algorithm = BI;
        break;
      case GOBI:
        cout << "*** Bidirectional, goal-directed Dijkstra" << endl;
        event_handler.set_filename_affix("C");
        algorithm = GOBI;
        break;
      default:
        cout << "Invalid algorithm specified. Bye!" << endl;
        exit(-1);
        break;
      }

      break;

    case 'c':
      coords_filename = optarg;
      break;
    case 'd':
      request.destination = atoi(optarg);
      request_handler.set_mode(SINGLE);
      break;
    case 'f':
      request_handler.set_mode(FILE_PAIRS);
      pairs_filename = optarg;
      request_handler.set_stream(pairs_filename);
      break;
    case 'g':
      network_filename = optarg;
      break;
    case 'i':
      event_handler.set_record(true);
      event_handler.set_trace_mode(true);
      break;
    case 'l':
      event_handler.set_detailed_vis(true);
      break;
    case 'n':
      nfa_filename = optarg;
      singleNFA = 1;
      break;
    case 'N':
      nfa_collection_filename = optarg;
      singleNFA = 0;
      break;
    case 'o':
      out_filename = optarg;
      break;
    case 'p':
      request_handler.set_random_seed(atoi(optarg));
      break;
    case 'r':
      request_handler.set_mode(RANDOM);
      request_handler.set_nmb_queries(atoi(optarg));
      break;
    case 's':
      request.source = atoi(optarg);
      request_handler.set_mode(SINGLE);
      break;
    case 't':
      request.start_time = atoi(optarg);
      request_handler.set_mode(SINGLE);
      break;
    case 'v':
      viz_filename = optarg;
      event_handler.set_visualize(true);
      break;
    case 'z':
      zap_file = 1;
      break;
    }
  }

  // control output
  cout << "Data:" << endl
       << " network: " << network_filename << endl
       << " coords:  " << coords_filename << endl
       << " NFA:     " << nfa_filename << endl
       << " NFAColl: " << nfa_collection_filename << endl
       << " visual:  " << viz_filename << endl;

  // build network
  LOG4CPLUS_DEBUG(main_logger, "Building network...");
  Network_Graph network(network_filename, coords_filename);

  LOG4CPLUS_DEBUG(main_logger, "Building NFA...");
  //LOG4CPLUS_FLUSH();

  //  NFA_Graph nfa(nfa_filename, network);

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
    for (unsigned int i = 0; i < nNFAs; ++i)
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
  event_handler.set_outfile_name(viz_filename);

  cout << "Status..." << endl;

  // initialize request handler
  if (request_handler.mode() == ALL_PAIRS ||
      request_handler.mode() == RANDOM ||
      request_handler.mode() == FILE_PAIRS)
    request_handler.set_network(&network);

  request_handler.init();

  if (request_handler.mode() == SINGLE)
    request_handler.set_request(request);

  cout << "Status...." << endl;

  // Set up file for results:
  // assuming -z for now.

  out_file.open(out_filename);
  if (!out_file)
  {
    cout << "Sorry, could not open file " << out_filename << ". Bye!" << endl;
    exit(-1);
  }

  // process queries
  while (!request_handler.finished())
  {
    Trip_Request trip_request = request_handler.request();

    //    cout << trip_request;

    // cout << "-->" << trip_request.id << '\t'
    //      << trip_request.source << '\t'
    //      << trip_request.destination << endl;

    // ------------------------------------------------------------

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

    //distance = plan.path.back().time;
    //event_handler.print_measurements();
    //cout << "time: " << time_elapsed << endl;

    // ------------------------------------------------------------

    // check for errors -- HSM: adapted.

    bool error = false;
    string error_message = "Differing distances:  request " + itos(trip_request.source) + "--" + itos(trip_request.destination) + "  distances";

    error_message += " " + ftos(distance);
    error_message += "  differences";

    if (error)
      LOG4CPLUS_ERROR(main_logger, error_message);

    //     else
    //       LOG4CPLUS_INFO(main_logger, "Distance: " + ftos(distance));

    // ------------------------------------------------------------

    request_handler.next_request();
  }

  out_file.close();

  return 0;
}
