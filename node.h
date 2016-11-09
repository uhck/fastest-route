#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

#ifndef NODE_H
#define NODE_H

#define DBL_MAX 41000

using namespace std;

struct edge
{
    string destinationIATA, destinationCity, distance;
    vector<string> airline;

    edge& operator=(const edge& other)
    {
        destinationIATA = other.destinationIATA;
        destinationCity = other.destinationCity;
        distance = other.distance;
        airline = other.airline;
    }

    friend
    ostream& operator<<(ostream& out, const edge& other)
    {
        out<<"dIATA="<<other.destinationIATA<<endl;
        out<<"dCity="<<other.destinationCity<<endl;
        out<<"distance="<<other.distance<<endl;
        for(int i = 0; i < other.airline.size() - 1; ++i){
            out<<"airline="<<other.airline[i]<<endl;
        }
        return out;
    }
};

class node
{
    public:
        node();
        node(string i, double w = DBL_MAX);
        node(string i, edge e, double w = DBL_MAX );
        node(const node& other);
        ~node();
        node& operator=(const node& other);
        const string& getID() const;
        const string& getCity() const;
        void setID(string s);

        const double& getWeight() const;
        void addToWeight(double w);
        void setWeight(double w);
        void addToVisited(const node& newNode);
        void addToItinerary(const node& newNode);
        void setToItinerary(const node& newNode);
        const vector<string>& getVisited() const;
        void setVisited(const node& newNode);
        void printItinerary(ostream &out, const string& fromAirport, const string& toAirport);
        size_t binarySearch(const vector<edge>& v, const string &d);
        void resetNode();

        friend
        bool operator<(const node& x, const node& y);

        friend
        bool operator>(const node& x, const node& y);

        friend
        bool operator<=(const node& x, const node& y);

        friend
        bool operator>=(const node& x, const node& y);

        friend
        bool operator==(const node& x, const node& y);

        friend
        bool operator!=(const node& x, const node& y);

        friend
        istream& operator>>(istream& in, const node& other);

        friend
        ostream& operator<<(ostream& out, const node& other);

        vector<edge> itinerary;

    private:
        string iata,city;
        double weight;
        vector<string> visited;
        void copy(const node& other);
};

struct classcomp
{
  bool operator() (const node& lhs, const node& rhs) const
  {return lhs.getID() < rhs.getID();}
};

struct edgecomp
{
    bool operator() (const node& lhs, const node& rhs) const
    {return lhs.getWeight() < rhs.getWeight();}
};

#endif // NODE_H
