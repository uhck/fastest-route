#include <iostream>
#include <cstdlib>
#include <map>
#include <queue>
#include <cmath>
#include <QDomDocument>
#include <QFile>
#include <algorithm>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QtXml>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <utility>
#include <cstdio>
#include <ctime>
#include "node.h"

using namespace std;

#define R 6373 //km
#define PI 3.14159265359
#define DBL_MAX 41000

bool getAirports(const vector<string> &validAirports);
bool findFastestRoute(node from, node& destination, map<node,map<node, double, classcomp>, classcomp> routeMap);
void loadRouteMap(map<node, map<node, double, classcomp>, classcomp> &routeMap, vector<string> &validAirports);
bool binarySearch(const vector<string>& v, const string &d);
void bubbleUp(vector<string>& sorted);
void readFromXml(QDomDocument &doc, const QString &filename);

int main()
{
    string fromAirport, toAirport;
    map<node, map<node, double, classcomp>, classcomp> routeMap;
    vector<string> validAirports;
    string answer;

    loadRouteMap(routeMap, validAirports);

    do {
        cout << "From airport: ";
        fromAirport = getAirports(validAirports);
        cout << "To airport: ";
        toAirport = getAirports(validAirports);
        node A(fromAirport), B(toAirport);

        if (findFastestRoute(A, B, routeMap))
            B.printItinerary(cout, fromAirport, toAirport);
        else
            cout << "No routes found from " << A.getID() << " to " << B.getID() << endl;

        cout << "Find another route from an airport to another airport? (Y/N)" << endl;
        getline(cin, answer);
        answer[0] = toupper(answer[0]);
        while (answer[0] != 'Y' && answer[0] != 'N')
        {
            cout << "Invalid option!! Y or N" << endl;
            getline(cin, answer);
            answer[0] = toupper(answer[0]);
        }
        fflush(stdin);
    } while (answer[0] == 'Y');

    return 0;
}

string getAirports(const vector<string>& validAirports)
{
    bool valid = false;
    string airport;

    while (!valid)
    {
        getline(cin, airport);
        for (int i = 0; i < airport.length(); ++i)
            airport[i] = toupper(airport[i]);
        if (binarySearch(validAirports, airport))
        {
            valid = true;
            return airport;
        }
        cout << "The airport " << airport << " does not exist!!!" << endl;
    }
}

bool findFastestRoute(node from, node& destination, map<node,map<node, double, classcomp>, classcomp> routeMap)
{
    std::clock_t start; //time
    double duration; //time
    node to, current;
    vector<string> visitedAirports;
    set<node> visitedRoutes;
    from.addToVisited(from);
    visitedAirports.push_back(from.getID());
    from.setWeight(0.0);
    visitedRoutes.insert(from);

    start = std::clock(); //time

    while (visitedRoutes.size() > 0)
    {
        current = *visitedRoutes.begin();
        if(current.getID() == destination.getID())
        {
            cout << "Reached destination!!!\n";
            break;
        }
        visitedRoutes.erase(visitedRoutes.begin());
        for(map<node,double>::iterator m = routeMap[current].begin(); m != routeMap[current].end(); ++m)
        {
            to = m->first;
            if (binarySearch(visitedAirports, to.getID()))
                continue;
            for(set<node>::iterator it = visitedRoutes.begin(); it != visitedRoutes.end(); ++it)
            {
                if(it->getID() == to.getID() && it->getWeight() < to.getWeight())
                {
                    to = *it;
                    visitedRoutes.erase(it);
                }
            }
            if(m->second + current.getWeight() < to.getWeight())
            {
                to.setWeight((*m).second + current.getWeight());
                to.setToItinerary(current);
                to.setVisited(current);
                to.addToItinerary((*m).first);
                to.addToVisited(to);
            }
            visitedRoutes.insert(to);
        }
        visitedAirports.push_back(current.getID());
        bubbleUp(visitedAirports);
    }

    duration = (std::clock()-start) / (double)CLOCKS_PER_SEC;
    cout << "Processing time: " << duration << "\n";

    if(visitedRoutes.empty())
        return false;
    else if((*visitedRoutes.begin()).getID() == destination.getID())
    {
        destination = *visitedRoutes.begin();
        return true;
    }
    return false;
}

void loadRouteMap(map<node, map<node, double, classcomp>, classcomp> &routeMap, vector<string>& validAirports)
{
    QDomDocument finalData;
    readFromXml(finalData, "finalData.xml");
    map<node, double, classcomp> toAndWeight;

    QDomNodeList elements = finalData.documentElement().elementsByTagName("vertex");
    cout << "Loading the map..." << endl;

    for (size_t i = 0; i < elements.size(); ++i)
    {
        QDomNode curr = elements.item(i);
        QDomNodeList elements2 = curr.firstChildElement("edges").childNodes();
        for (size_t k = 0; k < elements2.size(); ++k)
        {
            QDomNode curr2 = elements2.item(k);
            vector<string> foundDestAirports;
            edge e;
            e.destinationIATA = curr2.firstChildElement("destinationAirport").text().toStdString();
            e.destinationCity = curr2.firstChildElement("destinationCity").text().toStdString();
            e.distance = curr2.firstChildElement("distance").text().toStdString();
            if (!binarySearch(foundDestAirports, e.destinationIATA)){
                for (int l = k; l < elements2.size(); ++l){
                    QDomNode curr3 = elements2.item(l);
                    if (curr3.firstChildElement("destinationAirport").text().toStdString() == curr2.firstChildElement("destinationAirport").text().toStdString())
                        e.airline.push_back(curr3.firstChildElement("airlineName").text().toStdString());
                }
                if (e.airline.empty())
                    e.airline.push_back("Empty Airline");
            }
            foundDestAirports.push_back(e.destinationIATA);
            bubbleUp(foundDestAirports);
            e.airline.push_back("Empty Airline");
            toAndWeight.insert(make_pair(node(curr2.firstChildElement("destinationAirport").text().toStdString(), e), curr2.firstChildElement("distance").text().toDouble()));
        }
        routeMap.insert(make_pair(node(curr.firstChildElement("airport").text().toStdString()), toAndWeight));
        validAirports.push_back(curr.firstChildElement("airport").text().toStdString());
        toAndWeight.clear();
    }

    sort(validAirports.begin(),validAirports.end());
    cout << "Finished loading map." << endl;
}

void readFromXml(QDomDocument &doc, const QString &filename)
{
    QFile file(filename);
    cout << "Reading " << filename.toStdString() << endl;
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open " << filename << "!";
        return;
    }
    if (!doc.setContent(&file))
    {
        qDebug() << "Cannot parse " << filename << "!";
        return;
    }
    cout << "Finished reading " << filename.toStdString() << endl;
}

bool binarySearch(const vector<string>& v, const string &d)
{
    if (v.empty())
        return false;
    bool foundIt = false;
    size_t low = 0, high = v.size()-1;
    size_t pos = (high + low) >> 1;
    while (high >= low)
        if (v[pos] == d)
        {
            foundIt = true;
            break;
        }
        else
            if (d < v[pos])
            {
                if (!pos)
                    return false;
                high = pos - 1;
                pos = (high + low) >> 1;
            }
            else
            {
                low = pos + 1;
                pos = (high + low) >> 1;
            }
    return foundIt;
}

void bubbleUp(vector<string>& sorted)
{
    for(int i = sorted.size()-1; i >= 0; --i)
    {
        if(i && (sorted[i] < sorted[i-1]))
            swap(sorted[i], sorted[i-1]);
        else
            return;
    }
}
