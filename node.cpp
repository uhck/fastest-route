#include "node.h"

node::node()
{
    iata="";
    weight=DBL_MAX;
}

node::node(string i, double w)
{
    weight=w;
    iata=i;
}


node::node(string i, edge e, double w)
{
    weight = w;
    iata = i;
    itinerary.push_back(e);
}

node::node(const node &other)
{
    copy(other);
}

node::~node()
{

}

node& node::operator=(const node &other)
{
    if(this!=&other)
        copy(other);
    return *this;
}

size_t node::binarySearch(const vector<edge>& v, const string &d)
{
    if(v.empty())
        return false;
    bool foundIt = false;
    size_t low = 0, high = v.size()-1;
    size_t pos = (high + low)>>1;
    while(high >= low)
        if(v[pos].destinationIATA == d)
        {
            foundIt = true;
            break;
        }
        else
            if(d < v[pos].destinationIATA)
            {
                if(!pos)
                    return false;
                high = pos - 1;
                pos = (high + low)>>1;
            }
            else
            {
                low = pos + 1;
                pos = (high + low)>>1;
            }
    if(foundIt)
        return pos;
    else
        return string::npos;
}


const string& node::getID() const
{
    return iata;
}

const string &node::getCity() const
{
    return city;
}

void node::setID(string s)
{
    iata=s;
}

const double& node::getWeight() const
{
    return weight;
}

void node::setWeight(double w)
{
    weight=w;
}

void node::addToWeight(double w)
{
    weight+=w;
}

void node::addToVisited(const node& newNode)
{
    visited.push_back(newNode.getID());
}

void node::printItinerary(ostream &out, const string& fromAirport, const string& toAirport)
{
    cout << "Leave " << fromAirport << " on ";
    for(int i = 0; i < itinerary.back().airline.size() - 1; ++i)
    {
        if(i){
            cout << "or ";
        }
        cout << itinerary.back().airline[i] << " ";
    }
    cout << "to " << itinerary[0].destinationIATA << " in " << itinerary[0].destinationCity << ". " << endl;
    cout << "After flying " << itinerary[0].distance << " kilometers " << endl;
    if(!itinerary.empty())
    {
        for(int i = 0; i < itinerary.size()-1; ++i){
            cout << "Change planes at "<< itinerary[i].destinationIATA << "(" << itinerary[i].destinationCity
                 << ")" << " to ";
            for(int j = 0; j < itinerary[i].airline.size() - 1; ++j)
            {
                if(j){
                    cout << "or ";
                }
                cout << itinerary[i].airline[j] << " ";
            }
            cout<< "and fly " << itinerary[i+1].distance << " kilometers " << "to " << itinerary[i+1].destinationIATA << " in " << itinerary[i+1].destinationCity << endl;
        }
    }
    cout << "Arrive at " << toAirport << " in " <<  itinerary[itinerary.size()-1].destinationCity << endl;
    return;
}

void node::addToItinerary(const node &newNode)
{
    if (!newNode.itinerary.empty())
        itinerary.push_back(newNode.itinerary[0]);
}

void node::setToItinerary(const node &newNode)
{
    itinerary.clear();
    itinerary = newNode.itinerary;
}

const vector<string>& node::getVisited() const
{
    return visited;
}

void node::setVisited(const node& newNode)
{
    visited.clear();
    visited = newNode.getVisited();
}

void node::resetNode()
{
    weight = DBL_MAX;
    visited.clear();
    itinerary.clear();
}

void node::copy(const node& other)
{
    weight = other.weight;
    iata = other.iata;
    visited = other.visited;
    itinerary = other.itinerary;
}

bool operator<(const node &x, const node &y)
{
    return x.weight < y.weight;
}

bool operator>(const node &x, const node &y)
{
    return x.weight > y.weight;
}

bool operator<=(const node &x, const node &y)
{
    return x.weight <= y.weight;
}

bool operator>=(const node &x, const node &y)
{
    return x.weight >= y.weight;
}

bool operator==(const node &x, const node &y)
{
    return x.weight==y.weight;
}

bool operator!=(const node& x, const node& y)
{
    return x.weight!=y.weight;
}

istream& operator>>(istream &in, const node &other)
{

}

ostream& operator<<(ostream &out, const node &other)
{
    cout<<other.getID()<<"["<<other.getWeight()<<"][";
    for(vector<string>::const_iterator n=other.visited.begin(); n!=other.visited.end(); ++n)
        cout<<*n<<" ";
    cout<<"]\n";
    cout<<"The itinerary size is:"<<other.itinerary.size()<<endl;
    cout<<"It contains"<<endl;
    for (int i =(other.itinerary.size() - 1); i >= 0 ; --i)
    {
        cout<<other.itinerary[i]<<endl;
    }
    return out;
}
