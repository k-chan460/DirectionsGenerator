// main.cpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include <map>
#include "TripReader.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include <iostream>
#include <cmath>



void timeprint(double t){
	int hr = 0;
	int min = 0;
	double sec = 0;

	hr = t/3600.00;
	min = (((t/3600) - hr)*60);
	sec = (((t/60)- min)*60);

	if (hr > 1){
		std::cout << hr << " hours ";
	}
	if(min > 1){
		std::cout << min << " minutes ";
	}
	std::cout <<(double(std::round(sec*100))/100) << " seconds";

}

int main()
{

	struct storage
	{
		int vertex;
		std::string name;
		RoadSegment data;
	};


	RoadMapReader rm;
	InputReader in = InputReader(std::cin);
	RoadMap rmap = rm.readRoadMap(in);

	TripReader t_read;
	std::vector<Trip> trips = t_read.readTrips(in);
	std::function<double(const RoadSegment&)> besttime = [](RoadSegment rs){return rs.miles/rs.milesPerHour;};
	std::function<double(const RoadSegment&)> bestpath = [](RoadSegment rs){return rs.miles;};

	std::map<int,int> store;


	for(auto iter = trips.begin(); iter!= trips.end(); iter++){
		std::vector<storage> trip_return;
		if(iter->metric == TripMetric::Distance){
			store = rmap.findShortestPaths(iter->startVertex, bestpath);
		}
		else if (iter->metric == TripMetric::Time){
			store = rmap.findShortestPaths(iter->startVertex, besttime);
		}


		int next;
		next = iter->endVertex;
		storage last = storage{next,rmap.vertexInfo(next), rmap.edgeInfo(store[next], next)};
		trip_return.push_back(last);

		double totaltime = 0;

		while(next != iter ->startVertex){
			int temp = next;
			next = store[next];
			trip_return.push_back(storage{next,rmap.vertexInfo(next),rmap.edgeInfo(next,temp)});
		}



		if(iter->metric == TripMetric::Distance){
			double minitemp = 0;


			std::cout << "Shortest distance from " << rmap.vertexInfo(trip_return.back().vertex) << " to " << rmap.vertexInfo(trip_return.front().vertex) << std::endl;
			std::cout << "\t Begin at " << trip_return[trip_return.size()-1].name << std::endl;

			for(int i = trip_return.size() - 2; i >= 0; i--){
				std::cout << "\t Continue to " << trip_return[i].name << " ("<< trip_return[i+1].data.miles <<" miles)" << std::endl;
				minitemp += trip_return[i+1].data.miles;
			}
			std::cout << "Total Distance: " << minitemp << " miles"<<std::endl;
		}

		else if(iter->metric == TripMetric::Time){

			std::cout << "Shortest driving from " << rmap.vertexInfo(trip_return.back().vertex) << " to " << rmap.vertexInfo(trip_return.front().vertex) << std::endl;
			std::cout << "\t Begin at " << trip_return[trip_return.size() - 1].name << std::endl;
			for (int i = trip_return.size()-2 ; i >= 0 ; i--){
				double t = (trip_return[i+1].data.miles / trip_return[i+1].data.milesPerHour) * 3600;
				std::cout << "\t Continue to " << trip_return[i].name << " (" << trip_return[i+1].data.miles << "miles & " << trip_return[i+1].data.milesPerHour << "mph = ";

				totaltime += t;
				timeprint(t);
				std::cout << ")" << std::endl;

			}
			std::cout << "Total time: ";
			timeprint(totaltime);
			std::cout << std::endl;

		}

	}





}

