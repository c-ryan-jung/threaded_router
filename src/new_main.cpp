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

  //request_handler.init();
  vector<Trip_Request> trip_list = request_handler.thread_request();
  cout << "Status...." << endl;

  while(!trip_list.empty()){
    Trip_Request trip_request = trip_list.back();

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
    trip_list.pop_back();
  }
  /* while (!request_handler.finished())
  {
    Trip_Request trip_request = request_handler.request();

    float distance = 0.0;

    plan.path.clear();
    event_handler.clear();
    double time_elapsed;

    router.find_path((Algorithm)algorithm, request_handler.request(), plan,
                     time_elapsed, trip_request.nfaID);

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
  }*/
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

  //the start of the threaded method
  
  std::thread test(thread_method, std::ref(request_handler), plan, std::ref(network), algorithm, std::ref(out_file), singleNFA, nfa_filename, nfa_collection_filename);
  test.join();

  out_file.close();

  return 0;
}