#ifndef TOOLS_HPP
#define TOOLS_HPP

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

class Request_Handler
{
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
    Request_Handler() : request_mode(ON_LINE), requests_finished(false), trip_request(), stream_filename(0) {}

    /// Set request mode.
    void set_mode(const Request_Mode &m) { request_mode = m; }

    /// Set single request.
    void set_request(const Trip_Request &r)
    {
        trip_request.source = r.source;
        trip_request.destination = r.destination;
        trip_request.start_time = r.start_time;
        trip_request.id = r.id;
    }

    /// Set network graph.
    void set_network(const Network_Graph *n) { network = n; }

    /// Set stream for trips.

    void set_stream(const char *stream_filename)
    {
        this->stream_filename = const_cast<char *>(stream_filename);
    }

    /// Get mode.
    //Request_Mode mode() const { return request_mode; }

    /// Get trip request.
    Trip_Request request() const { return trip_request; }

    /// Set first request.
    void init()
    {
        trip_request.nfaID = 0; // HSM - default value.

        if (request_mode == FILE_PAIRS)
        {

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
        }
    }

    /// Is there request yet?
    bool finished() const { return requests_finished; }

    /// Set next request.
    void next_request()
    {

        if (request_mode == FILE_PAIRS)
        {

            long int t_id, src, dest, nfaID;
            double t0;
            trip_stream >> t_id >> src >> dest >> t0 >> nfaID;

            if (trip_stream.eof() || trip_stream.bad())
            {
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

    //condensed request
    vector<vector<Trip_Request>> thread_request()
    {
        vector<Trip_Request> request_vector;
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

        long int t_id, src, dest, nfaID;
        double t0;

        
        do
        {
            trip_stream >> t_id >> src >> dest >> t0 >> nfaID;
            if (trip_stream.eof() || trip_stream.bad())
            {
                requests_finished = true;
                continue;
            }
            
            trip_request.id = t_id;
            trip_request.source = src;
            trip_request.destination = dest;
            trip_request.start_time = t0;
            trip_request.nfaID = nfaID;
            request_vector.push_back(trip_request);
            //cout<<"okay"<<endl;
        } while (!finished());

        unsigned long const cores = std::thread::hardware_concurrency();
        cout<<cores<<endl;
        int vec_size = 0;
        int test = request_vector.size();
        cout<<test<<endl;
        if(test < (int) cores){
            vec_size = test;
        }
        else{
            vec_size = test/(int)cores;
        }
        int count = 0;
        vector<Trip_Request> temp;
        vector<vector<Trip_Request>> big_list;
        while(!request_vector.empty()){
           
            if(count > vec_size){
                count = 0;
                big_list.push_back(temp);
                temp.clear();

            }
            else{
            temp.push_back(request_vector.back());
            request_vector.pop_back();
            count++;
            }
        }
        return big_list;
    }
};

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

#endif